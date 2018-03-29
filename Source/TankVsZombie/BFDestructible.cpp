#include "TankVsZombie.h"
#include "BFGameState.h"
#include "BFDestructible.h"


ABFDestructible::ABFDestructible(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	BaseMomentum = 1.0f;
	bExploded = false;
	MaxEndurance = 100.0f;
	Endurance = MaxEndurance;
}
void ABFDestructible::BeginPlay()
{
	Super::BeginPlay();

	Endurance = MaxEndurance;
	bExploded = false;
}
void ABFDestructible::ExplodeInternal(AActor* DamageCauser, FVector DamageImpulse, FVector DamageImpactPoint, FVector DamageImpactNormal)
{
	if (!bExploded)
	{
		bExploded = true; 

		if (bRadialDamage)
		{
			TArray<AActor*> IgnoredActors;
			
			IgnoredActors.Add(this);
			
			HurtRadius(BaseMomentum, GetActorLocation(), DamageType , IgnoredActors, DamageCauser, DamageCauser->GetInstigatorController());
		}

		OnExploded(DamageCauser,DamageImpulse,DamageImpactPoint,DamageImpactNormal);
	}
}
bool ABFDestructible::HurtRadius(float BaseMomentumMag, const FVector& Origin, TSubclassOf<UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors , AActor* DamageCauser , AController* InstigatedByController )
{ 
	float DamageOuterRadius = DamageParams.OuterRadius;

	static FName NAME_ApplyRadialDamage = FName(TEXT("ApplyRadialDamage"));
	FCollisionQueryParams SphereParams(NAME_ApplyRadialDamage, true, DamageCauser);

	SphereParams.AddIgnoredActors(IgnoreActors);

	if (DamageCauser != NULL)
	{ 
		SphereParams.AddIgnoredActor(DamageCauser); 
	}

	TArray<FOverlapResult> Overlaps;
	UWorld* World = GEngine->GetWorldFromContextObject(this,  EGetWorldErrorMode::ReturnNull);
	ABFGameState* GS = World->GetGameState<ABFGameState>();

	FCollisionObjectQueryParams ObjectsParams;
	ObjectsParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	ObjectsParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);
	ObjectsParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectsParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);

	World->OverlapMultiByObjectType( Overlaps , Origin , FQuat::Identity , ObjectsParams , FCollisionShape::MakeSphere( DamageOuterRadius ) , SphereParams );

	TMap< AActor*, TArray<FHitResult> > OverlapComponentMap;
	for (int32 Idx = 0; Idx < Overlaps.Num(); ++Idx)
	{ 

		FOverlapResult const& Overlap = Overlaps[Idx]; 
		AActor* const OverlapActor = Overlap.GetActor();

		if (OverlapActor != NULL && OverlapActor->bCanBeDamaged && Overlap.Component.IsValid())
		{
			FHitResult Hit(OverlapActor, Overlap.Component.Get(), OverlapActor->GetActorLocation(), FVector(0, 0, 1.f));
		
			if ((OverlapActor->GetActorLocation() - Origin).Size() <= DamageOuterRadius)
			{

				TArray<FHitResult>& HitList = OverlapComponentMap.FindOrAdd(OverlapActor);
				HitList.Add(Hit);

			}
		}

	}

	TSubclassOf<UDamageType> const ValidDamageTypeClass = (DamageTypeClass == NULL) ? TSubclassOf<UDamageType>(UDamageType::StaticClass()) : DamageTypeClass;

	bool bAppliedDamage = false;

	for (TMap<AActor*, TArray<FHitResult> >::TIterator It(OverlapComponentMap); It; ++It)
	{

		AActor* const Victim = It.Key() ;
		APawn* const VictimPawn = Cast<APawn>(Victim) ;
		TArray<FHitResult> const& ComponentHits = It.Value() ;

		FBFRadialDamageEvent	DmgEvent ;
		DmgEvent.DamageTypeClass	= ValidDamageTypeClass ;
		DmgEvent.ComponentHits		= ComponentHits	;
		DmgEvent.Origin				= Origin ;
		DmgEvent.Params				= DamageParams;
		DmgEvent.BaseMomentumMag	= BaseMomentumMag ;
		DmgEvent.ShotDirection		= FVector(Victim->GetActorLocation() - Origin).GetSafeNormal() ; 
		
		AActor* VictimController	= VictimPawn ? VictimPawn->GetController() : Victim ;

		if ( ( GS != NULL && !GS->OnSameTeam( InstigatedByController , VictimController ) ) )
		{

			Victim->TakeDamage( DamageParams.BaseDamage , DmgEvent , InstigatedByController , DamageCauser )	;

		}

		bAppliedDamage = true ; 

	}

	return bAppliedDamage;

}


float ABFDestructible::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	

	float ActualDamage = DamageAmount; 
	FVector DamageImpactPoint = GetActorLocation();
	FVector DamageImpactNormal = FVector(0.0f, 0.0f, 1.0f);
	FVector DamageImpulse = FVector(0.0f, 0.0f, 1.0f);

	UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();

	if (DamageEvent.IsOfType(FBFPointDamageEvent::ClassID))
	{

		FBFPointDamageEvent* const PointDamageEvent = (FBFPointDamageEvent*)&DamageEvent;
		ActualDamage = InternalTakePointDamage(ActualDamage, *PointDamageEvent, EventInstigator, DamageCauser);
		DamageImpulse = PointDamageEvent->ShotDirection.GetSafeNormal()*DamageTypeCDO->DamageImpulse;
		DamageImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;
		DamageImpactNormal = PointDamageEvent->HitInfo.ImpactNormal;

	}
	else if (DamageEvent.IsOfType(FBFRadialDamageEvent::ClassID))
	{

		FBFRadialDamageEvent* const RadialDamageEvent = (FBFRadialDamageEvent*)&DamageEvent;
		ActualDamage = InternalTakeRadialDamage(ActualDamage, *RadialDamageEvent, EventInstigator, DamageCauser);
		DamageImpulse = RadialDamageEvent->ShotDirection.GetSafeNormal()*DamageTypeCDO->DamageImpulse;
		if (RadialDamageEvent->ComponentHits.Num())
		{
			DamageImpactPoint = RadialDamageEvent->ComponentHits[0].ImpactPoint;
			DamageImpactNormal = RadialDamageEvent->ComponentHits[0].ImpactNormal;
		}
	}

	Endurance -= ActualDamage ;

	PostDamageTaken( DamageCauser , DamageImpulse , DamageImpactPoint , DamageImpactNormal );

	if (Endurance <= 0.0f)
	{
		Endurance = 0.0f;
		ExplodeInternal(DamageCauser, DamageImpulse, DamageImpactPoint, DamageImpactNormal);
	}

	return ActualDamage;
}
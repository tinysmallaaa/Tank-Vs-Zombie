#include "TankVsZombie.h"
#include "BFThrowable.h"

ABFThrowable::ABFThrowable(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{


}

void ABFThrowable::AdjustOnDrag( FVector NewLocation , FVector ImpactNormal , bool DragOverEvent )
{
	FVector Location = NewLocation;
	Location.Z += Height;
	SetActorLocation(Location); 
	if(DragOverEvent)
		OnPlaced(Location);
} 

void ABFThrowable::ReleaseOnLocation(FVector Location)
{ 
	OnPlaced( Location );
}

void ABFThrowable::ShutDown()
{ 
	if (!IsPendingKillPending())
	{

		SetActorEnableCollision(false); 

		bool bFoundParticles = false;

		TArray<USceneComponent*> Components;
		GetComponents<USceneComponent>(Components);

		for (int32 i = 0; i < Components.Num(); i++)
		{
			UParticleSystemComponent* PSC = Cast<UParticleSystemComponent>(Components[i]);
			if (PSC != NULL)
			{
				PSC->TickComponent(0.0f, LEVELTICK_All, NULL);
				PSC->DeactivateSystem();
				PSC->bAutoDestroy = true;
				bFoundParticles = true;
			}
			else
			{
				UAudioComponent* Audio = Cast<UAudioComponent>(Components[i]);

				if (Audio != NULL)
				{
					if (Audio->Sound != NULL && Audio->Sound->GetDuration() >= INDEFINITELY_LOOPING_DURATION)
					{
						Audio->Stop();
					}
				}
				else
				{
					Components[i]->SetHiddenInGame(true);
					Components[i]->SetVisibility(false);
				}
			}
		}

		SetLifeSpan((bFoundParticles && GetNetMode() != NM_DedicatedServer) ? 2.0f : 0.2f);  
	}

	bExploded = true; 
} 
 
bool ABFThrowable::HurtRadius(UObject* WorldContextObject, float BaseDamage, float MinimumDamage, float BaseMomentumMag, const FVector& Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff,
	TSubclassOf<UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, AActor* DamageCauser, AController* InstigatedByController, float CollisionFreeRadius, const TArray<FVector>* AltVisibilityOrigins)
{ 

	static FName NAME_ApplyRadialDamage = FName(TEXT("ApplyRadialDamage"));
	FCollisionQueryParams SphereParams(NAME_ApplyRadialDamage, true, DamageCauser);

	SphereParams.AddIgnoredActors(IgnoreActors);

	if (DamageCauser != NULL)
	{ 
		SphereParams.AddIgnoredActor(DamageCauser); 
	}

	TArray<FOverlapResult> Overlaps;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,  EGetWorldErrorMode::ReturnNull);
	ABFGameState* GS = World->GetGameState<ABFGameState>();
	FCollisionObjectQueryParams ObjectsParams;
	ObjectsParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	ObjectsParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);
	ObjectsParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);


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

	for ( TMap<AActor*, TArray<FHitResult> >::TIterator It(OverlapComponentMap) ; It ; ++It )
	{

		AActor* const Victim = It.Key() ;
		APawn* const VictimPawn = Cast<APawn>(Victim) ;
		TArray<FHitResult> const& ComponentHits = It.Value() ;

		FBFRadialDamageEvent DmgEvent ;
		DmgEvent.DamageTypeClass	= ValidDamageTypeClass ;
		DmgEvent.ComponentHits		= ComponentHits	;
		DmgEvent.Origin				= Origin ;
		DmgEvent.Params				= FRadialDamageParams(BaseDamage, MinimumDamage, DamageInnerRadius, DamageOuterRadius, DamageFalloff) ;
		DmgEvent.BaseMomentumMag	= BaseMomentumMag ;
		DmgEvent.ShotDirection		= FVector(Victim->GetActorLocation() - GetActorLocation()).GetSafeNormal() ; 
		
		AActor* VictimController	= VictimPawn ? VictimPawn->GetController() : Victim ;

		if ( ( GS != NULL && !GS->OnSameTeam( InstigatedByController , VictimController ) ) )
		{
			Victim->TakeDamage( BaseDamage , DmgEvent , InstigatedByController , DamageCauser )	;
		}

		bAppliedDamage = true ; 

	}

	return bAppliedDamage;
} 

void ABFThrowable::Explode_Implementation(const FHitResult& HitInfo, const FVector& Location, UPrimitiveComponent* HitComp)
{
	if (!bExploded)
	{
		bExploded = true;
		float AdjustedMomentum = Momentum;
		FRadialDamageParams AdjustedDamageParams = DamageParams;

		if (AdjustedDamageParams.OuterRadius > 0.0f )
		{
			TArray<AActor*> IgnoreActors;
			 
				IgnoreActors.Add(this);
			 
			TArray<FVector> AltOrigins;
			HurtRadius(this, AdjustedDamageParams.BaseDamage, AdjustedDamageParams.MinimumDamage, AdjustedMomentum, HitInfo.ImpactPoint + HitInfo.Normal,
				AdjustedDamageParams.InnerRadius, AdjustedDamageParams.OuterRadius, AdjustedDamageParams.DamageFalloff,
				DamageType, IgnoreActors, this, GetInstigatorController(), 0.0f, &AltOrigins); 

		} 

		OnExploded();

		ShutDown();
	}
} 

void ABFThrowable::SetEnableCollision(bool flag)
{
	SetActorEnableCollision(flag);
}

float ABFThrowable::GetOuterRadius()
{
	return DamageParams.OuterRadius;
}
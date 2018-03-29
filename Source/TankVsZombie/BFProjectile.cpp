// Fill out your copyright notice in the Description page of Project Settings.

#include "TankVsZombie.h"
#include "Weapon/BFWeapon.h"
#include "BFGameState.h"
#include "BFProjectile.h"
#include <AI/Navigation/NavModifierVolume.h>
#include <AI/Navigation/NavMeshBoundsVolume.h>



// Sets default values
ABFProjectile::ABFProjectile(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	PrimaryActorTick.bCanEverTick = true ;

	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, "CollisionComp") ;
	CollisionComp->SetSphereRadius( 10.0f )			;
	CollisionComp->bReturnMaterialOnMove = true		; 
	CollisionComp->bGenerateOverlapEvents = true	;  
	//CollisionComp->OnComponentHit.AddDynamic( this , &ABFProjectile::OnComponentHit )			;
	CollisionComp->OnComponentBeginOverlap.AddDynamic( this , &ABFProjectile::OnOverlapBegin )	;
	CollisionComp->bTraceComplexOnMove	= true		;
	CollisionComp->bReceivesDecals		= false		;
	

	RootComponent = CollisionComp ;

	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, "ProjectileMove") ;
	ProjectileMovement->UpdatedComponent = CollisionComp ;
	ProjectileMovement->InitialSpeed = 3000.f ;
	ProjectileMovement->MaxSpeed = 3000.f ;
	ProjectileMovement->bRotationFollowsVelocity = true ;
	ProjectileMovement->bShouldBounce = false ;
	ProjectileMovement->OnProjectileStop.AddDynamic( this , &ABFProjectile::OnStop ) ;
	ProjectileMovement->OnProjectileBounce.AddDynamic( this , &ABFProjectile::OnBounce ) ;

	ProjectileMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "ProjectileMesh") ;
	ProjectileMesh->bHiddenInGame = false ; 
	ProjectileMesh->SetCollisionProfileName("NoCollision");
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMesh->AttachToComponent(CollisionComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false)) ;

	DamageParams.BaseDamage = 20; 
	DamageParams.MinimumDamage = 5;
	DamageParams.DamageFalloff = 1.0;
	DamageParams.InnerRadius = 30.0f;
	DamageParams.OuterRadius = 100.0f; 
 
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	Momentum = 10000.0f; 
	bInteractWithProjectile = false; 
}

void  ABFProjectile::Init(FVector Direction)
{

}

void ABFProjectile::BeginPlay()
{
	if (IsPendingKillPending())
	{
		// engine bug that we need to do this
		return;
	}
	Super::BeginPlay();

	bHasSpawnedFully = true;
	 
	if (LaunchSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	} 
	SetActorEnableCollision(true); 
}
void  ABFProjectile::EnableCollision()
{
	SetActorEnableCollision(true);
}
 void ABFProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}
 
void  ABFProjectile::OnStop(const FHitResult& Hit)
{

}
 
 
void  ABFProjectile::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if ( ImpactResult.Actor != NULL	)
	{ 

	}
}
 
 
bool ABFProjectile::HurtRadius(UObject* WorldContextObject, float BaseDamage, float MinimumDamage, float BaseMomentumMag, const FVector& Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff,
	TSubclassOf<UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, AActor* DamageCauser, AController* InstigatedByController, AController* FFInstigatedBy, 	float CollisionFreeRadius, const TArray<FVector>* AltVisibilityOrigins) 
{
	return UGameplayStatics::ApplyRadialDamage(WorldContextObject, BaseDamage, Origin, DamageOuterRadius, DamageTypeClass, IgnoreActors, DamageCauser, InstigatedByController); 

}

void ABFProjectile::Explode(const FHitResult& HitInfo, const FVector& Location, UPrimitiveComponent* HitComp)
{
	if (!bExploded)
	{

		bExploded = true;
		float AdjustedMomentum = Momentum;
		FRadialDamageParams AdjustedDamageParams = GetDamageParams(NULL, HitInfo.ImpactPoint, AdjustedMomentum);

 		if (AdjustedDamageParams.OuterRadius > 0.0f && bInflictRadialDamage)
		{
			TArray<AActor*> IgnoreActors;
			if (ImpactedActor != NULL)
			{
				IgnoreActors.Add(ImpactedActor);
			}
			 
			TArray<FVector> AltOrigins;
      		HurtRadius( this , AdjustedDamageParams.BaseDamage , AdjustedDamageParams.MinimumDamage , AdjustedMomentum , HitInfo.ImpactPoint + HitInfo.Normal ,
				AdjustedDamageParams.InnerRadius , AdjustedDamageParams.OuterRadius , AdjustedDamageParams.DamageFalloff ,
				DamageType, IgnoreActors , InstigatorController->GetPawn() , InstigatorController , NULL , 0.0f ,  &AltOrigins ); 
		} 

		if (ImpactEffectClass != NULL)
		{
			FTransform SpawnTransform;
			SpawnTransform.SetTranslation(Location);
			ABFImpactEffect* Effect = Cast<ABFImpactEffect>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ImpactEffectClass, SpawnTransform));
			Effect->SurfaceHit = HitInfo;
			UGameplayStatics::FinishSpawningActor(Effect, SpawnTransform);
		}

		OnExploded();

		ShutDown();

	}
}

void ABFProjectile::ShutDown()
{

	if (!IsPendingKillPending())
	{
		SetActorEnableCollision(false);
		ProjectileMovement->SetActive(false);
 
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

 

void ABFProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  	if (!bInOverlap)
	{
		TGuardValue<bool> OverlapGuard(bInOverlap, true);

		UE_LOG(BFLOG, Verbose, TEXT("%s::OnOverlapBegin OtherActor:%s bFromSweep:%d"), *GetName(), OtherActor ? *OtherActor->GetName() : TEXT("NULL"), int32(bFromSweep));

		FHitResult Hit;
		float Radius = CollisionComp->GetScaledSphereRadius();
		
		if (bFromSweep)
		{
			Hit = SweepResult;
		}
		else if (CollisionComp != NULL)
		{
			USphereComponent* TestComp = CollisionComp ;
			OtherComp->SweepComponent( Hit , GetActorLocation() - GetVelocity() * Radius , GetActorLocation() + GetVelocity() , FQuat::Identity , TestComp->GetCollisionShape() , TestComp->bTraceComplexOnMove );
		}
		else   
		{
			OtherComp->LineTraceComponent( Hit , GetActorLocation() - GetVelocity() * Radius , GetActorLocation() + GetVelocity() , FCollisionQueryParams(GetClass()->GetFName(), false, this) );
		}
		 
		ProcessHit(OtherActor, OtherComp, Hit, Hit.ImpactPoint, Hit.ImpactNormal);
		 
	 
		
	}
}

bool ABFProjectile::ShouldIgnoreHit(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{   
	ABFProjectile* OtherProj = Cast<ABFProjectile>(OtherActor);
	
	return ( ( ( Cast<ANavModifierVolume>( OtherActor ) != NULL   ) && !GetVelocity().IsZero() )
			|| ( OtherProj != NULL && !InteractsWithProj( OtherProj ) ) 
			|| (Cast<ABFWeapon>( OtherActor ) != NULL )
			|| (Cast<ANavMeshBoundsVolume>(OtherActor) != NULL ) );
}

bool ABFProjectile::InteractsWithProj(ABFProjectile* OtherProj)
{ 
	 
	return bInteractWithProjectile;
}

void ABFProjectile::ProcessHit(  AActor* OtherActor , UPrimitiveComponent* OtherComp , const FHitResult& HitInfo , const FVector& HitLocation , const FVector& HitNormal )
{
 	if (OtherActor != this && (OtherActor != Instigator || Instigator == NULL || bCanHitInstigator) && OtherComp != NULL && !bExploded && (Role == ROLE_Authority || bHasSpawnedFully))
	{
		if ( ShouldIgnoreHit(OtherActor, OtherComp) )
		{ 
			if ( ( Role != ROLE_Authority ) && OtherActor && OtherActor->bTearOff )
			{
				DamageImpactedActor( OtherActor , OtherComp , HitLocation , HitNormal );
			}
		}
		else
		{

			if (!bCanHitTeammates && Instigator != OtherActor)
			{
				ABFGameState* GS = GetWorld()->GetGameState<ABFGameState>();

				APawn* const Pawn = Cast<APawn>(OtherActor);

				if ( GS != nullptr  && Pawn != NULL )
				{
					if (GS->OnSameTeam(Pawn->GetController(), this->GetInstigatorController()))
					{
						return;
					}
				}
			}

			if (OtherActor != NULL)
			{
				DamageImpactedActor( OtherActor , OtherComp , HitInfo.ImpactPoint, HitInfo.Normal );
				 
			}

			ImpactedActor = OtherActor;
			if (HitInfo.IsValidBlockingHit())
			{
				Explode( HitInfo , HitLocation , OtherComp );
			}
			else
			{
				Explode( HitInfo , GetActorLocation() , OtherComp );
			}

			ImpactedActor = NULL; 

		} 
	}
	
}

void ABFProjectile::DamageImpactedActor(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FVector& HitLocation, const FVector& HitNormal)
{

	AController* ResolvedInstigator = InstigatorController;
	TSubclassOf<UDamageType> ResolvedDamageType = DamageType;
  
	float AdjustedMomentum = Momentum;
	if ( (AdjustedMomentum == 0.f ) && Cast<APawn>( OtherActor ) )
		AdjustedMomentum = 20000.f;

 	if (DamageParams.OuterRadius > 0.0f && bInflictRadialDamage)
	{
		FVector ShotDirection = GetVelocity().GetSafeNormal();
		if (ShotDirection.IsZero())
		{
			ShotDirection = OtherActor->GetActorLocation() - HitLocation;
			ShotDirection = ShotDirection.GetSafeNormal();
		}

  		FBFRadialDamageEvent Event;
		
		Event.Params						= GetDamageParams(OtherActor, HitLocation, Event.BaseMomentumMag) ;
		Event.Params.MinimumDamage			= Event.Params.BaseDamage ; // force full damage for direct hit
		Event.DamageTypeClass				= ResolvedDamageType ;
		Event.Origin						= HitLocation		 ;
		new(Event.ComponentHits) FHitResult( OtherActor , OtherComp , HitLocation , HitNormal ) ; 
		Event.ComponentHits[0].TraceStart	= HitLocation - ShotDirection ;
		Event.ComponentHits[0].TraceEnd		= HitLocation + ShotDirection ;
		Event.ShotDirection					= ShotDirection		;
		Event.BaseMomentumMag				= AdjustedMomentum  ; 

		OtherActor->TakeDamage( Event.Params.BaseDamage , Event , ResolvedInstigator , this ) ;
	}
	else
	{
		FBFPointDamageEvent Event; 
		Event.Damage = GetDamageParams( OtherActor , HitLocation , AdjustedMomentum ).BaseDamage;
		Event.DamageTypeClass = ResolvedDamageType ;
		Event.HitInfo = FHitResult( OtherActor , OtherComp , HitLocation , HitNormal );
		Event.ShotDirection = GetVelocity().GetSafeNormal();
 		Event.Momentum = Event.ShotDirection * AdjustedMomentum;
		OtherActor->TakeDamage( Event.Damage , Event , ResolvedInstigator , this );
	}

}

FRadialDamageParams ABFProjectile::GetDamageParams_Implementation(AActor* OtherActor, const FVector& HitLocation, float& OutMomentum) const
{
	OutMomentum = Momentum ;
	return DamageParams ;
}

 
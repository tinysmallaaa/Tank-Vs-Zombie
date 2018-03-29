#include "TankVsZombie.h"
#include "BFBotAIController.h"
#include "Gamemodes/BFGameModeBase.h" 
#include "BFAIBot.h"


ABFAIBot::ABFAIBot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	 

	HeadCollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, "HeadComp");
	HeadCollisionComponent->SetSphereRadius(10.0f);
	HeadCollisionComponent->AttachToComponent( GetMesh() , FAttachmentTransformRules(EAttachmentRule::KeepRelative, false) , "HeadSocket");
	 

	Health = 100;
	HealthMax = 100;
	HealthBarDrawOffsetY = 20.0f;
	RagdollLifeSpan = 5.0f;
	RagdollGravityScale = 1.0f;
	bDetachWeaponInRagdollMode = true;
	 
	bAllowMobility = true;
	PrimaryActorTick.bCanEverTick = true; 

	bAllowCrouch = false;

	DamageMultiplier = 1.0f;
	bRagdollOnDeath = true; 
	bIsDead = false;
	bDrawHealthBar = false;

}

void ABFAIBot::BeginPlay()
{
	Super::BeginPlay(); 
	InitializeWeapon();

	if ( !bAllowMobility )
	{
		GetCharacterMovement()->GravityScale = 0.0f;  
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}
}

void ABFAIBot::InitializeWeapon()
{ 

	if (WeaponClass != NULL)
	{
		FTransform Transform;
		AttachedWeapon = Cast<ABFWeapon>( UGameplayStatics::BeginDeferredActorSpawnFromClass( this , WeaponClass , Transform ) ) ;
		AttachedWeapon->WeaponOwner = this;
		AttachedWeapon->AttachToOwner(GetMesh());
		UGameplayStatics::FinishSpawningActor(AttachedWeapon, Transform);
	}

}

void ABFAIBot::Tick(float DeltaSeconds)
{

	Super::Tick(DeltaSeconds);  
 

}

 
void ABFAIBot::SetTarget(AActor* NewTarget)
{

	CurrentTarget = NewTarget;

}

FVector ABFAIBot::GetFireDirection() const
{

	if ( IsValid(CurrentTarget) )
	{ 
			return FVector(CurrentTarget->GetActorLocation() - GetFireLocation()).GetSafeNormal();
	}

	return GetActorForwardVector();

}

FVector  ABFAIBot::GetFireLocation() const
{

	if(GetMesh()->GetSocketByName("FireLocation"))
	return GetMesh()->GetSocketLocation("FireLocation"); 

	return GetActorLocation();

}

 
bool ABFAIBot::IsRagdoll() const
{

	return (RootComponent == GetMesh() && GetMesh()->IsSimulatingPhysics());

}

void ABFAIBot::Fire()
{ 

	if (IsSimulatingRagdoll())return;

	
	if (AttachedWeapon != NULL && CurrentTarget !=NULL )
	{
		if (AttachedWeapon->FireWeapon(CurrentTarget, CurrentTarget->GetActorLocation()))
		{
			UAnimMontage* Montage = FireMontage;
			if ( FireCrouchMontage != NULL )
			{
				if (FMath::RandBool())
				{
					Montage = FireCrouchMontage;
				}
			}

			if (Montage != NULL && GetCurrentMontage() != Montage)
			{
				PlayAnimMontage(Montage);
			}
		}

	}else if ( CanFire() && ProjectileClass != NULL )
	{   
		FVector FireLocation = GetFireLocation();
		FVector FireDirection = GetFireDirection();
  
		LastFireTime = GetWorld()->GetTimeSeconds();  

		FireProjectile(FireLocation, FireDirection);

		if (FireMontage != NULL && GetCurrentMontage() != FireMontage)
		{
			PlayAnimMontage(FireMontage);
		}
	}

}
bool ABFAIBot::CanFire()
{

	if (IsSimulatingRagdoll())return false;

	float CurrenTime = GetWorld()->GetTimeSeconds();

	if (CurrenTime - LastFireTime > FireDelay)
	{
		return true ;
	}

	return false ;

}
void ABFAIBot::FireProjectile(FVector FireLocation, FVector FireDirection)
{

	if (ProjectileClass != NULL)
	{
		FTransform Transform;
		Transform.SetTranslation(FireLocation);

		ABFProjectile* Projectile = Cast<ABFProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, Transform));
		
		Projectile->ProjectileMovement->Velocity = FireDirection;
		Projectile->DamageParams.BaseDamage *= DamageMultiplier;
		
		UGameplayStatics::FinishSpawningActor(this, Transform);
	}

}

void ABFAIBot::Reload()
{

}

void  ABFAIBot::PossessedBy(AController* AIController)
{

	Super::PossessedBy(AIController);  
	
	BotAIController = Cast<ABFBotAIController>(AIController);

	if ( BotAIController != NULL )
	{ 
		ABFGameModeBase* GameMode = Cast<ABFGameModeBase>(GetWorld()->GetAuthGameMode());
		if(GameMode!=NULL)
		GameMode->InitController(BotAIController);
	}

}

void ABFAIBot::UpdateTargeting(float DeltaSeconds)
{

	if (IsValid(CurrentTarget) && !IsDead() && !bInRagdoll)
	{
		FVector TargetLocation = CurrentTarget->GetTargetLocation(this);

		FVector Dir = TargetLocation - GetActorLocation();		
		FRotator TargetRotation = Dir.ToOrientationRotator();
		FRotator CurRotation = GetActorRotation();

		float Yaw = FMath::FInterpTo(CurRotation.Yaw, TargetRotation.Yaw, DeltaSeconds, 2.f);
		CurRotation.Yaw = Yaw;

		SetActorRotation(CurRotation);
	}

}

void ABFAIBot::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{

	OutLocation = GetActorLocation();
	OutRotation = GetActorRotation();

}

FVector ABFAIBot::GetTargetLocation(AActor* RequestedBy) const
{ 
	FVector Location = GetActorLocation();
	
	Location.Z += ( GetCapsuleComponent()->GetScaledCapsuleHalfHeight() ) ;

	return Location; 
}

float ABFAIBot::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{ 
 	if (DamageCauser == this )return 0.0f;

	if (Health < 0.0f || IsSimulatingRagdoll()) return 0.0f;

	FHitResult Hit;
	FVector DamageOrigin = FVector::ZeroVector;
	float ActualDamage = Damage; 
	UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	
 
	FBotDamageParamEx BotDamageParams;
	BotDamageParams.bHeadShot		  = false ;
	BotDamageParams.bRoadKill		  = false ;
	BotDamageParams.bVehicleSmashed	  = false ;
	BotDamageParams.DamageType = DamageEvent.DamageTypeClass ;

	if (DamageEvent.IsOfType(FBFPointDamageEvent::ClassID))
	{   
		FBFPointDamageEvent* const PointDamageEvent = (FBFPointDamageEvent*)&DamageEvent;
		ActualDamage = InternalTakePointDamage(ActualDamage, *PointDamageEvent, EventInstigator, DamageCauser);
		DamageOrigin = PointDamageEvent->HitInfo.ImpactPoint;
		Hit = PointDamageEvent->HitInfo;  

		if (Hit.Component != NULL)
		{
			BotDamageParams.bHeadShot = Hit.Component == HeadCollisionComponent;
			BotDamageParams.HeadBoneLocation = HeadCollisionComponent->GetComponentLocation();
		}
 
	}
	else if (DamageEvent.IsOfType(FBFRadialDamageEvent::ClassID))
	{  
		FBFRadialDamageEvent* const RadialDamageEvent = (FBFRadialDamageEvent*)&DamageEvent;
		ActualDamage = InternalTakeRadialDamage(ActualDamage, *RadialDamageEvent, EventInstigator, DamageCauser);
		DamageOrigin = RadialDamageEvent->Origin;  
		if(RadialDamageEvent->ComponentHits.Num()>0)
		Hit = RadialDamageEvent->ComponentHits[0];
	}


	Health -= ModifyDamageTaken( ActualDamage , BotDamageParams );
	DamageReceived();
	float Momentum = 0.0f;
	FVector MommentumVec  = FVector(0.0f);


	if (Health <= 0.0f)
	{
		

		if (DamageEvent.IsOfType(FBFPointDamageEvent::ClassID))
		{
			FBFPointDamageEvent* const PointDamageEvent = (FBFPointDamageEvent*)&DamageEvent;
			MommentumVec = PointDamageEvent->Momentum;

		}
		else if (DamageEvent.IsOfType(FBFRadialDamageEvent::ClassID))
		{			 
			FBFRadialDamageEvent* const RadialDamageEvent = (FBFRadialDamageEvent*)&DamageEvent;
			MommentumVec = RadialDamageEvent->BaseMomentumMag * RadialDamageEvent->ShotDirection;		
		}  
 
		ABFGameModeBase* GameMode = Cast<ABFGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode != NULL)
			GameMode->ScoreKill(BotAIController, EventInstigator, BotDamageParams );


		SavedLastDamageOrigin = DamageOrigin;
		SavedLastMomentumVec  = MommentumVec;
		bIsDead = true;
		Kill( MommentumVec , BotDamageParams );
		
	}
	return Damage;
}

float ABFAIBot::ModifyDamageTaken(float Damage, FBotDamageParamEx DamageParams)
{
	return Damage;
}

void ABFAIBot::OnKilled_Implementation(bool WasHeadShot)
{

}

void ABFAIBot::Kill( FVector MomentumVec , const FBotDamageParamEx& DamageParams)
{

	DestroyBot();   
}

void ABFAIBot::AllowWeaponFiring(bool Allowance)
{
	bFiringAllowed = Allowance;
}

bool ABFAIBot::IsAlive()
{
	return !bIsDead;
}

void  ABFAIBot::StartRagdoll(FVector Momentum)
{
	if (IsActorBeingDestroyed() || !GetMesh()->IsRegistered())
	{
		return;
	}

	// force standing
	 GetCharacterMovement()->UnCrouch(true);
	if (RootComponent == GetMesh() && GetMesh()->IsSimulatingPhysics())
	{
 		return;
	} 
	if (!bStartingRagdoll)
	{
		
 		TGuardValue<bool> RagdollGuard(bStartingRagdoll, true); 

		SetActorEnableCollision(true); 
		bInRagdoll = true;

		AllowWeaponFiring(false);

		if ( GetMesh() != NULL &&  !GetMesh()->ShouldTickPose() )
		{
			//GetMesh()->RefreshBoneTransforms()  ;
		//	GetMesh()->UpdateComponentToWorld() ; 
		}

		GetCharacterMovement()->ApplyAccumulatedForces(0.0f);

 		if (GetMesh()->GetBodyInstance() != nullptr)
		{
			GetMesh()->GetBodyInstance()->bUseCCD = true;
		}

		GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetAllBodiesNotifyRigidBodyCollision(true);  
		GetMesh()->UpdateKinematicBonesToAnim(GetMesh()->GetComponentSpaceTransforms(), ETeleportType::TeleportPhysics, true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->RefreshBoneTransforms();
		GetMesh()->SetAllBodiesPhysicsBlendWeight(1.0f);
		GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		RootComponent = GetMesh();
		GetMesh()->bGenerateOverlapEvents = true;
		GetMesh()->bShouldUpdatePhysicsVolume = true;
		GetMesh()->RegisterClothTick(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		GetCapsuleComponent()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
		GetMesh()->SetAllPhysicsLinearVelocity(Momentum, false);
		
		GetCharacterMovement()->StopActiveMovement();
		GetCharacterMovement()->Velocity = FVector::ZeroVector;		 

		// set up the custom physics override, if necessary
		SetRagdollGravityScale(RagdollGravityScale);

		if (bDetachWeaponInRagdollMode)
		{
			DetachWeapon(true);
		}

		GetWorldTimerManager().SetTimer( DeathTimer , this , &ABFAIBot::DestroyBot , RagdollLifeSpan , false );
	}
}

void ABFAIBot::DestroyBot()
{
   	if ( BotAIController )
	{
		BotAIController->UnPossess();
		BotAIController->ShutdownAI();
		BotAIController->Destroy(false, true); 
		BotAIController = NULL;
	}
	
	DetachWeapon(true);
	 
	EndEncounterIfAny();

	Destroy(false, true);  
}

void ABFAIBot::DetachWeapon(bool ForceDestroy)
{
	if (AttachedWeapon != NULL)
	{
		AttachedWeapon->DetachFromOwner();
		
		if (ForceDestroy)
		{
			AttachedWeapon->Destroy(false,true);
			AttachedWeapon = NULL;
		}
	}
}

void ABFAIBot::SetRagdollGravityScale(float NewScale)
{
	for (FBodyInstance* Body : GetMesh()->Bodies)
	{
		if (Body != NULL)
		{
			Body->SetEnableGravity(NewScale != 0.0f);
		}
	}

	RagdollGravityScale = NewScale;
}

void ABFAIBot::SetRandomRVO(int32 MaxMask)
{

	if (GetCharacterMovement() != NULL)
	{
		//	FNavAvoidanceMask Mask;
		//	Mask.SetGroup( FMath::RandRange(1,MaxMask) );
		//	GetCharacterMovement()->SetAvoidanceGroupMask(Mask);
	}

}

void ABFAIBot::EndEncounterIfAny()
{
	 
}
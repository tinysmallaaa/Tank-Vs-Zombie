#include "TankVsZombie.h"
#include "BFWeaponAttachment.h"
#include "AI/BFAIBot.h"
#include "BFWeapon.h"

ABFWeapon::ABFWeapon(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Mesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, FName("Mesh"));
	RootComponent = Mesh; 

	FiringSoundComp = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, FName("FiringSound"));
	FiringSoundComp->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	FiringDelay = 1.0f;
	FireOffset	= 30.0f;

	DamageMultiplier = 1.0f;
	InstantHitDamage = 20.0f;

	AmmoCount		 = 100;
	AmmoClipSize	 = 100;
	AmmoCap			 = -1;
	FireRate		 = 5;
	FireDelay		 = 1.0f / FireRate;
	AimSpread		 = 0.1f;
	DamageMultiplier = 1.0f;

	ClipReloadTime		= 3.0f;
	InstantHitImpulse	= 2000.0f;

	MinAimSpread		= 0.0f;
	MaxAimSpread		= 0.4f;
	bWeaponInitialized  = false;
}

void ABFWeapon::BeginPlay()
{
	Super::BeginPlay();	
}

void ABFWeapon::InitWeapon(FSavedVehicleWeaponState& WeaponState)
{ 
	if (!bWeaponInitialized)
	{
		bWeaponInitialized = true;
		AmmoCap = -1;
		bInfiniteAmmo = AmmoCap == -1;
		
		AmmoCount = AmmoClipSize;

		DamageMultiplier = WeaponState.Power;
		ClipReloadTime = WeaponState.ReloadTime;
		AimSpread = WeaponState.Accuracy;
		FireRate = WeaponState.FireRate;
		FireDelay = 1.0f / FireRate;
		MinAimSpread = 0.0f;
		MaxAimSpread = 0.4f;

		bClipReloading = false;
		bHasPendingReload = false;
	}
}

void ABFWeapon::Setup(FSavedVehicleWeaponState& WeaponState)
{   
	InitWeapon(WeaponState);
}

void ABFWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

APawn*  ABFWeapon::GetOwnerPawn()
{
	return Cast<APawn>(WeaponOwner);
}

AActor* ABFWeapon::GetOwner()
{
	return WeaponOwner;
}

FVector ABFWeapon::CalculateFireDirection( FVector FireLocation , FVector FireDirection )
{

	float Spread = (MinAimSpread + (MaxAimSpread - MinAimSpread)*(1.0f - AimSpread));

	FVector EndLocation = FireLocation + FireDirection*20000.0f;
	FHitResult OutHit;

	FCollisionObjectQueryParams QueryParams ;
	QueryParams.AddObjectTypesToQuery(ECC_Pawn);
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	QueryParams.AddObjectTypesToQuery(ECC_Visibility);
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	FCollisionQueryParams Params ;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(WeaponOwner);
	  
	FVector HitLocation = EndLocation ;
	bool bHadValidHit = false ; 
 
	if (GetWorld()->LineTraceSingleByObjectType(OutHit, FireLocation, EndLocation, QueryParams, Params))
	{
		HitLocation = OutHit.Location ;   
		
		FVector RightVector = FireDirection.CrossProduct(FireDirection, FVector::UpVector).GetSafeNormal() ;
		FVector UpVector	= RightVector.CrossProduct(FireDirection, RightVector).GetSafeNormal() ;

		float Radius		= 50.0f*Spread ;

		HitLocation			= FMath::RandPointInBox(FBox(HitLocation - FVector(Radius, Radius, Radius), HitLocation + FVector(50.0f, 50.0f, 50.0f))) ; 
		 
		return (HitLocation - FireLocation).GetSafeNormal() ;
	}
	else
	{

		FRotator Aim		= FireDirection.ToOrientationRotator(); 
	
		FRotationMatrix Mat( Aim ) ;
		FVector X , Y , Z ;
		Mat.GetScaledAxes( X , Y , Z ) ;

		float RandY		=	0.5f * (FMath::FRand() + FMath::FRand() - 1.f) ;
		float RandZ		=	FMath::Sqrt(0.25f - FMath::Square(RandY)) * (FMath::FRand() + FMath::FRand() - 1.f) ;
		return (X + RandY * Spread * Y + RandZ * Spread * Z).GetSafeNormal() ;
	}

}

void ABFWeapon::PlayFiringSoundFX_Implementation()
{
	if (FiringSoundComp != NULL)
	{
		if (!FiringSoundComp->IsPlaying())
		{
			FiringSoundComp->Play();
		}
	}
}

FVector ABFWeapon::CalculateFireLocation(FVector Direction)
{
	return GetFireLocation() + Direction * FireOffset;
}

bool ABFWeapon::FireWeapon(const FVector Direction)
{
	bool Success = false;

	LastFireTime = GetWorld()->GetTimeSeconds(); 

	FVector FireLocation = CalculateFireLocation(Direction);

	if (ProjectileClass != NULL)
	{ 
		FireProjectile( FireLocation , CalculateFireDirection( FireLocation , Direction ) );
	}
	else
	{
		FireInstantHit( FireLocation , CalculateFireDirection( FireLocation , Direction ) );
	}

	return Success;
}

bool ABFWeapon::FireWeapon(AActor* Target, FVector TargetLocation)
{
	bool Success = false;

	if (IsValid(Target))
	{
		TargetLocation = Target->GetTargetLocation();
	}

	FVector FireDirection = TargetLocation - GetFireLocation();

	return FireWeapon(FireDirection);
}

void ABFWeapon::PauseFire()
{

	StopFiringFX();
	GetWorldTimerManager().ClearTimer(NextFireTimer);

}

void ABFWeapon::ResumeFire()
{

	if (bFirePressed)
	{
		StartFiringFX();
		Fire();
	}

}

void ABFWeapon::ReloadClip()
{

	if (bClipReloading) return;

	if (AmmoCount == AmmoClipSize) return;

	PauseFire();

	bClipReloading = true;

	bHasPendingReload = false;

	ABFTankPlayerController* PC = Cast<ABFTankPlayerController>(GetOwnerPawn()->GetController());

	if (PC != NULL)
	{
		PC->OnClipReloadBegin(EWeaponMode::EWEAPONMODE_MOUNTEDGUN);
	}

	GetWorldTimerManager().SetTimer(ClipReloadingTimer, this, &ABFWeapon::ClipReloaded, ClipReloadTime, false);

}

void ABFWeapon::ClipReloaded()
{

	bClipReloading = false;

	int ClipSize = AmmoClipSize;

	if (!bInfiniteAmmo)
	{
		if (AmmoCap - ClipSize < 0)
		{
			ClipSize = AmmoCap;
		}

		int CalculatedAmmo = AmmoCount + ClipSize;

		if (CalculatedAmmo > AmmoClipSize)
		{
			ClipSize = AmmoClipSize - AmmoCount;
		}
	}
	else
	{
		ClipSize = AmmoClipSize - AmmoCount;
		AmmoCap -= ClipSize;
	}

	AmmoCount += ClipSize;

	if (ClipSize > 0)
	{
		if (WeaponOwner != NULL && GetOwnerPawn()!=NULL)
		{
			ABFTankPlayerController* PC = Cast<ABFTankPlayerController>(GetOwnerPawn()->GetController());
			if (PC != NULL)
			{
				PC->ReportWeaponReloaded(this);
			}
		}

	}

	ResumeFire();

}

void ABFWeapon::CancelReload()
{

	if (bClipReloading) {
		bHasPendingReload = true;
	}
	else {
		bHasPendingReload = false;
	}

	bClipReloading = false;

	GetWorldTimerManager().ClearTimer(ClipReloadingTimer);

}

void ABFWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}


void ABFWeapon::StopFire()
{

	bFirePressed = false;
	GetWorldTimerManager().ClearTimer(NextFireTimer);
	StopFiringFX();

	if (bClipReloading)
	{
		ClipReloaded();
		GetWorldTimerManager().ClearTimer(ClipReloadingTimer);
	}

}

void ABFWeapon::FirePressed(bool Pressed)
{

	bFirePressed = Pressed;
	if (Pressed)
	{
		StartFiringFX() ;
		Fire() ;
	}
	else
	{
		StopFire() ;
	}

}

void ABFWeapon::Fire()
{

	if (CanFire())
	{	
		int32 ShotsFired = 1;
	
		
		AmmoCount -= ShotsFired;

		FireWeapon(GetFireDirection());

		if ( AmmoCount <= 0 )
		{
			AmmoCount = 0 ;
			ReloadClip() ;
		}
		else
		{
			ABFTankPlayerController* PC = Cast<ABFTankPlayerController>( GetOwnerPawn()->GetController() );
			if (PC != NULL)
			{
				PC->OnAmmoUpdated(AmmoCount, AmmoClipSize);
			}
		}

		if (!bClipReloading)
		{
			GetWorldTimerManager().SetTimer( NextFireTimer , this , &ABFWeapon::Fire , 1.0f/FireRate , false );
		}
	}

}

bool ABFWeapon::HasEnoughAmmo()
{ 

	if ( AmmoCount > 0 && ( bInfiniteAmmo || AmmoCap > 0 ) )
		return true ;

	return false ; 

}

bool ABFWeapon::CanFire()
{

	if ( bFirePressed &&  HasEnoughAmmo() && !bClipReloading )
	{
		return true;
	}

	return false;

}

void ABFWeapon::FireProjectile(const FVector FireLocation, const FVector Direction)
{

	if (ProjectileClass != NULL)
	{
		FVector LaunchPos = FireLocation + Direction * FireOffset;
		FTransform SpawnTransform = FTransform::Identity;
		SpawnTransform.SetTranslation(LaunchPos);

		ABFProjectile* Projectile = Cast<ABFProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTransform));
		Projectile->InstigatorController = WeaponOwner->GetInstigatorController();
		Projectile->ProjectileMovement->Velocity = Direction;
		Projectile->Instigator = GetOwnerPawn();
		Projectile->DamageParams.BaseDamage *= DamageMultiplier;
		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);

		PostFireEffects();
	}

}
 

void ABFWeapon::FireInstantHit(const FVector FireLocation, const FVector Direction)
{

	FVector EndLocation = FireLocation + Direction*20000.0f;
	FTransform SpawnTransform;
	SpawnTransform.SetTranslation(FireLocation);

	

	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery( ECC_Pawn );
	QueryParams.AddObjectTypesToQuery( ECC_WorldStatic  );
	QueryParams.AddObjectTypesToQuery( ECC_Visibility	); 
	QueryParams.AddObjectTypesToQuery( ECC_WorldDynamic ) ; 
	QueryParams.AddObjectTypesToQuery( ECC_PhysicsBody  );
	QueryParams.AddObjectTypesToQuery( ECC_Destructible );

	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor( this );
	Params.AddIgnoredActor(WeaponOwner);
	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;
	
	 
	FVector HitLocation = EndLocation; 
	FHitResult OutHit;
	bool bHadValidHit = false;

	FCollisionShape SweepShape;
	SweepShape.SetSphere(30.0f);
	if ( GetWorld()->SweepSingleByChannel( OutHit , FireLocation , EndLocation , FQuat::Identity , COLLISION_TRACE_WEAPON , SweepShape , Params ) )
	{
		HitLocation = OutHit.ImpactPoint;
		bHadValidHit = true ;
	} 

	if (InstantFireFX != NULL)
	{
		SpawnInstantBeamFX(FireLocation, HitLocation) ; 
	}

	if (bHadValidHit && InstantHitImpactFXClass != NULL)
	{
		SpawnTransform = FTransform::Identity	;
		SpawnTransform.SetTranslation(HitLocation) ;

		ABFImpactEffect* ImpactHit = Cast<ABFImpactEffect>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), InstantHitImpactFXClass, SpawnTransform));
		ImpactHit->SurfaceHit = OutHit ;
		UGameplayStatics::FinishSpawningActor( ImpactHit , SpawnTransform ) ;
	}

	AActor* HitActor = OutHit.GetActor() ;

	if (OutHit.IsValidBlockingHit() )
	{ 

		float DamageAmount = InstantHitDamage * DamageMultiplier ;
		AController* InstigatorController = NULL ;

		if (WeaponOwner != NULL)
		{
			InstigatorController = GetOwnerPawn()->GetController();
		}

		if (InstantProjectileClass != NULL)
		{
			SpawnTransform = FTransform::Identity;
			SpawnTransform.SetTranslation(HitLocation);
			ABFProjectile* Projectile = Cast<ABFProjectile>( UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), InstantProjectileClass, SpawnTransform) );
			
				Projectile->DamageParams.BaseDamage		= DamageAmount ;
				Projectile->DamageParams.MinimumDamage	= DamageAmount ;  
				Projectile->InstigatorController = InstigatorController ;
				Projectile->Instigator = GetOwnerPawn();
				Projectile->SetLifeSpan(1.0f) ;

			UGameplayStatics::FinishSpawningActor( Projectile , SpawnTransform );

		} else {  

			APawn* Pawn = Cast<APawn>(HitActor);
			if (Pawn != NULL)
			{
				if (Cast<ABFAIBot>(Pawn) != NULL)
				{
				}
			}


			FBFPointDamageEvent	DamageEvent;
			DamageEvent.Damage = InstantHitDamage * DamageMultiplier;
			DamageEvent.HitInfo = OutHit;
			DamageEvent.ShotDirection = Direction;
			DamageEvent.Momentum = InstantHitImpulse *DamageMultiplier * Direction;

			HitActor->TakeDamage(DamageAmount, DamageEvent, InstigatorController, WeaponOwner);

		}
	} 

}

void ABFWeapon::StartFiringFX_Implementation()
{

}

void ABFWeapon::StopFiringFX_Implementation()
{

}

void ABFWeapon::AttachToOwner(USkeletalMeshComponent* ParentMesh)
{

	if (WeaponAttachment == NULL)
	{
		WeaponAttachment = NewObject<UBFWeaponAttachment>(this, WeaponAttachmentClass);
	}

	if (this->GetRootComponent()->GetAttachParent() != NULL)
	{
		GetRootComponent()->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	}

	RootComponent->AttachToComponent(ParentMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), WeaponAttachment->PrimarySocketName);
	RootComponent->SetRelativeLocation(WeaponAttachment->PrimarySocketLocation);
	RootComponent->SetRelativeRotation(WeaponAttachment->PrimarySocketRotation);

}

void ABFWeapon::DetachFromOwner()
{

	if (this->GetRootComponent()->GetAttachParent() != NULL)
	{
		GetRootComponent()->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	}

}

FVector ABFWeapon::GetFireDirection_Implementation()
{
 
	if (WeaponOwner != NULL)
	{
		ABFTankPlayerController* PC = Cast<ABFTankPlayerController>(GetOwnerPawn()->GetController());

		if (PC != NULL && PC->GetViewTarget() != NULL)
		{
			 return (PC->GetTracedPoint() - Mesh->GetSocketLocation(FireLocationSocket)).GetSafeNormal();

		}
	}

	return Mesh->GetSocketLocation(FireLocationSocket) - Mesh->GetSocketLocation("Root");

}

FVector ABFWeapon::GetFireLocation_Implementation()
{

	if (Mesh->GetSocketByName(FireLocationSocket))
		return Mesh->GetSocketLocation(FireLocationSocket);

	return GetActorLocation();

}


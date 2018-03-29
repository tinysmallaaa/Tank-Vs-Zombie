#include "TankVsZombie.h"
#include "BFMiniTankBotAIController.h"
#include "BFMiniTankBot.h"

ABFMiniTankBot::ABFMiniTankBot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void ABFMiniTankBot::BeginPlay()
{
	Super::BeginPlay();

}

void ABFMiniTankBot::ReleaseOnLocation(FVector Location)
{
	SetActorLocation(Location);
}

void ABFMiniTankBot::AdjustOnDrag( FVector NewLocation , FVector ImpactNormal ,  bool DragOverEvent )
{
	FVector Location = GetActorLocation();
	Location.Z += Height ;
	SetActorLocation(Location);
	if (DragOverEvent)
		OnPlaced(Location);
}

void ABFMiniTankBot::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABFMiniTankBot::PossessedBy(AController* AIController)
{
	Super::PossessedBy(AIController);
	BotAIController = Cast<ABFMiniTankBotAIController>(AIController);
}

void ABFMiniTankBot::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABFMiniTankBot::SetEnableCollision(bool flag)
{
	SetActorEnableCollision(flag);
} 

FVector ABFMiniTankBot::CalculateFireDirection(FVector FireLocation, FVector FireDirection)
{
	 

	FVector EndLocation = FireLocation + FireDirection*20000.0f;
	FHitResult OutHit;

	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_Pawn);
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	QueryParams.AddObjectTypesToQuery(ECC_Visibility);
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetInstigator());

	FVector HitLocation = EndLocation;
	bool bHadValidHit = false;

	if (GetWorld()->LineTraceSingleByObjectType(OutHit, FireLocation, EndLocation, QueryParams, Params))
	{
		HitLocation = OutHit.Location;

		FVector RightVector = FireDirection.CrossProduct(FireDirection, FVector::UpVector).GetSafeNormal();
		FVector UpVector = RightVector.CrossProduct(FireDirection, RightVector).GetSafeNormal();

		float Radius = 50.0f;

		HitLocation = FMath::RandPointInBox(FBox(HitLocation - FVector(Radius, Radius, Radius), HitLocation + FVector(50.0f, 50.0f, 50.0f)));

		
	} 
	return (HitLocation - FireLocation).GetSafeNormal();
} 


void ABFMiniTankBot::PlayFiringSoundFX_Implementation()
{
	if (FiringSoundComp != NULL)
	{
		if (!FiringSoundComp->IsPlaying())
		{
			FiringSoundComp->Play();
		}
	}
}
 
bool ABFMiniTankBot::FireWeapon(const FVector Direction)
{
	bool Success = false;

	LastFireTime = GetWorld()->GetTimeSeconds();

	FVector FireLocation = CalculateFireLocation(Direction);

	if (ProjectileClass != NULL)
	{
		FireProjectile(FireLocation, CalculateFireDirection(FireLocation, Direction));
	}
	else
	{
		FireInstantHit(FireLocation, CalculateFireDirection(FireLocation, Direction));
	}

	return Success;
} 

bool ABFMiniTankBot::FireWeapon(AActor* Target, FVector TargetLocation)
{
	bool Success = false;

	if (IsValid(Target))
	{
		TargetLocation = Target->GetTargetLocation();
	}

	FVector FireDirection = TargetLocation - GetFireLocation();

	return FireWeapon(FireDirection);
} 

void ABFMiniTankBot::StopFire()
{
	bFirePressed = false;
	GetWorldTimerManager().ClearTimer(NextFireTimer);
	StopFiringFX();
 
}
 

void ABFMiniTankBot::UnPossessed()
{
	Super::UnPossessed();
}

 
FVector ABFMiniTankBot::CalculateFireDirection()
{
	return GetActorForwardVector();
}

void ABFMiniTankBot::StartFiringFX_Implementation()
{

}

void ABFMiniTankBot::StopFiringFX_Implementation()
{

}

void ABFMiniTankBot::Fire()
{
	if (CanFire())
	{  
		FireWeapon(BotAIController->GetFireDirection());

		GetWorldTimerManager().SetTimer(NextFireTimer, this, &ABFMiniTankBot::Fire, 1.0f / FireRate, false);
		
	}
} 

bool ABFMiniTankBot::CanFire()
{
	if (  BotAIController != NULL )
	{
		return true;
	}

	return false;
}

void ABFMiniTankBot::FireProjectile(const FVector FireLocation, const FVector Direction)
{
	if (ProjectileClass != NULL)
	{
		FVector LaunchPos = FireLocation + Direction * FireOffset;
		FTransform SpawnTransform = FTransform::Identity;
		SpawnTransform.SetTranslation(LaunchPos);

		ABFProjectile* Projectile = Cast<ABFProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTransform));
		Projectile->InstigatorController = GetInstigator()->GetController();
		Projectile->ProjectileMovement->Velocity = Direction;
		Projectile->Instigator = GetInstigator();
		Projectile->DamageParams.BaseDamage *= DamageMultiplier;
		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);

		PostFireEffects();
	}
} 

void ABFMiniTankBot::FireInstantHit(const FVector FireLocation, const FVector Direction)
{

	FVector EndLocation = FireLocation + Direction*20000.0f;
	FTransform SpawnTransform;
	SpawnTransform.SetTranslation(FireLocation); 

	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_Pawn);
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	QueryParams.AddObjectTypesToQuery(ECC_Visibility);
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	QueryParams.AddObjectTypesToQuery(ECC_Destructible);


	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetInstigator());
	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;


	FVector HitLocation = EndLocation;
	FHitResult OutHit;
	bool bHadValidHit = false;

	FCollisionShape SweepShape;
	SweepShape.SetSphere(30.0f);
	if (GetWorld()->SweepSingleByChannel(OutHit, FireLocation, EndLocation, FQuat::Identity, COLLISION_TRACE_WEAPON, SweepShape, Params))
	{
		HitLocation = OutHit.ImpactPoint;
		bHadValidHit = true;
	}

	if (InstantFireFX != NULL)
	{
		SpawnInstantBeamFX(FireLocation, HitLocation);
	}

	if (bHadValidHit && InstantHitImpactFXClass != NULL)
	{
		SpawnTransform = FTransform::Identity;
		SpawnTransform.SetTranslation(HitLocation);

		ABFImpactEffect* ImpactHit = Cast<ABFImpactEffect>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), InstantHitImpactFXClass, SpawnTransform));
		ImpactHit->SurfaceHit = OutHit;
		UGameplayStatics::FinishSpawningActor(ImpactHit, SpawnTransform);
	}

	AActor* HitActor = OutHit.GetActor();
	float InstantHitDamage = DamagePower;
	if (OutHit.IsValidBlockingHit())
	{

		float DamageAmount = InstantHitDamage * DamageMultiplier;

		AController* InstigatorController = NULL;

		if (GetInstigator() != NULL)
		{
			InstigatorController = GetInstigator()->GetController();
		}
		
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
			DamageEvent.Momentum = DamageMultiplier * Direction;

			HitActor->TakeDamage(DamageAmount, DamageEvent, InstigatorController, GetInstigator());
 
	}
} 

void ABFMiniTankBot::SetupBot(FSavedMiniTankBotState& State)
{

} 

FVector  ABFMiniTankBot::GetFireLocation_Implementation()
{
	FVector Location = GetActorLocation();

	if (FireLocationSocketList.Num())
	{
		if (GetMesh()->GetSocketByName(FireLocationSocketList[FireLocationItr]))
			Location = GetMesh()->GetSocketLocation(FireLocationSocketList[FireLocationItr]);

		FireLocationItr++;

		if (FireLocationItr == FireLocationSocketList.Num())
		{
			FireLocationItr = 0;
		}
	}

	return Location;
}

FVector ABFMiniTankBot::CalculateFireLocation(FVector Direction)
{

	
	return ( GetFireLocation() + Direction * FireOffset );
}

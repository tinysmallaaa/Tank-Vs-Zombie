#include "TankVsZombie.h"
#include "Gamemodes/BFGameModeBase.h"
#include "AI/BFAIController.h" 
#include "BFTankPlayerController.h"
#include "BFTankMovementComponent.h"
#include "BFTankVehicle.h"

// Sets default values
ABFTankVehicle::ABFTankVehicle(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UBFTankMovementComponent>(Super::VehicleMovementComponentName))
{

 	PrimaryActorTick.bCanEverTick = true;
 
	VehicleBody = CreateDefaultSubobject<UStaticMeshComponent>(FName("VehicleBody"));
	VehicleBody->bHiddenInGame = false;
	VehicleBody->SetCanEverAffectNavigation(false);
	VehicleBody->AttachToComponent(GetMesh(), FAttachmentTransformRules( EAttachmentRule::KeepRelative , false ));
	
	UpperBody = CreateDefaultSubobject<UStaticMeshComponent>(FName("UpperBody"));
	UpperBody->bHiddenInGame = false;
	UpperBody->SetCanEverAffectNavigation(false);
	UpperBody->AttachToComponent( VehicleBody , FAttachmentTransformRules( EAttachmentRule::KeepRelative , false ) );

	CannonBody = CreateDefaultSubobject<UStaticMeshComponent>(FName("CannonBody"));
	CannonBody->bHiddenInGame = false;
	CannonBody->SetCanEverAffectNavigation(false);
	CannonBody->AttachToComponent( UpperBody , FAttachmentTransformRules( EAttachmentRule::KeepRelative , false ) ); 
	   
	MuzzleFXComp = CreateDefaultSubobject<UParticleSystemComponent>(FName("CannonMuzzleFX"));
	MuzzleFXComp->SetCanEverAffectNavigation(false);
	MuzzleFXComp->bHiddenInGame = false;
	MuzzleFXComp->AttachToComponent( CannonBody , FAttachmentTransformRules( EAttachmentRule::KeepRelative , false ) );

	CannonFireOffset = 20.0f ;
	WorldYawOffset = 0.0f ;
	ArmorStrength = 50 ;
	Health =	100	;
	HealthMax = 100	;
	HealthBarDrawOffsetY = 20.0f ; 
	bDestroyed = false ;

}

UBFTankMovementComponent*  ABFTankVehicle::GetTankMovementComponent() const
{
	return Cast<UBFTankMovementComponent>(Super::GetVehicleMovementComponent());
}

// Called when the game starts or when spawned
void ABFTankVehicle::BeginPlay()
{
	Super::BeginPlay();	

 

}

// Called every frame
void ABFTankVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	UpdateTrackWheelsParam(DeltaTime);
}

// Called to bind functionality to input
void ABFTankVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABFTankVehicle::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ABFTankVehicle::FireProjectile()
{

	if ( ActiveCamera != NULL )
	{

		FVector forward = ActiveCamera->GetForwardVector();

		FVector FireDirection = forward;
		FVector FireLocation = CannonBody->GetSocketTransform(FName("CannonFireLoc")).GetLocation();
		FireLocation = FireLocation + FireDirection*CannonFireOffset;

		if (ProjectileClass != NULL)
		{ 

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(FireLocation);
			SpawnTransform.SetRotation(FQuat::Identity);
			SpawnTransform.SetScale3D(FVector(1.0f));

			ABFProjectile* Projectile = Cast<ABFProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTransform));
			Projectile->ProjectileMovement->Velocity = FireDirection;
			UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
			OnShellFired(FireLocation,FireDirection);

		}
	}

}

bool  ABFTankVehicle::ModifyDamageTaken(int32& AppliedDamage, int32 DamageToApply, const FHitResult& HitInfo, AController* EventInstigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType)
{
 
	int32 CurrentArmor = ArmorStrength ;
	if ( DamageToApply > 0 && CurrentArmor > 0)
	{
		int32 AbsorbedDamage = 0 ;
		int32 InitialDamage = DamageToApply;
		if (CurrentArmor > 0 )
		{
			AbsorbedDamage = FMath::Min( DamageToApply , CurrentArmor - DamageToApply ) ;
			DamageToApply -= AbsorbedDamage ;
		}

		if (DamageToApply > 0 )
		{
			int32 PartialAbsorbedDamage = FMath::Min( CurrentArmor , FMath::Max<int32>( 1 , DamageToApply * 0.5f ) );
			DamageToApply -= PartialAbsorbedDamage  ;
			AbsorbedDamage += PartialAbsorbedDamage ;
		}

		ArmorStrength = FMath::Max( 0 , ArmorStrength - AbsorbedDamage );

		OnArmorUpdated( ArmorStrength );

		AppliedDamage = DamageToApply;

		return true;
	}

	return false;
}

float ABFTankVehicle::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Damage <0.0f)return Damage;


	if ( ShouldTakeDamage( Damage , DamageEvent , EventInstigator , DamageCauser ) && Health>0.0f )
	{
		const UDamageType* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	
		int32 ResultDamage = FMath::TruncToInt(Damage);
		float bRadialDamage = false;
		if ( DamageEvent.IsOfType(FRadialDamageEvent::ClassID) )
		{
			float AdjustedDamage = InternalTakeRadialDamage(Damage, (const FRadialDamageEvent&)DamageEvent, EventInstigator, DamageCauser);
			ResultDamage = FMath::TruncToInt(AdjustedDamage);
			bRadialDamage = true;
		} 
		 
		FHitResult HitInfo;
		{
			FVector UnusedDir;
			DamageEvent.GetBestHitInfo( this , DamageCauser , HitInfo , UnusedDir );
		}

 		if (ResultDamage > 0)
		{ 

			ModifyDamageTaken( ResultDamage , ResultDamage , HitInfo , EventInstigator , DamageCauser , DamageEvent.DamageTypeClass );

			float ActualDamage = float(ResultDamage);   
			 
			if (EventInstigator != NULL)
			{
				EventInstigator->InstigatedAnyDamage( ActualDamage , DamageTypeCDO , this , DamageCauser );
			}
			if (DamageEvent.IsOfType(FBFPointDamageEvent::ClassID))
			{
				// point damage event, pass off to helper function
				FBFPointDamageEvent* const PointDamageEvent = (FBFPointDamageEvent*)&DamageEvent;

				// K2 notification for this actor
				if (ActualDamage != 0.f)
				{
					SpawnDamageEffects( false , HitInfo );
					ReceivePointDamage(ActualDamage, DamageTypeCDO, PointDamageEvent->HitInfo.ImpactPoint, PointDamageEvent->HitInfo.ImpactNormal, PointDamageEvent->HitInfo.Component.Get(), PointDamageEvent->HitInfo.BoneName, PointDamageEvent->ShotDirection, EventInstigator, DamageCauser, PointDamageEvent->HitInfo);
					OnTakePointDamage.Broadcast(this, ActualDamage, EventInstigator, PointDamageEvent->HitInfo.ImpactPoint, PointDamageEvent->HitInfo.Component.Get(), PointDamageEvent->HitInfo.BoneName, PointDamageEvent->ShotDirection, DamageTypeCDO, DamageCauser);
				}
			}
			else if (DamageEvent.IsOfType(FBFRadialDamageEvent::ClassID))
			{
				// radial damage event, pass off to helper function
				FBFRadialDamageEvent* const RadialDamageEvent = (FBFRadialDamageEvent*)&DamageEvent;

				// K2 notification for this actor
				if (ActualDamage != 0.f)
				{
					SpawnDamageEffects( true , HitInfo );
					FHitResult const& Hit = (RadialDamageEvent->ComponentHits.Num() > 0) ? RadialDamageEvent->ComponentHits[0] : FHitResult();
					ReceiveRadialDamage(ActualDamage, DamageTypeCDO, RadialDamageEvent->Origin, Hit, EventInstigator, DamageCauser);
				}
			}  
		}

		 
		Health -= ResultDamage;
		OnHealthUpdated( Health );
		
		if ( Health <= 0 )
		{  
			if (!bDestroyed)
			{			
		 
				bDestroyed = true;
				  


				DestroyVehicle();
			}
		}
		 
	}
 
	return Damage;

} 

void  ABFTankVehicle::SpawnDamageEffects( bool RadialDamage , const FHitResult& HitInfo )
{

}

void ABFTankVehicle::SpawnArmorDamageEffects()
{

}

void  ABFTankVehicle::OnHealthUpdated( float NewHealth )
{ 

}

FVector  ABFTankVehicle::GetTargetLocation(AActor* RequestedBy) const
{
	return UpperBody->GetComponentLocation();
}

void ABFTankVehicle::UpdateTreadMotions_Implementation( float DeltaSeconds , float NormalizedSpeed , float TurnDelta )
{  
	 
}

void ABFTankVehicle::OnArmorUpdated( float NewArmorAmount )
{

}

void ABFTankVehicle::Kill()
{ 
	DestroyVehicle();
}

void ABFTankVehicle::DestroyVehicle_Implementation()
{
	bDestroyed = true ;

	if ( DestroyedVehicleClass != NULL )
	{
		AActor* DestroyedVehicle = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, DestroyedVehicleClass, GetTransform());
		DestroyedVehicle->SetActorEnableCollision(false);
		UGameplayStatics::FinishSpawningActor(DestroyedVehicle, GetTransform());
	} 
	

	
	SetActorHiddenInGame(true);
} 

void ABFTankVehicle::UpdateTrackWheelsParam(float DeltaSeconds)
{
	 
	FVector Velocity = GetVelocity();
	float VehicleYaw = VehicleBody->GetComponentRotation().Yaw;

	//if (GetCharacterMovement() != NULL)
	{

		FVector2D MovementAxis = FVector2D(0.0f, 0.0f);

 
		float VelocitySize = Velocity.Size();
		float Speed = VelocitySize / (MaxSpeed) ;
		float TurnAxis = VehicleYaw - LastYaw;

		Speed		= FMath::Clamp( Speed , 0.0f , 1.0f );  

		LastYaw = VehicleYaw;
 

		ABFTankPlayerController* PlayerController = Cast<ABFTankPlayerController>(GetController());
		if (PlayerController != NULL)
		{
			MovementAxis = PlayerController->GetMovementAxis();
		}
		else
		{
			MovementAxis.Y = Speed;
			if(TurnAxis!=0.0f)
			MovementAxis.X = TurnAxis > 0.0f ? 1.0f : -1.0f;
		} 

		float TrackDirForward = MovementAxis.Y	;

		if ( TrackDirForward < 0.0f )
			TrackDirForward = -1.0f;
		else if (TrackDirForward > 0.0f)	
			TrackDirForward = 1.0f; 
		
		if (TrackDirForward != 0.0f )
		{
			TreadSpeed +=  1.2f * DeltaSeconds ;
			WheelSpeed +=  1.2f * DeltaSeconds ;
			PrevTrackDir = TrackDirForward ;
		 
		
		} 
		else if (TurnAxis != 0.0f)
		{
			TreadSpeed += 1.2f * DeltaSeconds	;
			WheelSpeed += 1.2f * DeltaSeconds	;
			PrevTrackDir = 1.0f;

		}else
		{
			TreadSpeed -= 10.0f*DeltaSeconds	;
			if(MovementAxis.X==0.0f)
			WheelSpeed -= 4.0f*DeltaSeconds		;
		}
     
		TreadSpeed = FMath::Clamp( TreadSpeed , 0.0f , 1.2f ) ;
		WheelSpeed = FMath::Clamp( WheelSpeed , 0.0f , 1.2f ) ;

		float LeftTrackSpeed = TreadSpeed;
		float RightTrackSpeed = TreadSpeed;

		if (MovementAxis.X > 0.0f)
		{ 
			if (PrevTrackDir < 0.0f)
				RightTrackSpeed *= 0.1f;
			else LeftTrackSpeed *= 0.1f;
			
		}
		else if (MovementAxis.X < 0.0f)
		{
			if (PrevTrackDir < 0.0f)
				LeftTrackSpeed *= 0.1f;
			else RightTrackSpeed *= 0.1f;
		}

		 LeftTrackOffset   += LeftTrackSpeed * PrevTrackDir * DeltaSeconds	;
		 RightTrackOFfset  += RightTrackSpeed * PrevTrackDir * DeltaSeconds ;

	 
		if (LeftTrackMatInst != NULL)
		{
			LeftTrackMatInst->SetScalarParameterValue( TEXT("TrackOffset") , LeftTrackOffset  ) ;
		}
		
		if (RightTrackMatInst != NULL)
		{
			RightTrackMatInst->SetScalarParameterValue( TEXT("TrackOffset") , RightTrackOFfset ) ;
		}
  
		float LeftWheelSpeed	= WheelSpeed	;
		float RightWheelSpeed	= WheelSpeed	;

		if (MovementAxis.X > 0.0f)
		{
			if (PrevTrackDir < 0.0f)
				RightWheelSpeed *=0.5f	;
			else LeftWheelSpeed *= 0.5f	; 
		}
		else if (MovementAxis.X < 0.0f)
		{
			if (PrevTrackDir < 0.0f)
				LeftWheelSpeed *= 0.5f		;
			else  RightWheelSpeed *= 0.5f	; 
		}

		LeftWheelRotation  += LeftWheelSpeed  *  PrevTrackDir * DeltaSeconds ;
		RightWheelRotation += RightWheelSpeed *  PrevTrackDir * DeltaSeconds ;
		 
		if ( LeftWheelRotation > 1.0f )		LeftWheelRotation -= 1.0f ;
		if ( LeftWheelRotation < 0.0f )		LeftWheelRotation += 1.0f ;

		if ( RightWheelRotation > 1.0f )	RightWheelRotation -= 1.0f ;
		if ( RightWheelRotation < 0.0f )	RightWheelRotation += 1.0f ;

		if (LeftWheelsMatInst != NULL)
		{
			LeftWheelsMatInst->SetScalarParameterValue( TEXT("WheelSpeed") , LeftWheelRotation );
		}
		if (RightWheelsMatInst != NULL)
		{
			RightWheelsMatInst->SetScalarParameterValue( TEXT("WheelSpeed") , RightWheelRotation );
		} 

	}
}

bool ABFTankVehicle::ShouldTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const
{

	return true;
}

void ABFTankVehicle::OnShellFired_Implementation(FVector FiredLocation, FVector FiredDirection)
{

	if ( CannonMuzzleFX != NULL )
	{  
		//UGameplayStatics::SpawnEmitterAttached( CannonMuzzleFX,CannonBody, FiredLocation, Rotation, true);  
	}

}  
 
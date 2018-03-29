#include "TankVsZombie.h"
#include "BFTankPlayerController.h"
#include "Weapon/BFMountedWeaponAttachment.h"
#include "BFLocalPlayer.h"
#include "BFPlayerTank.h"

ABFPlayerTank::ABFPlayerTank(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

 	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
 	SpringArm->bHiddenInGame = true;
 	SpringArm->AttachToComponent(CannonBody, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

 	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(FName("ThirdPersonCamera"));
 	ThirdPersonCamera->bHiddenInGame = true;
 	ThirdPersonCamera->AttachToComponent(SpringArm, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	CannonCamera = CreateDefaultSubobject<UCameraComponent>(FName("CannonCamera"));
	CannonCamera->bHiddenInGame = true;
	CannonCamera->AttachToComponent(CannonBody, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	EngineAudioComp = CreateDefaultSubobject<UAudioComponent>(FName("EngineSound"));
	EngineAudioComp->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	EngineAudioLoop = CreateDefaultSubobject<UAudioComponent>(FName("EngineLoop"));
	EngineAudioLoop->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	CannonRotationYawMin = 0.0f;
	CannonRotationYawMax = 359.0f;

	CannonRotationPitchMin = 0.0f;
	CannonRotationPitchMax = 359.0f;   

	ShellReloadTime = 3.0f;
	
	AmmoCount	 = 10 ;
	AmmoClipSize = 10 ;
	AmmoCap		 = -1 ;
	bIsReloading = false;
	CameraJitterStrength = 0.2f;

	

}

void ABFPlayerTank::ReleaseCamera()
{

	if (ThirdPersonCamera != NULL)
	{
		ThirdPersonCamera->Deactivate();
	}

	if (CannonCamera != NULL)
	{
		CannonCamera->Deactivate();
	}

	ABFTankPlayerController* TankController = Cast<ABFTankPlayerController>(GetController());
	if (TankController != NULL)
	{
		TankController->SetViewTarget(NULL); 
	}

}

void ABFPlayerTank::SwitchCamera()
{

	APlayerController*  PC = (APlayerController*)GetController();

	if (PC != NULL)
	{

		float Delta = GetWorld()->GetTimeSeconds() - LastCameraSwitchTime;

		if (Delta > 1.0f)
		{

			LastCameraSwitchTime = GetWorld()->GetTimeSeconds();

			if ( ActiveCamera == CannonCamera )
			{
				SwitchThirdPersonCamera();
			}
			else if ( ActiveCamera == ThirdPersonCamera )
			{
				SwitchCannonCamera();
			}
			else if (MountedWeapon && ActiveCamera == MountedWeapon->CameraComponent)
			{
				SwitchMountedWeaponCamera();
			}

		}

	}

}

void  ABFPlayerTank::UpdateSoundParams_Implementation(float Speed, bool bDeccelerating, bool bStopped)
{
 
}

void  ABFPlayerTank::HideCannon()
{

	if (CannonBody != NULL)
	{

		CannonBody->SetVisibility(false);

	}

}

void  ABFPlayerTank::ShowCannon()
{

	if (CannonBody != NULL)
	{

		CannonBody->SetVisibility(true);

	}

}

void  ABFPlayerTank::SwitchMountedWeaponCamera()
{

	if (MountedWeapon != NULL)
	{

		if (ActiveCamera != NULL)
		{
			ActiveCamera->Deactivate();
		}

		ActiveCamera = MountedWeapon->CameraComponent;

		if (ActiveCamera != NULL)
		{

			ActiveCamera->Activate(true);
			OnMountedWeaponCameraSwitched(); 

		} 

		CurrentCannonRotationPitch = 0.0f;
		BlendToNewViewTarget(0.0f, MountedWeapon);

	}
} 

void  ABFPlayerTank::SwitchThirdPersonCamera()
{
	if ( ActiveCamera != NULL )
	{

		ActiveCamera->Deactivate() ;

	}

	ActiveCamera = ThirdPersonCamera ;

	if ( ActiveCamera != NULL )
	{

		ActiveCamera->Activate( true ) ;
		OnThirdPersonCameraSwitched()  ;

	} 

	BlendToNewViewTarget( 0.0f , this ) ; 

} 

void  ABFPlayerTank::SwitchCannonCamera()
{ 
	if (ActiveCamera != NULL)
	{

		ActiveCamera->Deactivate() ;

	} 

	ActiveCamera = CannonCamera ;

	if (ActiveCamera != NULL)
	{

		ActiveCamera->Activate( true ) ;
		OnCannonCameraSwitched() ;

	}  

	BlendToNewViewTarget( 0.0f , this ); 
} 

void ABFPlayerTank::BlendToNewViewTarget(float BlendTime, AActor* Target)
{

	FViewTargetTransitionParams Params	;
	Params.BlendTime = BlendTime		;
	Params.BlendFunction = EViewTargetBlendFunction::VTBlend_Cubic ;
	Params.BlendExp = 0.1f	;

	ABFTankPlayerController* PC = Cast<ABFTankPlayerController>(GetController()) ;

	if ( PC != NULL )
	{

		PC->SetViewTarget( Target , Params ) ;

	}
}

const UBFWeaponAttachment* ABFPlayerTank::GetWeaponAttachment(TSubclassOf<ABFWeapon> WeaponClass) 
{ 
	for ( int32 i = 0 ; i < WeaponAttachmentsList.Num() ; i++ )
	{ 
		const UBFWeaponAttachment* Attachment = Cast<const UBFWeaponAttachment>( WeaponAttachmentsList[i]->GetDefaultObject() );

		if ( Attachment->WeaponClass == WeaponClass )
		{ 
			return Attachment ; 
		}
	}

	return NULL ; 
}

  
void ABFPlayerTank::OnMountedWeaponCameraSwitched()
{
	   
	if ( MountedWeapon != NULL )
	{

		MountedWeapon->SetActorHiddenInGame( false ); 
		MountedWeapon->OnEquipped() ;

	}

	VehicleBody->SetVisibility( true );

}

void ABFPlayerTank::OnCannonCameraSwitched()
{

	if ( MountedWeapon != NULL )
	{

		MountedWeapon->SetActorHiddenInGame(true) ;
		MountedWeapon->OnUnequipped() ;

	}

	VehicleBody->SetVisibility( true );

}

void ABFPlayerTank::OnThirdPersonCameraSwitched()
{

	if ( MountedWeapon != NULL )
	{

		MountedWeapon->SetActorHiddenInGame( false );

	}

	VehicleBody->SetVisibility( true );

}

void ABFPlayerTank::BeginPlay()
{
	Super::BeginPlay(); 

	EngineAudioLoop->SetVolumeMultiplier( 0.2f );
	EngineAudioComp->SetVolumeMultiplier( 1.0f );
	EngineAudioComp->SetFloatParameter( "Engine" , 0.0f );  

}


void ABFPlayerTank::UpdateCannonOrientation(float DeltaSeconds)
{

	
	if ( InCannonCamera() )
	{ 

		CurrentCannonRotationPitch	=	FMath::FInterpTo( CurrentCannonRotationPitch	, TargetCannonRotationPitch , DeltaSeconds	, 10.0f );
		CurrentCannonRotationYaw	=	FMath::FInterpTo( CurrentCannonRotationYaw	, TargetCannonRotationYaw	, DeltaSeconds	, 10.0f ); 

		SetCannonRotation(CurrentCannonRotationPitch);
		SetUpperbodyRotation(CurrentCannonRotationYaw);

	}
	else if( MountedWeapon && InMountedWeaponCamera() )
	{ 
		
		CurrentCannonRotationYaw = FMath::FInterpTo( CurrentCannonRotationYaw , TargetCannonRotationYaw , DeltaSeconds , 10.0f );
		SetUpperbodyRotation( CurrentCannonRotationYaw );

	}else 
	{
		CurrentCannonRotationPitch = FMath::FInterpTo( CurrentCannonRotationPitch , TargetCannonRotationPitch , DeltaSeconds , 10.0f );
		CurrentCannonRotationYaw = FMath::FInterpTo( CurrentCannonRotationYaw , TargetCannonRotationYaw , DeltaSeconds , 10.0f );

		SetCannonRotation( CurrentCannonRotationPitch )  ;
		SetUpperbodyRotation( CurrentCannonRotationYaw ) ;

	}


}

void  ABFPlayerTank::SetCannonRotation( float RotationInDegree )
{

	FRotator	CurCannonRot = CannonBody->GetComponentRotation() ;
	FVector		CurCannonLocation = CannonBody->GetComponentLocation() ;

	FVector UpperBodyRightAxis = UpperBody->GetRightVector() ;
	FRotator NewCannonRot = FQuat( UpperBodyRightAxis , FMath::DegreesToRadians( CurrentCannonRotationPitch )).Rotator() ;
	NewCannonRot.Pitch = CurrentCannonRotationPitch ;
	CannonBody->SetRelativeRotation( FRotator( 0.0f , 0.0f , CurrentCannonRotationPitch ) );

}

void  ABFPlayerTank::SetUpperbodyRotation( float RotationInDegree )
{

	FVector UpperBodyLocation = UpperBody->GetComponentLocation() ;
	FVector UpperBodyRotAxis = this->GetActorUpVector() ;
	FRotator UpperBodyRot = FQuat( UpperBodyRotAxis , FMath::DegreesToRadians(CurrentCannonRotationYaw)).Rotator() ;
	UpperBodyRot.Yaw += WorldYawOffset ;
	UpperBody->SetWorldRotation( FRotator( 0.0f , CurrentCannonRotationYaw - 90.0f , 0.0f ));

} 

void ABFPlayerTank::UpdateViewRotation( float DeltaYaw , float DeltaPitch )
{ 

	AccumulatedRotationYaw += DeltaYaw ;

	if ( InCannonCamera() )
	{
		AccumulatedRotationPitch += DeltaPitch ;
		TargetCannonRotationPitch = FMath::ClampAngle( AccumulatedRotationPitch , CannonZoomedPitchMin , CannonZoomedPitchMax ) ;
	}
	else if( MountedWeapon != NULL && InMountedWeaponCamera() )
	{ 
		MountedWeapon->UpdateViewRotation( 0.0f , DeltaPitch );
	} 

	TargetCannonRotationYaw = AccumulatedRotationYaw ;   

}

void ABFPlayerTank::PossessedBy( AController* NewController )
{ 

	Super::PossessedBy( NewController );  

}

void ABFPlayerTank::SetTarget( AActor* Target )
{ 

	TargetLocation = Target->GetActorLocation(); 

}

void ABFPlayerTank::SetTargetLocation( FVector Location )
{ 
	TargetLocation = Location ;
}

void ABFPlayerTank::SetTargeting( bool Targeting )
{ 

	bIsTargeting = Targeting ; 

}

void ABFPlayerTank::Tick( float DeltaSeconds )
{  

	Super::Tick( DeltaSeconds );

	UpdateCannonOrientation( DeltaSeconds ); 
	UpdateCameraJitterAnim( DeltaSeconds );

	if ( GetTankMovementComponent() != NULL )
	{
		UBFTankMovementComponent* TankMovementComp = GetTankMovementComponent() ;
		float WalkSpeed = TankMovementComp->GetMaxSpeed();

		float Frac = GetVelocity().Size() / (WalkSpeed == 0.0f ? 1.0f : WalkSpeed);
		EmitSmokeEffects( Frac );
		UpdateSoundParams( Frac , false , false );
	} 

	int32 CriticalLevel  = HealthMax / ( Health == 0 ? 1 : Health ) ; 
	UpdateMaintainenceSound( CriticalLevel );  

	UpdateAttachedZombies( DeltaSeconds );

} 
 
FVector ABFPlayerTank::GetFireDirection()
{  
	if ( ActiveCamera != NULL )
	{ 
		return ActiveCamera->GetForwardVector() ; 
	}

	FVector FireLocation	= CannonBody->GetSocketTransform( FName("CannonFireLoc") ).GetLocation() ;
	return ( TargetLocation - FireLocation ).GetSafeNormal() ; 
}

FVector ABFPlayerTank::GetCannonFireDirection()
{ 
	FVector EndLocation = CannonBody->GetSocketTransform(FName("CannonFireLoc")).GetLocation();
	FVector StartLocation = CannonBody->GetSocketTransform(FName("CannonRoot")).GetLocation();

	return FVector( EndLocation - StartLocation ).GetSafeNormal() ; 
}

FVector ABFPlayerTank::GetFireLocation()
{  
	if ( CannonBody != NULL )
	{
		return CannonBody->GetSocketLocation( FName("CannonFireLoc") );
	} 

	return GetActorLocation() ; 
}

void ABFPlayerTank::Fire( bool FirePressed)
{
	bIsFirePressed = FirePressed;

	if ( bIsFirePressed )
	{ 
		FireCannon() ; 
	}  
} 
 
void ABFPlayerTank::ReloadShell()
{
	if ( !bIsReloading && AmmoCount > 0 )
	{
		bIsReloading = true ;
		bHasPendingReload = false ;
		ABFTankPlayerController* PC = Cast<ABFTankPlayerController>( GetController() ) ;
		
		if (  PC != NULL )
		{
			PC->OnShellReloadBegin() ;
		}
		
		GetWorldTimerManager().SetTimer( ReloadingTimer , this , &ABFPlayerTank::ShellReloaded , ShellReloadTime==0.0f?1.0f:ShellReloadTime , false );
	}
} 

void ABFPlayerTank::ReloadShellClip()
{
	if ( AmmoCount <= 0 )
	{ 
		bIsReloading = false ;
		ABFTankPlayerController* PC = Cast<ABFTankPlayerController>(GetController()) ;
		AmmoCount = AmmoClipSize ;
		if ( PC != NULL )
		{
			PC->OnShellClipReloaded( AmmoCount , AmmoClipSize );
		}
	}
}

void ABFPlayerTank::ShellReloaded()
{   
	bIsReloading = false ;
	ABFTankPlayerController* PC = Cast<ABFTankPlayerController>( GetController() ) ;
	
	if ( PC != NULL )
	{
		PC->ReportShellReloaded( AmmoCount );
	}

	if ( bIsFirePressed )
	{ 
		FireCannon() ; 
	} 
}

void ABFPlayerTank::FireCannon()
{  
	if ( AmmoCount > 0 && !IsReloadingShell() )
	{ 
		FireProjectile() ;		
		AmmoCount-- ; 	
		
		if ( AmmoCount < 0 )
		{
			AmmoCount = 0 ;
		}
		else
		{
			ABFTankPlayerController* PC = Cast<ABFTankPlayerController>( GetController() ) ;
			if ( PC != NULL )
			{
				PC->OnAmmoUpdated( AmmoCount , AmmoClipSize );
			}
			ReloadShell() ;
		}

	}else if( AmmoCount == 0 )
	{	   
		GetWorldTimerManager().SetTimer( ReloadingTimer , this , &ABFPlayerTank::FireCannon , ShellReloadTime * 0.5f , false ) ;
		
		ABFTankPlayerController* PC = Cast<ABFTankPlayerController>( GetController() ) ;

		if ( PC != NULL )
		{
			PC->ReportEmptyAmmo() ;
		}   
	}

}

void ABFPlayerTank::FireProjectile()
{ 
	if (ActiveCamera == NULL)
		return ;

	bMountedWeaponFiring = false ;

	FVector FireLocation  = GetFireLocation() ;
	FVector FireDirection = GetCannonFireDirection() ;

	if ( ActiveCamera != NULL )
	{
		FireDirection = ActiveCamera->GetForwardVector() ;
	}
		
	FireLocation = FireLocation + FireDirection.GetSafeNormal() * CannonFireOffset ;
		 
	if ( ProjectileClass != NULL )
	{ 
		FTransform SpawnTransform = FTransform::Identity ;
		SpawnTransform.SetLocation(FireLocation) ;
		SpawnTransform.SetRotation(FQuat::Identity) ;
		ABFProjectile* Projectile = Cast<ABFProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass( this , ProjectileClass , SpawnTransform ) ) ;
		 
		Projectile->ProjectileMovement->Velocity = ActiveCamera->GetForwardVector() ;
		Projectile->InstigatorController = GetController() ;   
		Projectile->Instigator = this ;
		Projectile->DamageParams.BaseDamage *= ShellDamageAmount ;
		Projectile->DamageParams.MinimumDamage *= ShellDamageAmount ;
	
		UGameplayStatics::FinishSpawningActor( Projectile , SpawnTransform ) ;
		OnShellFired( FireLocation , FireDirection ) ;   
	}  
}

bool ABFPlayerTank::IsOutOfAmmo()
{
	return AmmoCount <= 0 && AmmoCap == 0 ;
}

void ABFPlayerTank::CancelReload()
{ 
	if ( bIsReloading ) {

		bHasPendingReload = true; 

	} else { 

		bHasPendingReload = false; 

	}

	bIsReloading = false; 
	GetWorldTimerManager().ClearTimer(ReloadingTimer);  
}

bool ABFPlayerTank::IsReloadingShell()
{
	return bIsReloading ;
} 

float ABFPlayerTank::CauseDamage(int32 DamageToApply)
{ 
	if (bDestroyed)return 0.0f;

	int32 AppliedDamage = 0 ;  

	int32 CurrentArmor = ArmorStrength ;
	if ( DamageToApply > 0 && CurrentArmor > 0 )
	{
		int32 AbsorbedDamage = 0			;
		int32 InitialDamage = DamageToApply	;

		if ( CurrentArmor > 0 )
		{
			AbsorbedDamage = FMath::Min( DamageToApply , CurrentArmor - DamageToApply );
			DamageToApply -= AbsorbedDamage ;
		}

		if ( DamageToApply > 0 )
		{
			int32 PartialAbsorbedDamage = FMath::Min( CurrentArmor , FMath::Max<int32>( 1 , DamageToApply * 0.5f ) );
			DamageToApply  -= PartialAbsorbedDamage	;
			AbsorbedDamage += PartialAbsorbedDamage	;
		}

		ArmorStrength = FMath::Max(0, ArmorStrength - AbsorbedDamage);

		OnArmorUpdated(ArmorStrength);

		AppliedDamage = DamageToApply; 
	}
	  
	Health -= AppliedDamage; 

	OnHealthUpdated(Health);

	if ( Health <= 0 )  
		Health = 0 ;  

	return AppliedDamage;
}

bool ABFPlayerTank::InCannonCamera()
{
	return ActiveCamera == CannonCamera ;
}
 
bool ABFPlayerTank::InMountedWeaponCamera()
{
	return MountedWeapon && ActiveCamera == MountedWeapon->GetAttachedCamera();
}
void ABFPlayerTank::OnHealthUpdated(float NewHealth)
{ 

	if (NewHealth < 0.0f)
	{
		NewHealth = 1.0f	;
	} 

	ABFTankPlayerController* PlayerController = Cast<ABFTankPlayerController>(GetController())	;

	if (PlayerController != NULL)
	{ 
		UBFLocalPlayer* LocalPlayer = Cast<UBFLocalPlayer>(PlayerController->GetLocalPlayer())	;
		if (LocalPlayer != NULL)
		{
			float TotalHealth = HealthMax;

			if (TotalHealth < 0.0f)
			{
				TotalHealth = 1.0f;
			}
			PlayerController->NotifyHealthUpdated(TotalHealth, NewHealth);
		}   

	}

}

void ABFPlayerTank::OnArmorUpdated(float NewArmorAmount)
{ 
	if (NewArmorAmount < 0.0f)
	{
		NewArmorAmount = 1.0f ;
	}

	ABFTankPlayerController* PlayerController = Cast<ABFTankPlayerController>(GetController())	;	
	if (PlayerController != NULL)
	{
		UBFLocalPlayer* LocalPlayer = Cast<UBFLocalPlayer>(PlayerController->GetLocalPlayer())	;

		float TotalArmor = 0.0f;

		PlayerController->NotifiyArmorUpdated( TotalArmor , NewArmorAmount )	;
	} 
}

void  ABFPlayerTank::TriggerCameraJitter(float Strength,float PlayLength)
{
	if (!bCameraJitterAnim)
	{  
		if (ActiveCamera != NULL)
		{  
			CameraJitterPlayLength			= PlayLength ;
			bCameraJitterAnim				= true ;
			CameraJitterAmount				= Strength ;
			PreJitterSavedCameraLocation	= ActiveCamera->GetRelativeTransform().GetTranslation() ;
			JitterAnimStartTime				= GetWorld()->GetTimeSeconds() ; 
		}
	}
} 

void ABFPlayerTank::SetTankSpeed(float Speed)
{
	//	if(GetTankMovementComponent()!=NULL)
	//	GetTankMovementComponent()->MaxWalkSpeed *= Speed	;
}

void ABFPlayerTank::UpdateCameraJitterAnim(float DeltaSeconds)
{
	if (bCameraJitterAnim)
	{		

		float Seconds	= GetWorld()->GetTimeSeconds() ;
		float Frac		= ( Seconds - JitterAnimStartTime ) / CameraJitterPlayLength;

		if ( Frac > 1.0f )
		{
			Frac = 1.0f	;
			bCameraJitterAnim = false ;
		}   

		if (ActiveCamera )
		{
			if (CameraJitterCurve)
			{
				float ShakeFrac = CameraJitterCurve->GetFloatValue(Frac);
				FVector JitterOffset = ShakeFrac * CameraJitterAmount * CameraJitterMask;
				ActiveCamera->SetRelativeLocation(PreJitterSavedCameraLocation + JitterOffset);
			}
			else
			{
				float ShakeFrac = FMath::Sin(Frac*360.0f);
				FVector JitterOffset = ShakeFrac * CameraJitterAmount * CameraJitterMask;
				ActiveCamera->SetRelativeLocation(PreJitterSavedCameraLocation + JitterOffset);
			}
		}
	}
}

void ABFPlayerTank::RefillHealth()
{
	Health = HealthMax;

}
void ABFPlayerTank::StopWeaponFiring()
{
	if ( MountedWeapon != NULL )
	{
		bMountedWeaponFiring = false ;
		MountedWeapon->FirePressed( false )	; 
	}
} 

void ABFPlayerTank::FireMountedWeapon()
{
	if ( MountedWeapon != NULL )
	{
		bMountedWeaponFiring = true	; 
		MountedWeapon->FirePressed( true ) ;
	}
} 

void ABFPlayerTank::EmitSmokeEffects_Implementation( float Speed )
{

}

void ABFPlayerTank::SetupMountedWeapon(FSavedVehicleWeaponState WeaponState)
{
	if (WeaponState.ActiveWeaponClass != NULL)
	{
		AttachMountedGun(WeaponState); 
	}
}

void ABFPlayerTank::AttachMountedGun(FSavedVehicleWeaponState WeaponState)
{
	if (WeaponState.ActiveWeaponClass != NULL)
	{ 
		DetachMountedGun( true ); 
		 
		FName AttachmentSocket = MountedWeaponSocket == NAME_None ? "MountedSocket" : MountedWeaponSocket	;
		FTransform AttachmentTransform = FTransform::Identity;

		const UBFWeaponAttachment* WeaponAttachment = GetWeaponAttachment( WeaponState.ActiveWeaponClass );
		
		if (WeaponAttachment != NULL)
		{
			AttachmentSocket = WeaponAttachment->PrimarySocketName ;
			AttachmentTransform.SetLocation( WeaponAttachment->PrimarySocketLocation );
			AttachmentTransform.SetRotation( WeaponAttachment->PrimarySocketRotation.Quaternion() );
			AttachmentTransform.SetScale3D( WeaponAttachment->PrimarySocketScale );
		}	 

			MountedWeapon = Cast<ABFMountedWeapon>( UGameplayStatics::BeginDeferredActorSpawnFromClass( this , WeaponState.ActiveWeaponClass , AttachmentTransform )) ;
			MountedWeapon->WeaponOwner = this ;
			MountedWeapon->AttachToComponent( UpperBody , FAttachmentTransformRules( EAttachmentRule::KeepRelative , EAttachmentRule::KeepRelative , EAttachmentRule::KeepWorld , false ) , AttachmentSocket );
			MountedWeapon->Setup( WeaponState );

			UGameplayStatics::FinishSpawningActor( MountedWeapon , AttachmentTransform ); 
	}
}

void ABFPlayerTank::PostInitSetup()
{

}
 
void ABFPlayerTank::DetachMountedGun( bool ForceDestroy )
{ 
	if ( MountedWeapon != NULL )
	{ 
		MountedWeapon->DetachFromActor( FDetachmentTransformRules(EDetachmentRule::KeepRelative, false) ) ;

		if ( ForceDestroy )
		{
			MountedWeapon->Destroy(false, true) ;
			MountedWeapon = NULL ;
		}
	} 
}
 
void ABFPlayerTank::UpdateMaintainenceSound_Implementation( int32 CriticalHealthLevel )
{
	
} 
 
FVehicleAttachmentSlot* ABFPlayerTank::GetNearestAttachmentSlot(ABFZombie* Zombie)
{
	FVector Location = Zombie->GetActorLocation() ;
	FVehicleAttachmentSlot* Slot0 =  LeftSideSpline->GetNearestSlot( Location ) ;
  	FVehicleAttachmentSlot* Slot1 = RightSideSpline->GetNearestSlot( Location ) ;
	
	float Dist0 = FVector(Location - Slot0->Spline->GetWorldLocationAtSplinePoint( Slot0->PointIndex ) ).Size();
	float Dist1 = FVector(Location - Slot1->Spline->GetWorldLocationAtSplinePoint( Slot1->PointIndex ) ).Size();
	if (Dist0 > Dist1)
	{
		return Slot1;
	}

	return Slot0; 
}

void ABFPlayerTank::AttachZombieToSlot(FVehicleAttachmentSlot* Slot, ABFZombie* Zombie)
{
	bool bFound = false;
	for (int32 i = 0; i < AttachedZombies.Num(); i++)
	{
		if (AttachedZombies[i].Zombie  == Zombie)
				bFound = true;
	}

	if ( !bFound )
	{
		FZombieAttachment Attachment;
		Attachment.Zombie = Zombie;
		Attachment.SlotAttachedTo = Slot;
		AttachedZombies.Add(Attachment);
	} 
}

void ABFPlayerTank::DetachZombie(ABFZombie* Zombie)
{
	for (int32 i = 0; i < AttachedZombies.Num(); i++)
	{
		if (AttachedZombies[i].Zombie == Zombie)
		{
			AttachedZombies.RemoveAt(i);
		}
	} 
}

void ABFPlayerTank::UpdateAttachedZombies(float DeltaSeconds)
{
	for (int32 i = 0; i < AttachedZombies.Num(); i++)
	{
		ABFZombie* Zombie			 = AttachedZombies[i].Zombie;
		FVehicleAttachmentSlot* Slot = AttachedZombies[i].SlotAttachedTo;

		if (Zombie && Zombie->IsPendingKill() && Zombie->IsPendingKillPending())
		{
			FVector Location = Slot->Spline->GetWorldLocationAtSplinePoint(Slot->PointIndex);
			FVector Direction = GetActorRightVector();
			if (Slot->Spline == LeftSideSpline)
			{
				Direction = -Direction;
			}
			FRotator Rotation = Direction.ToOrientationRotator();
			Zombie->SetActorRotation(Rotation);
			
			Location += Direction*Zombie->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
			Zombie->SetActorLocation(Location);

		}
	}
}

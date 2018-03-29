// Fill out your copyright notice in the Description page of Project Settings.

#include "TankVsZombie.h"
#include "BFGameHUD.h"
#include "BFRoadSpline.h"
#include "AI/Navigation/NavModifierVolume.h"
#include "BFLocalPlayer.h"
#include "AI/BFBotAIController.h" 
#include "BFTankPlayerController.h"
#include "BFGameInstance.h"
#include "Gadgets/BFThrowable.h"
#include "BFWorldSettings.h"

ABFTankPlayerController::ABFTankPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	MoveSpeed = 200.0f;
	Damping = 10.0f;
	Acceleration = 100.0f;
	ViewSmoothMultiplier = 1.0f;
}

ABFWorldSettings* ABFTankPlayerController::GetWorldSettings()
{
	return Cast<ABFWorldSettings>(GetWorld()->GetWorldSettings());
}

void ABFTankPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();



}

void  ABFTankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	BattleHUD = Cast<ABFGameHUD>(GetHUD());
	SwitchShellCannon();
	ApplyPlayerSettings();
}

void ABFTankPlayerController::PlayEntryAnim(ABFRoadSpline* RoadSpline, ABFCinematicCamera* EntryCamera)
{
	EntryAnimSpline = RoadSpline;

	SetViewTarget(EntryCamera);

	bInCinematicMode = true;
}

void ABFTankPlayerController::UpdateCinematicAnim(float DeltaSeconds)
{
	if (bInCinematicMode)
	{

	}
}

bool ABFTankPlayerController::HasEnoughHealthRefill()
{
	UBFLocalPlayer* LocalPlayer = Cast<UBFLocalPlayer>(GetLocalPlayer());

	if (LocalPlayer != NULL)
	{
		if (LocalPlayer->GetLivesCount() > 0)
		{
			return true;
		}
	}
	return false;
}

void ABFTankPlayerController::RefillHealth()
{
	if (TankPawn != NULL)
	{
		UBFLocalPlayer* LocalPlayer = Cast<UBFLocalPlayer>(GetLocalPlayer());

		if (LocalPlayer != NULL)
		{
			if(LocalPlayer->GetLivesCount() > 0)
			{
				if ( TankPawn->Health<TankPawn->HealthMax)
				{
					LocalPlayer->AddLives(-1); 
					TankPawn->RefillHealth();
					
					NotifyHealthUpdated(TankPawn->HealthMax, TankPawn->Health);
				}
				
			}

			NotfiyHealthRefillUpdated(LocalPlayer->GetLivesCount());
		}
	}
}

void ABFTankPlayerController::ApplyPlayerSettings()
{
	if (TankPawn != NULL)
	{
		UBFLocalPlayer* LocalPlayer = Cast<UBFLocalPlayer>(GetLocalPlayer());

		if (LocalPlayer != NULL)
		{
			UBFGameSaveData* SaveData = LocalPlayer->GetGameSaveData();

			if (SaveData != NULL)
			{


				TankPawn->SetTankSpeed(TankPawn->MovementSpeed);

				TankPawn->Health = SaveData->PlayerState.VehicleState.Durability;
				TankPawn->HealthMax = TankPawn->Health;
				TankPawn->AmmoCap = -1;
				TankPawn->AmmoClipSize = SaveData->PlayerState.VehicleState.ShellClipSize;
				TankPawn->AmmoCount = TankPawn->AmmoCount;
				TankPawn->Alerter = SaveData->PlayerState.VehicleState.Alerter;
				TankPawn->ShellDamageAmount = SaveData->PlayerState.VehicleState.ShellDamageAmount;
				TankPawn->ShellReloadTime = SaveData->PlayerState.VehicleState.ShellReloadTime;

				FSavedVehicleWeaponState WeaponState = SaveData->GetWeaponStateByName(SaveData->PlayerState.VehicleState.LightWeaponName);
				if (WeaponState.IsValid())
				{
					TankPawn->SetupMountedWeapon(WeaponState);
				}
				NotifyHealthUpdated(TankPawn->HealthMax, TankPawn->Health);
				NotfiyHealthRefillUpdated(LocalPlayer->GetLivesCount());
				ReportShellReloaded(TankPawn->AmmoCount);
			}
		}
	}

}

FVector ABFTankPlayerController::GetTracedPoint()
{
	return FocusedPosition;
}

FVector ABFTankPlayerController::GetFireDirection()
{
	if (TankPawn !=NULL && TankPawn->ActiveCamera != NULL)
	{
		return TankPawn->ActiveCamera->GetForwardVector();
	}

	return GetViewTarget()->GetActorForwardVector();
}

bool ABFTankPlayerController::HasFocusedTarget()
{
	return  (Cast<ABFZombie>(FocusedTarget.Get())) != NULL;
}


void ABFTankPlayerController::AllowMovement()
{
	bAllowMovement = true;
}

void ABFTankPlayerController::DisallowMovement()
{
	bAllowMovement = false;
}

int32 ABFTankPlayerController::ThrowMine(UClass* ThrowableClass, float LocationX, float LocationY)
{
	UBFLocalPlayer* LocalPlayer= GetLocalPlayer();
	if (LocalPlayer != NULL)
	{
		 
		if (LocalPlayer->GetMinesCount() > 0)
		{
			LocalPlayer->AddMines(-1);
			EnterItemPlacement(ThrowableClass, LocationX, LocationY);
		} 
		return LocalPlayer->GetMinesCount();
	}
	return 0;
}

int32 ABFTankPlayerController::ThrowGrenade( UClass* ThrowableClass , float LocationX , float LocationY )
{
	UBFLocalPlayer* LocalPlayer = GetLocalPlayer() ;
	if ( LocalPlayer != NULL )
	{

		if ( LocalPlayer->GetGrenadesCount() > 0)
		{
			LocalPlayer->AddGrenades( -1 );
			EnterItemPlacement( ThrowableClass , LocationX , LocationY );
		}
		return LocalPlayer->GetGrenadesCount();
	}
	return 0;
}


void  ABFTankPlayerController::EnterItemPlacement( UClass* PlaceableController, float LocationX , float LocationY )
{
	
	if ( PlaceableController != NULL )
	{
		if ( ActivePlaceable != NULL )
		{
			IBFPlaceableGadgetInterface* PlaceableGadget = Cast<IBFPlaceableGadgetInterface>( ActivePlaceable->GetGadgetItem() );
			PlaceableGadget->ReleaseOnLocation(ActivePlaceable->GetActorTransform().GetLocation() );
			ActivePlaceable = NULL ;
		}		
		 
		FVector ForwardDir = GetFireDirection() ;
		
		bHandlingThrowable = true ;

		FTransform Transform = FTransform::Identity ;

		ActivePlaceable = Cast<ABFPlaceableItemController>( UGameplayStatics::BeginDeferredActorSpawnFromClass( GetWorld() , PlaceableController, Transform ));

		UGameplayStatics::FinishSpawningActor(ActivePlaceable, Transform );

		int32 Width, Height;
		GetViewportSize( Width , Height );

		FVector StartPosition  ;
		FVector WorldDirection ;

		UGameplayStatics::DeprojectScreenToWorld( this , FVector2D( Width / 2 , Height - ( Height / 4 )), StartPosition , WorldDirection );
		FVector EndPosition = StartPosition + WorldDirection * 20000 ;
	 
		if ( TankPawn != NULL )
		{
			EndPosition = TankPawn->GetActorLocation() ;
			//EndPosition.Z -= TankPawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() ;
			EndPosition += WorldDirection * 5000 ;
		}

		
		FCollisionQueryParams QueryParams ;
		QueryParams.AddIgnoredActor( TankPawn ) ;
		
		FCollisionResponseParams ResponseParams( ECR_Block ) ;
		
		FHitResult Result ;
		if ( GetWorld()->LineTraceSingleByChannel( Result , StartPosition , EndPosition , ECollisionChannel::ECC_Visibility , QueryParams , ResponseParams ) )
		{
			if ( Result.IsValidBlockingHit() )
			{
				Transform.SetLocation( Result.ImpactPoint );
			 
				ActivePlaceable->SetActorTransform( Transform );
				ActivePlaceable->SetActorEnableCollision( false );
				ActivePlaceable->Instigator = TankPawn ;
			}
		} 

		if ( TankPawn != NULL )
		{
			TankPawn->HideCannon() ;
		}
	}

}
 

void  ABFTankPlayerController::ExitItemPlacement()
{
	if (ActivePlaceable != NULL )
	{
		bHandlingThrowable = false ;  
		ActivePlaceable->SetActorEnableCollision( true );
		ActivePlaceable = NULL ;
	}

	if ( TankPawn != NULL )
	{
		TankPawn->ShowCannon() ;
	}
}

void ABFTankPlayerController::OnTouch( bool Pressed , bool Released , bool Moved , FVector2D Location , int32 FingerIndex )
{
	if ( bInCinematicMode ) 
		 return ; 

	if ( TankPawn == NULL )
		 return ; 

	if ( Pressed )
	{
		LastTouchPos	= Location ;
		LastTouchIndex	= FingerIndex ;

		if ( bHandlingThrowable && ActivePlaceable)
		{  
			ActivePlaceable->OnDragStart( this , Location.X , Location.Y );
		} 
	}
	else if ( Released )
	{
		AxisDelta = FVector2D( 0.0f , 0.0f ) ;

		if ( bHandlingThrowable && ActivePlaceable)
		{ 
			ActivePlaceable->OnDragEnd( this , Location.X , Location.Y );
			ExitItemPlacement() ;  
		}
	}
	else if ( Moved )
	{
		if ( bHandlingThrowable && ActivePlaceable)
		{ 
			ActivePlaceable->OnDragMove( this , Location.X , Location.Y );

		}else if ( LastTouchIndex == FingerIndex )
		{ 
			int32 Width , Height  ;
			GetViewportSize( Width , Height ) ;
			FVector2D ScreenDelta( Width / 3 , Height / 3 ) ;
			FVector2D Delta = Location - LastTouchPos ;   

			LastTouchPos = Location ; 

			TankPawn->UpdateViewRotation( Delta.X * ViewSmoothMultiplier , Delta.Y * ViewSmoothMultiplier ) ; 
		}	
	}

}

void ABFTankPlayerController::UpdateMovement( float DeltaSeconds )
{ 
	if ( PathSpline == NULL && TankPawn ==NULL ) return ; 
}

float ABFTankPlayerController::GetCurrentHealth()
{
	if ( TankPawn != NULL )
	{
		return TankPawn->Health < 0.0f ?  0.0f : TankPawn->Health ;
	}

	return 0.0f ;
}
float ABFTankPlayerController::GetArmorAmount()
{
	if ( TankPawn != NULL )
	{
		return TankPawn->ArmorStrength < 0.0f ?  0.0f : TankPawn->ArmorStrength	;
	}

	return 0.0f ;
}
 
void  ABFTankPlayerController::Tick( float DeltaSeconds )
{ 

	Super::Tick( DeltaSeconds )	;

	if( TankPawn != NULL ) 
	{  
		UpdateMovement( DeltaSeconds ) ;
 		 
		if ( BattleHUD != NULL )
		{
			BattleHUD->bShellReloading = IsReloading() ;
			BattleHUD->CannonRotation  = ( TankPawn->GetActorRotation().Yaw - TankPawn->TargetCannonRotationYaw ) ;
		} 

		UpdateTracePoint();
	}

} 
 
void ABFTankPlayerController::UpdateTracePoint()
{ 
	FVector WorldPosition  ;
	FVector WorldDirection ;
	int32 Width, Height ;
	GetViewportSize( Width , Height );
	const float FarTraceDist = 20000.0f ;
	 
	if ( BattleHUD != NULL && TankPawn != NULL ) 
	{
		FVector2D ScreenPosition = FVector2D( float(Width) * 0.5f , float(Height) * 0.5f ) ;

		UGameplayStatics::DeprojectScreenToWorld( this , ScreenPosition , WorldPosition , WorldDirection )	;	
		 
		FHitResult HitOut ; 
		FVector TraceEnd	= WorldPosition + WorldDirection * FarTraceDist	;

		FCollisionObjectQueryParams QueryParams( FCollisionObjectQueryParams::InitType::AllObjects ) ;
		QueryParams.AddObjectTypesToQuery( ECollisionChannel::ECC_Visibility ) ; 

		FCollisionQueryParams CollisionQueryParams ;
		CollisionQueryParams.bTraceComplex	= true ;
		CollisionQueryParams.AddIgnoredActor( TankPawn ) ;

		GetWorld()->LineTraceSingleByChannel( HitOut , WorldPosition , TraceEnd , ECollisionChannel::ECC_Visibility );

		if ( HitOut.IsValidBlockingHit() )
		{
			FocusedTarget =  ( HitOut.Actor.Get() ) ;
			FocusedPosition = HitOut.ImpactPoint ;			 
		}      
	} 
}

UBFLocalPlayer* ABFTankPlayerController::GetLocalPlayer()
{
	return Cast<UBFLocalPlayer>( Super::Player ) ;
}

void ABFTankPlayerController::PerformMovement( float XAxis , float YAxis )
{
	MovementDir = FVector2D( XAxis , YAxis ) ;

	if ( bAllowMovement && TankPawn != NULL && !TankPawn->bDestroyed )
	{ 
		AActor* View = TankPawn	;	 
		FVector Forward = View->GetActorForwardVector()	;
		FVector Right = -View->GetActorRightVector()	;

		Forward.Z = 0.0f	;
		Right.Z = 0.0f		;
		
		TankPawn->AddMovementInput(	Forward	, YAxis	)	;
		TankPawn->AddMovementInput( Right, XAxis )		;	 
	}
}

uint8 ABFTankPlayerController::GetTeamNum()const
{
	return TeamIndex ;
}

void ABFTankPlayerController::SetPath(ABFRoadSpline* Spline)
{    
	this->PathSpline = Spline ;
	DistanceTravelled = 0.0f  ;  
}

UBFGameSaveData* ABFTankPlayerController::GetSaveGameData()
{ 
	if ( Player != NULL )
		return Cast<UBFLocalPlayer>( Player )->GetGameSaveData();

	return NULL; 
}

void  ABFTankPlayerController::OnCrossHairMoveHorizontal(float Axis)
{    
	AxisDelta.X = Axis ;   
}

void  ABFTankPlayerController::OnCrossHairMoveVerticle( float Axis )
{     
	AxisDelta.Y = Axis ;   
} 

void  ABFTankPlayerController::OnCannonMoveVerticle( float Axis )
{    
	if ( TankPawn != NULL && !TankPawn->bDestroyed )
	{  
		AxisDelta.Y = Axis ;  
	}
}

void   ABFTankPlayerController::OnCannonMoveHorizontal( float Axis )
{    
	if ( TankPawn != NULL )
	{  
		AxisDelta.X = Axis ;    
	}   
}

void  ABFTankPlayerController::FireShell()
{    
 	if ( TankPawn != NULL && !TankPawn->bDestroyed )
	{   
		TankPawn->Fire( true ); 
	}   
}
void ABFTankPlayerController::StopShellFire()
{
	if ( TankPawn != NULL )
	{
		TankPawn->Fire( false );
	}
}

void ABFTankPlayerController::FireMountedGun()
{
	if ( TankPawn != NULL && !TankPawn->bDestroyed )
	{
		TankPawn->FireMountedWeapon();
	}
}

void ABFTankPlayerController::StopMountedGunFire()
{
	if (TankPawn != NULL)
	{
		TankPawn->StopWeaponFiring();
	}
}

void  ABFTankPlayerController::Possess(APawn* aPawn)
{     
	Super::Possess( aPawn );

	if (aPawn != NULL)
	{ 
		TankPawn = Cast< ABFPlayerTank >( aPawn );  
		if( TankPawn != NULL )
		TankPawn->SetTargeting( true );
	}   
}

bool ABFTankPlayerController::IsReloading()
{
	return TankPawn && TankPawn->IsReloadingShell() ;
}

void  ABFTankPlayerController::EnterZoomOptics_Implementation()
{
	if ( BattleHUD != NULL )
	{
		BattleHUD->bShowZoomOptics = true ;
		BattleHUD->bShowCrossHair = false ;
	}
}

void  ABFTankPlayerController::ExitZoomOptics_Implementation()
{
	if ( BattleHUD != NULL )
	{
		BattleHUD->bShowZoomOptics = false ;
		BattleHUD->bShowCrossHair = true ;
	}
} 

void ABFTankPlayerController::CauseDamage(EDamageType DamageType , float Damage, AActor* CausedBy ,FVector DamageBonePos)
{ 

	if ( TankPawn != NULL && !TankPawn->bDestroyed )
	{  
		ABFGameModeBase* GameMode = Cast<ABFGameModeBase>( GetWorld()->GetAuthGameMode() );

		if ( GameMode != NULL )
		{	
			float FinalDamage = GameMode->ModifyDamageTaken( Damage );

			FinalDamage = TankPawn->CauseDamage( FinalDamage );

			if ( FinalDamage > 0.0f )
			{
				if ( BattleHUD != NULL )
				{
					BattleHUD->AddDamageAlert( CausedBy->GetActorLocation() , CausedBy->GetActorRotation().Yaw , 0.1f );
					if ( FinalDamage > 40.0f )
					{
						FVector2D ScreenPosition ;
						float size = FMath::FRandRange( 0.2f , 0.5f ) ; 


						if (DamageBonePos.IsNearlyZero() == false)
						{
							UGameplayStatics::ProjectWorldToScreen(this, DamageBonePos, ScreenPosition, true) ;

						}
						else
						{ 
							  ScreenPosition = FVector2D(FMath::FRandRange(0.3f, 0.7f), FMath::FRandRange(0.3f, 0.7f)) ;
							  
						} 

						TankPawn->TriggerCameraJitter( 4.0f , 0.4f ) ; 
						
						BattleHUD->AddScratchMark(ScreenPosition, 90.0f, size , CausedBy->GetActorLocation()) ;
					}
				}
			} 


			NotifyHealthUpdated( TankPawn->HealthMax , TankPawn->Health ) ;

 			if ( TankPawn->Health <= 1 )
			{ 
				AttemptRevival() ; 
			}
		} 
	}

}
ABFPlayerState*  ABFTankPlayerController::GetPlayerState()
{
	return Cast<ABFPlayerState>(PlayerState) ;
}

void ABFTankPlayerController::OnRevivalSucceeded()
{
	if (TankPawn != NULL)
	{
		TankPawn->Health = TankPawn->HealthMax ;
		NotifyHealthUpdated(TankPawn->HealthMax, TankPawn->Health) ;
	}
}

void ABFTankPlayerController::OnRevivalCanceled()
{
	if (TankPawn != NULL)
	{
		TankPawn->Kill()	;

		TArray<AActor*> BossActors	;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABFZombieBoss::StaticClass(), BossActors) ;

		for (int i = 0 ; i < BossActors.Num() ; i++ )
		{
			ABFZombieBoss* Boss = Cast<ABFZombieBoss>(BossActors[i]) ;

			if (Boss != NULL)
			{
				Boss->bVictorious = true ;
			}
		}
	}
}

ABFGameHUD* ABFTankPlayerController::GetBattleHUD()
{
	return BattleHUD;
}
void ABFTankPlayerController::ReportShellReloaded( int32 NewAmmoCount )
{
	OnShellReloaded(NewAmmoCount, EWeaponMode::EWEAPONMODE_SHELL);
}

void ABFTankPlayerController::ReportWeaponReloaded(ABFWeapon* Weapon)
{
	OnWeaponReloaded(Weapon->AmmoCount, WeaponMode);
}

void  ABFTankPlayerController::SwitchToMountedWeapon_Implementation()
{

}

void  ABFTankPlayerController::SwitchToCannon_Implementation()
{

}

bool  ABFTankPlayerController::InCannonCamera()
{

	if (TankPawn != NULL)
	{
		return TankPawn->InCannonCamera();
	}

	return false;
}

 
 
void ABFTankPlayerController::OnPointsScoredForTarget_Implementation(AController* TargetKilled)
{

}

void ABFTankPlayerController::TriggerCameraShake_Implementation()
{

}

void ABFTankPlayerController::SwitchShellCannon()
{
	if (WeaponMode == EWeaponMode::EWEAPONMODE_MOUNTEDGUN)
	{
		if ( TankPawn != NULL && TankPawn->MountedWeapon != NULL )
		{
			if ( TankPawn->MountedWeapon->IsReloading() )
			{
				TankPawn->MountedWeapon->CancelReload()	 ;
				OnReloadCanceled(WeaponMode)			 ;
			}
		} 
	}

	WeaponMode = EWeaponMode::EWEAPONMODE_SHELL;

	if (TankPawn)
	{
		if (TankPawn->HasPendingReload())
		{
			TankPawn->ReloadShell()	;
		}
		
		OnAmmoUpdated(TankPawn->AmmoCount, TankPawn->AmmoClipSize);
		TankPawn->SwitchCannonCamera();
	}

	if (BattleHUD != NULL)
	{
		BattleHUD->bShowZoomOptics		= false ;
		BattleHUD->bShowCrossHair		= true  ;
		BattleHUD->bShowTurretCrosshair = false ;
	}

}

void ABFTankPlayerController::SwitchMountedGun()
{
	if (WeaponMode == EWeaponMode::EWEAPONMODE_SHELL)
	{

		if (TankPawn != NULL && TankPawn->IsReloadingShell())
		{
			TankPawn->CancelReload() ;
			OnReloadCanceled(WeaponMode) ;
		}

	}

	WeaponMode = EWeaponMode::EWEAPONMODE_MOUNTEDGUN ;

	if (TankPawn->MountedWeapon != NULL)
	{

		if ( TankPawn->MountedWeapon->HasPendingReload() )
		{

			TankPawn->MountedWeapon->ReloadClip() ;

		}
		 
		OnAmmoUpdated( TankPawn->MountedWeapon->AmmoCount , TankPawn->MountedWeapon->AmmoClipSize ) ; 
		TankPawn->SwitchMountedWeaponCamera() ;
		TankPawn->SetCannonRotation(0.0f);

	}

	if ( BattleHUD != NULL )
	{    

		BattleHUD->bShowZoomOptics		= false ;
		BattleHUD->bShowCrossHair		= false ;
		BattleHUD->bShowTurretCrosshair = true	;

	}
}
 

void ABFTankPlayerController::DestroyTank_Implementation()
{
	
}

void ABFTankPlayerController::ZoomForSelectedWeapon()
{   
	if ( bCannonWeaponMode )
	{
		
	}else if ( bMountedWeaponMode )
	{

	}  
} 
void ABFTankPlayerController::ReloadWeapon()
{
	if ( TankPawn != NULL && !TankPawn->bDestroyed )
	{

		if ( WeaponMode == EWEAPONMODE_MOUNTEDGUN && TankPawn->MountedWeapon != NULL )
		{

			if ( !TankPawn->MountedWeapon->IsReloading() && TankPawn->MountedWeapon->HasEnoughAmmo())
			{

				TankPawn->MountedWeapon->ReloadClip() ;

			}
		}
		else
		{
		 
			TankPawn->ReloadShellClip()	;
		}
	}
}

float ABFTankPlayerController::GetWeaponReloadingTime()
{
	if ( TankPawn != NULL )
	{

		if ( WeaponMode == EWEAPONMODE_MOUNTEDGUN && TankPawn->MountedWeapon != NULL )
		{

			return TankPawn->MountedWeapon->ClipReloadTime ;

		}
		else if ( WeaponMode == EWEAPONMODE_SHELL )
		{

			return TankPawn->ShellReloadTime ;

		} 
	} 

	return 0.0f ;
}

void ABFTankPlayerController::UpdateScoreForKilled_Implementation(ABFPlayerState* PS, ABFZombie* Killed)
{
	 
}

 
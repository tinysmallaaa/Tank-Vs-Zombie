// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Vehicle/BFPlayerTank.h"
#include "BFTeamInterface.h"
#include "InputCoreTypes.h"
#include "BFPlayerState.h" 
#include "BFCinematicCamera.h"
#include "BFRoadSpline.h"
#include "BFPlaceableGadgetInterface.h"
#include "Gadgets/BFPlaceableItemController.h"
#include "BFTankPlayerController.generated.h"

class UBFGameSaveData;
class ABFRoadSpline;
class ABFGameHUD;
class ABFThrowable; 

UENUM(BlueprintType)
enum EDamageType
{
	EZombie_Melee,
	EZombie_Projectile,
	EZombie_Blast,
};

UENUM(BlueprintType)
enum EWeaponMode
{
	EWEAPONMODE_SHELL,
	EWEAPONMODE_MOUNTEDGUN,
};


UCLASS(Blueprintable, Abstract)
class TANKVSZOMBIE_API ABFTankPlayerController : public APlayerController, public IBFTeamInterface
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "TankPlayerController")
		TEnumAsByte<EWeaponMode> WeaponMode	;
	UPROPERTY(BlueprintReadWrite, Category = "TankPlayerController")
		uint8 TeamIndex	;

	UPROPERTY(BlueprintReadWrite, Category = "TankPlayerController")
		ABFPlayerTank* TankPawn	;
	UPROPERTY(BlueprintReadWrite, Category = "TankPlayerController")
		bool bInCinematicMode	;

	UPROPERTY(BlueprintReadWrite, Category = "TankPlayerController")
		bool  GunSelected	;
	UPROPERTY(EditDefaultsOnly, Category = "TankPlayerController")
		float MoveSpeed	;

	UPROPERTY(EditDefaultsOnly, Category = "TankPlayerController")
		float MaxMoveSpeed	;
	UPROPERTY(EditDefaultsOnly, Category = "TankPlayerController")
		float Acceleration	;

	UPROPERTY(EditDefaultsOnly, Category = "TankPlayerController")
		float Damping	;  
	UPROPERTY(EditDefaultsOnly, Category = "TankPlayerController")
		float ViewSmoothMultiplier;

	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void OnCrossHairMoveVerticle(float Axis)	;
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void OnCrossHairMoveHorizontal(float Axis)	;

	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void OnCannonMoveVerticle(float Axis)		;
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void OnCannonMoveHorizontal(float Axis)		;

	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void FireShell()		;	
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void FireMountedGun()	;

	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void StopMountedGunFire()	; 
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void StopShellFire()		;

	virtual void Possess(APawn* aPawn) override	;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TankPlayerController")
		void EnterZoomOptics();
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		bool IsReloading();
	 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TankPlayerController")
		void ExitZoomOptics();
	UFUNCTION(BlueprintNativeEvent, Category = "TankPlayerController")
		void SwitchToMountedWeapon();

	UFUNCTION(BlueprintNativeEvent, Category = "TankPlayerController")
		void SwitchToCannon();
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		bool InCannonCamera();
 

	UFUNCTION(BlueprintNativeEvent, Category = "TankPlayerController")
		void TriggerCameraShake();
 

	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void SwitchShellCannon();
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void SwitchMountedGun();

	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void ZoomForSelectedWeapon();
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void UpdateTracePoint();

	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void UpdateMovement(float DeltaSeconds);
	UFUNCTION(BlueprintImplementableEvent, Category = "TankPlayerController")
		FHitResult TraceLine(FVector StartPosition, FVector EndPosition);

	UFUNCTION(BlueprintImplementableEvent, Category = "TankPlayerController")
		void NotifyHealthUpdated(float MaxHealth, float NewHealth);
	UFUNCTION(BlueprintImplementableEvent, Category = "TankPlayerController")
		void NotifiyArmorUpdated(float MaxArmorAmount, float NewArmorAmount);
	UFUNCTION(BlueprintImplementableEvent, Category = "TankPlayerController")
		void NotfiyHealthRefillUpdated(int32 NewRefillCount);

	
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void ApplyPlayerSettings();
	UFUNCTION(BlueprintCallable , Category = "TankPlayerController")
		void PlayEntryAnim(ABFRoadSpline* RoadSpline, ABFCinematicCamera* EntryCamera);  
	void UpdateCinematicAnim(float DeltaSeconds);


	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void PerformMovement(float XAxis, float YAxis);
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void SetPath(ABFRoadSpline* PathSpline);

	UFUNCTION(	BlueprintCallable	, Category = "TankPlayerController"	)
		ABFWorldSettings* GetWorldSettings();
	UFUNCTION(	BlueprintCallable	, Category = "TankPlayerController"	)
		UBFLocalPlayer* GetLocalPlayer();

	UFUNCTION(	BlueprintCallable	, Category = "TankPlayerController"	)
		UBFGameSaveData* GetSaveGameData();
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		ABFPlayerState* GetPlayerState();


	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void RefillHealth();
	UFUNCTION(BlueprintNativeEvent, Category = "TankPlayerController")
		void DestroyTank();

	virtual uint8 GetTeamNum() const override;
	void ScheduleReloadTimer(); 

	void SetTeamNum(uint8 TeamNum)
	{
		TeamIndex = TeamNum;
	}
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		float GetCurrentHealth();
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		float GetArmorAmount();

	UFUNCTION(BlueprintNativeEvent, Category = "TankPlayerController")
		void OnPointsScoredForTarget(AController* TargetKilled);
	UFUNCTION(BlueprintNativeEvent, Category = "TankPlayerController")
		void UpdateScoreForKilled(ABFPlayerState* PS, ABFZombie* Killed);
	 
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	void AnimateCamera(float DeltaSeconds);
	ABFGameHUD* GetBattleHUD();

	UFUNCTION(BlueprintImplementableEvent, Category = "TankPlayerController")
		void OnShellReloadBegin()	;
	UFUNCTION(BlueprintImplementableEvent, Category = "TankPlayerController")
		void OnClipReloadBegin(EWeaponMode Mode);
	UFUNCTION(BlueprintImplementableEvent, Category = "TankPlayerController")
		void OnAmmoUpdated(int32 NewAmmoCount,int32 ClipSize)	; 

	UFUNCTION(BlueprintImplementableEvent, Category = "TankPlayerController")
		void OnWeaponReloaded( int32 NewAmmoCount , EWeaponMode  mode )	;
	UFUNCTION(BlueprintImplementableEvent, Category = "TankPlayerController")
		void OnShellReloaded( int32 NewAmmoCount, EWeaponMode  mode )	;
	UFUNCTION(BlueprintImplementableEvent, Category = "TankPlayerController")
		void OnShellClipReloaded( int32 NewAmmoCount , int32 ClipSize );

	 UFUNCTION(BlueprintCallable , Category = "TankPlayerController" )
		void CauseDamage(EDamageType DamageType, float Damage, AActor* CausedBy, FVector DamageBonePos);
	 UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void ReloadWeapon();

	 UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		float GetWeaponReloadingTime();

		void ReportShellReloaded( int32 NewAmmoCount )	;
		 
		void ReportWeaponReloaded(ABFWeapon* Weapon)	;

	UFUNCTION(BlueprintImplementableEvent, Category = "TankPlayerController")
		void OnReloadCanceled(EWeaponMode Mode);

		bool HasFocusedTarget()	;

	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void OnTouch(bool Pressed, bool Released, bool Moved, FVector2D Location, int32 FingerIndex)	;

		FVector GetFireDirection();
	 
		FVector GetTracedPoint();
		FVector2D GetMovementAxis()
		{
			return MovementDir;
		}
		void AllowMovement();
		void DisallowMovement();

	UFUNCTION( BlueprintImplementableEvent , Category = "TankPlayerController" )
		void ReportEmptyAmmo() ;

	
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		int32 ThrowMine(	UClass* ThrowableClass	, float LocationX	, float LocationY	);
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		int32 ThrowGrenade(	UClass* ThrowableClass	, float LocationX	, float LocationY	);


	UFUNCTION( BlueprintCallable , Category = "TankPlayerController" )
		void EnterItemPlacement( UClass* PlaceableController , float LocationX , float LocationY );

		void ExitItemPlacement();

	UFUNCTION(BlueprintImplementableEvent, Category = "TankPlayerController")
		void AttemptRevival(); 

	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void OnRevivalSucceeded();

	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		void OnRevivalCanceled();
		 
	UFUNCTION(BlueprintCallable, Category = "TankPlayerController")
		bool HasEnoughHealthRefill();


private:

	float							ShakeStartTime		;
	float							CurrentShakeTime	;

	bool							bAllowMovement		;
	float							ShellLastFireTime	;
	float							ReloadTimeMultiplier; 
	bool							bMountedWeaponMode	;
	bool							bCannonWeaponMode	;
	ABFGameHUD						*BattleHUD			;
	FVector2D						AxisDelta			;
	ABFRoadSpline*					PathSpline			;
	FVector2D						MovementDir			;
	FVector2D						LastTouchPos		;
	int32							LastTouchIndex		;
	float							PrevMovementDir		;
	float							CurrentSpeed		;
	float							DistanceTravelled	;
	TWeakObjectPtr<AActor>			FocusedTarget		;
	FVector						    FocusedPosition		;
	
	ABFRoadSpline				   *EntryAnimSpline		;
	bool						   bPlayingEntryAnim	;
	float						   AnimDistance			;

	bool							bHandlingThrowable	;
	ABFPlaceableItemController*		ActivePlaceable;

};

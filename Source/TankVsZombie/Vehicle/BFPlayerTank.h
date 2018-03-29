#pragma once
#include "BFTankVehicle.h"
#include "Weapon/BFWeapon.h"
#include "../BFGameSaveData.h"
#include "BFVehicleSideSpline.h"
#include "Weapon/BFWeaponAttachment.h"
#include "BFPlayerTank.Generated.h"

  
USTRUCT(BlueprintType)
struct FZombieAttachment
{
	GENERATED_USTRUCT_BODY()

	struct FVehicleAttachmentSlot*	SlotAttachedTo	;
	class  ABFZombie* Zombie	;
};

class ABFZombie;
class ATankPlayerController;
class UBFVehicleSideSpline;
UCLASS( Blueprintable , Abstract )
class TANKVSZOMBIE_API ABFPlayerTank : public ABFTankVehicle
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY( Category = BFPlayerTank, VisibleAnywhere , BlueprintReadOnly , meta = (AllowPrivateAccess = "true") )
		UBFVehicleSideSpline*		LeftSideSpline	;
	UPROPERTY( Category = BFPlayerTank, VisibleAnywhere , BlueprintReadOnly , meta = (AllowPrivateAccess = "true") )
		UBFVehicleSideSpline*		RightSideSpline	;
	UPROPERTY( Category = "BFPlayerTank" , VisibleAnywhere , BlueprintReadOnly , meta = (AllowPrivateAccess = "true") )
		USpringArmComponent*	SpringArm ;
	
	UPROPERTY( Category = "BFPlayerTank" , VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true") )
		UCameraComponent*		ThirdPersonCamera	;		
	UPROPERTY( Category = "BFPlayerTank" , VisibleAnywhere , BlueprintReadOnly , meta = (AllowPrivateAccess = "true") )
		UCameraComponent*		CannonCamera ; 
 	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Tank|Cannon" )
		FName					MountedWeaponSocket ;

	UPROPERTY(	Category = "BFPlayerTank|Audio" , VisibleAnywhere	, BlueprintReadOnly	, meta = (AllowPrivateAccess = "true") )
		UAudioComponent*		EngineAudioComp ;	 
	UPROPERTY(	Category = "BFPlayerTank|Audio"	, VisibleAnywhere	, BlueprintReadOnly	, meta = (AllowPrivateAccess = "true") )
		UAudioComponent*		EngineAudioLoop	;
	UPROPERTY( EditDefaultsOnly , Category = "Tank|Cannon" )
		TArray<TSubclassOf<UBFWeaponAttachment>> WeaponAttachmentsList ;


	UPROPERTY(BlueprintReadWrite, Category = "BFPlayerTank")
		int32					AmmoCount;
	UPROPERTY(BlueprintReadWrite, Category = "BFPlayerTank")
		int32					AmmoClipSize;
	UPROPERTY(BlueprintReadWrite, Category = "BFPlayerTank")
		int32					AmmoCap; // -1 for infinite  

	UPROPERTY(BlueprintReadWrite, Category = "BFPlayerTank")
		float					NitroAmount;
	UPROPERTY(BlueprintReadWrite, Category = "BFPlayerTank")
		float					Alerter;
	UPROPERTY(BlueprintReadWrite, Category = "BFPlayerTank")
		float					ShellDamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "BFPlayerTank|Cannon")
		float					ShellReloadTime;
	UPROPERTY(EditDefaultsOnly, Category = "BFPlayerTank|Cannon")
		float					CameraJitterStrength;


	UPROPERTY( EditDefaultsOnly , Category = "BFPlayerTank|Audio" )
		 float				   StableEngineVolume ;
	UPROPERTY( EditAnywhere , BlueprintReadOnly , Category = "BFPlayerTank|Cannon" )
		float				   CannonRotationPitchMin ;
	UPROPERTY( EditAnywhere , Category = "BFPlayerTank|Cannon" )
		float				   CannonRotationPitchMax ;

	UPROPERTY( EditAnywhere , BlueprintReadOnly , Category = "BFPlayerTank|Cannon" )
		float				   CannonZoomedPitchMin ;
	UPROPERTY( EditAnywhere, Category = "BFPlayerTank|Cannon" )
		float				   CannonZoomedPitchMax	;
	UPROPERTY( EditAnywhere , Category = "BFPlayerTank|Cannon" )
		float				   CannonRotationYawMin	;	

	UPROPERTY( EditAnywhere , Category = "BFPlayerTank|Cannon" )
		float				   CannonRotationYawMax	;
	UPROPERTY( EditDefaultsOnly , Category = "BFPlayerTank|Cannon" )
		float				   TankOffset ;		
	UPROPERTY( EditDefaultsOnly , Category = "BFPlayerTank|Cannon" )
		float				   UpperBodyOffsetYaw ;

	UPROPERTY( BlueprintReadWrite , Category = "BFPlayerTank|Cannon" )
		bool					bMountedWeaponFiring ;
	UPROPERTY( BlueprintReadWrite , Category = "BFPlayerTank|TankPlayerController" )
		bool					IsRigidCannonRotation ;
	UPROPERTY( BlueprintReadWrite , Category = "BFPlayerTank" )
		ABFWeapon*				MountedWeapon		; 

	UPROPERTY( BlueprintReadWrite , Category = "BFPlayerTank|Cannon" )
		bool					bIsReloading ;
	UPROPERTY( EditDefaultsOnly	, BlueprintReadWrite , Category = "BFPlayerTank|Cannon" )
		UCurveFloat*			CameraJitterCurve ;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = "BFPlayerTank|Cannon" )
		FVector					CameraJitterMask ;

	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		ReleaseCamera() ;
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		SwitchCamera() ;
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		SwitchThirdPersonCamera() ;

	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		SwitchCannonCamera() ;   
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		SwitchMountedWeaponCamera();
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		SetCannonRotation( float RotationInDegree );

	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		SetUpperbodyRotation(float RotationInDegree);
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		UpdateViewRotation( float DeltaYaw , float DeltaPitch ) ;	 
		void		UpdateCannonOrientation( float DeltaSeconds ) ;

	virtual void	PossessedBy(AController* NewController) override ;
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		SetTarget( AActor* Target ) ;
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		SetTargetLocation(FVector TargetLocation) ;

	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		SetTargeting( bool Targeting ) ;
		void		UpdateTargeting( float DeltaSeconds ) ;		
		bool		InCannonCamera() ; 

		bool		InMountedWeaponCamera();
	virtual void	BeginPlay() override ; 
	virtual void	Tick(float DeltaSeconds) override ;
	  

	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		Fire( bool bFirePressed ) ;
	virtual void	FireProjectile() override ; 
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		FVector		GetFireLocation() ;
	
		FVector		GetCannonFireDirection() ;
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		FVector		GetFireDirection() ;
		void		UpdateCameraJitterAnim( float DeltaSeconds ) ;


	UFUNCTION( BlueprintCallable ,Category = "BFPlayerTank" )
		void		TriggerCameraJitter( float Strength , float PlayLength ) ;
		void		SetTankSpeed(float Speed) ;
	UFUNCTION( BlueprintNativeEvent , Category = "BFPlayerTank" )
		void		UpdateSoundParams( float Speed , bool bDeccelerating , bool bStopped ) ;
	
	virtual void	OnHealthUpdated( float NewHealth ) override ; 
	virtual void	OnArmorUpdated( float NewArmorAmount ) override ; 
	UFUNCTION( BlueprintNativeEvent , Category = "BFPlayerTank" )
		void		EmitSmokeEffects(float Speed) ; 

		void		FireMountedWeapon() ; 
		void		StopWeaponFiring()  ;
	UFUNCTION()
		void		FireCannon() ; 

	UFUNCTION()
		void		ReloadShell() ;
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		ReloadShellClip();
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		HideCannon();

	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		ShowCannon();
	UFUNCTION()
		void		ShellReloaded() ;
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		SetupMountedWeapon( FSavedVehicleWeaponState WeaponState ); 

	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		AttachMountedGun( FSavedVehicleWeaponState WeaponState ) ;
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		DetachMountedGun( bool ForceDestroy ) ;
	void			PostInitSetup();

	const UBFWeaponAttachment* GetWeaponAttachment(	TSubclassOf<ABFWeapon> Weapon	) ;	

	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		OnMountedWeaponCameraSwitched();
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		OnCannonCameraSwitched();
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		OnThirdPersonCameraSwitched(); 

	void			BlendToNewViewTarget( float BlendTime , AActor* Target );
	UFUNCTION( BlueprintNativeEvent , Category = "BFPlayerTank" )
		void		UpdateMaintainenceSound( int32 CriticalHealthLevel ) ;
	void			ReportWeaponClipReloaded() ; 

	bool			IsReloadingShell() ;
	inline bool		HasPendingReload(){	return bHasPendingReload ;	}
	UFUNCTION( BlueprintCallable , Category = "BFPlayerTank" )
		void		RefillHealth();

	bool			IsOutOfAmmo() ;
	float			CauseDamage( int32 DamageToApply ) ;  
	void			CancelReload();

	FVehicleAttachmentSlot* GetNearestAttachmentSlot( ABFZombie* Zombie	);
	void			AttachZombieToSlot( FVehicleAttachmentSlot* Slot , ABFZombie* Zombie );
	void			DetachZombie( ABFZombie* Zombie );
	void			UpdateAttachedZombies(	float DeltaSeconds );

public: 

	TArray<FZombieAttachment> AttachedZombies;
	
	bool						bHasPendingReload	;
	bool						bInfiniteAmmo		;

	bool						bIsFirePressed		;

	float						CameraJitterPlayLength;
	bool						bCameraJitterAnim	; 
	float						JitterAnimStartTime ;	 
	FVector						PreJitterSavedCameraLocation ;
	float						CameraJitterAmount	;

	ABFWeapon*					ActiveWeapon		;
	TArray<ABFWeapon*>			AttachedWeapons		; 
	FTimerHandle				ReloadingTimer		;
	
	FRotator					InitialRotation		 ;
	float						LastCameraSwitchTime ;	 
	bool						bIsMountedGunActive  ;

	bool						bIsTargeting			 ;   
	FVector						TargetLocation			 ;	

	float						CurrentCannonRotationYaw ; 
	float						CurrentCannonRotationPitch  ;	
	
	float						AccumulatedRotationPitch;
	float						AccumulatedRotationYaw;

	float						TargetCannonRotationPitch   ;
	float						TargetCannonRotationYaw		;
};

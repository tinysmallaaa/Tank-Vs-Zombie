#pragma once  
#include "BFProjectile.h"
#include "../BFGameSaveData.h"
#include "BFWeapon.Generated.h"
 
#define DEFAULT_FIRE_RATE 5

UCLASS( Blueprintable , Abstract )
class TANKVSZOMBIE_API ABFWeapon : public  AActor
{ 
	GENERATED_UCLASS_BODY() 
public:

	UPROPERTY( Category = "BFBaseWeapon"  , VisibleDefaultsOnly , BlueprintReadOnly , meta = (AllowPrivateAccess = "true") )
		USkeletalMeshComponent*		Mesh			; 
	UPROPERTY( Category = "BFBaseWeapon"  , VisibleDefaultsOnly , BlueprintReadOnly , meta = (AllowPrivateAccess = "true") )
		UParticleSystem*			MuzzleEffectFX	;
	UPROPERTY( Category = "BFBaseWeapon"  , VisibleDefaultsOnly , BlueprintReadOnly , meta = (AllowPrivateAccess = "true") )
		UAudioComponent*			FiringSoundComp	;
	UPROPERTY( Category = "MountedWeapon" , VisibleDefaultsOnly , BlueprintReadOnly , meta = (AllowPrivateAccess = "true") )
		UCameraComponent*			CameraComponent;

 	UPROPERTY()
		class UBFWeaponAttachment*	Attachment		;
	UPROPERTY( BlueprintReadWrite	,	Category = "BFBaseWeapon" )
		int32						AmmoCount		;
	UPROPERTY(  BlueprintReadWrite	,	Category = "BFBaseWeapon" )
		int32						AmmoClipSize	;

	UPROPERTY( BlueprintReadWrite	,	Category = "BFBaseWeapon" )
		int32						AmmoCap			;//-1 for infinite 
	UPROPERTY(BlueprintReadWrite	,	Category = "BFBaseWeapon" )
		bool						bInfiniteAmmo	;
	UPROPERTY(  BlueprintReadWrite	,	Category = "BFBaseWeapon" )
		float						DamageRadius	;

	UPROPERTY(  BlueprintReadWrite	,	Category = "BFBaseWeapon" )
		int32						FireRate			;
	UPROPERTY( BlueprintReadWrite	,	Category = "BFBaseWeapon" )
		float						ClipReloadTime		;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = "BFBaseWeapon")
		float						DamageMultiplier	;//Power 

	UPROPERTY( EditDefaultsOnly		,	category = "BFBaseWeapon" )
		float						InstantHitDamage	;
	UPROPERTY(EditDefaultsOnly		,	category = "BFBaseWeapon" )
		float						InstantHitImpulse	;
	UPROPERTY(BlueprintReadWrite	,	Category = "BFBaseWeapon" )
		float						AimSpread			;

	UPROPERTY(EditDefaultsOnly		,	Category = "BFBaseWeapon" )
		float						MinAimSpread	 ;
	UPROPERTY(EditDefaultsOnly		,	Category = "BFBaseWeapon" )
		float						MaxAimSpread	 ; 
	UPROPERTY( EditDefaultsOnly		,	Category = "BFBaseWeapon" )
		float						FireOffset		 ;

	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly		,	Category = "Weapon" )
		TSubclassOf<ABFProjectile>		ProjectileClass			;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite	,	Category = "BFBaseWeapon" )
		UParticleSystem*				InstantFireFX			; 
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite	,	Category = "BFBaseWeapon" )
		TSubclassOf<ABFImpactEffect>	InstantHitImpactFXClass ;

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite	,	Category = "BFBaseWeapon" )
		FName					FireLocationSocket	;
	UPROPERTY(EditDefaultsOnly,  Category = "BFBaseWeapon")
		bool					bMultiBarrelEnabled	;
	UPROPERTY(EditDefaultsOnly, Category = "BFBaseWeapon")
		int32					MultiFireCount		;
	
	UPROPERTY(  BlueprintReadWrite	,	Category = "BFBaseWeapon" )
		float					FiringDelay			;
	UPROPERTY(  BlueprintReadWrite	,	Category = "BFBaseWeapon" )
		bool					bFiringLoop			;
	UPROPERTY(EditAnywhere ,Category = "BFBaseWeapon" )
		TSubclassOf<ABFProjectile>	InstantProjectileClass	;	

	UPROPERTY( BlueprintReadOnly	,	Category = "BFBaseWeapon"	)
		AActor*						WeaponOwner		;
	UPROPERTY(	BlueprintReadWrite	,	Category = "BFBaseWeapon"	)
		UBFWeaponAttachment*		WeaponAttachment	;
	UPROPERTY(	EditDefaultsOnly	, BlueprintReadWrite	,	Category = "BFBaseWeapon" )
		TSubclassOf<UBFWeaponAttachment> WeaponAttachmentClass	;	 
	UPROPERTY(	EditDefaultsOnly	, BlueprintReadWrite	,	Category = "BFBaseWeapon"	)
		TSubclassOf<UBFDamageType>	DamageType	;
	 

	UFUNCTION( BlueprintNativeEvent		,		Category = "BFBaseWeapon"	)
		FVector		GetFireLocation()	;
	UFUNCTION( BlueprintNativeEvent		,		Category = "BFBaseWeapon"	)
		FVector		GetFireDirection()	;
	virtual FVector CalculateFireLocation(FVector Direction);


	virtual bool	CanFire()			;
	virtual UCameraComponent* GetAttachedCamera(){return NULL; 	} 
	UFUNCTION(	BlueprintCallable		,		Category = "BFBaseWeapon"	)
			void	FirePressed(bool bPressed); 


	UFUNCTION(	BlueprintCallable		,		Category = "BFBaseWeapon"	)
			void	StopFire()			;
	UFUNCTION()
			void	ClipReloaded()		;
			void	PauseFire()			;


			void	ResumeFire()		;
	UFUNCTION()
			void	ReloadClip()		;		
	virtual void	Tick(float DeltaSeconds) override;


	UFUNCTION(	BlueprintCallable		,		Category = "BFBaseWeapon"	)
			bool	HasEnoughAmmo()		;	
			void	CancelReload()		;			 
	UFUNCTION()
	virtual void	Fire()				;


	virtual bool	FireWeapon(	const FVector Direction	)						;
	virtual bool	FireWeapon(	AActor* Target	, FVector TargetLocation	)	;
	virtual void	FireProjectile(	const FVector FireLocation	, const FVector Direction	);


	virtual void	FireInstantHit(	const FVector FireLocation	, const FVector Direction	);
	UFUNCTION(BlueprintImplementableEvent, Category = "BFBaseWeapon")
			void	SpawnInstantBeamFX(		FVector StartLocation	,	FVector EndLocation	);
	virtual void	BeginPlay() override;


	virtual void	PostInitializeComponents();
			void	AttachToOwner(	USkeletalMeshComponent* ParentMesh	);
			void	DetachFromOwner();

	UFUNCTION(	BlueprintCallable		,		Category = "BFBaseWeapon" )
			APawn*  GetOwnerPawn()		;
	UFUNCTION(	BlueprintCallable		,		Category = "BFBaseWeapon"	)
			AActor* GetOwner()			;


	FVector CalculateFireDirection( FVector FireLocation ,  FVector FireDirection )  ;
	inline bool		IsReloading()	   { 	return bClipReloading;	  }	 
	inline bool		HasPendingReload() {    return bHasPendingReload; }


	UFUNCTION(	BlueprintNativeEvent	,		Category = "BFBaseWeapon"	)
			void	StartFiringFX()		;
	UFUNCTION(	BlueprintNativeEvent	,		Category = "BFBaseWeapon"	)
			void	StopFiringFX()		;
	UFUNCTION(	BlueprintNativeEvent	,		Category = "BFBaseWeapon"	)
			void	PlayFiringSoundFX() ; 


	UFUNCTION(	BlueprintCallable		,		Category = "BFBaseWeapon" )
			void	Setup(  FSavedVehicleWeaponState& WeaponState) ;
	virtual void	PostFireEffects() {}
	UFUNCTION(	BlueprintCallable		,		Category = "BFBaseWeapon")
			void	InitWeapon(  FSavedVehicleWeaponState& WeaponState)	;

private:
	int32		 FireLocaionItr		;
	FTimerHandle NextFireTimer		;
	FTimerHandle ClipReloadingTimer	;

	float		 LastFireTime		;
	float		 FireDelay			;
	bool		 bFirePressed		;

	bool		 bClipReloading		;
	bool		 bHasPendingReload	;
	bool		 bWeaponInitialized	;
};
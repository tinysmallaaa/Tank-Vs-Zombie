#pragma once
#include "Gameframework/Character.h"
#include "BFProjectile.h" 
#include "BFVehiclePawn.h"
#include "BFTankVehicle.generated.h"
 
class UBFMountedWeaponAttachment;
class UBFWeaponAttachment;

USTRUCT(BlueprintType)
struct FVehicleComponentInfo
{

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "TankComponent")
		FName Name;
	
	UPROPERTY(EditAnywhere, Category = "TankComponent")
		FName Description;
	
	UPROPERTY(EditAnywhere, Category = "TankComponent")
		UTexture* IconTexture;
	
	UPROPERTY(EditAnywhere, Category = "TankComponent")
		int32  Cost;
	
	UPROPERTY(EditAnywhere, Category = "TankComponent")
		TSubclassOf<UBFMountedWeaponAttachment> WeaponClass;

	UPROPERTY(EditAnywhere, Category = "TankComponent")
		TSubclassOf<UBFWeaponAttachment> WeaponAttachmentClass;

};

 


UCLASS(Blueprintable,Abstract)
class TANKVSZOMBIE_API ABFTankVehicle : public ABFVehiclePawn 
{
	GENERATED_UCLASS_BODY()

public:
 
	UPROPERTY( Category = Vehicle , VisibleAnywhere , BlueprintReadOnly , meta = (AllowPrivateAccess = "true") )
		UStaticMeshComponent*		VehicleBody;
	UPROPERTY( Category = Vehicle , VisibleAnywhere , BlueprintReadOnly , meta = (AllowPrivateAccess = "true") )
		UStaticMeshComponent*		CannonBody ;
	UPROPERTY( Category = Vehicle , VisibleAnywhere , BlueprintReadOnly , meta = (AllowPrivateAccess = "true") )
		UStaticMeshComponent*		UpperBody ;

	UPROPERTY( Category = Vehicle , VisibleAnywhere , BlueprintReadOnly , meta = (AllowPrivateAccess = "true") )
		UParticleSystemComponent*	MuzzleFXComp;
	UPROPERTY( Category = Vehicle , VisibleAnywhere , BlueprintReadOnly , meta = (AllowPrivateAccess = "true") )
		UParticleSystem*			DamageSmokeEffects;
	UPROPERTY( Category = Vehicle , VisibleAnywhere , BlueprintReadOnly , meta = (AllowPrivateAccess = "true") )
		UParticleSystem*			CannonMuzzleFX;

	UPROPERTY(  EditAnywhere , Category = "Vehicle|Cannon" )
		TSubclassOf<ABFProjectile>	ProjectileClass ;	  
	UPROPERTY(  EditAnywhere , Category = "Vehicle|Cannon" )
		float	CannonFireOffset ;
	UPROPERTY(  EditAnywhere , Category = "Vehicle" )
		int32	CollisionCount	 ;
	 
	UPROPERTY(	EditDefaultsOnly	, Category = "Vehicle")
		float	WorldYawOffset	 ;
	UPROPERTY(	EditAnywhere , Category = "Vehicle" )
		float	BrakeRatioLeft	 ;
	UPROPERTY(	EditAnywhere , Category = "Vehicle" )
		float	LeftTreadSpeed	 ;
	UPROPERTY(	EditAnywhere , Category = "Vehicle" )
		float	RightTreadSpeed	 ;	
	 
	UPROPERTY( BlueprintReadWrite , Category = "Vehicle")
		bool bDestroyed		 ;
	UPROPERTY(BlueprintReadWrite, Category = "Vehicle")
		bool bPositionLocked ;
	UPROPERTY( EditAnywhere , Category = "Vehicle" )
		TArray<FVehicleComponentInfo> AttachedComponents ; 
	UPROPERTY(	BlueprintReadWrite , Category = "Vehicle" )
		UCameraComponent*	ActiveCamera ; 
	
	UPROPERTY(	BlueprintReadWrite	, Category = "Vehicle" )
		float	MaxSpeed ;
	UPROPERTY(	BlueprintReadWrite	, Category = "Vehicle" )
		int32   Health ;
	UPROPERTY(	BlueprintReadWrite  , Category = "Vehicle" )
		int32	HealthMax ;
	UPROPERTY(	BlueprintReadWrite	, Category = "Vehicle" )
		int32	ArmorStrength ;

	UPROPERTY(	BlueprintReadWrite  , Category = "Vehicle" )
		float	MovementSpeed  ;
	UPROPERTY(  BlueprintReadWrite	, Category = "Vehicle")
		float   DamageMultiplier ;	
	UPROPERTY(	EditAnywhere	, Category = "Vehicle")
		float HealthBarDrawOffsetY ;
	UPROPERTY(	EditDefaultsOnly , BlueprintReadWrite , Category = "Vehicle" )
		TSubclassOf<AActor> DestroyedVehicleClass	;	 
	 
	UPROPERTY(	BlueprintReadWrite	, Category = "Vehicle")
	UMaterialInstanceDynamic* LeftTrackMatInst ;
	UPROPERTY(	BlueprintReadWrite	, Category = "Vehicle")
	UMaterialInstanceDynamic* RightTrackMatInst ;
	UPROPERTY(	BlueprintReadWrite	, Category = "Vehicle")
	UMaterialInstanceDynamic* LeftWheelsMatInst ;
	UPROPERTY(	BlueprintReadWrite	, Category = "Vehicle")
	UMaterialInstanceDynamic* RightWheelsMatInst ;

	UFUNCTION(	BlueprintCallable	, Category = "Vehicle")
		void		Kill();
	UFUNCTION(	BlueprintCallable	, Category = Pawn)
		bool		IsDead() const;
	UFUNCTION(	BlueprintNativeEvent	, Category = "Vehicle")
		void		DestroyVehicle(); 
	UFUNCTION(	BlueprintNativeEvent	, Category = "Vehicle")
		void		OnShellFired(FVector FiredLocation, FVector FiredDirection);

	class  UBFTankMovementComponent* GetTankMovementComponent() const;


	virtual bool	ModifyDamageTaken( int32& AppliedDamage , int32 DamageToApply , const FHitResult& HitInfo , AController* EventInstigator , AActor* DamageCauser , TSubclassOf<UDamageType> DamageType );
	virtual float	TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;	
	virtual bool	ShouldTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const;
	virtual void	SpawnDamageEffects(bool RadialDamage , const FHitResult& HitInfo);

	virtual void	SpawnArmorDamageEffects();
	virtual void	OnHealthUpdated( float NewHealth );
	virtual void	OnArmorUpdated( float NewArmorAmount ); 
	virtual FVector GetTargetLocation(AActor* RequestedBy) const override;
	 
	UFUNCTION(	BlueprintCallable		, Category = "BaseVehicle" )
	virtual void	EndEncounterIfAny() {}
	UFUNCTION(	BlueprintNativeEvent	, Category = "BaseVehicle" )
		void		UpdateTreadMotions(float DeltaSeconds,float NormalizedSpeed, float TurnDelta); 
	virtual void	UpdateTrackWheelsParam(float DeltaSeconds);

	

public:
	
	// Called when the game starts or when spawned
	virtual void	BeginPlay() override;
	virtual void	PossessedBy(AController* NewController) override; 
	virtual void	FireProjectile();
	// Called every frame
	virtual void	Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void	SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//set by controller 
	float YawDelta;
	float LastYaw;

	float TreadSpeed;
	float WheelSpeed;
	float PrevTrackDir;

	float LeftWheelRotation;
	float RightWheelRotation;
	float LeftTrackOffset;
	float RightTrackOFfset;

	
};


inline bool ABFTankVehicle::IsDead() const
{
	return bTearOff || IsPendingKillPending();
}

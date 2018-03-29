#pragma once 
#include "Weapon/BFWeapon.h"
#include "BFAIInfo.h"
#include "BFAIBot.Generated.h"
 
class ABFEncounterVolume	;
class ABFAIBotAIController	;

struct FBotDamageParamEx
{
	TSubclassOf<UDamageType>	DamageType;
	bool					    bHeadShot;
	bool						bVehicleSmashed;
	bool						bRoadKill;
	FVector						HeadBoneLocation;
};

UCLASS( Blueprintable , Abstract )
class TANKVSZOMBIE_API ABFAIBot : public ACharacter
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY( VisibleAnywhere , BlueprintReadWrite , Category = "BFBot" )
		USphereComponent* HeadCollisionComponent;

	UPROPERTY()
		bool bStartingRagdoll;

	UPROPERTY( EditAnywhere , Category = "BFBot" )
		int32 Health	;
	UPROPERTY( EditAnywhere , Category = "BFBot" )
		int32 HealthMax	;
	UPROPERTY( EditAnywhere , Category = "BFBot" )
		float HealthBarDrawOffsetY	;
	UPROPERTY(EditAnywhere, Category = "BFBot" )
		bool bDrawHealthBar;

	UPROPERTY( EditAnywhere , Category = "BFBot" )
		float AlertnessTime	;
	UPROPERTY( EditAnywhere , Category = "BFBot" )
		bool bAllowCrouch	;
	UPROPERTY( EditAnywhere , Category = "BFBot" )
		float AimSpread		;

	UPROPERTY( EditAnywhere , Category = "BFBot" )
		float DamageMultiplier	;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "BFBot" )
		bool  bAllowMobility	;
	UPROPERTY( EditAnywhere , Category = "BFBot" )
		UAnimMontage* FireMontage	;

	UPROPERTY( EditAnywhere , Category = "BFBot" )
		UAnimMontage* FireCrouchMontage	;
	UPROPERTY( EditAnywhere , Category = "BFBot" ) 
		TSubclassOf<ABFProjectile>	ProjectileClass	;
	UPROPERTY( EditAnywhere , Category = "BFBot" )
		TSubclassOf<ABFWeapon>		WeaponClass ;

	UPROPERTY( BlueprintReadWrite , Category = "BFBot" )
		ABFWeapon* AttachedWeapon ;
	UPROPERTY( BlueprintReadOnly , Category = "BFBot" )
		float RagdollGravityScale ;
	UPROPERTY( EditDefaultsOnly , Category = "BFBot" )
		float RagdollLifeSpan ;

	UPROPERTY( EditDefaultsOnly , Category = "BFBot" )
		bool bDetachWeaponInRagdollMode ;
	UPROPERTY( EditDefaultsOnly , Category = "BFBot" )
		bool bRagdollOnDeath ;
	UPROPERTY( EditDefaultsOnly , Category = "BFBot")
		bool bFirstDamageReceived; 
 
		virtual void BeginPlay()	override ;
		virtual void Tick(float DeltaSeconds)	override;
		virtual void PossessedBy(AController* AIController)	override;

		UFUNCTION(BlueprintCallable, Category = "BFBot" )
			bool IsAlive();
		UFUNCTION( BlueprintCallable , Category = "BFBot" )
			void InitializeWeapon();
		virtual void Fire();

		virtual bool CanFire();
		virtual FVector GetFireLocation() const;
		virtual FVector GetFireDirection() const;

		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
		virtual void FireProjectile(FVector FireLocation, FVector FireDirection);
		virtual void Reload();

		UFUNCTION( BlueprintCallable , BlueprintAuthorityOnly , Category = "BFBot|AI" )
		virtual void SetTarget(AActor* NewTarget);

		virtual void DamageReceived() {}
		
		inline AActor* GetTarget() const { return CurrentTarget ; }

		UFUNCTION( BlueprintCallable , Category  = "BFBot" )
		bool IsRagdoll() const;
		
		UFUNCTION( BlueprintCallable , Category = Pawn)
		bool IsDead() const;

		void UpdateTargeting(float DeltaSeconds);		

		virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;		
		virtual FVector GetTargetLocation(AActor* RequestedBy = NULL) const override;
		virtual void Kill( FVector Momentum ,const FBotDamageParamEx& DamageParams);


		void StartRagdoll(FVector Momentum);
		inline bool IsSimulatingRagdoll() { 
			return bInRagdoll; 
		}
		void DetachWeapon(bool ForceDestroy);
		UFUNCTION( BlueprintCallable , Category = "BFBot" )
			void AllowWeaponFiring(bool Allowance);
		UFUNCTION()
			void DestroyBot(); 

		void SetRagdollGravityScale(float NewScale);
		virtual float ModifyDamageTaken(float Damage,FBotDamageParamEx DamageParams);
		void SetEncounterVolume(ABFEncounterVolume* Volume)  ;

		UFUNCTION( BlueprintCallable , Category = "BFBot" )
	 	void EndEncounterIfAny();		
		UFUNCTION(BlueprintNativeEvent, Category = "BFBot")
			void OnKilled(bool WasHeadShot);
		UFUNCTION( BlueprintCallable , Category = "BFBot")
		void SetRandomRVO(int32 MaxMask);

		
public: 

	FVector							    SavedLastDamageOrigin   ;
	FVector							    SavedLastMomentumVec	;
	bool								bFiringAllowed			;
	bool								bInRagdoll				;
	bool								bIsDead					;

	AActor*								CurrentTarget			; 
	float								LastFireTime			;
	float								FireDelay				; 
	UBFWeaponAttachment*				WeaponAttachment		;	 
	class ABFBotAIController*					BotAIController			;
	FTimerHandle						DeathTimer				; 

};

inline bool ABFAIBot::IsDead() const
{
	return bTearOff || IsPendingKillPending();
}

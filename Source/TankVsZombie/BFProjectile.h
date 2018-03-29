#pragma once

#include "GameFramework/Actor.h"
#include "BFProjectileImpactEffect.h"
#include "DamageTypes/BFDamageType.h"
#include "BFProjectile.generated.h" 

USTRUCT(BlueprintType)
struct FBFPointDamageEvent : public FPointDamageEvent
{
	GENERATED_USTRUCT_BODY()

	FBFPointDamageEvent()
		: FPointDamageEvent()
	{

	}

	FBFPointDamageEvent(float InDamage, const FHitResult& InHitInfo, const FVector& InShotDirection, TSubclassOf<UDamageType> InDamageTypeClass, const FVector& InMomentum = FVector::ZeroVector)
		: FPointDamageEvent(InDamage, InHitInfo, InShotDirection, InDamageTypeClass), Momentum(InMomentum)
	{

	}

	UPROPERTY(	EditAnywhere	,	BlueprintReadWrite	, Category = Damage	)
		FVector Momentum;

	/** ID for this class. NOTE this must be unique for all damage events. */
	static const int32 ClassID = 101;
	virtual int32 GetTypeID() const { return FBFPointDamageEvent::ClassID; };
	virtual bool IsOfType(int32 InID) const { return (FBFPointDamageEvent::ClassID == InID) || FPointDamageEvent::IsOfType(InID); };
};


USTRUCT(BlueprintType)
struct FBFRadialDamageEvent : public FRadialDamageEvent
{
	GENERATED_USTRUCT_BODY()

	FBFRadialDamageEvent()
		: FRadialDamageEvent(), bScaleMomentum(true)
	{

	}
 
 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float BaseMomentumMag;
 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		bool bScaleMomentum;
	UPROPERTY()
		FVector ShotDirection;



 	static const int32 ClassID = 102;
	virtual int32 GetTypeID() const { return FBFRadialDamageEvent::ClassID; };
	virtual bool IsOfType(int32 InID) const { return (FBFRadialDamageEvent::ClassID == InID) || FRadialDamageEvent::IsOfType(InID); };
};


USTRUCT(BlueprintType)
struct FBFRadialDamageParams : public FRadialDamageParams
{
	GENERATED_USTRUCT_BODY() 

};

UCLASS(Blueprintable)
class TANKVSZOMBIE_API ABFProjectile : public AActor
{
	GENERATED_UCLASS_BODY()
public:
		
		
		UPROPERTY(Category = Projectile, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) 
			USphereComponent*				CollisionComp ;
		UPROPERTY(Category = Projectile, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
			UProjectileMovementComponent	*ProjectileMovement ;	
		UPROPERTY(Category = Projectile, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
			UStaticMeshComponent*			ProjectileMesh ;  
 
		UPROPERTY(Category = Projectile, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
			USoundCue*					LaunchSound ; 
		UPROPERTY(Category = Projectile, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
			UParticleSystem*			 BounceEffectFX ;
		UPROPERTY(	EditAnywhere	, BlueprintReadWrite , Category = Projectile )
			TSubclassOf<ABFImpactEffect> ImpactEffectClass ;	

		UPROPERTY(	EditAnywhere	, BlueprintReadWrite , Category = Projectile )
			TSubclassOf<UDamageType>	 DamageType ; 
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
			FRadialDamageParams			DamageParams ;
		UPROPERTY(BlueprintReadWrite, Category = Projectile)
			AController*				InstigatorController;

		UPROPERTY(  EditAnywhere	, BlueprintReadWrite	, Category = Projectile )
			bool						bAlwaysShootable ;  
		UPROPERTY(	EditAnywhere	, BlueprintReadWrite	, Category = Projectile	)
			bool					    bInteractWithProjectile;
		UPROPERTY(  EditAnywhere	, BlueprintReadWrite	, Category = Projectile )
			bool bIsEnergyProjectile ;
	 
		// important to set this to false to prevent accidental radial damage to actos , to prevent headshot in tank shell damage 
		UPROPERTY(	EditAnywhere	, BlueprintReadWrite	, Category = Projectile )
			bool bInflictRadialDamage ;		

		UPROPERTY(	EditAnywhere	, BlueprintReadWrite	, Category = Projectile )
			float Momentum ;

		UPROPERTY()
			AActor* ImpactedActor ;

		UPROPERTY(EditDefaultsOnly, Category = Projectile)
			bool bCanHitTeammates;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
			bool bCanHitInstigator;
 		UPROPERTY()
			bool bHasSpawnedFully;

		UPROPERTY(EditDefaultsOnly, Category = Projectile)
			TArray<TEnumAsByte<ECollisionChannel>> ExplosionHurtChannels;

		bool bInOverlap;
		bool bExploded;


		UFUNCTION()
			void EnableCollision();
		virtual void Init(FVector Direction); 
		virtual void BeginPlay() override;


public:	
 		virtual void Tick(float DeltaTime) override; 
	UFUNCTION()
		virtual void OnStop(const FHitResult& Hit);
	UFUNCTION()
		virtual void OnBounce(const struct FHitResult& ImpactResult, const FVector& ImpactVelocity); 

		bool ShouldIgnoreHit(AActor* OtherActor, UPrimitiveComponent* OtherComp);
		void ProcessHit( AActor* OtherActor , UPrimitiveComponent* OtherComp , const FHitResult& HitInfo, const FVector& HitLocation , const FVector& HitNormal );
		void DamageImpactedActor(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FVector& HitLocation, const FVector& HitNormal);
 
 	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
		FRadialDamageParams GetDamageParams(AActor* OtherActor, const FVector& HitLocation, float& OutMomentum) const;	 
		void Explode(const FHitResult& HitInfo,const FVector& Location ,  UPrimitiveComponent* HitComp = NULL);
	UFUNCTION(BlueprintImplementableEvent, Category = "Projectile")
		void OnExploded();

	bool HurtRadius( UObject* WorldContextObject , float BaseDamage , float MinimumDamage , float BaseMomentumMag, const FVector& Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff,
		TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, AActor* DamageCauser, AController* InstigatedByController, AController* FFInstigatedBy ,
		float CollisionFreeRadius, const TArray<FVector>* AltVisibilityOrigins);
	void  ShutDown();
 	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable, Category = "Projectile")
		virtual bool InteractsWithProj(ABFProjectile* OtherProj);	
};

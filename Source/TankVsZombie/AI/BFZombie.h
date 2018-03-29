#pragma once
#include "BFGibSkeletalMesh.h"
#include "BFAIBot.h"
#include "DamageTypes/BFDamageType_Coil.h"
#include "DamageTypes/BFDamageType_Fire.h"
#include "DamageTypes/BFDamageType_Shell.h"

#include "BFZombie.Generated.h"

UENUM(BlueprintType)
enum EZombieClassType
{
	EZOMBIE_TYPE_HORDE		,
	EZOMBIE_TYPE_ROADRAGE	,
	EZOMBIE_TYPE_MINIBOSS	,
	EZOMBIE_TYPE_BOSS		,
} ;

USTRUCT(BlueprintType)
struct FAttackMontage
{
	GENERATED_USTRUCT_BODY()
	FAttackMontage()
		:AnimMontage(NULL),CauseDamageDelay(0.0f),DamageScale(1.0f)
	{

	}
	UPROPERTY(  EditDefaultsOnly , BlueprintReadWrite	, Category = "AttackMontage" )
		UAnimMontage*   AnimMontage	;
	UPROPERTY(	EditDefaultsOnly , BlueprintReadWrite	, Category = "AttackMontage" )
		float		    CauseDamageDelay ;
	UPROPERTY(	EditDefaultsOnly , BlueprintReadWrite	, Category = "AttackMontage" )
		float			DamageScale ;
} ;

UCLASS( Blueprintable , Abstract )
class TANKVSZOMBIE_API ABFZombie : public ABFAIBot
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY()
		USoundCue* Active3DSound ;

	UPROPERTY(	EditDefaultsOnly	,	BlueprintReadWrite	, Category = "Zombie" )
		TSubclassOf<ABFGibSkeletalMesh> ZombieGibsClass ;
	UPROPERTY(	EditDefaultsOnly	,	BlueprintReadWrite	, Category = "Zombie"	)
		UParticleSystemComponent*		HeadBlastFXComp	;
	UPROPERTY(	EditDefaultsOnly	,	BlueprintReadWrite	, Category = "Zombie"	)
		UAnimMontage* DeathMontage ;

	UPROPERTY(EditDefaultsOnly, Category = "Zombie")
		float LifeSpanPostDeath ;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zombie")
		float DamageAmount ;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zombie")
		float WalkSpeedOnDamageMultiplier ;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zombie")
		UAudioComponent*		AudioComponent ;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zombie")
		bool bAllowHeadShot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zombie")
		TArray<FAttackMontage>		AttackMontageList ;
	
	UPROPERTY(	BlueprintReadWrite	, Category = "Zombie"	)
		bool bIsDead ;
	UPROPERTY(  BlueprintReadWrite	, Category = "Zombie"	)
		bool bWasHeadShot ;
	UPROPERTY(	BlueprintReadWrite	, Category = "Zombie"	)
		UMaterialInstanceDynamic* MaterialInstance ;
	
	UPROPERTY(	BlueprintReadWrite	, Category = "Zombie")
		bool bAttackWasCanceled ;
	UPROPERTY(	BlueprintReadWrite	, Category = "Zombie")
		float			OverlayDamageFrac ;
	UPROPERTY(	BlueprintReadWrite	, Category = "Zombie")
		FLinearColor	OverlayDamagedSkinColor	;
	
	UFUNCTION(	BlueprintCallable	, Category = "Zombie")
		void PlaySound3D(USoundCue* Cue) ;
	UFUNCTION(	BlueprintCallable	, Category = "Zombie")
		void StopSound3D() ;
	UFUNCTION(	BlueprintCallable	, Category = "Zombie" )
		void Explode(FVector Origin, FVector Momentum) ;
	
	UFUNCTION(	BlueprintCallable	, Category = "Zombie" )
		void CauseDamage(AActor* Target) ;	
	UFUNCTION(	BlueprintCallable	, Category = "Zombie")
		FAttackMontage GetRandomAttackMontage() ;
	UFUNCTION(	BlueprintImplementableEvent	, Category = "Zombie")
		void ExecuteDeath_BP() ;

	UFUNCTION()
		void ResetDamageFrac() ; 	 	
	UFUNCTION(	BlueprintCallable	, Category = "Zombie")
		bool CloseEnoughForMelee(AActor* OtherActor) ;
	UFUNCTION(	BlueprintCallable	, Category = "Zombie" )
		void NotifyNavigationFailed() ;

	virtual void Kill(FVector Momentum, const FBotDamageParamEx& DamageParams) override ;
	virtual void DamageReceived() override ;
	void CacheMaterial() ;

private:

	float DamageFrac ;
	float LastTime	 ;	
	float LastNavigationFailedTime ;	

	FTimerHandle DamageFracTimer ;	
} ;
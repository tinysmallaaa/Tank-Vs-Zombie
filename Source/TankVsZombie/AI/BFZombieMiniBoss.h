#pragma once
#include "BFZombie.h"
#include "BFZombieMiniBoss.Generated.h"


UCLASS( Blueprintable , Abstract )
class TANKVSZOMBIE_API ABFZombieMiniBoss : public ABFZombie
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZombieMiniBoss" )
		TArray<UAnimMontage*> ReactAnimMontages ;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ZombieMiniBoss" )
		TArray<FBFBloodDecalInfo> BloodDecals ;
	UPROPERTY(EditDefaultsOnly, Category = "ZombieMiniBoss")
		float HeadShotDamageMultiplier ;
	UPROPERTY(BlueprintReadWrite, Category = "ZombieMiniBoss")
		bool bVictorious ;

	virtual void BeginPlay()	override ;
	virtual void DamageReceived() override ;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override ;

	UFUNCTION(BlueprintCallable , Category = "ZombieMiniBoss")
		void SpawnHeavyDamageDacals( float Radius , int32 Count ) ;
	UFUNCTION(BlueprintCallable, Category = "ZombieMiniBoss")
		void PlayReactAnim() ;
	UFUNCTION(BlueprintCallable, Category = "ZombieMiniBoss")
		void CancelAttackAttempt() ; 

	virtual float ModifyDamageTaken(float Damage, FBotDamageParamEx DamageParams) override ;

};

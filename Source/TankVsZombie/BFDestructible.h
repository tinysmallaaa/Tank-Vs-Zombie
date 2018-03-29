#pragma once
#include "BFDestructible.Generated.h"

UCLASS(Blueprintable)
class ABFDestructible : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "BFDestructible")
		bool bRadialDamage;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite , Category = "BFDestructible")
		FRadialDamageParams DamageParams;
	UPROPERTY(EditDefaultsOnly, Category = "BFDestructible")
		float BaseMomentum;
	UPROPERTY( BlueprintReadWrite, Category = "BFDestructible")
	bool bExploded;
	UPROPERTY(BlueprintReadWrite, Category = "BFDestructible")
		float Endurance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFDestructible")
		float MaxEndurance;



	UPROPERTY(EditDefaultsOnly, Category = "BFDestructible")
		TSubclassOf<UDamageType> DamageType;
	 
		void ExplodeInternal(AActor* DamageCauser, FVector DamageImpulse, FVector DamageImpactPoint, FVector DamageImpactNormal);

	UFUNCTION(BlueprintImplementableEvent, Category = "BFDestructible")
		void OnExploded(AActor* DamageCauser, FVector DamageImpulse, FVector DamageImpactPoint, FVector DamageImpactNormal);


	bool HurtRadius( float BaseMomentumMag , const FVector& Origin , TSubclassOf<class UDamageType> DamageTypeClass , 
		const TArray<AActor*>& IgnoreActors , AActor* DamageCauser , AController* InstigatedByController  );
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "BFDestructible")
		void PostDamageTaken(AActor* DamageCauser, FVector DamageImpulse, FVector DamageImpactPoint, FVector DamageImpactNormal);

	virtual void BeginPlay() override;

private:

};
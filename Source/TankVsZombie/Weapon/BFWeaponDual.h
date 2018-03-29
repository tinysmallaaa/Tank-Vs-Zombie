#pragma once 
#include "BFProjectile.h"
#include "BFGameSaveData.h"
#include "BFMountedWeapon.h"
#include "BFWeaponDual.Generated.h"
 
#define DEFAULT_FIRE_RATE 5

UCLASS(Blueprintable)
class TANKVSZOMBIE_API ABFWeaponDual : public  ABFWeapon
{
	GENERATED_UCLASS_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFWeapon")
		FName FireLocationSocket2;

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override; 
	 
	virtual FVector CalculateFireLocation(FVector Direction) override;

private:

	int32 FireLocationItr;
};
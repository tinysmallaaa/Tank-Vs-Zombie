#pragma once
#include "Weapon/BFWeapon.h"
#include "BFAnimInstance.Generated.h"


UCLASS(Blueprintable)
class UBFAnimInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY( BlueprintReadWrite , Category = "BFAnimInstance" )
		AActor* AnimInstanceOwner ;

	UFUNCTION( BlueprintCallable , Category = "BFAnimInstance" )	
		ABFWeapon* GetMountedWeaponOwner() ;

};

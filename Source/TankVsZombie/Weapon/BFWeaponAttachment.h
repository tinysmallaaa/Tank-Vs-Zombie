#pragma once 
#include "BFWeaponAttachment.Generated.h"
class ABFWeapon;

UCLASS(Blueprintable,Abstract)
class TANKVSZOMBIE_API UBFWeaponAttachment : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY( EditDefaultsOnly , Category = "WeaponAttachment" )
		FName		PrimarySocketName  ;

	UPROPERTY( EditDefaultsOnly , Category = "WeaponAttachment" )
		FVector		PrimarySocketLocation  ;

	UPROPERTY( EditDefaultsOnly , Category = "WeaponAttachment" )
		FRotator	PrimarySocketRotation  ;  

	UPROPERTY( EditDefaultsOnly , Category = "WeaponAttachment")
		FVector	PrimarySocketScale;

	UPROPERTY( EditDefaultsOnly , Category = "WeaponAttachment")
		TSubclassOf<ABFWeapon> WeaponClass	;

	UPROPERTY( BlueprintReadWrite , Category = "WeaponAttachment" )
		ABFWeapon* WeaponRef ;
	 
	ABFWeapon* GetAttachedWeapon()
	{
		return WeaponRef;
	}   

	virtual void Tick(float DeltaSeconds) {}

};

#pragma once
#include "BFDamageType.Generated.h"

 
UCLASS(MinimalAPI, const, Blueprintable, BlueprintType)
class UBFDamageType : public UDamageType
{
	GENERATED_BODY()
public:

	UPROPERTY( EditDefaultsOnly , Category = "BFDamageType" )
		bool bForceRagdollDeath;
	UPROPERTY( EditDefaultsOnly , Category = "BFDamageType" )
		bool bCanHeadShot;
	UPROPERTY( EditDefaultsOnly , Category = "BFDamageType" )
		bool bForceHeadShotDeath;

	UBFDamageType()
	{

	} 

};

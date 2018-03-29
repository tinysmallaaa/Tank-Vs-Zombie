#pragma once
#include "BFGameSingleton.Generated.h"


 

class UBFConfigDBUtil;
class UBFGameSaveData;
 
UCLASS(Blueprintable)
class TANKVSZOMBIE_API UBFGameSingleton : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION( BlueprintCallable , Category = "BFGameSingleton" )
		static	UBFGameSingleton* Get(); 

	UPROPERTY(	EditDefaultsOnly	, Category = "BFGameSingleton" )
		TSubclassOf<UBFGameSaveData> SaveGameClass	;	    

	UPROPERTY(EditDefaultsOnly, Category = "BFGameSingleton")
		TSubclassOf<UBFConfigDBUtil> ConfigDBClass	;


};

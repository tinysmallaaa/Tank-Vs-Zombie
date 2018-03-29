#pragma once
#include "BFSpiderMine.Generated.h"


UCLASS(Blueprintable)
class ABFSpiderMine : public ACharacter
{
	GENERATED_UCLASS_BODY()
public:
 

	UPROPERTY( EditDefaultsOnly , Category = "BFSpiderMine" )
		float TravelSpeed ;
	UPROPERTY( EditDefaultsOnly , Category = "BFSpiderMine" )
		float DamagePower ;

	UFUNCTION( BlueprintCallable , Category = "BFSpiderMine" ) 
	void Throw(FVector Location, FVector Direction);

	
	UFUNCTION(BlueprintImplementableEvent, Category = "BFSpiderMine")
		void Explode(); 


};
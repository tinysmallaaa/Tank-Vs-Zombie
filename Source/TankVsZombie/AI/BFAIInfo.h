#pragma once
#include "BFAIInfo.Generated.h"
 
 
USTRUCT(BlueprintType)
struct FBotAIParams
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BotAIParams")
		float Endurance	;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BotAIParams")
		float Alertness	;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BotAIParams")
		float DamageMultiplier ;
}; 



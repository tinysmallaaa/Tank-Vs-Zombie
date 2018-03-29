#pragma once
#include "BFRoadRageInfo.Generated.h"

class ABFZombie;
USTRUCT(BlueprintType)
struct FRoadRageMissionDBInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(	BlueprintReadOnly	,	Category = "EndlessRunMission"	)
		FString LevelPath	;

};


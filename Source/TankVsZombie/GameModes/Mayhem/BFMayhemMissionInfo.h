#pragma once
#include "BFMayhemMissionInfo.Generated.h"
class ABFZombie;

USTRUCT(BlueprintType)
struct FMayhemMissionDBInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY() 

	UPROPERTY(BlueprintReadOnly, Category = "EndlessRunMission")
		FString LevelPath;

};

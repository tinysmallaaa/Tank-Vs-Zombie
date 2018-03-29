#pragma once
#include "../BFGameObjective.h" 
#include "BFConfigDBUtil.h"
#include "RoadRage/BFRoadRageInfo.h"
#include "Mayhem/BFMayhemMissionInfo.h"
#include "Horde/BFHordeMissionInfo.h"
#include "BFUnlockableInventory.h"
#include "BFActiveMission.Generated.h"
 


UCLASS()
class ABFActiveMission : public AActor
{
	GENERATED_UCLASS_BODY()
public:
	
	UPROPERTY(BlueprintReadOnly	, Category = "ActiveMission")
		FName					LevelName ;
	UPROPERTY(BlueprintReadOnly	, Category = "ActiveMission")
		FMissionObjectiveList	ObjectiveList ;
	
	UPROPERTY(BlueprintReadOnly	, Category = "ActiveMission")
		float					DifficultyMultiplier ;
	UPROPERTY(BlueprintReadWrite , Category = "ActiveMission")
		FMissionDBInfo			MissionInfo ;

	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	FHordeMissionDBInfo		GetHordeMissionInfo()		;
	FMayhemMissionDBInfo	GetMayhemMissionInfo()		;
	FRoadRageMissionDBInfo	GetRoadRageMissionInfo()	; 

};

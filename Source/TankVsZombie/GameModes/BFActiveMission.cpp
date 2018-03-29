#include "TankVsZombie.h"
#include "BFActiveMission.h"

ABFActiveMission::ABFActiveMission(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void ABFActiveMission::BeginPlay()
{
	Super::BeginPlay();
}

void ABFActiveMission::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

FHordeMissionDBInfo		ABFActiveMission::GetHordeMissionInfo()
{
	FHordeMissionDBInfo Info ;

	if (!MissionInfo.MissionDataHandle.IsNull())
	{
		Info = *MissionInfo.MissionDataHandle.GetRow<FHordeMissionDBInfo>(FString("FHordeMissionDBInfo"));
	}
	return Info;
}

FMayhemMissionDBInfo	ABFActiveMission::GetMayhemMissionInfo()
{
	FMayhemMissionDBInfo Info;

	if (!MissionInfo.MissionDataHandle.IsNull())
	{
		Info = *MissionInfo.MissionDataHandle.GetRow<FMayhemMissionDBInfo>(FString("FMayhemMissionDBInfo"));
	}
	return Info;
}

FRoadRageMissionDBInfo	ABFActiveMission::GetRoadRageMissionInfo()
{
	FRoadRageMissionDBInfo Info;

	if (!MissionInfo.MissionDataHandle.IsNull())
	{
		Info = *MissionInfo.MissionDataHandle.GetRow<FRoadRageMissionDBInfo>(FString("FRoadRageMissionDBInfo"));
	}
	return Info;
}

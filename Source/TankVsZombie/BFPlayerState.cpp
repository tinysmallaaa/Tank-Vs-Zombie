#include "TankVsZombie.h"
#include "BFPlayerState.h"

ABFPlayerState::ABFPlayerState(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void  ABFPlayerState::ResetState()
{ 
	TotalZombiesKilled		= 0 ; 
	HeadShotsZombiesCount	= 0 ;
	RoadKillZombiesCount	= 0 ;
	KilledByMinesCount		= 0 ;
	KilledByShellCount		= 0 ;
	KilledByGrenadeCount	= 0 ;
	KilledByMinesCount		= 0 ;
	KilledByGunCount		= 0 ;
	KilledByFireCount		= 0 ;
	VehicleSmearedCount		= 0 ;
	CreditsEarned			= 0 ;
}

int32 ABFPlayerState::CalculateStarsEarned(FMissionObjectiveList& ObjectiveList)
{
	return CheckObjective(ObjectiveList.Objective0).Stars + CheckObjective(ObjectiveList.Objective1).Stars + CheckObjective(ObjectiveList.Objective2).Stars;
}
FMissionObjective  ABFPlayerState::CheckObjective(FMissionObjective Objective)
{
	FMissionObjective Modified = Objective;
	Modified.Stars = 0;
	switch(Objective.Type)
	{
		case EOBJECTIVE_KILLED_TOTAL:
		{

			Modified.CurrentAmount = TotalZombiesKilled;
			if (Modified.TargetAmount <= TotalZombiesKilled)
			{
				Modified.Stars = 1; 
			}
		}break;
		case EOBJECTIVE_MISSION_COMPLETED:
		{

			if (bMissionCompleted)
				Modified.Stars = 1;
		}break;
		case EOBJECTIVE_HEADSHOT:
		{
			Modified.CurrentAmount = HeadShotsZombiesCount ;
			if (Modified.TargetAmount <= HeadShotsZombiesCount)
			{
				Modified.Stars = 1;
			}
		}break;
		case EOBJECTIVE_KILLED_BY_GRENADE:
		{
			Modified.CurrentAmount = KilledByGrenadeCount ;
			if (Modified.TargetAmount <= KilledByGrenadeCount)
				Modified.Stars = 1;
		}break;
		case EOBJECTIVE_KILLED_BY_TURRET:
		{
			Modified.CurrentAmount = KilledByGunCount ;
			if (Modified.TargetAmount <= KilledByGunCount)
				Modified.Stars = 1;
		}break;
		case EOBJECTIVE_KILLED_BY_COIL:
		{
			Modified.CurrentAmount = KilledByCoilCount ;
			if (Modified.TargetAmount <= KilledByCoilCount)
				Modified.Stars = 1;
		}break;
		case EOBJECTIVE_KILLED_BY_FIRE:
		{
			Modified.CurrentAmount = KilledByFireCount ;
			if (Modified.TargetAmount <= KilledByFireCount)
				Modified.Stars = 1;
		}break;
		case EOBJECTIVE_KILLED_BY_MINES:
		{
			Modified.CurrentAmount = KilledByMinesCount ;
			if (Modified.TargetAmount <= KilledByMinesCount)
				Modified.Stars = 1;
		}break; 

	}

	return Modified;
}
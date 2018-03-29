#include "TankVsZombie.h"
#include "BFLevelSummery.h"

UBFLevelSummary::UBFLevelSummary(const FObjectInitializer& OI)
	:Super(OI)
{
	Author = TEXT("Unknown");
	Description = NSLOCTEXT("LevelSummary", "DefaultDesc", "");
	OptimalPlayerCount = 6;
	OptimalTeamPlayerCount = 10;
}
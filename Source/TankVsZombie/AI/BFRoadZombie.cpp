#include "TankVsZombie.h"
#include "BFRoadZombie.h"


ABFRoadZombie::ABFRoadZombie(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	Type = ERoadZombieType::ROAD_ZOMBIE_STATIONARY;
	bInterpolatingToVehicle = false;

}



bool ABFRoadZombie::CanAttachToVehicle(ABFTankVehicle* VehicleTank, float Range)
{


	return false;
}

void ABFRoadZombie::InitiateAttachmentAnim(ABFTankVehicle* VehicleTank, float Time)
{

}
#include "TankVsZombie.h"
#include "Vehicle/BFPlayerTank.h"
#include "BFRoadZombieAIController.h"

ABFRoadZombieAIController::ABFRoadZombieAIController(const FObjectInitializer& ObjectInitializer)
	:Super( ObjectInitializer )
{

}

bool ABFRoadZombieAIController::CanAttachToVehicle(ABFTankVehicle* Vehicle , float Range )
{
	ABFPlayerTank* PlayerTank = Cast<ABFPlayerTank>( Vehicle ) ;

	if ( PlayerTank != NULL && ZombiePawn != NULL )
	{ 
		FVector ZombiePos = ZombiePawn->GetActorLocation() ; 
	}

	return false ;
}

void ABFRoadZombieAIController::InitiateAttachmentAnim(ABFTankVehicle* Vehicle)
{

}

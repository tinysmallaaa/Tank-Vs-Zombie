#include "TankVsZombie.h"
#include "BFMiniTankBotAIController.h"

ABFMiniTankBotAIController::ABFMiniTankBotAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

FVector ABFMiniTankBotAIController::GetFireDirection()
{
	if (ActiveTarget.IsValid() && Bot)
	{
		return ActiveTarget->GetActorLocation() - Bot->GetFireLocation();
	}
	else if (Bot)
	{
		return Bot->CalculateFireDirection();
	}

	return FVector(0.0f, 0.0f, 1.0f);
}


void ABFMiniTankBotAIController::Tick(float DeltaSeconds)
{

} 

void  ABFMiniTankBotAIController::FirePressed(bool Pressed)
{
	bFirePressed = Pressed;
	if (Pressed && Bot)
	{
		Bot->StartFiringFX();
		Bot->Fire();
	}
	else if(Bot)
	{
		Bot->StopFire();
	}
}


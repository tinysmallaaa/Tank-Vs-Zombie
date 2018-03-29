#include "TankVsZombie.h"
#include "BFBotAIController.h"


ABFBotAIController::ABFBotAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ReloadTime = 1.0f;
}

void ABFBotAIController::DestroyPawn()
{
	if (IsValid(BotPawn))
	{ 
		BotPawn->Kill(FVector(0.0f) , FBotDamageParamEx());
	}
}
void ABFBotAIController::Tick(float  DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsValid(BotPawn))
	{
		BotPawn->UpdateTargeting(DeltaSeconds);
	}
}

void ABFBotAIController::Fire()
{ 
	
	if( IsValid(BotPawn) && IsFiringAllowed()) BotPawn->Fire(); 
}

bool ABFBotAIController::IsReadyToFire()
{ 
	return BotPawn->CanFire();
}

bool  ABFBotAIController::IsTargetInRange(AActor* Target)
{

	if (IsValid(BotPawn) && IsValid(Target) )
	{
		FVector FireLocation = BotPawn->GetFireLocation();
		 
		return  LineOfSightTo( Target , FireLocation , true );
	}

	return false;
}

void ABFBotAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	BotPawn = Cast<ABFAIBot>(InPawn);
}

void ABFBotAIController::UnPossess()
{
	Super::UnPossess();
	BotPawn = NULL;
}

bool ABFBotAIController::IsPawnMovable()
{
	return BotPawn && BotPawn->bAllowMobility;
}
 
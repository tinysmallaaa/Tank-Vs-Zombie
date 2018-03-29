#include "TankVsZombie.h"
#include "BFZombieAIController.h"


ABFZombieAIController::ABFZombieAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void ABFZombieAIController::Possess(APawn* InPawn)
{

	Super::Possess(InPawn);
	ZombiePawn = Cast<ABFZombie>(InPawn);

}

void ABFZombieAIController::UnPossess()
{

	Super::UnPossess();
	ZombiePawn = NULL;

}

AActor* ABFZombieAIController::GetTarget()
{

	if (ZombiePawn != NULL )
	{
		return ZombiePawn->CurrentTarget;
	} 

	return NULL;

}

FVector ABFZombieAIController::GetTargetPathLocation()
{	

	if (ZombiePawn != NULL && ZombiePawn->CurrentTarget != NULL)
	{
		return ZombiePawn->CurrentTarget->GetActorLocation();
	}

	return TargetLocation;

}

void  ABFZombieAIController::SetTarget(AActor* Target)
{

	if (ZombiePawn != NULL)
	{
		ZombiePawn->SetTarget(Target);
	} 

}

void  ABFZombieAIController::SetTargetLocation(FVector Location)
{

	if (ZombiePawn != NULL)
	{
		this->TargetLocation = Location;
	}

}

void  ABFZombieAIController::SetAIParams(const FBotAIParams& Params)
{

	Super::SetAIParams(Params);

	if (ZombiePawn != NULL)
	{
		
		ZombiePawn->HealthMax *= (Params.Endurance>0.0f?Params.Endurance:1.0f);
		ZombiePawn->Health = ZombiePawn->HealthMax;
		ZombiePawn->DamageAmount *= (Params.DamageMultiplier>0.0f?Params.DamageMultiplier:1.0f);
		ZombiePawn->AlertnessTime *= (Params.Alertness>0.0f?Params.Alertness:1.0f);

	}

}
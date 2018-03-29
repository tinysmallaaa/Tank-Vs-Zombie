#include "TankVsZombie.h"
#include "BrainComponent.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "BFAIController.h"

ABFAIController::ABFAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	bFiringAllowed = true		;
	TeamIndex = TEAM_ENEMY		;
	ActivateAIOnSpawn = false	;
}

void ABFAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ActivateAIOnSpawn)
	{
		ActivateAI();
	}
}

void ABFAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

 
void ABFAIController::ShutdownAI()
{
	if (BrainComponent != NULL)
	{
		BrainComponent->StopLogic("Dead");
	}
}

uint8 ABFAIController::GetTeamNum() const
{ 
	return   TeamIndex  ;
}


FVector ABFAIController::FindNextCoverLocation(float Radius, FVector CurrentLocation)
{
	if (GetPawn()!=NULL && GetWorld()->GetNavigationSystem()!=NULL)
	{
		ACharacter* CharPawn = Cast<ACharacter>(GetPawn());
		float PawnRadius = CharPawn->GetCapsuleComponent()->GetScaledCapsuleRadius();

		UNavigationSystem* NavigationSystem = GetWorld()->GetNavigationSystem() ;

		if ( NavigationSystem  != NULL )
		{
			FNavLocation NavLocation ;
			if ( NavigationSystem->GetRandomReachablePointInRadius( CharPawn->GetActorLocation() , Radius , NavLocation ) )
			{
				return NavLocation.Location ;
			}
		}
	}

	if (GetPawn() != NULL )
	{
		return GetPawn()->GetActorLocation();
	}

	return CurrentLocation;
}
void ABFAIController::SetAIParams(const FBotAIParams& Params)
{
 
	AIParams = Params;

}

bool ABFAIController::GetPawnMobility()
{
	return IsPawnMovable();
}
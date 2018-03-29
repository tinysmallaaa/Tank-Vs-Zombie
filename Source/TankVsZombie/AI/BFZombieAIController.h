#pragma once
#include "BFBotAIController.h"
#include "BFZombie.h"
#include "BFZombieAIController.Generated.h"

UCLASS(Blueprintable, Abstract)
class ABFZombieAIController : public ABFBotAIController
{
	GENERATED_UCLASS_BODY()
public:
	 
	UPROPERTY( BlueprintReadWrite , Category = "ZombieAIController")
		ABFZombie* ZombiePawn;
 
	virtual void Possess(APawn* InPawn) override;
	virtual void UnPossess() override;

	UFUNCTION( BlueprintCallable , Category = "ZombieAIController")
		void SetTarget(AActor* Target);	
	UFUNCTION( BlueprintCallable , Category = "ZombieAIController")
		void SetTargetLocation(FVector TargetLocation);

	UFUNCTION( BlueprintCallable , Category = "ZombieAIController")
		FVector GetTargetPathLocation();
	
	UFUNCTION( BlueprintCallable , Category = "ZombieAIController")
		AActor* GetTarget();

	virtual void SetAIParams(const FBotAIParams& Params) override;

private:

	FVector TargetLocation;
};
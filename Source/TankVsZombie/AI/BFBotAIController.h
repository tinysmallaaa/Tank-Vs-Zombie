#pragma once
#include "BFAIController.h"
#include "BFTeamInterface.h"
#include "BFBotAIController.Generated.h"

UCLASS( Blueprintable , Abstract )
class TANKVSZOMBIE_API ABFBotAIController : public ABFAIController
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "BotAIController")
		float ReloadTime;

	UPROPERTY(BlueprintReadWrite, Category = "BotAIController")
		ABFAIBot* BotPawn;
	
	UFUNCTION(BlueprintCallable, Category = "BotAIController")
		void Fire();

	UFUNCTION(BlueprintCallable, Category = "BotAIController")
		bool IsReadyToFire();

	UFUNCTION(BlueprintCallable, Category = "BotAIController")
		bool IsTargetInRange(AActor* Target); 

	virtual void DestroyPawn();
	virtual void Tick(float  DeltaSeconds)override;


	virtual void Possess(APawn* InPawn) override;
	virtual void UnPossess() override;

	virtual bool IsPawnMovable() override;

private:

	float LastFireTime;
};

#pragma once
#include "BFTeamInterface.h"
#include "BFGameState.Generated.h"
 
UCLASS(Blueprintable)
class TANKVSZOMBIE_API ABFGameState : public AGameState
{

	GENERATED_UCLASS_BODY()

public:
	
	UFUNCTION( BlueprintCallable , Category = GameState )
		virtual bool OnSameTeam(const AActor* Actor1, const AActor* Actor2);

};

#pragma once
#include "BFGameModeHorde.h"
#include "BFGameModeEndlessHorde.Generated.h"


UCLASS(Blueprintable)
class TANKVSZOMBIE_API ABFGameModeEndlessHorde : public ABFGameModeHorde
{
	GENERATED_UCLASS_BODY()

public:

	virtual void BeginMissionPlay()  override;
	virtual void EndMissionPlay()	override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void ScoreKill(AController* Killed, AController* KilledBy, const FBotDamageParamEx& DamageParam) override;
 
};

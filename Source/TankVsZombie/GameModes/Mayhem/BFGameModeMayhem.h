#pragma once
#include "GameModes/BFGameModeBase.h"
#include "BFActiveMissionMayhem.h"
#include "BFGameModeMayhem.Generated.h"


UCLASS(Blueprintable)
class ABFGameModeMayhem : public ABFGameModeBase
{
	GENERATED_UCLASS_BODY()


public:

	virtual void BeginMissionPlay()  override;
	virtual void EndMissionPlay()	override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void ScoreKill(AController* Killed, AController* KilledBy, const FBotDamageParamEx& DamageParam) override;


private:

	ABFActiveMissionMayhem* ActiveMission;
};

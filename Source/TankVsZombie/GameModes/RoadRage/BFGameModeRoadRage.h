#pragma once
#include "GameModes/BFGameModeBase.h"
#include "BFTrackBuilder.h"
#include "BFTrackVisualPart.h"
#include "BFGameModeRoadRage.Generated.h"
 



UCLASS(Blueprintable)
class ABFGameModeRoadRage : public ABFGameModeBase
{
	GENERATED_UCLASS_BODY()
public:
	virtual void BeginMissionPlay()  override;
	virtual void EndMissionPlay()	override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void ScoreKill(AController* Killed, AController* KilledBy, const FBotDamageParamEx& DamageParam) override;

private:
	ABFTrackBuilder* TrackBuilder;  
};





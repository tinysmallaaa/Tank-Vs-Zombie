#pragma once
#include "../BFActiveMission.h"
#include "BFActiveMissionRoadRage.Generated.h"


UCLASS()
class ABFActiveMissionRoadRage : public ABFActiveMission
{
	GENERATED_UCLASS_BODY()

public:
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

};

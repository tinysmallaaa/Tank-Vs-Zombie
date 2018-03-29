#pragma once
#include "../BFActiveMission.h"
#include "BFActiveMissionMayhem.Generated.h"


UCLASS()
class ABFActiveMissionMayhem : public ABFActiveMission
{
	GENERATED_UCLASS_BODY()

public:
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

};

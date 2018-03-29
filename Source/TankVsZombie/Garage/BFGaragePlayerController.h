#pragma once
#include "BFGaragePlayerController.Generated.h"


UCLASS(Blueprintable)
class ABFGaragePlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()
public:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override; 

};
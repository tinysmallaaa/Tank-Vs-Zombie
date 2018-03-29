#pragma once
#include "Vehicle/BFPlayerTank.h"
#include "BFGarageTank.Generated.h"

UCLASS( Blueprintable , Abstract )
class ABFGarageTank : public ABFPlayerTank
{
	GENERATED_UCLASS_BODY()

public:
	 
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override; 

};



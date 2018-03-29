#pragma once
#include "SimpleWheeledVehicleMovementComponent.h"
#include "BFTankMovementComponent.Generated.h"

UCLASS(Blueprintable)
class UBFTankMovementComponent : public USimpleWheeledVehicleMovementComponent
{
	GENERATED_BODY()
public:

	float GetMaxSpeed() const 
	{ 
		return Super::GetForwardSpeed();
	}
};

#pragma once
#include "BFPlaceableGadgetInterface.Generated.h"


UINTERFACE(MinimalAPI)
class UBFPlaceableGadgetInterface : public UInterface
{
	GENERATED_BODY()
public: 

};

class IBFPlaceableGadgetInterface
{
	GENERATED_IINTERFACE_BODY()
public: 
	virtual void SetEnableCollision(bool flag) = 0;  
	virtual void ReleaseOnLocation( FVector Location ) = 0;
	virtual void AdjustOnDrag( FVector NewLocation , FVector ImpactNormal , bool DragOverEvent ) =0;
};



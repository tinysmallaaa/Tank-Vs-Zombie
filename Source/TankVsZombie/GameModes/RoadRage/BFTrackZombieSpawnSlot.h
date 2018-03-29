#pragma once
#include "BFTrackZombieSpawnSlot.Generated.h"


UCLASS(ClassGroup = Utility, hidecategories = (Object, LOD, Physics, Lighting, TextureStreaming, Activation, "Components|Activation", Collision), editinlinenew, meta = (BlueprintSpawnableComponent))
class UBFTrackZombieSpawnSlot : public UArrowComponent
{
	GENERATED_BODY()
public:

	UBFTrackZombieSpawnSlot()
		:bDynamic(false)
	{

	}
	UPROPERTY( EditAnywhere  , Category = "BFTrackZombieSpawnSlot")
		bool	bDynamic;


};

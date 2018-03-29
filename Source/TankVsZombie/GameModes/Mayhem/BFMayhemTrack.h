#pragma once
#include "BFRoadSpline.h"
#include "BFMayhemTrack.Generated.h"


USTRUCT(BlueprintType)
struct FTrackPointSpec
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "TrackPointSpec")
		float SpeedFactor;
	UPROPERTY(EditDefaultsOnly, Category = "TrackPointSpec")
		int32 PointIndex; 

};

UCLASS(Blueprintable)
class ABFMayhemTrack : public ABFRoadSpline
{
	GENERATED_UCLASS_BODY()

public: 
	 


};

#pragma once
#include <Components/SplineComponent.h>
#include "BFTrackZombieSpawnSlot.h"
#include "BFTrackVisualPart.Generated.h"


class ABFTrackBuilder;
UCLASS(Blueprintable)
class ABFTrackVisualPart : public AActor
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY( VisibleAnywhere , Category = "TrackVisualPart" )
		UArrowComponent* TrackStartAnchor	;
	UPROPERTY( VisibleAnywhere , Category = "TrackVisualPart" )
		UArrowComponent* TrackEndAnchor		;

	UPROPERTY(VisibleAnywhere, Category = "TrackVisualPart")
		USplineComponent* TrackSpline		;
	UPROPERTY(VisibleAnywhere, Category = "TrackVisualPart")
		USplineComponent* LeftTrackSpline	;
	UPROPERTY(VisibleAnywhere, Category = "TrackVisualPart")
		USplineComponent* RightTrackSpline	;
	
	UPROPERTY(VisibleAnywhere, Category = "TrackVisualPart")
		UBoxComponent*	  ZoneVolume		;

	UPROPERTY()
		ABFTrackBuilder* TrackBuilder		; 

	UPROPERTY()
		TArray<UBFTrackZombieSpawnSlot*> DynamicSpawnSlots ;

	virtual void BeginPlay() override ;

	FVector GetStartAnchorPosition() const ;
	FVector GetEndAnchorPosition() const ;
	float GetTrackLength() const ;
	float GetRelativeOffset(APawn* Pawn) const ;

};



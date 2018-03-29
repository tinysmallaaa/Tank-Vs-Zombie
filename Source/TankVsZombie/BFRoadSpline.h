#pragma once 
#include "Components/SplineComponent.h"
#include "BFRoadSpline.Generated.h"


UCLASS(Blueprintable)
class TANKVSZOMBIE_API ABFRoadSpline : public AActor
{ 
	GENERATED_UCLASS_BODY()

public: 

	UPROPERTY( EditDefaultsOnly , Category = "RoadSpline" )
		USplineComponent* SplineComp; 
	 
	float GetLength(); 
	FVector GetWorldLocationAtTime(float Distance);
	FVector GetDirectionAtTime(float Distance);
};

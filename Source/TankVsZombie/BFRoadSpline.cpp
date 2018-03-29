#include "TankVsZombie.h"
#include "BFRoadSpline.h"
 
ABFRoadSpline::ABFRoadSpline(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{ 

	SplineComp =  ObjectInitializer.CreateDefaultSubobject<USplineComponent>( this , TEXT("Path") )  ;
	RootComponent = SplineComp; 

} 

float ABFRoadSpline::GetLength()
{ 
	return SplineComp->GetSplineLength(); 
}

FVector ABFRoadSpline::GetWorldLocationAtTime(float Distance)
{ 
	return SplineComp->GetWorldLocationAtDistanceAlongSpline( Distance   ); 
}

FVector ABFRoadSpline::GetDirectionAtTime(float Distance)
{ 
	return SplineComp->GetDirectionAtDistanceAlongSpline( Distance , ESplineCoordinateSpace::World );  
}
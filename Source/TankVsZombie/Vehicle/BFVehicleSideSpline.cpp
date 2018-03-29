#include "TankVsZombie.h"
#include "BFVehicleSideSpline.h"



UBFVehicleSideSpline::UBFVehicleSideSpline(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UBFVehicleSideSpline::Setup()
{
	AttachmentSlots.Reset();

	for (int32 i = 1; i < MaxAllowed-1; i++)
	{
		FVehicleAttachmentSlot Slot;
		Slot.PointIndex = i ;
		Slot.Spline = this  ;
		AttachmentSlots.Add( Slot );
	}

}

FVehicleAttachmentSlot* UBFVehicleSideSpline::GetNearestSlot(FVector Location)
{

	float ShortestDist = 10000000.0f;
	float ShortestIndex = 0;
	for ( int32 i = 1 ; i < AttachmentSlots.Num()-1 ; i++ )
	{
		float Dist = FVector(GetWorldLocationAtSplinePoint(i) - Location).Size() ;
		if (ShortestDist > Dist)
		{
			ShortestDist = Dist;
			ShortestIndex = i;
		}
	}

	return &AttachmentSlots[ShortestIndex];

}

FVehicleAttachmentSlot* UBFVehicleSideSpline::GetAttachmentSlot(int32 Index) 
{

	return &AttachmentSlots[Index];

}

int32 UBFVehicleSideSpline::GetAttachmentSlotCount()
{
	return AttachmentSlots.Num();
} 
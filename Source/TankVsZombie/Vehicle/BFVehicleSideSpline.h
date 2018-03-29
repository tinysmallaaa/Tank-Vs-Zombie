#pragma once
#include "Components/SplineComponent.h"
#include "BFVehicleSideSpline.Generated.h"


USTRUCT(BlueprintType)
struct FVehicleAttachmentSlot
{
	GENERATED_USTRUCT_BODY()
		 
	int32   PointIndex ;

	UPROPERTY()
		class UBFVehicleSideSpline* Spline ;  

};

UCLASS(Blueprintable)
class UBFVehicleSideSpline : public USplineComponent
{
	GENERATED_UCLASS_BODY()
public:
	 
	UPROPERTY( EditDefaultsOnly , Category = "VehicleSideSpline" )
		int32 MaxAllowed ; 
  
	UPROPERTY( EditDefaultsOnly , Category = "VehicleSideSpline" )
		float Offset ;

	void Setup();

	FVehicleAttachmentSlot* GetAttachmentSlot(int32 Index);
	int32 GetAttachmentSlotCount(); 
	FVehicleAttachmentSlot* GetNearestSlot(FVector Location);


private:

	TArray<FVehicleAttachmentSlot> AttachmentSlots;

};
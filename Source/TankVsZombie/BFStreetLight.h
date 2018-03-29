#pragma once
#include "WorldCollision.h"
#include "BFStreetLight.Generated.h"


UCLASS(Blueprintable)
class TANKVSZOMBIE_API ABFStreetLight : public AActor
{ 
	GENERATED_UCLASS_BODY() 
public:
	UPROPERTY(VisibleAnywhere, Category = "StreetLight")
		UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere , Category = "StreetLight" ) 
		UMaterialBillboardComponent* BillboardComponent;

	virtual void BeginPlay() override;

	UFUNCTION()
		void CheckVisibility();

	virtual void BeginDestroy() override;
	
	void TraceResult( const FTraceHandle& TraceHandle , FTraceDatum& TraceData ); 

private:
	FTimerHandle VisibilityTimer;
	FTraceHandle CurTraceHandle;
};

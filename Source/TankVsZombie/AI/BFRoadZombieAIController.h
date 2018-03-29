#pragma once
#include "BFZombieAIController.h"
#include "BFRoadZombieAIController.Generated.h"


UCLASS( Blueprintable , Abstract )
class TANKVSZOMBIE_API ABFRoadZombieAIController : public ABFZombieAIController
{
	GENERATED_UCLASS_BODY()
public:
  
	UFUNCTION( BlueprintCallable , Category= "RoadZombieAIController" )
		bool CanAttachToVehicle( ABFTankVehicle* Vehicle , float Range ) ;

	UFUNCTION( BlueprintCallable , Category = "RoadZombieAIController" )
		void InitiateAttachmentAnim(ABFTankVehicle* Vehicle ) ;

};
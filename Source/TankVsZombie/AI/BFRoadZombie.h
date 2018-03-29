#pragma once
#include "BFZombie.h"
#include "BFRoadZombie.Generated.h"


UENUM(BlueprintType)
enum ERoadZombieType
{
	ROAD_ZOMBIE_STATIONARY,
	ROAD_ZOMBIE_ACTIVE,
};

UENUM(BlueprintType)
enum ERoadZombieTravelMode
{
	ROAD_ZOMBIE_STANDING,
	ROAD_ZOMBIE_ATTACKING_VEHICLE,
};

UCLASS( Blueprintable , Abstract )
class TANKVSZOMBIE_API ABFRoadZombie : public ABFZombie
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly , Category = "RoadZombie" )
		TEnumAsByte<ERoadZombieType> Type;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly , Category = "RoadZombie" )
		UAnimMontage* AttackMontage;  

	UPROPERTY( BlueprintReadOnly , Category = "RoadZombie" )
		bool bInterpolatingToVehicle;
	UPROPERTY( BlueprintReadWrite , Category = "RoadZombie" )
		ABFTankVehicle* AttachedVehicle; 
	 
	UFUNCTION( BlueprintImplementableEvent , Category = "RoadZombie" )
		void OnAttachedToVehicle();
	UFUNCTION( BlueprintImplementableEvent , Category = "RoadZombie" )
		void OnSmashedInVehicle();
	UFUNCTION( BlueprintImplementableEvent , Category = "RoadZombie" )
		void OnRoadKilled();  
 
	bool CanAttachToVehicle(ABFTankVehicle* VehicleTank, float Range);
	void InitiateAttachmentAnim(ABFTankVehicle* VehicleTank,float Time);

private:

	float InterpStartTime; 
	float InterpAnimTime;

};

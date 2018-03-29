#pragma once
#include "BFGameObjective.Generated.h"
 
UENUM(BlueprintType)
enum EMissionObjectiveType
{
	EOBJECTIVE_NONE				 ,
	EOBJECTIVE_HEADSHOT			 ,
	EOBJECTIVE_MISSION_COMPLETED ,
	EOBJECTIVE_KILLED_BY_TURRET	 ,
	EOBJECTIVE_KILLED_BY_SHELL	 ,
	EOBJECTIVE_KILLED_BY_GRENADE ,
	EOBJECTIVE_KILLED_BY_MINES	 ,
	EOBJECTIVE_KILLED_BY_FIRE	 ,
	EOBJECTIVE_KILLED_BY_COIL	 ,
	EOBJECTIVE_KILLED_BY_LASER	 ,
	EOBJECTIVE_SHOOTING_ACCURACY ,
	EOBJECTIVE_KILLED_TOTAL		 , 
}; 

USTRUCT(BlueprintType)
struct FMissionObjective
{
	GENERATED_USTRUCT_BODY()

	FMissionObjective() :
		Type(EMissionObjectiveType::EOBJECTIVE_NONE),
		TargetAmount(0),
		DisplayText(FText::FromString(TEXT(""))), 
		CurrentAmount(0), 
		Stars(0)
	{

	}

	UPROPERTY(	EditAnywhere	, BlueprintReadWrite	, Category = "MissionObjective" )
		TEnumAsByte<EMissionObjectiveType>	Type	;
	UPROPERTY(	EditAnywhere	, BlueprintReadWrite	, Category = "MissionObjective" )
		int32	TargetAmount	;
	UPROPERTY(	BlueprintReadWrite , Category = "MissionObjective" )
		FText	DisplayText		;
	UPROPERTY(	BlueprintReadWrite , Category = "MissionObjective" )
		int32	CurrentAmount	;
	UPROPERTY(	BlueprintReadWrite , Category = "MissionObjective" )
		int32	Stars			;
}; 

USTRUCT(BlueprintType)
struct FMissionObjectiveList : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMissionObjectiveList")
		FMissionObjective Objective0	;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMissionObjectiveList")
		FMissionObjective Objective1	;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMissionObjectiveList")
		FMissionObjective Objective2	;	

};

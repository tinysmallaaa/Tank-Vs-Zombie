  #pragma once 
#include "GameModes/BFGameModeBase.h"
#include "BFGameObjective.h"
#include "BFPlayerState.Generated.h"

UCLASS(Blueprintable)
class TANKVSZOMBIE_API ABFPlayerState : public APlayerState
{
	GENERATED_UCLASS_BODY()
public:  
	UPROPERTY(BlueprintReadWrite, Category = "BFPlayerState")
		bool bMissionCompleted;
	UPROPERTY( BlueprintReadWrite , Category = "BFPlayerState" )
		int32 TotalZombiesKilled	;
	UPROPERTY( BlueprintReadWrite , Category = "BFPlayerState" )
		int32 HeadShotsZombiesCount ;
	
	UPROPERTY( BlueprintReadWrite , Category = "BFPlayerState" )
		int32 RoadKillZombiesCount	; 
	UPROPERTY(BlueprintReadWrite, Category = "BFPlayerState")
		int32 KilledByGrenadeCount;
	UPROPERTY(BlueprintReadWrite, Category = "BFPlayerState")
		int32 KilledByMinesCount;
	UPROPERTY(BlueprintReadWrite, Category = "BFPlayerState")
		int32 KilledByGunCount;
	UPROPERTY(BlueprintReadWrite, Category = "BFPlayerState")
		int32 KilledByShellCount;
	UPROPERTY(BlueprintReadWrite, Category = "BFPlayerState")
		int32 KilledByCoilCount;
	UPROPERTY(BlueprintReadWrite, Category = "BFPlayerState")
		int32 KilledByFireCount; 


	UPROPERTY( BlueprintReadWrite , Category = "BFPlayerState" )
		int32 VehicleSmearedCount	; 

	UPROPERTY( BlueprintReadWrite , Category = "BFPlayerState" )
		int32 CreditsEarned		;


	UFUNCTION(BlueprintCallable, Category = "BFPlayerState")
		void ResetState();
	UFUNCTION(BlueprintCallable, Category = "BFPlayerState")
		int32 CalculateStarsEarned(FMissionObjectiveList& ObjectiveList);

	 
	UFUNCTION(BlueprintCallable, Category = "BFPlayerState")
		 FMissionObjective CheckObjective(FMissionObjective Objective); 

};



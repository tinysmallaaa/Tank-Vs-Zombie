#pragma once
#include "../BFActiveMission.h"
#include "BFActiveMissionHorde.Generated.h"
 

USTRUCT(BlueprintType)
struct FHordeWaveState
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite, Category = "HordeWaveState")
		int32 WaveID;
	UPROPERTY(BlueprintReadWrite, Category = "HordeWaveState")
		float CompletionTime;
};


UCLASS()
class ABFActiveMissionHorde : public ABFActiveMission
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(	BlueprintReadWrite	,	Category = "BFPlayerState"	)
		int32 WavesCompleted;
	UPROPERTY(	BlueprintReadWrite	,	Category = "BFPlayerState"	)
		int32 WavesMax;
	UPROPERTY(	BlueprintReadWrite	,	Category = "BFPlayerState"	)
		TArray<FHordeWaveState> HordeWaveStates;
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override; 
	 
};

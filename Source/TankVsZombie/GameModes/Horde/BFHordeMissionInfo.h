#pragma once
#include "BFHordeMissionInfo.Generated.h"

class ABFZombie;
UENUM(BlueprintType)
enum ESpawnDirType
{
	ESPAWN_DIR_LEFT,
	ESPAWN_DIR_RIGHT,
	ESPAWN_DIR_FRONT,
	ESPAWN_DIR_BACK,
	ESPAWN_DIR_RANDOM,
};

USTRUCT(BlueprintType)
struct FZombieBossInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZombieBossInfo")
		TSubclassOf<ABFZombie>		BossClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZombieBossInfo")
		TEnumAsByte<ESpawnDirType>  SpawnDirTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZombieBossInfo")
		int32						RewardCredits;

	AActor*		BossRef;
};

USTRUCT(BlueprintType)
struct FHordeSpawnGroup
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HordeSpawn")
		int32						Count;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HordeSpawn")
		TEnumAsByte<ESpawnDirType>  SpawnDirTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HordeSpawn")
		float						SpawnDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HordeSpawn")
		TArray<FZombieBossInfo>		BossSpawns;

	void Remove(AActor* Actor)
	{
		ActiveMembers.Remove(Actor);

		for (int i = 0; i < BossSpawns.Num(); i++)
		{
			if (BossSpawns[i].BossRef == Actor)
			{
				BossSpawns[i].BossRef = NULL;
			}
		}
	}
	TArray<AActor*> ActiveMembers;
	bool			bSpawned;
};

USTRUCT(BlueprintType)
struct FHordeWave
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HordeSpawn")
		TArray<FHordeSpawnGroup> SpawnGroups;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HordeWave")
		int32	MinSpawnDelay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HordeWave")
		int32	MaxSpawnDelay;

	void RemoveFromGroup(AActor* Actor)
	{
		for (int32 i = 0; i < SpawnGroups.Num(); i++)
		{
			SpawnGroups[i].Remove(Actor);
		}
	}

	bool AllGroupsSpawned()
	{
		return GroupItr >= SpawnGroups.Num();
	}

	int32 GetNumActiveMembers()
	{
		int32 MembersCount = 0;

		for (int32 i = 0; i < SpawnGroups.Num(); i++)
		{
			MembersCount += SpawnGroups[i].ActiveMembers.Num();
		}

		return MembersCount;
	}


	int32	GroupItr;
	float	StartTime;
	float   EndTime;

};



USTRUCT(BlueprintType)
struct FHordeMissionDBInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite, Category = "HordeMissionInfo")
		bool bLeftSpawnDir ;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite, Category = "HordeMissionInfo")
		bool bRightSpawnDir ;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite, Category = "HordeMissionInfo")
		bool bFrontSpawnDir ;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite, Category = "HordeMissionInfo")
		bool bBackSpawnDir ;

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite, Category = "HordeMissionInfo")
		TArray<TSubclassOf<ABFZombie>> ZombieClasses ;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite, Category = "HordeMissionInfo")
		TArray<FHordeWave>	HordeWaves ; 

	int32 WavesCompleted;
};


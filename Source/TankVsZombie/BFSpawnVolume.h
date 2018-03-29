#pragma once
#include "Gamemodes/BFGameModeBase.h"
#include "BFSpawnVolume.Generated.h"

 
struct FSpawnGroup
{
	int32						   SpawnGroupID;
	FTimerHandle				   SpawnTimerHandle;
	TArray<TSubclassOf<ABFZombie>> SpawnClasses;
	int32						   SpawnItr;
	int32						   SpawnCount;
};
 
UCLASS(Blueprintable , Abstract)
class ABFSpawnVolume : public AActor
{
	GENERATED_UCLASS_BODY()
public: 

	UPROPERTY( EditAnywhere , BlueprintReadWrite, Category = "BFSpawnVolume")
		bool bRandomSpeed;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "BFSpawnVolume" )
		UBoxComponent*					BoxComponent;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "BFSpawnVolume" )
		TEnumAsByte<ESpawnDirType>		SpawnDirTag; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BFSpawnVolume")
		TArray<TSubclassOf<ABFZombie>>	ZombieClasses;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "BFSpawnVolume" )
		float							SpawnDelay;
	
	UFUNCTION( BlueprintCallable , Category = "BFSpawnVolume" )
	void Spawn( TArray<TSubclassOf<ABFZombie>>& Classes , int32 Count , int32 SpawnGroupID ) ;

	
	UFUNCTION( BlueprintCallable , Category = "BFSpawnVolume" )
		void SpawnZombies( int32 Count );

	ABFZombie* SpawnZombie( UClass* BotClass , int32 SpawnGroupID) ;

	void SpawnNextZombie() ;

	void FindValidSpot(UClass* BotClass , FVector& SrcPosition  , FRotator& Rotator ) ;

private:
	TArray<FSpawnGroup> ActiveSpawnGroups;
};
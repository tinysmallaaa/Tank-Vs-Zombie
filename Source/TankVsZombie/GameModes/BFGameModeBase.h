// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
 
#include "BFGameSingleton.h"
#include "AI/BFAIBot.h"
#include "BFActiveMission.h"
#include "BFGameModeBase.generated.h"
 



class ABFTankPlayerController	;
UCLASS(Abstract)
class TANKVSZOMBIE_API ABFGameModeBase : public AGameMode
{ 
	GENERATED_UCLASS_BODY() 
public:  
	// The Unique ID for this 
	FGuid ContextGUID; 
	/** Handle for efficient management of DefaultTimer timer */
	FTimerHandle TimerHandle_DefaultTimer;

 

	UPROPERTY( BlueprintReadWrite , Category = "BFGameModeBase")
		int32 ExtraCreditsOnEachKill;


	UFUNCTION(BlueprintCallable, Category = "BFGameModeBase")
		void BeginMission();

	virtual void BeginMissionPlay() {}
	virtual void EndMissionPlay() {}

	virtual void PreInitializeComponents() override;
	virtual void DefaultTimer() { };

	virtual void OnLoadingMovieBegin() { };
	virtual void OnLoadingMovieEnd() { };

	virtual void PostInitProperties();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState(); 
	virtual void ScoreKill( AController* Killed , AController* KilledBy  , const FBotDamageParamEx& DamageParam) {}

	virtual void OnZombieKilled(class ABFZombie* Zombie) {}
	virtual void InitController(AController* Controller);

	virtual ABFAIBot* SpawnBot(UClass* BotClass, int32 TeamNum = 255);

	virtual void OnZombieSpawned(ABFZombie* Zombie, int32 SpawnGroupID) {}

	virtual void CheckMapStatus(FString MapPackageName, bool& bIsEpicMap, bool& bIsMeshedMap, bool& bHasRights);
	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;

	UClass*  GetSpawnClassForPawn(ABFTankPlayerController* PlayerController);
	UFUNCTION(	BlueprintImplementableEvent , Category = "BFGameMode")
		UClass* GetDefaultSpawnClass();

	UFUNCTION(	BlueprintCallable , Category = "BFGameMode" )
	virtual void DisallowFiring();
	UFUNCTION(	BlueprintCallable , Category = "BFGameMode" )
	virtual void AllowFiring();

	UFUNCTION( BlueprintCallable,BlueprintNativeEvent , Category = "BFGameMode" )
		void GameOver();	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent , Category = "BFGameMode")
		void MissionCompleted();

	virtual float ModifyDamageTaken(float Damage) { return Damage; }
	  
};

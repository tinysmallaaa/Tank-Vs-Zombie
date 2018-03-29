#pragma once 
#include "../BFGameModeBase.h" 
#include "BFGameObjective.h"
#include "BFHordeMissionInfo.h"
#include "BFActiveMissionHorde.h"
#include "AI/BFAIInfo.h"
#include "BFGameModeHorde.Generated.h"

class ABFSpawnVolume ;
class ABFZombieBoss;
class ABFZombieMiniBoss;
  
UCLASS(Blueprintable)
class TANKVSZOMBIE_API ABFGameModeHorde : public ABFGameModeBase
{
	GENERATED_UCLASS_BODY()
		 
	UPROPERTY( BlueprintReadWrite , Category = "BFGameModeHorde" )
		int32 CompletedWaves ;	
	UPROPERTY( BlueprintReadWrite , Category = "BFGameModeHorde" )
		FHordeMissionDBInfo MissionInfo ;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "GameModeHorde" )
		FBotAIParams AIParams ;



public:
 

	void	 SpawnNextWave() ;
	void	 CacheEncounterVolumes() ;	
	UFUNCTION(BlueprintNativeEvent, Category = "BFGameModeHorde")
		void OnMissionStarted();

	virtual void BeginMissionPlay();
	virtual void EndMissionPlay();
		void OnWaveEnded(FHordeWave* Wave) ; 

	UFUNCTION(BlueprintCallable, Category = "BFGameModeHorde" )
		void FinishMissionAndSave();
	UFUNCTION( BlueprintNativeEvent , Category = "BFGameModeHorde")
		void OnBossKilled(ABFZombieBoss* Zombie) ;	
	UFUNCTION( BlueprintNativeEvent , Category = "BFGameModeHorde" )
		void OnMiniBossKilled( ABFZombieMiniBoss* Zombie ) ;

	virtual void OnZombieKilled(ABFZombie* Zombie) override ; 
	UFUNCTION( BlueprintNativeEvent , Category = "BFGameModeHorde" )
		void SetupMission() ;
		void FinishMission();

	UFUNCTION( BlueprintImplementableEvent , Category = "BFGameModeHorde" )
		void OnMissionEnded();
	UFUNCTION( BlueprintImplementableEvent , Category = "BFGameModeHorde" )
		void OnWaveFinished(int32 WaveID);
	UFUNCTION( BlueprintImplementableEvent , Category = "BFGameModeHorde" )
		void OnWaveStarted(int32 WaveID);

	UFUNCTION(BlueprintImplementableEvent, Category = "BFGameModeHorde")
		void ReportWavesStatus( int32 TotalWaves , int32 WavesCompleted );
	virtual void Tick(float DeltaSeconds) override ;
	virtual void BeginPlay() override ;

 	virtual void  ScoreKill( AController* Killed , AController* KilledBy  , const FBotDamageParamEx& DamageParam ) override ;
	void		  SpawnScoreText( APlayerController* PlayerController , FVector ScreenPosition , int ScoreAmount ) ;
	virtual float ModifyDamageTaken( float Damage ) override; 
	
	TArray<ABFSpawnVolume*> GetSpawnVolumes(ESpawnDirType DirType);

	void			SpawnDistributed( int32  GroupItr , TArray<ABFSpawnVolume*>& SpawnVolumes , TArray<TSubclassOf<ABFZombie>>& ZombieClasses , int32 Count);

	ABFSpawnVolume* GetSpawnVolume(ESpawnDirType DirType);
	
	UFUNCTION()
	void	SpawnNextGroupInWave();
	
	void	ScheduleNextGroupInWave();

	int32	GetRewardCreditForKilled( const FBotDamageParamEx& DamageParam , ABFZombie* Zombie , int32 Credits );	
	virtual void OnZombieSpawned(ABFZombie* Zombie, int32 SpawnGroupID) override;

	
	UFUNCTION()
	void	CheckIfWaveEnded();  


public: 

	UPROPERTY()
		TArray<ABFSpawnVolume*>		LeftDirSpawnVolumes;	
	UPROPERTY()
		TArray<ABFSpawnVolume*>		RightDirSpawnVolumes;	
	UPROPERTY()
		TArray<ABFSpawnVolume*>		FrontDirSpawnVolumes;	
	UPROPERTY()
		TArray<ABFSpawnVolume*>		BackDirSpawnVolumes;

private: 

	ABFActiveMissionHorde*		ActiveMission	 ;

	FHordeWave*					CurrentWave			 ;		
	int32						NumMiniBossKilled	 ;
	int32						NumMainBossKilled	 ;

	FTimerHandle				NextWaveTimer		 ;
	ESpawnDirType				PrevSpawnDir		 ; 


};

#include "TankVsZombie.h" 
#include "Vehicle/BFTankPlayerController.h" 
#include "AI/BFBotAIController.h" 
#include "BFGameHUD.h"
#include "AI/BFZombieBoss.h"
#include "AI/BFZombieMiniBoss.h"
#include "BFLocalPlayer.h"
#include "DamageTypes/BFDamageType_Grenade.h"
#include "DamageTypes/BFDamageType_Mine.h"
#include "DamageTypes/BFDamageType_Fire.h"
#include "DamageTypes/BFDamageType_Coil.h"
#include "DamageTypes/BFDamageType_Shell.h"

#include "BFGameModeHorde.h"

ABFGameModeHorde::ABFGameModeHorde(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}  

void ABFGameModeHorde::BeginMissionPlay() 
{   
	OnMissionStarted()		;

	CurrentWave = NULL		; 
	NumMiniBossKilled = 0	;
	NumMiniBossKilled = 0	;

	UBFLocalPlayer* LocalPlayer = Cast<UBFLocalPlayer>( GetWorld()->GetFirstPlayerController()->GetLocalPlayer() );

	ActiveMission = Cast<ABFActiveMissionHorde>(LocalPlayer->ActiveMission);

	if ( LocalPlayer != NULL )
	{
 		MissionInfo = ActiveMission->GetHordeMissionInfo() ;
		
		ActiveMission->ObjectiveList.Objective0.DisplayText = LocalPlayer->GetDisplayTextForObjective(ActiveMission->ObjectiveList.Objective0 ) ;
		ActiveMission->ObjectiveList.Objective1.DisplayText = LocalPlayer->GetDisplayTextForObjective(ActiveMission->ObjectiveList.Objective1 ) ;
		ActiveMission->ObjectiveList.Objective2.DisplayText = LocalPlayer->GetDisplayTextForObjective(ActiveMission->ObjectiveList.Objective2 ) ;
	} 
	 

	SpawnNextWave()	;  
}

void ABFGameModeHorde::EndMissionPlay()
{

}
  
void ABFGameModeHorde::ScheduleNextGroupInWave()
{ 
	CurrentWave->GroupItr++ ;

	if ( CurrentWave->GroupItr < CurrentWave->SpawnGroups.Num() )
	{ 
		FTimerHandle Timer ;
		float Delay = CurrentWave->SpawnGroups[CurrentWave->GroupItr].SpawnDelay;
		GetWorldTimerManager().SetTimer( Timer , this , &ABFGameModeHorde::SpawnNextGroupInWave , Delay <= 0.0f ? 2.0f : Delay , false );
 	}
	else
	{
		CheckIfWaveEnded() ;
	} 
}

void ABFGameModeHorde::SpawnDistributed( int32  GroupItr , TArray<ABFSpawnVolume*>& SpawnVolumes , TArray<TSubclassOf<ABFZombie>>& ZombieClasses , int32 Count )
{
	if (SpawnVolumes.Num())
	{
		int32 Offset = Count / SpawnVolumes.Num();
		int32 z	= 0 ;
		for ( int32 volumeIndex = 0 ; volumeIndex < SpawnVolumes.Num() ; volumeIndex++ )
		{ 
			
			if(SpawnVolumes[volumeIndex]!=NULL)
				SpawnVolumes[volumeIndex]->Spawn( ZombieClasses , Offset ,  GroupItr );
			
			z += Offset;

		}
	} 
}

void ABFGameModeHorde::SpawnNextGroupInWave()
{
	if ( CurrentWave != NULL && CurrentWave->GroupItr < CurrentWave->SpawnGroups.Num() )
	{ 
		FHordeSpawnGroup& SpawnGroup = CurrentWave->SpawnGroups[ CurrentWave->GroupItr ]; 

		TArray<ABFSpawnVolume*> SpawnVolumes ; 
		 
			if (SpawnGroup.Count > 0)
			{  
				SpawnVolumes = GetSpawnVolumes(SpawnGroup.SpawnDirTag);

				if (SpawnVolumes.Num()==0)
				{
					SpawnVolumes.Add( GetSpawnVolume(ESPAWN_DIR_RANDOM) );
				} 

				if ( SpawnVolumes.Num() )
				{
					SpawnDistributed( CurrentWave->GroupItr , SpawnVolumes , MissionInfo.ZombieClasses , SpawnGroup.Count );
				}  
			}

			if (SpawnGroup.BossSpawns.Num() > 0)
			{  
				ABFSpawnVolume* SpawnVolume = NULL;

				for (int32 i = 0; i < SpawnGroup.BossSpawns.Num(); i++)
				{    
					FZombieBossInfo& Info = SpawnGroup.BossSpawns[i] ; 

					SpawnVolume = GetSpawnVolume( Info.SpawnDirTag ) ;

					if (SpawnVolume == NULL)
					{
						SpawnVolume = GetSpawnVolume( ESPAWN_DIR_RANDOM ) ;
					}

					if (Info.BossClass != NULL)
					{ 
						TArray<TSubclassOf<ABFZombie>> ZombieClasses;
						ZombieClasses.Add( Info.BossClass.Get() ); 

						if (SpawnVolume != NULL)
						{
							SpawnVolume->Spawn(ZombieClasses, 1, CurrentWave->GroupItr);
						}
					}  
				}   
			}

			SpawnGroup.bSpawned = true;
		 
		ScheduleNextGroupInWave() ; 
	}
}


void ABFGameModeHorde::OnZombieSpawned(ABFZombie* Zombie,int32 SpawnGroupID)
{ 
	if ( CurrentWave != NULL )
	{ 
		FHordeSpawnGroup& CurrentGroup = CurrentWave->SpawnGroups[ SpawnGroupID ] ;
		CurrentGroup.ActiveMembers.Add( Zombie ); 

		ABFAIController* AIController = Cast<ABFAIController>(Zombie->GetController());

		if (AIController != NULL)
		{
			AIController->SetAIParams( ActiveMission->MissionInfo.AIParams );
		} 

	}
}

void ABFGameModeHorde::SpawnNextWave()
{

	if ( MissionInfo.WavesCompleted < MissionInfo.HordeWaves.Num() )
	{     
		FHordeWave* HordeWave = &MissionInfo.HordeWaves[ MissionInfo.WavesCompleted ] ;  
		if ( HordeWave != NULL )
		{ 	 
			OnWaveStarted( MissionInfo.WavesCompleted + 1 );

			ReportWavesStatus(MissionInfo.HordeWaves.Num(), MissionInfo.WavesCompleted + 1);
			
			CurrentWave = HordeWave ;  
			FTimerHandle Timer		;
			float Delay = CurrentWave->SpawnGroups[CurrentWave->GroupItr].SpawnDelay;
			
			GetWorldTimerManager().SetTimer( Timer , this ,  &ABFGameModeHorde::SpawnNextGroupInWave , Delay <= 0.0f ? 2.0f : Delay, false );	
		} 

	} else if ( MissionInfo.HordeWaves.Num() == 0 )
	{ 
		OnWaveEnded( NULL ); 
	}

}

void ABFGameModeHorde::FinishMissionAndSave()
{ 

	ABFTankPlayerController* PC = Cast<ABFTankPlayerController>( GetWorld()->GetFirstPlayerController() );

	if ( PC != NULL )
	{
		UBFLocalPlayer* LocalPlayer = PC->GetLocalPlayer() ;

		ABFPlayerState* PlayerState = PC->GetPlayerState() ;

		
		if ( LocalPlayer != NULL )
		{
			LocalPlayer->AddCredits( PlayerState->CreditsEarned	)  ;
			LocalPlayer->CurrentMissionState.bUnlocked		= true ;
			LocalPlayer->CurrentMissionState.CreditsEarned	= PlayerState->CreditsEarned  ;
			LocalPlayer->CurrentMissionState.StarsEarned	= PlayerState->CalculateStarsEarned( ActiveMission->ObjectiveList ) ;
			LocalPlayer->SaveMissionState( LocalPlayer->CurrentChapterID , LocalPlayer->CurrentMissionState ) ;
		}
	}

}

void ABFGameModeHorde::BeginPlay()
{	 

	Super::BeginPlay(); 

	CacheEncounterVolumes();  

	SetupMission();   

} 

void ABFGameModeHorde::OnMissionStarted_Implementation()
{

}

void ABFGameModeHorde::SetupMission_Implementation()
{

}
 
void ABFGameModeHorde::CheckIfWaveEnded()
{ 
	if ( CurrentWave != NULL )
	{  
		if (CurrentWave->AllGroupsSpawned())
		{
			if (CurrentWave->GetNumActiveMembers() == 0)
			{
				MissionInfo.WavesCompleted++;
				OnWaveEnded(CurrentWave);
				CurrentWave = NULL;
				FTimerHandle Handle;
				GetWorldTimerManager().SetTimer( Handle , this , &ABFGameModeHorde::SpawnNextWave , 4.0f , false );
			}
		}
	} 
}

void ABFGameModeHorde::OnZombieKilled( ABFZombie* Zombie )
{
  	UClass* Class = Zombie->GetClass() ; 

	if ( CurrentWave != NULL )
	{   
		CurrentWave->RemoveFromGroup( Zombie ) ; 

		CheckIfWaveEnded(); 
	} 
}

void ABFGameModeHorde::OnBossKilled_Implementation(ABFZombieBoss* Zombie)
{

}

void ABFGameModeHorde::OnMiniBossKilled_Implementation(ABFZombieMiniBoss* Zombie)
{

}

void ABFGameModeHorde::CacheEncounterVolumes()
{	
	UWorld* World = GetWorld() ; 
  
	if ( World != NULL )
	{

		ULevel* Level = World->GetLevel( 0 ) ;
		TArray<AActor*>& Actors = Level->Actors ;

		for ( int32 i = 0 ; i < Actors.Num() ; i++ )
		{
			AActor* Actor = Actors[i] ;
			ABFSpawnVolume* SpawnVolume = Cast<ABFSpawnVolume>( Actor ) ;
			
			if ( SpawnVolume != NULL  )
			{  
				if (SpawnVolume->SpawnDirTag == ESpawnDirType::ESPAWN_DIR_LEFT)
				{
					LeftDirSpawnVolumes.Add(SpawnVolume); 
				}
				else if (SpawnVolume->SpawnDirTag == ESpawnDirType::ESPAWN_DIR_RIGHT)
				{
					RightDirSpawnVolumes.Add(SpawnVolume);
				}
				else if (SpawnVolume->SpawnDirTag == ESpawnDirType::ESPAWN_DIR_FRONT)
				{
					FrontDirSpawnVolumes.Add(SpawnVolume);
				}
				else if (SpawnVolume->SpawnDirTag == ESpawnDirType::ESPAWN_DIR_BACK)
				{
					FrontDirSpawnVolumes.Add(SpawnVolume);
				} 
			}
		}  
	} 

}

void ABFGameModeHorde::OnWaveEnded( FHordeWave* Wave )
{    
	if ( Wave != NULL )
	{  

		Wave->EndTime = GetWorld()->GetTimeSeconds() ;

		ABFTankPlayerController* PC = Cast<ABFTankPlayerController>( GetWorld()->GetFirstPlayerController() ) ;
		ABFPlayerState* PlayerState = Cast<ABFPlayerState>( PC->PlayerState ) ;
		
		if ( PlayerState != NULL )
		{

			FHordeWaveState WaveState ;
			WaveState.CompletionTime = Wave->EndTime - Wave->StartTime ;
			WaveState.WaveID		 = ActiveMission->WavesCompleted ;

			ActiveMission->HordeWaveStates.Add( WaveState );

			if (ActiveMission->WavesCompleted <  MissionInfo.HordeWaves.Num())
			{
				ActiveMission->WavesCompleted++;
			}

		} 

		OnWaveFinished( ActiveMission->WavesCompleted );
		ReportWavesStatus( MissionInfo.HordeWaves.Num() , ActiveMission->WavesCompleted );

	}

	if ( MissionInfo.WavesCompleted < MissionInfo.HordeWaves.Num() ) 
	{  
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &ABFGameModeHorde::SpawnNextWave, 2.0f); 
	} else
	{ 
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &ABFGameModeHorde::FinishMission, 2.0f); 
	}
}

 
void ABFGameModeHorde::FinishMission()
{

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	ABFPlayerState* State = Cast<ABFPlayerState>(PlayerController->PlayerState);

	if (State != NULL)
	{
		State->bMissionCompleted = true;
	}

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &ABFGameModeHorde::OnMissionEnded, 3.0f);

}
 
void ABFGameModeHorde::Tick( float DeltaSeconds )
{
	Super::Tick(DeltaSeconds) ;

}

void ABFGameModeHorde::SpawnScoreText( APlayerController * PlayerController , FVector  ScreenPosition , int32 ScoreAmount )
{

	FString Score( "+" ) ;
	Score.AppendInt(ScoreAmount);
 
	ABFGameHUD* GameHUD = Cast<ABFGameHUD>( PlayerController->GetHUD() );

	if (GameHUD != NULL)
	{
		GameHUD->SpawnBonusText(Score, ScreenPosition ) ;
	}

}

float ABFGameModeHorde::ModifyDamageTaken( float Damage )
{  
	return Damage ;
}
TArray<ABFSpawnVolume*> ABFGameModeHorde::GetSpawnVolumes(ESpawnDirType DirType)
{
	TArray<ABFSpawnVolume*> Volumes;

	switch (DirType)
	{
		case ESPAWN_DIR_LEFT:
		{
			if (LeftDirSpawnVolumes.Num())
				return LeftDirSpawnVolumes;

		}break;

		case ESPAWN_DIR_RIGHT:
		{
			if (RightDirSpawnVolumes.Num())
				return RightDirSpawnVolumes;

		}break;

		case ESPAWN_DIR_FRONT:
		{	
			if (FrontDirSpawnVolumes.Num())
				return FrontDirSpawnVolumes;
		}break;

		case ESPAWN_DIR_BACK:
		{
			if (BackDirSpawnVolumes.Num())
				return BackDirSpawnVolumes;

		}break;
		case ESPAWN_DIR_RANDOM:
		{
			ABFSpawnVolume* SpawnVolume = NULL;
			int32 maxAttempts = 10;
			while (maxAttempts--)
			{
				int32 RandomDir = FMath::RandRange(0, 3);
				if (RandomDir == 0)
				{
					SpawnVolume = GetSpawnVolume(ESPAWN_DIR_FRONT);
				}
				else if (RandomDir == 1)
				{
					SpawnVolume = GetSpawnVolume(ESPAWN_DIR_BACK);
				}
				else if (RandomDir == 2)
				{
					SpawnVolume = GetSpawnVolume(ESPAWN_DIR_LEFT);
				}
				else if (RandomDir == 3)
				{
					SpawnVolume = GetSpawnVolume(ESPAWN_DIR_RIGHT);
				}
				if (SpawnVolume != NULL)
				{
					Volumes.Add(SpawnVolume);
				}
			}

		}break;

	}

	return Volumes;
}
ABFSpawnVolume* ABFGameModeHorde::GetSpawnVolume(ESpawnDirType DirType)
{
	ABFSpawnVolume* SpawnVolume = NULL;

	switch (DirType)
	{
		case ESPAWN_DIR_LEFT:
		{ 
			if (LeftDirSpawnVolumes.Num())
			SpawnVolume = LeftDirSpawnVolumes[ FMath::RandRange( 0 , LeftDirSpawnVolumes.Num() - 1 ) ];  

		}break;

		case ESPAWN_DIR_RIGHT:
		{
			if(RightDirSpawnVolumes.Num())
 			SpawnVolume = RightDirSpawnVolumes[ FMath::RandRange( 0 , RightDirSpawnVolumes.Num() - 1 ) ]; 

		}break;

		case ESPAWN_DIR_FRONT:
		{
			if (FrontDirSpawnVolumes.Num())
			SpawnVolume = FrontDirSpawnVolumes[	FMath::RandRange( 0 , FrontDirSpawnVolumes.Num() - 1 ) ]; 

		}break;

		case ESPAWN_DIR_BACK:
		{
			if (BackDirSpawnVolumes.Num())
			SpawnVolume = BackDirSpawnVolumes[	FMath::RandRange( 0 , BackDirSpawnVolumes.Num() - 1	) ]; 

		}break;
		case ESPAWN_DIR_RANDOM:
		{

			int32 maxAttempts = 10;
			while (maxAttempts--)
			{
				int32 RandomDir = FMath::RandRange(0, 3);
				if (RandomDir == 0)
				{
					SpawnVolume = GetSpawnVolume(ESPAWN_DIR_FRONT);
				}
				else if (RandomDir == 1)
				{
					SpawnVolume = GetSpawnVolume(ESPAWN_DIR_BACK);
				}
				else if (RandomDir == 2)
				{
					SpawnVolume = GetSpawnVolume(ESPAWN_DIR_LEFT);
				}
				else if (RandomDir == 3)
				{
					SpawnVolume = GetSpawnVolume(ESPAWN_DIR_RIGHT);
				}
				if (SpawnVolume != NULL)
				{
					return SpawnVolume;
				}
			} 

		}break;

	} 

	return SpawnVolume;
}

int32 ABFGameModeHorde::GetRewardCreditForKilled( const FBotDamageParamEx& DamageParam  , ABFZombie* Zombie , int32 Credits )
{

	int32 CreditsEarned = Credits;

 
	if (CurrentWave != NULL)
	{
		for (int32 i = 0; i < CurrentWave->SpawnGroups.Num(); i++)
		{
			FHordeSpawnGroup& SpawnGroup = CurrentWave->SpawnGroups[i];

			for (int32 b = 0; b < SpawnGroup.BossSpawns.Num();b++)
			{
				if (SpawnGroup.BossSpawns[b].BossRef == Zombie)
				{
					CreditsEarned = SpawnGroup.BossSpawns[b].RewardCredits;
				}
			}
		}
	}

	return CreditsEarned ;

}

void  ABFGameModeHorde::ScoreKill(AController* Killed, AController* KilledBy, const FBotDamageParamEx& DamageParam )
{

	if (KilledBy && KilledBy->IsA(ABFTankPlayerController::StaticClass()))
	{

		if (Killed)
		{
			
			FVector WorldLocation						= Killed->GetPawn()->GetActorLocation() ;
			ECreditType CreditType						= ECREDIT_5 ;
			ABFTankPlayerController* PlayerController	= Cast<ABFTankPlayerController>(KilledBy) ;
			ABFPlayerState* PlayerState					= Cast<ABFPlayerState>(PlayerController->PlayerState) ; 
			ABFGameHUD* HUD								= Cast<ABFGameHUD>(PlayerController->GetHUD()) ;
			ABFZombie* KilledZombie						= Cast<ABFZombie>(Killed->GetPawn());
			
			UDamageType const* const DamageType = DamageParam.DamageType ? DamageParam.DamageType->GetDefaultObject<UDamageType>() : NULL ;
			int32 CreditsEarned = 0 ;   
			
			if ( PlayerState != NULL && KilledZombie != NULL )
			{ 

				if ( Cast<ABFZombieAIController>( Killed ) != NULL)
				{
					CreditType = ECREDIT_5 ; 
					CreditsEarned = 5 ;
					if ( DamageParam.bHeadShot )
					{
						CreditsEarned = 10 ;
						CreditType = ECREDIT_10 ; 
						PlayerState->HeadShotsZombiesCount++ ; 
					}
					else if ( DamageParam.bVehicleSmashed )
					{
						CreditsEarned = 15 ;
						CreditType = ECREDIT_15 ; 
						PlayerState->VehicleSmearedCount++ ;
					}
					else if ( DamageParam.bRoadKill )
					{
						CreditsEarned = 5 ;
						CreditType = ECREDIT_5 ; 
						PlayerState->RoadKillZombiesCount++ ;
					}
					else if ( Cast<UBFDamageType_Grenade>( DamageType ))
					{
						PlayerState->KilledByGrenadeCount++ ;
					}
					else if ( Cast<UBFDamageType_Mine>( DamageType ))
					{
						PlayerState->KilledByMinesCount++ ;
					}
					else if ( Cast<UBFDamageType_Fire>( DamageType ))
					{
						PlayerState->KilledByFireCount++ ;
					}
					else if ( Cast<UBFDamageType_Shell>( DamageType ))
					{
						PlayerState->KilledByShellCount++ ;
					}
					else if ( Cast<UBFDamageType_Coil>( DamageType ))
					{
						PlayerState->KilledByCoilCount++ ;
					} 

					CreditsEarned = GetRewardCreditForKilled( DamageParam , KilledZombie , CreditsEarned );

					PlayerState->TotalZombiesKilled++ ;

					if (HUD != NULL)
					{  
						FString BonusText = "$";
						BonusText.AppendInt(CreditsEarned);

						HUD->SpawnBonusText( BonusText , WorldLocation ) ;
						
						if ( DamageParam.bHeadShot )
						{
   							HUD->SpawnHeadShotAnim( FLinearColor::White, DamageParam.HeadBoneLocation, 1.5f);
						} 
					} 

				}
				
				PlayerState->CreditsEarned += CreditsEarned ;
				PlayerController->UpdateScoreForKilled( PlayerState , KilledZombie ) ; 
				OnZombieKilled( KilledZombie );
			}
		}
	}

}
 
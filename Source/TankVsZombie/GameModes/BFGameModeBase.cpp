#include "TankVsZombie.h"
#include "BFGameState.h"
#include "Vehicle/BFTankPlayerController.h"
#include "AI/BFAIController.h"
#include "BFGameSaveData.h"
#include "BFGameModeBase.h"

ABFGameModeBase::ABFGameModeBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void ABFGameModeBase::BeginMission()
{
	BeginMissionPlay();
}

void ABFGameModeBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &ABFGameModeBase::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation() / GetWorldSettings()->DemoPlayTimeDilation, true);
}

void ABFGameModeBase::PostInitProperties()
{
	Super::PostInitProperties(); 

}

ABFAIBot* ABFGameModeBase::SpawnBot( UClass* BotClass , int32 TeamNum )
{

	ABFAIBot* Bot = GetWorld()->SpawnActor<ABFAIBot>(BotClass);


	return Bot;
}

void ABFGameModeBase::InitController(AController* Controller)
{

	IBFTeamInterface* TeamInterface = Cast<IBFTeamInterface>(Controller);

	if ( TeamInterface != NULL )
	{
		if ( Cast<ABFTankPlayerController>(Controller) )
		{
			TeamInterface->SetTeamNum(TEAM_PLAYER);
		}
		else if (Cast<ABFAIController>(Controller))
		{
			TeamInterface->SetTeamNum(TEAM_ENEMY);  
		}
	}

}

void ABFGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	 
	//Make an instance GUID to report back in analytics. This is to track all actions that happened in a single context.
	ContextGUID = FGuid::NewGuid();
 
	Super::InitGame(MapName, Options, ErrorMessage);

#ifndef FINAL_RELEASE
	GEngine->Exec(this->GetWorld(), TEXT("stat fps"));
#endif 	 

}

void ABFGameModeBase::InitGameState()
{
	Super::InitGameState();
	ABFGameState* GS = Cast<ABFGameState>(GameState);

	if ( GS != NULL )
	{

	}
} 

void ABFGameModeBase::CheckMapStatus(FString MapPackageName, bool& bIsEpicMap, bool& bIsMeshedMap, bool& bHasRights)
{ 

}

UClass* ABFGameModeBase::GetSpawnClassForPawn(ABFTankPlayerController* PlayerController)
{
	UBFGameSaveData* PlayerSettings =  PlayerController->GetSaveGameData();
	
	if (PlayerSettings != NULL && PlayerSettings->PlayerState.VehicleState.VehicleClass != NULL )
	{
		return PlayerSettings->PlayerState.VehicleState.VehicleClass;
	}

	return GetDefaultSpawnClass();
} 

void ABFGameModeBase::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{ 

	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	if (!StartSpot)
	{
		UE_LOG( BFLOG , Warning , TEXT("RestartPlayerAtPlayerStart: Player start not found") );
		return;
	}

	FRotator SpawnRotation = StartSpot->GetActorRotation();

	SpawnRotation.Pitch		= 0.0f ;
	SpawnRotation.Roll		= 0.0f  ;	
	
	ABFTankPlayerController* PlayerController = Cast<ABFTankPlayerController>(NewPlayer);
	
	if (PlayerController != NULL)
	{ 
		UClass* PawnClass = GetSpawnClassForPawn(PlayerController);
		FTransform SpawnTransform = StartSpot->GetTransform();
		SpawnTransform.SetRotation( SpawnRotation.Quaternion() );

		if (PawnClass != NULL)
		{
			APawn* NewPawn = Cast<APawn>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, PawnClass, SpawnTransform));
			UGameplayStatics::FinishSpawningActor(NewPawn, SpawnTransform);
			NewPlayer->SetPawn(NewPawn);
		} 
	}

	if ( NewPlayer != NULL && NewPlayer->GetPawn() == nullptr)
	{ 
		NewPlayer->FailedToSpawnPawn(); 
	}
	else
	{ 
		// Tell the start spot it was used
		InitStartSpot(StartSpot, NewPlayer);
		FinishRestartPlayer(NewPlayer, SpawnRotation); 
	}

}

void  ABFGameModeBase::GameOver_Implementation()
{

}

void  ABFGameModeBase::MissionCompleted_Implementation()
{

}

void ABFGameModeBase::AllowFiring()
{
	for (FConstControllerIterator Itr = GetWorld()->GetControllerIterator(); Itr; Itr++)
	{ 
		TWeakObjectPtr<AController> Controller = *Itr;

		ABFAIController* AIController = Cast<ABFAIController>(Controller.Get());

		if (AIController != NULL)
		{
			AIController->AllowFiring();
		}
	}
}

void  ABFGameModeBase::DisallowFiring()
{ 

	for (FConstControllerIterator Itr = GetWorld()->GetControllerIterator(); Itr; Itr++)
	{

		TWeakObjectPtr<AController> Controller =  *Itr;

		ABFAIController* AIController = Cast<ABFAIController>(Controller.Get());
		
		if ( AIController!=NULL )
		{
			AIController->DisallowFiring();
		}

	}

}

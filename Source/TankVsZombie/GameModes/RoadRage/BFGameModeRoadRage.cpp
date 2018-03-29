#include "TankVsZombie.h"
#include "BFGameModeRoadRage.h"


ABFGameModeRoadRage::ABFGameModeRoadRage(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void ABFGameModeRoadRage::BeginMissionPlay()
{

}

void ABFGameModeRoadRage::EndMissionPlay()
{
	    
} 

void  ABFGameModeRoadRage::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void  ABFGameModeRoadRage::BeginPlay()
{
	Super::BeginPlay(); 

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass( GetWorld() , ABFTrackBuilder::StaticClass() , Actors );

	for ( int32 i = 0 ; i < Actors.Num() ; i++ )
	{
		ABFTrackBuilder* Builder = Cast<ABFTrackBuilder>(Actors[i]);
		if (Builder != NULL)
		{
			TrackBuilder = Builder;
			break;
		}
	} 

}

void  ABFGameModeRoadRage::ScoreKill(AController* Killed, AController* KilledBy, const FBotDamageParamEx& DamageParam)
{

}

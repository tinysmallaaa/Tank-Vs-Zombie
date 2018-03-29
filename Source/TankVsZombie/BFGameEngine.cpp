#include "TankVsZombie.h"
#include "MoviePlayer.h"
#include "BFLocalPlayer.h"
#include "BFGameEngine.h"

UBFGameEngine::UBFGameEngine(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UBFGameEngine::OnLoadingMoviePlaybackFinished()
{
	 
}

void UBFGameEngine::Init(IEngineLoop* InEngineLoop)
{
	if (bFirstRun)
	{ 
		if (GetMoviePlayer()->IsMovieCurrentlyPlaying())
		{ 
			GetMoviePlayer()->OnMoviePlaybackFinished().AddUObject(this, &UBFGameEngine::OnLoadingMoviePlaybackFinished);
		} 
	}

	Super::Init(InEngineLoop);

 	GetGameUserSettings()->ApplySettings(true);  
}

void UBFGameEngine::Tick(float DeltaSeconds, bool bIdleMode)
{ 

 	for (int32 WorldIdx = 0; WorldIdx < WorldList.Num(); ++WorldIdx)
	{
		FWorldContext& Context = WorldList[WorldIdx];
		if (!Context.TravelURL.IsEmpty())
		{
			UBFLocalPlayer*   LocalPlayer = Cast<UBFLocalPlayer>(GetLocalPlayerFromControllerId(GWorld, 0));
			if (LocalPlayer != NULL)
			{

			}
		}
	}

	Super::Tick(DeltaSeconds, bIdleMode); 
}
void UBFGameEngine::PreExit()
{ 
	Super::PreExit(); 
}

void UBFGameEngine::InitializeObjectReferences()
{
	Super::InitializeObjectReferences(); 
}

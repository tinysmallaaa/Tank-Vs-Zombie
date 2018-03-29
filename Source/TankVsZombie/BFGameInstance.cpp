#include "TankVsZombie.h"
#include "Gamemodes/BFGameModeBase.h"
#include "BFLevelSummery.h"
#include "BFLocalPlayer.h"
#include "BFGameUserSettings.h" 
#include "BFGameInstance.h"

DEFINE_LOG_CATEGORY(BFLOG);

UBFGameInstance::UBFGameInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{


}  

void UBFGameInstance::CleanupLevel()
{  
	FString Options = "";
	UGameplayStatics::OpenLevel( GetWorld() , TEXT("Loading") , false , Options ); 

	GetTimerManager().SetTimer(LoadingEventTimer, this, &UBFGameInstance::LoadPendingLevel, 0.1f, false); 
}

void UBFGameInstance::LoadPendingLevel()
{  
	bTransitionLevel = false;

	FString Options = "";
	UGameplayStatics::OpenLevel(GetWorld(), *LoadingMapFriendlyName , false , Options); 
}

void UBFGameInstance::LoadLevel(FString Level)
{ 
	bTransitionLevel = true;
	PendingLevel = Level; 
	LoadingMapFriendlyName = TEXT("");

	UGameplayStatics::SetGamePaused(GetWorld(), false);


	if (LoadingMapFriendlyName.IsEmpty())
	{
		int32 Pos = INDEX_NONE;
		PendingLevel.FindLastChar('/', Pos);
		LoadingMapFriendlyName = (Pos == INDEX_NONE) ? PendingLevel : PendingLevel.Right(PendingLevel.Len() - Pos - 1);
	}  

	if (LoadingWidget)
	{
		LoadingWidget->AddToViewport(100.0f);
		LoadingWidget->BeginLoadingAnimation();
		GetTimerManager().ClearAllTimersForObject(this);
		GetTimerManager().ClearTimer(LoadingEventTimer);
		GetTimerManager().SetTimer(LoadingEventTimer, this , &UBFGameInstance::LoadPendingLevel, LoadingWidget->PreLoadingAnimTime , false );
	} 
}
void UBFGameInstance::HideLoadingWidget()
{ 
	if (LoadingWidget != NULL)
	{
		LoadingWidget->RemoveFromParent(); 
	} 
} 

void UBFGameInstance::BeginLevelLoading(const FString& LevelName)
{
	if (bIgnoreLevelLoad)
	{
		bIgnoreLevelLoad = false;
		return;
	}

	if (!bTransitionLevel)
		bLevelIsLoading = true;

} 

void UBFGameInstance::EndLevelLoading(UWorld* LoadedWorld)
{
	//if(!bTransitionLevel)
	{
		bLevelIsLoading = false;
		LevelLoadText = FText::GetEmpty();
		ActiveMissionLevel = PendingLevel;
		if (LoadingWidget != NULL)
		{
			GetTimerManager().ClearAllTimersForObject(this);
			LoadingWidget->AddToViewport(100.0f);
			LoadingWidget->EndLoadingAnimation(); 
			GetTimerManager().ClearTimer(LoadingEventTimer);
			GetTimerManager().SetTimer(LoadingEventTimer, this, &UBFGameInstance::HideLoadingWidget, LoadingWidget->PreLoadingAnimTime, false);
		}
	}
}

UBFGameInstance* UBFGameInstance::Get(UObject* ContextObject)
{
	UBFGameInstance* GameInstance = NULL;
	UWorld* OwningWorld = GEngine->GetWorldFromContextObject(ContextObject,EGetWorldErrorMode::ReturnNull);

	if (OwningWorld)
	{
		GameInstance = Cast<UBFGameInstance>(OwningWorld->GetGameInstance());
	}
	return GameInstance;
} 

void UBFGameInstance::Init()
{
	Super::Init();

	InitPerfCounters();

	IPerfCounters* PerfCounters = IPerfCountersModule::Get().GetPerformanceCounters();
	if (PerfCounters)
	{
		// Attach a handler for exec commands passed in via the perf counter query port
		PerfCounters->OnPerfCounterExecCommand() = FPerfCounterExecCommandCallback::CreateUObject(this, &ThisClass::PerfExecCmd);
	}
	 
	if ( !FParse::Param( FCommandLine::Get() , TEXT("server") ) )
	{
		
		FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UBFGameInstance::BeginLevelLoading);
		FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UBFGameInstance::EndLevelLoading);

	} 

	if (UserSettingsClass != NULL)
	{
		UserSettings = NewObject<UBFGameUserSettings>(GetTransientPackage(), UserSettingsClass);
		GEngine->GameUserSettings = UserSettings;
		
		UserSettings->LoadConfig();
		UserSettings->ApplySettings(true);
	}

	ExecutePreGameCommands(); 
}

void UBFGameInstance::ExecutePreGameCommands()
{

	IConsoleManager::Get().FindConsoleVariable(TEXT("r.vsync"))->Set(false, ECVF_RenderThreadSafe);  
	 
}

void UBFGameInstance::SetMobileContentScaleFactor(float ScaleFactor)
{
	auto MobileContentScaleFactorCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileContentScaleFactor"));
	MobileContentScaleFactorCVar->Set( ScaleFactor , ECVF_SetByDeviceProfile);
}

void UBFGameInstance::Shutdown()
{
	Super::Shutdown();  
}

void UBFGameInstance::PurchaseProduct(FString ProductID)
{
	
}

bool UBFGameInstance::PerfExecCmd(const FString& ExecCmd, FOutputDevice& Ar)
{
	FWorldContext* CurrentWorldContext = GetWorldContext();
	if (CurrentWorldContext)
	{
		UWorld* World = CurrentWorldContext->World();
		if (World)
		{
			if (GEngine->Exec(World, *ExecCmd, Ar))
			{
				return true;
			}
			Ar.Log(FString::Printf(TEXT("ExecCmd %s not found"), *ExecCmd));
			return false;
		}
	}

	Ar.Log(FString::Printf(TEXT("WorldContext for ExecCmd %s not found"), *ExecCmd));
	return false;
}

#if WITH_EDITOR
FGameInstancePIEResult UBFGameInstance::StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params)
{
	Super::StartPlayInEditorGameInstance(LocalPlayer, Params);

	UBFLocalPlayer* BFLocalPlayer = Cast<UBFLocalPlayer>(LocalPlayer);
	if (BFLocalPlayer)
	{
		BFLocalPlayer->InitSaveGameSlot();
	}  

	return FGameInstancePIEResult::Success();
}
#endif 
void UBFGameInstance::StartGameInstance()
{
	if (IsRunningCommandlet())
	{
		return;
	}

	Super::StartGameInstance();

	UWorld* World = GetWorld();
	if (World)
	{
		FTimerHandle UnusedHandle;
		World->GetTimerManager().SetTimer(UnusedHandle, FTimerDelegate::CreateUObject(this, &UBFGameInstance::DeferredStartGameInstance), 0.1f, false);
	}
	else
	{
		UE_LOG(BFLOG, Warning, TEXT("Failed to schedule DeferredStartGameInstance because the world was null during StartGameInstance"));
	}  

}
 
void UBFGameInstance::DeferredStartGameInstance()
{
 	 
	UBFGameUserSettings* Settings = Cast<UBFGameUserSettings>(GEngine->GetGameUserSettings());
	UBFLocalPlayer* LocalPlayer = Cast<UBFLocalPlayer>(GetFirstGamePlayer());
	if (LocalPlayer)
	{ 
		LocalPlayer->InitSaveGameSlot();
	}

}

void UBFGameInstance::InitPerfCounters()
{
	if (!bDisablePerformanceCounters)
	{
		IPerfCountersModule& PerfCountersModule = FModuleManager::LoadModuleChecked<IPerfCountersModule>("PerfCounters");
		IPerfCounters* PerfCounters = PerfCountersModule.CreatePerformanceCounters();

		if (PerfCounters != nullptr)
		{
	 
			//UE_LOG(UT,Log,TEXT("GEngineNetVersion %i"),GEngineNetVersion);
			PerfCounters->Set(TEXT("BuildVersion"),1);
		}
		else
		{
			UE_LOG(BFLOG, Warning, TEXT("Could not initialize performance counters."));
		}
	}
}

void UBFGameInstance::PlayLevel(const FString& LevelName)
{
	//BeginLevelLoading(LevelName); 
	LoadingMapFriendlyName = TEXT("");

	if (LoadingMapFriendlyName.IsEmpty())
	{
		int32 Pos = INDEX_NONE;
		LevelName.FindLastChar('/', Pos);
		LoadingMapFriendlyName = (Pos == INDEX_NONE) ? LevelName : LevelName.Right(LevelName.Len() - Pos - 1);
	}

	FString Error; 
	UGameplayStatics::OpenLevel(  GetWorld() , FName( *LoadingMapFriendlyName  )  , false );
	 
}


EVisibility UBFGameInstance::GetLevelLoadThrobberVisibility() const
{
	return bLevelIsLoading ? EVisibility::Visible : EVisibility::Hidden;
}

EVisibility UBFGameInstance::GetLevelLoadTextVisibility() const
{
	return EVisibility::Visible;
}

EVisibility UBFGameInstance::GetEpicLogoVisibility() const
{
	return bSuppressLoadingText ? EVisibility::Collapsed : EVisibility::Visible;
}

EVisibility UBFGameInstance::GetVignetteVisibility() const
{
	return bSuppressLoadingText ? EVisibility::Collapsed : EVisibility::Visible;
}
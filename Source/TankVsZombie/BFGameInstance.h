#pragma once
#include "../../Engine/Source/Runtime/PerfCounters/Private/PerfCounters.h"
#if !UE_SERVER
#include "MoviePlayer.h"
#endif
#include "BFLoadingWidget.h"
#include "BFGameSingleton.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "BFGameUserSettings.h"
#include "BFGameInstance.Generated.h"

class UBFIAPurchase; 

UCLASS(Blueprintable,Abstract)
class TANKVSZOMBIE_API UBFGameInstance : public UPlatformGameInstance
{
	GENERATED_UCLASS_BODY()

public:
 
	UPROPERTY(  EditDefaultsOnly	, Category = "BFGameInstance" )
		TSubclassOf<UBFGameUserSettings> UserSettingsClass ;  
	UPROPERTY(	EditDefaultsOnly	, Category = "BFGameInstance" )
		bool   bAllowTransitionLoading;

	UPROPERTY( BlueprintReadWrite	, Category = "BFGameInstance" )
		FString ActiveMissionLevel ;  
 
	UPROPERTY( BlueprintReadWrite	, Category = "BFGameInstance" )
		UBFLoadingWidget* LoadingWidget ;

	UPROPERTY( BlueprintReadWrite	, Category = "BFGameInstance" )
		bool	bMovieWillAutocompleteWhenLoadFinishes;


	UPROPERTY(	BlueprintReadWrite	, Category = "BFGameInstance" )
		FString TransitionLevel;

	UPROPERTY(  BlueprintReadWrite	, Category = "BFGameInstance" )
		UBFGameUserSettings* UserSettings ;


	UPROPERTY()
		bool bDisablePerformanceCounters ;

	UPROPERTY()
		FTimerHandle LoadingEventTimer ;

	static UBFGameInstance* Get(UObject* ContextObject);

	virtual void DeferredStartGameInstance();
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual bool PerfExecCmd( const FString& ExecCmd , FOutputDevice& Ar );
	virtual void StartGameInstance() override;

#if WITH_EDITOR
	virtual FGameInstancePIEResult StartPlayInEditorGameInstance( ULocalPlayer* LocalPlayer , const FGameInstancePIEParameters& Params ) override;
#endif
	void ExecutePreGameCommands();

	UFUNCTION( BlueprintCallable , Category = "BFGameInstance" )
		void PlayLevel( const FString& LevelName );

	UFUNCTION( BlueprintCallable , Category = "BFGameInstance" )
		virtual void BeginLevelLoading( const FString& LevelName );
	UFUNCTION( BlueprintCallable , Category = "BFGameInstance" )
		virtual void EndLevelLoading( UWorld* LoadedWorld ); 

	void InitPerfCounters(); 
 
	virtual EVisibility GetLevelLoadThrobberVisibility() const; 
	virtual EVisibility GetLevelLoadTextVisibility() const;	 
	virtual EVisibility GetEpicLogoVisibility() const;
	virtual EVisibility GetVignetteVisibility() const;

	void PurchaseProduct( FString ProductID );

	UFUNCTION( BlueprintCallable , Category = "BFGameInstance" )
		void SetMobileContentScaleFactor(float ScaleFactor);

	UFUNCTION( BlueprintCallable , Category = "BFGameInstance" )
	void LoadLevel(FString Level);

	UFUNCTION( BlueprintCallable , Category = "BFGameInstance" )
		void CleanupLevel();

	UFUNCTION( BlueprintCallable , Category = "BFGameInstance" )
	void LoadPendingLevel();

	UFUNCTION( BlueprintCallable , Category = "BFGameInstance" )
		void HideLoadingWidget();

private:

	FString PendingLevel	       ;
	bool	bTransitionLevel       ;
	bool	bLevelIsLoading        ;
	FString LoadingMovieToPlay     ;
	FText	LevelLoadText	       ;
	bool	bIgnoreLevelLoad       ;  
	bool	bSuppressLoadingText   ;
	bool	bShowCommunityBadge    ;
	FString LoadingMapFriendlyName ; 

};
#pragma once  
#include "Online/BFGameAchievementManager.h"
#include "BFGameSaveData.Generated.h"
	
class ABFWeapon;

USTRUCT(BlueprintType)
struct FGuideContainerSavedState
{
	GENERATED_USTRUCT_BODY()

		FGuideContainerSavedState()
		:	 GuideClass(NULL)	,
			 bCompleted(false)
	{

	}
	UPROPERTY( BlueprintReadWrite , Category = "GuideContainerSavedState" )
		TSubclassOf<class UBFUserWidget> GuideClass;
	UPROPERTY( BlueprintReadWrite , Category = "GuideContainerSavedState")
		bool bCompleted; 
};
 
USTRUCT(BlueprintType)
struct FSavedMissionState
{
	GENERATED_USTRUCT_BODY()
		FSavedMissionState()
		:MissionID(0)		,
		StarsEarned(0)		,
		CreditsEarned(0)	,
		bUnlocked(false)
	{

	}    
 
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = "Mission" )
		int32		MissionID		;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite, Category = "Mission" )
		int32	    StarsEarned		;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = "Mission" )
		int32		CreditsEarned	;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = "Mission" )
		bool		bUnlocked		;
};

USTRUCT(BlueprintType)
struct FSavedChapterState
{
	GENERATED_USTRUCT_BODY()

		FSavedChapterState()
		:ChapterID(0)	,	
		StarsEarned(0)	,
		bUnlocked(false)
	{
		Missions.Reset();
	}

	UPROPERTY(BlueprintReadWrite, Category = "ChapterState")
		int32		ChapterID	;

	UPROPERTY(BlueprintReadWrite, Category = "ChapterState")
		int32		StarsEarned	;
		
	UPROPERTY(BlueprintReadWrite, Category = "ChapterState")
		bool        bUnlocked;
	UPROPERTY(BlueprintReadWrite, Category = "ChapterState")
		TArray<FSavedMissionState> Missions ;
};

UENUM(BlueprintType)
enum EAttachedWeaponUpgrade
{
	EATTACHEDWEAPON_UPGRADE_AMMOCAP			,	
	EATTACHEDWEAPON_UPGRADE_POWER			,
	EATTACHEDWEAPON_UPGRADE_ACCURACY	,
	EATTACHEDWEAPON_UPGRADE_FIRERATE		,
	EATTACHEDWEAPON_UPGRADE_RELOAD			,
	EATTACHEDWEAPON_UPGRADE_MAX				,
};

USTRUCT(BlueprintType)
struct FSavedVehicleWeaponState
{
	GENERATED_USTRUCT_BODY()

	FSavedVehicleWeaponState()
		:Name("")			,	ActiveWeaponClass(NULL)	,
		AmmoClipSize(0)		,	AmmoClipSizeLevel(0)	, 
		Power(0.0f)			,	PowerLevel(0)			,
		Accuracy(0.0f)		,	AccurayLevel(0)			, 
		FireRate(0.0f)		,	FireRateLevel(0)		,
		ReloadTime(0.0f)	,	ReloadTimeLevel(0)
	{

	} 

	bool IsValid()
	{
		return Name==NAME_None;
	}

	UPROPERTY(BlueprintReadWrite , Category = "VehicleWeaponState" )
		FName	Name ;
	UPROPERTY( BlueprintReadWrite , Category = "VehicleWeaponState" )
		TSubclassOf<class ABFWeapon> ActiveWeaponClass ;
	
	UPROPERTY(BlueprintReadWrite, Category = "VehicleWeaponState")
		bool		bUnlocked		;
	UPROPERTY(BlueprintReadWrite, Category = "VehicleWeaponState")
		bool		bPurchased		;

	UPROPERTY( BlueprintReadWrite , Category = "VehicleWeaponState" )
		int32		AmmoClipSize    ;
	UPROPERTY( BlueprintReadWrite , Category = "VehicleWeaponState" )
		int32		AmmoClipSizeLevel ;

	UPROPERTY( BlueprintReadWrite , Category = "VehicleWeaponState" )
		float		Power			;
	UPROPERTY( BlueprintReadWrite , Category = "VehicleWeaponState" )
		int32		PowerLevel		;
	
	UPROPERTY( BlueprintReadWrite , Category = "VehicleWeaponState" )
		float		Accuracy		;
	UPROPERTY( BlueprintReadWrite , Category = "VehicleWeaponState" )
		int32		AccurayLevel	;
	
	UPROPERTY( BlueprintReadWrite , Category = "VehicleWeaponState" )
		int32		FireRate		;   
	UPROPERTY( BlueprintReadWrite , Category = "vehicleWeaponState" )
		int32		FireRateLevel	; 

	UPROPERTY(BlueprintReadWrite, Category = "VehicleWeaponState")
		float		ReloadTime		;
	UPROPERTY(BlueprintReadWrite, Category = "VehicleWeaponState")
		int32		ReloadTimeLevel	;

};

USTRUCT(BlueprintType)
struct FSavedMiniTankBotState
{
	GENERATED_USTRUCT_BODY()

	FSavedMiniTankBotState()	
		:MiniTankBotClass(NULL)	,
		bUnlocked(false)		,
		bPurchased(false)
	{

	}

	UPROPERTY( BlueprintReadWrite , Category = "SpiderBombState" )
		TSubclassOf<class ABFMiniTankBot>	MiniTankBotClass ;
	UPROPERTY( BlueprintReadWrite , Category = "SpiderBombState" )
		bool bUnlocked ;
	UPROPERTY( BlueprintReadWrite , Category = "SpiderBombState" )
		bool bPurchased ;

};

USTRUCT(BlueprintType)
struct FSavedSpiderMineState
{
	GENERATED_USTRUCT_BODY()

	FSavedSpiderMineState() :
		SpiderMineClass( NULL )	,	bUnlocked( false ),
		bPurchased( false )		,   TravelSpeed( 0.0f ),
		TravelSpeedLevel( 0 )	,	DamagePower( 0.0f ),
		DamagePowerLevel( 0 )
	{

	}
	
	UPROPERTY( BlueprintReadWrite	 , Category = "SpiderBombState" )
		TSubclassOf<class ABFSpiderMine>	SpiderMineClass;
	UPROPERTY(BlueprintReadWrite	, Category = "SpiderBombState" )
		bool bUnlocked;
	UPROPERTY(BlueprintReadWrite	, Category = "SpiderBombState")
		bool bPurchased;

	UPROPERTY(BlueprintReadWrite, Category = "SpiderBombState")
		float TravelSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "SpiderBombState")
		float TravelSpeedLevel;

	UPROPERTY(BlueprintReadWrite, Category = "SpiderBombState")
		float DamagePower;
	UPROPERTY(BlueprintReadWrite, Category = "SpiderBombState")
		float DamagePowerLevel; 

};   

UENUM(BlueprintType)
enum EVehicleUpgrade
{
	EVEHICLE_UPGRADE_DURABILITY		,
	EVEHICLE_UPGRADE_ALERTER		,  
	EVEHICLE_UPGRADE_SHELL_RELOAD	,
	EVEHICLE_UPGRADE_SHELL_DAMAGE	,
	EVEHICLE_UPGRADE_SHELL_CLIP_SIZE,
	EVEHICLE_UPGRADE_NITRO_CAP		,
	EVEHICLE_UPGRADE_MAX			,	
};

USTRUCT(BlueprintType)
struct FSavedPlayerVehicleState
{
	GENERATED_USTRUCT_BODY()

	FSavedPlayerVehicleState()
		:GarageVehicleClass(NULL) ,	VehicleClass(NULL),
		LightWeaponName("")		 ,	Name(""),
		Durability(0.0f)		 ,	DurabilityLevel(0),
		Alerter(0.0f)			 ,	AlerterLevel(0),
		ShellReloadTime(0.0f)	 ,	ShellReloadLevel(0),
		ShellDamageAmount(0.0f)	 ,	ShellDamageLevel(0),
		NitroCap(0.0f)			 ,	NitroCapLevel(0),
		ShellClipSize(0)		 ,	ShellClipSizeLevel(0)
	{

	}
		 
	bool IsValid()
	{
		return (Name==NAME_None || Name==NAME_None);
	}

	UPROPERTY(BlueprintReadWrite, Category = "PlayerVehicleState")
		TSubclassOf<class ABFTankVehicle>	GarageVehicleClass;
	UPROPERTY( BlueprintReadWrite , Category = "PlayerVehicleState" )
		TSubclassOf<class ABFTankVehicle>	VehicleClass ;
 
	UPROPERTY(BlueprintReadWrite, Category = "VehicleWeaponState")
		bool		bUnlocked;
	UPROPERTY(BlueprintReadWrite, Category = "VehicleWeaponState")
		bool		bPurchased;

	UPROPERTY( BlueprintReadWrite , Category = "PlayerVehicleState" )
		FName	    LightWeaponName ;
	UPROPERTY( BlueprintReadWrite , Category = "PlayerVehicleState" )
		FName		Name			;	
	
	UPROPERTY( BlueprintReadWrite , Category = "PlayerVehicleState" )
		float		Durability;
	UPROPERTY( BlueprintReadWrite , Category = "PlayerVehicleState" )
		int32		DurabilityLevel ; 
	
	UPROPERTY( BlueprintReadWrite , Category = "PlayerVehicleState" )
		float		Alerter		    ;
	UPROPERTY( BlueprintReadWrite , Category = "PlayerVehicleState" )
		int32		AlerterLevel    ; 
	 
	UPROPERTY( BlueprintReadWrite , Category = "VehicleWeaponState" )
		float		ShellReloadTime  ;
	UPROPERTY( BlueprintReadWrite , Category = "VehicleWeaponState" )
		int32       ShellReloadLevel ;

	UPROPERTY( BlueprintReadWrite , Category = "PlayerVehicleState" )
		float		ShellDamageAmount ;	
	UPROPERTY( BlueprintReadWrite , Category = "PlayerVehicleState" )
		int32		ShellDamageLevel  ;

	UPROPERTY( BlueprintReadWrite , Category = "PlayerVehicleState" )
		float		NitroCap		  ;
	UPROPERTY( BlueprintReadWrite , Category = "PlayerVehicleState" )
		int32		NitroCapLevel	  ;

	UPROPERTY( BlueprintReadWrite , Category = "PlayerVehicleState" )
		int32		ShellClipSize	   ;
	UPROPERTY( BlueprintReadWrite , Category = "PlayerVehicleState" )
		int32		ShellClipSizeLevel ;
};

USTRUCT(BlueprintType)
struct FSavedPlayerState
{

	GENERATED_USTRUCT_BODY()

		FSavedPlayerState()
		:	CreditsEarned(0) ,	CurrentChapterID(0) ,
			GrenadesCount(0) ,	MinesCount(0) ,	LivesCount(0),
			TotalZombiesKilled(0) ,	TotalHeadShots(0) ,	TotalZombiesKilledByEnv(0)
	{

	}
	UPROPERTY( BlueprintReadWrite , Category = "PlayerState" )
		FSavedPlayerVehicleState	VehicleState ;
	UPROPERTY( BlueprintReadWrite , Category = "PlayerState" )
		int32	CreditsEarned		;
	UPROPERTY( BlueprintReadWrite , Category = "PlayerState")
		int32   CurrentChapterID	;	

	UPROPERTY( BlueprintReadWrite , Category = "PlayerState")
		int32	GrenadesCount	;	
	UPROPERTY( BlueprintReadWrite , Category = "PlayerState")
		int32	MinesCount		;		
	UPROPERTY( BlueprintReadWrite , Category = "PlayerState" )
		int32   LivesCount		;

	UPROPERTY( BlueprintReadWrite , Category = "PlayerState" )
		int32	TotalZombiesKilled		;		
	UPROPERTY( BlueprintReadWrite , Category = "PlayerState" )
		int32	TotalHeadShots			;
	UPROPERTY( BlueprintReadWrite , Category = "PlayerState" )
		int32   TotalZombiesKilledByEnv ;

};
 

USTRUCT(BlueprintType)
struct FBFAchievementLevelSavedState
{
	GENERATED_USTRUCT_BODY()
		FBFAchievementLevelSavedState()
		:Type(0) , ProgressAmount(0) , CurrentLevel(0) 
	{

	}
	UPROPERTY( BlueprintReadWrite, Category = "AchievementLevelState")
		int32 Type;
	UPROPERTY( BlueprintReadWrite , Category = "AchievementLevelState" )
		int32 ProgressAmount;
	UPROPERTY( BlueprintReadWrite , Category = "AchievementLevelState" )
		int32 CurrentLevel; 
};

 
USTRUCT(BlueprintType)
struct FBFGameStatsForDay
{
	GENERATED_USTRUCT_BODY()

		FBFGameStatsForDay()
		:	TotalKilled(0)	 ,	 PlayerDeathCount(0)		
	{

	}
	UPROPERTY(BlueprintReadWrite, Category = "GameStatsForDay")
		int32 TotalKilled;
	UPROPERTY(BlueprintReadWrite, Category = "GameStatsForDay")
		int32 PlayerDeathCount;  

};

class UBFLocalPlayer; 
UCLASS(Blueprintable)
class TANKVSZOMBIE_API UBFGameSaveData : public USaveGame
{
	GENERATED_UCLASS_BODY() 

public:    

	UPROPERTY( BlueprintReadWrite ,	Category = "BFGameSaveData"	)
		TArray<FSavedChapterState>		  ChapterStates ;		
	UPROPERTY( BlueprintReadWrite , Category = "BFGameSaveData" )
		TArray<FSavedVehicleWeaponState>   VehicleWeaponStates ;	
	UPROPERTY( BlueprintReadWrite , Category = "BFGameSaveData" )
		TArray<FSavedPlayerVehicleState> VehicleStates  ;

	UPROPERTY(BlueprintReadOnly, Category = "BFGameSaveData")
		FDateTime YesterdayDate	;
	UPROPERTY(BlueprintReadOnly, Category = "BFGameSaveData")
		FDateTime TodayDate		;
	UPROPERTY(BlueprintReadOnly, Category = "BFGameSaveData")
		FDateTime FirstTimeLaunchDate	; 

	UPROPERTY( BlueprintReadWrite , Category = "BFGameSaveData" )
		FSavedPlayerState	 PlayerState ;
	UPROPERTY( BlueprintReadWrite , Category = "BFGameSaveData" )
		bool	bAdFreeInAppPurchaseMade ; 			
	UPROPERTY( BlueprintReadWrite , Category = "BFGameSaveData" )
		float	LastTimeInSecondsRewardVideoPlayed ;

	UPROPERTY(BlueprintReadWrite, Category = "BFGameSaveData")
		TArray<FBFAchievementLevelSavedState> SavedAchievementStates ;

	UFUNCTION(BlueprintCallable, Category = "BFGameSaveData")
		FSavedPlayerVehicleState GetPlayerVehicleState();

	UFUNCTION(BlueprintCallable, Category = "BFGameSaveData")
		FSavedPlayerVehicleState GetVehicleStateByName(FName Name) ;

	UFUNCTION(BlueprintCallable, Category = "BFGameSaveData")
		FSavedVehicleWeaponState GetWeaponStateByName(FName Name);
	
	UFUNCTION(	BlueprintCallable , Category = "BFGameSaveData" )
		void ChangeVehicleState( FSavedVehicleWeaponState WeaponState ) ;	

	float GetHealthMax()
	{ 
		return PlayerState.VehicleState.Durability * PlayerState.VehicleState.DurabilityLevel	;
	}

	void AddCredits(int32 Amount) {

		PlayerState.CreditsEarned += Amount ; 
		if ( PlayerState.CreditsEarned < 0 ) {
			PlayerState.CreditsEarned = 0	;
		} 

	}   

	UFUNCTION( BlueprintImplementableEvent , Category = "BFGameSaveData" )
		int32 GetMaxDurabilityLevel( UClass* VehicleClass )		;
	UFUNCTION( BlueprintImplementableEvent , Category = "BFGameSaveData" )
		int32 GetMaxAlerterLevel( UClass* VehicleClass )		;	

	UFUNCTION( BlueprintImplementableEvent , Category = "BFGameSaveData" )
		int32 GetMaxShellReloadLevel( UClass* VehicleClass )	;
	UFUNCTION( BlueprintImplementableEvent , Category = "BFGameSaveData" )
		int32 GetMaxShellDamageLevel( UClass* VehicleClass )	;
		UFUNCTION( BlueprintImplementableEvent , Category = "BFGameSaveData" )
		float GetMaxDurability( UClass* VehicleClass )			;

	UFUNCTION( BlueprintImplementableEvent , Category = "BFGameSaveData" )
		float GetMaxAlerter( UClass* VehicleClass )				;
	UFUNCTION( BlueprintImplementableEvent , Category = "BFGameSaveData" )
		float GetMaxShellReloadTime( UClass* VehicleClass )		;	
	UFUNCTION( BlueprintImplementableEvent , Category = "BFGameSaveData" )
		float GetMaxShellDamageAmount( UClass* VehicleClass )	;

 
	UFUNCTION(BlueprintImplementableEvent, Category = "BFGameSaveData")
		TArray<FSavedPlayerVehicleState>			ResetVehicleStates();
	UFUNCTION( BlueprintCallable , BlueprintImplementableEvent	, Category = "BFGameSaveData"	)
		FSavedPlayerState			ResetPlayerState()			;	
	UFUNCTION( BlueprintCallable , BlueprintImplementableEvent	, Category = "BFGameSaveData"	)
		TArray<FSavedVehicleWeaponState> ResetWeaponStates()	;	

	UFUNCTION( BlueprintCallable , BlueprintImplementableEvent	, Category = "BFGameSaveData"	)
		TArray<FSavedChapterState>		ResetChapterStates()	;

	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
		void UpgradeVehicle( int32 UpgradeLevel , float UpgradeAmount , TEnumAsByte<EVehicleUpgrade> VehicleUpgrade );	

	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
		void UpgradeWeapon(FName WeaponName , int32 UpgradeLevel , float UpgradeAmount , TEnumAsByte<EAttachedWeaponUpgrade> WeaponUpgrade );
	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData")
		void SelectVehicleWeapon(int32 WeaponIndex)	 ;
	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData")
		void SelectPrimaryVehicle(int32 VehicleIndex) ;

	virtual void PostInitProperties() override	;
	UFUNCTION( BlueprintCallable , BlueprintNativeEvent , Category = "BFGameSaveData" )
		void ResetProfile()		;	
	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
		int32 GetVehicleIndex(FName Name) ;

	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
		int32 GetWeaponIndex(FName Name) ;
	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData")
		int32 GetPrimaryWeaponIndex() ;		
	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData")
		int32 GetPrimaryVehicleIndex() ;		

	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData")
		int32 GetCurrentChapterIndex() ;
	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData")
		FSavedChapterState GetCurrentChapterState()	 ;
	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData")
		FSavedMissionState UnlockNextMission( int32 ChapterID , int32 MissionID )	;	

	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData")
		FSavedMissionState UnlockNextChapter( int32 ChapterID )		;
	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
		FSavedMissionState GetMissionForID( int32 ChapterID , int32 MissionID )		;
	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
		void SaveMissionState( int32 ChapterID , FSavedMissionState NewState )		;

	void UpdateFromPlayer( UBFLocalPlayer* ProfilePlayer )		; 
	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
		int32 GetCurrentVehicleUpgradeLevel ( EVehicleUpgrade Upgrade )	;	
	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
		float GetCurrentVehicleUpgradeAmount( EVehicleUpgrade Upgrade )	;	

	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
		int32 GetCurrentWeaponUpgradeLevel ( EAttachedWeaponUpgrade Upgrade )	;
	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
		float GetCurrentWeaponUpgradeAmount( EAttachedWeaponUpgrade Upgrade )	;
	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
		FName GetPrimaryWeaponName()		;

	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
		FName GetPrimaryVehicleName()		;
		void UnlockWeapon( TSubclassOf<ABFWeapon> WeaponClass )					;	 
		void UnlockVehicle( TSubclassOf<ABFTankVehicle> VehicleClass )			;

		void MarkVehiclePurchased( TSubclassOf<ABFTankVehicle> VehicleClass )	;
		void MarkWeaponPurchased( TSubclassOf<ABFWeapon> WeaponClass )			;
	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
		int32 GetVehicleStateIndex(FSavedPlayerVehicleState State)				;		

	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
		int32 GetVehicleWeaponStateIndex(FSavedVehicleWeaponState State)		;
 
		FBFAchievementLevelSavedState GetAchievementSavedState(int32 AchievementIndex);
		void SaveAchievementState(FBFAchievementLevelSavedState State);

	UFUNCTION( BlueprintCallable , Category = "BFGameSaveData" )
			void SetupDefault(UBFConfigDBUtil* Config);

	UFUNCTION(BlueprintImplementableEvent, Category = "BFGameSaveData")
		FSavedPlayerVehicleState GetDefaultVehicleState();
	UFUNCTION(BlueprintImplementableEvent, Category = "BFGameSaveData")
		FSavedPlayerVehicleState GetWeaponUpgrade();
  
	UPROPERTY(BlueprintReadWrite,category = "BFGameSaveData")
		UBFConfigDBUtil* RefConfigDB;

private:


};


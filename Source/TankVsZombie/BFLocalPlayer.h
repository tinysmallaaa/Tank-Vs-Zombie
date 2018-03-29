#pragma once
#include "BFGameSaveData.h"
#include "GameModes/Horde/BFHordeMissionInfo.h"
#include "GameModes/Mayhem/BFMayhemMissionInfo.h"
#include "GameModes/RoadRage/BFRoadRageInfo.h"
#include "Online/BFGameAchievementManager.h"

#include "GameModes/BFActiveMission.h"

#include "BFLocalPlayer.Generated.h"

USTRUCT(BlueprintType)
struct FAchievementInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY( BlueprintReadWrite ,	Category = "AchievementInfo" )
		FString		Name ; 
};

UENUM(BlueprintType)
enum EGameModeType
{
	GAMEMODE_STORY		,
	GAMEMODE_MAYAHEM	,
	GAMEMODE_ROADRAGE	,
};

class ABFPlayerState;

UCLASS(Blueprintable, config = Game)
class TANKVSZOMBIE_API UBFLocalPlayer : public ULocalPlayer
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(BlueprintReadWrite, Category = "BFPlayerState")
		TEnumAsByte<EGameModeType>		ActiveGameMode ;
	UPROPERTY(BlueprintReadOnly, Category = "BFLocalPlayer")
		TSubclassOf<UBFConfigDBUtil>	ConfigDBClass;
	UPROPERTY(BlueprintReadWrite, Category = "BFLocalPlayer")
		UBFConfigDBUtil*				ConfigDB ;

	UPROPERTY(BlueprintReadWrite, Category = "BFLocalPlayer")
		TSubclassOf<UBFGameSaveData>			SaveDataClass;
	UPROPERTY(BlueprintReadWrite, Category = "BFLocalPlayer")
		UBFGameSaveData*						SaveData;
	UPROPERTY(EditDefaultsOnly, Category = "BFLocalPlayer")
		TSubclassOf<UBFGameAchievementManager>	AchievementClass ; 
	
	UPROPERTY(BlueprintReadOnly , Category = "BFLocalPlayer")
		ABFActiveMission*				ActiveMission;	
	UPROPERTY(BlueprintReadWrite, Category = "BFLocalPlayer")
		FSavedMissionState				CurrentMissionState	;
	UPROPERTY(BlueprintReadWrite, Category = "BFLocalPlayer")
		int32							CurrentChapterID	;

	UPROPERTY()
		bool		bHideMenuCalledDuringMoviePlayback		;
	UPROPERTY()
		bool		bCloseUICalledDuringMoviePlayback		;
	UPROPERTY()
		bool		bDelayedCloseUIExcludesDialogs			;

	UPROPERTY( BlueprintReadWrite , Category = "ActiveMission" )
		TArray<TSubclassOf<UBFUnlockableInventory>> PendingUnlockableInventories;

	virtual			~UBFLocalPlayer();


	virtual void	 PostInitProperties() override;
	void			 InitSaveGameSlot();
	FString			 GetSaveSlotName() const;
	

	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		void		 UpgradeVehicle( int32 UpgradeLevel , float UpgradeAmount , TEnumAsByte<EVehicleUpgrade> VehicleUpgrade );	
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		void		 UpgradeWeapon( FName WeaponName , int32 UpgradeLevel , float UpgradeAmount , TEnumAsByte<EAttachedWeaponUpgrade> WeaponUpgrade );	
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		FSavedMissionState		 GetMissionForChapter( int32 ChapterID , int32 MissionID );	


	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		FSavedVehicleWeaponState GetVehicleWeaponState( UClass* WeaponClass );	
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		FSavedPlayerVehicleState GetVehicleState( UClass* VehicleClass );	
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		FSavedPlayerVehicleState GetVehicleStateByIndex( int32 Index );


	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		FSavedVehicleWeaponState GetVehicleWeaponStateByIndex( int32 Index );
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		FSavedPlayerVehicleState GetPrimaryVehicleState();
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		FSavedVehicleWeaponState GetPrimaryWeaponState();


	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		FSavedChapterState GetChapterState(int32 ChapterIndex);
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		void SelectChapter(int32 ChapterIndex);	
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		void AdvanceMissionAndSave();


	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		void SaveMissionState( int32 ChapterID , FSavedMissionState NewState );	
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		UBFGameSaveData* GetGameSaveData();
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		bool ShouldDisplayAd();

	
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		void			 SaveGameData();
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		void			 ReadGameSaveData();
	UFUNCTION(	BlueprintImplementableEvent	, Category = "BFLocalPlayer"	)
		void			 SetToDefault();

	
	virtual bool	 IsMenuGame();
	void			 HideMenu();
	virtual void	 CloseAllUI( bool bExceptDialogs );


	UFUNCTION(	BlueprintCallable	, BlueprintImplementableEvent	, Category = "BFGameSaveData"	)
		FText GetDisplayTextForObjective( FMissionObjective Objective );
	UFUNCTION(	BlueprintCallable	, Category = "BFGameSaveData"	)
		void SetupMission();
	UBFGameSaveData* CreateProfileSettingsObject( const TArray<uint8>& Buffer );


	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		void AddCredits( int32 Count );
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		int32 TakeCredits( int32 Count );
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		int32 GetAvailableCredits();

	
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		bool HasEnoughCredits( int32 Count );
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		int32 GetGrenadesCount();
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		int32 GetMinesCount();


	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		int32 GetLivesCount();
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		int32 AddGrenades( int32 Count );
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		int32 AddMines( int32 Count );

	
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		int32 AddLives(int32 Count);
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		void SelectVehicleWeapon( int32 WeaponIndex );
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		void SelectPrimaryVehicle( int32 VehicleIndex );


	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		int32 GetVehicleStateIndex( FSavedPlayerVehicleState State );
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		int32 GetVehicleWeaponStateIndex( FSavedVehicleWeaponState State );
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		FSavedPlayerVehicleState MarkVehiclePurchased(TSubclassOf<ABFTankVehicle> VehicleClass);


	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		FSavedVehicleWeaponState MarkWeaponPurchased(TSubclassOf<ABFWeapon> WeaponClass);
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		FSavedVehicleWeaponState MarkVehicleWeaponPurchasedByName(FName Name);
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		FSavedPlayerVehicleState MarkVehiclePurchasedByName(FName Name);


	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		int32 GetActiveMissionIndex();

	
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		void UnlockWeapon( TSubclassOf<ABFWeapon> WeaponClass );
	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		void UnlockVehicle( TSubclassOf<ABFTankVehicle> VehicleClass );


	UFUNCTION(	BlueprintCallable	, Category = "BFLocalPlayer"	)
		void DisableAd();
	UFUNCTION(	BlueprintImplementableEvent , Category = "BFLocalPlayer" )
		bool HasActiveMissionSetup();	
		void UnlockAchievement( ABFPlayerState* PlayerState , ABFZombie* LastZombieKilled );


		FBFAchievementLevelSavedState GetAchievementSavedState(EAchievementType Type);
		void SaveAchievementState(FBFAchievementLevelSavedState State);
	UFUNCTION(	BlueprintCallable			, Category = "BFLocalPlayer"	)
		FBFAchievementUIData GetAchievementUIData(EAchievementType Type, UBFLocalPlayer* Player);


protected:
	UBFGameAchievementManager* AchievementManager ;
	FString DefaultSaveGameSlot ;
};
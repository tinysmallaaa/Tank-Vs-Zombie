#pragma once
#include "Vehicle/BFPlayerTank.h"
#include "Garage/BFGarageTank.h"
#include "GameModes/Horde/BFHordeMissionInfo.h"
#include "GameModes/RoadRage/BFRoadRageInfo.h"
#include "GameModes/Mayhem/BFMayhemMissionInfo.h"
#include "AI/BFAIInfo.h"
#include "BFConfigDBUtil.Generated.h"
 
USTRUCT(BlueprintType)
struct FUpgradeItemDBUnlockCondition : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
		FUpgradeItemDBUnlockCondition()
		:UnlockAtMission(-1)
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UpgradeItemDBUnlockCondition")
		int32	UnlockAtMission;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UpgradeItemDBUnlockCondition")
		FText	PromptDisplayText;
};

USTRUCT(BlueprintType)
struct FUpgradeItemDBInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	FUpgradeItemDBInfo()
		:UpgradeAmount(0.0f),UpgradeCost(0.0f) 
	{

	}

	UPROPERTY(	EditAnywhere , BlueprintReadOnly , Category = "UpgradeItemDBInfo")
		float	UpgradeAmount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UpgradeItemDBInfo")
		FText	UpgradeAmountDisplayText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UpgradeItemDBInfo")
		float	UpgradeCost;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UpgradeItemDBInfo")
		FText	UpgradeCostDisplayText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UpgradeItemDBInfo")
		FDataTableRowHandle UnlockCondition;
};


USTRUCT(BlueprintType)
struct FWeaponUpgradeDBList : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponUpgradeDBList")
		FUpgradeItemDBInfo AmmoCap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponUpgradeDBList")
		FUpgradeItemDBInfo Power;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponUpgradeDBList")
		FUpgradeItemDBInfo Accuracy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponUpgradeDBList")
		FUpgradeItemDBInfo FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponUpgradeDBList")
		FUpgradeItemDBInfo Reload;
};

USTRUCT(BlueprintType)
struct FWeaponDBInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponDBInfo" )
		FName			Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponDBInfo" )
		FText			DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponDBInfo" )
		TSubclassOf<ABFWeapon>	WeaponClass; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponDBInfo" )
		UDataTable*		Upgrades; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponDBInfo" )
		int32			PurchaseCost; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponDBInfo" )
		FText			PurchaseCostDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponDBInfo" )
		FText			UnlockDisplayText	;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponDBInfo" )
		bool			bUnlocked			;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponDBInfo" )
		UTexture2D*		Icon				;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponDBInfo" )
		FText			PurchaseConfirmText	;

	TArray<FWeaponUpgradeDBList*> GetUpgradesCount()
	{
		checkSlow( Upgrades == NULL && Upgrades->GetRowStructName() == FWeaponUpgradeDBList::GetName() );
		TArray<FWeaponUpgradeDBList*> InfoList ;
		Upgrades->GetAllRows<FWeaponUpgradeDBList>( FString("FWeaponUpgradeDBList") , InfoList );

		return InfoList ; 
	}  

};


USTRUCT(BlueprintType)
struct FVehicleUpgradeDBList : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FVehicleUpgradeDBList()
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleUpgradeDBList")
		FUpgradeItemDBInfo Durability;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleUpgradeDBList")
		FUpgradeItemDBInfo Alerter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleUpgradeDBList")
		FUpgradeItemDBInfo ShellReload;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleUpgradeDBList")
		FUpgradeItemDBInfo ShellDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleUpgradeDBList")
		FUpgradeItemDBInfo ShellClipSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleUpgradeDBList")
		FUpgradeItemDBInfo NitroCap;

};


USTRUCT(BlueprintType)
struct FVehicleDBInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	FVehicleDBInfo()
	{

	}
	
	bool IsValid()
	{
		return Name == NAME_None;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleDBInfo" )
		FName	Name ;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleDBInfo" )
		FText   DisplayName ;  

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleDBInfo" )
		TSubclassOf <ABFPlayerTank> VehicleClass ;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleDBInfo" )
		TSubclassOf <ABFGarageTank>  GarageVehicleClass ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleDBInfo")
		FName		LightWeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleDBInfo" )
		UDataTable* Upgrades ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleDBInfo" )
		int32 CostAmount ;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleDBInfo" )
		int32 CostAmountDisplayText ;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleDBInfo" )
		bool bUnlocked;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleDBInfo" )
		FText UnlockText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleDBInfo" )
		UTexture2D* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VehicleDBInfo" )
		FText PurchaseConfirmText; 

	TArray<FVehicleUpgradeDBList*> GetUpgradesCount()
	{
		checkSlow( Upgrades == NULL && Upgrades->GetRowStructName() == FVehicleUpgradeDBList::GetName() );
		TArray<FVehicleUpgradeDBList*> InfoList ;
		Upgrades->GetAllRows<FVehicleUpgradeDBList>( FString("FVehicleUpgradeDBList") , InfoList );

		return InfoList ;
	} 

};
 

USTRUCT(BlueprintType)
struct FMissionDBInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	FMissionDBInfo()
		:MissionID(-1)
	{

	}
	
	bool IsValid()
	{
		return MissionID == -1;
	}

	UPROPERTY(	EditAnywhere	, BlueprintReadOnly	, Category = "MissionDBInfo" )
		int32				MissionID			;
	UPROPERTY(	EditAnywhere	, BlueprintReadOnly	, Category = "MissionDBInfo" )
		FString				LevelPath			;

	UPROPERTY(	EditAnywhere	, BlueprintReadOnly	, Category = "MissionDBInfo" )
		FString				Description			;
	UPROPERTY(	EditAnywhere	, BlueprintReadOnly	, Category = "MissionDBInfo" )
		UTexture2D*			Screenshot			;

	UPROPERTY(	EditAnywhere	, BlueprintReadOnly	, Category = "MissionDBInfo" )
		FText				DisplayName			;	  
	UPROPERTY(	EditAnywhere	, BlueprintReadOnly	, Category = "MissionDBInfo" )
		float				Difficulty			;

	UPROPERTY(	EditAnywhere	, BlueprintReadOnly	, Category = "MissionDBInfo" )
		FDataTableRowHandle MissionDataHandle;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HordeMissionInfo")
		FDataTableRowHandle ObjectivesDataHandle	;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HordeMissionInfo")
		FBotAIParams		AIParams; 

};

USTRUCT(BlueprintType)
struct FUnlockedItemInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UnlockedItemInfo")
		int32		ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UnlockedItemInfo")
		UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UnlockedItemInfo")
		FText		DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UnlockedItemInfo")
		TSubclassOf<AActor> UnlockItemClass;	

};

USTRUCT(BlueprintType)
struct FChapterDBInfo : public FTableRowBase 
{
	GENERATED_USTRUCT_BODY()

	FChapterDBInfo()
		:ChapterID(-1)
	{

	}
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChpaterDBInfo")
		int32	ChapterID; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChapterDBInfo" )
		FText	DisplayName		;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChapterDBInfo" )
		UDataTable* MissionsTable	;  
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChapterDBInfo")
		TArray<FUnlockedItemInfo> ItemsToUnlock;

	bool IsValid()
	{
		return ChapterID == -1;
	} 

	int32 GetMissionsCount() const 
	{
		return MissionsTable->GetRowNames().Num();
	}

	TArray<FMissionDBInfo*> GetMissions()
	{
		TArray<FMissionDBInfo*> Missions;

		MissionsTable->GetAllRows<FMissionDBInfo>( FString("FChapterDBInfo") , Missions );

		return Missions;
	}   
	 
};

UCLASS(Blueprintable,Abstract)
class UBFConfigDBUtil : public UObject
{
	GENERATED_UCLASS_BODY()

public:  

	UPROPERTY(	EditDefaultsOnly	, Category = "BFConfigDBUtil")
		UDataTable* WeaponInfoTable  ;
	UPROPERTY(	EditDefaultsOnly	, Category = "BFConfigDBUtil" )
		UDataTable* ChaptersTable	 ;	 
	UPROPERTY(	EditDefaultsOnly	, Category = "BFConfigDBUtil" )
		UDataTable* VehicleInfoTable ;

	UPROPERTY(EditDefaultsOnly, Category = "BFConfigDBUtil")
		FString		DefaultWeaponName	;
	UPROPERTY(EditDefaultsOnly, Category = "BFConfigDBUtil")
		FString		DefaultVehicleName	;

	UFUNCTION(	BlueprintCallable	, Category = "BFConfigDBUtil" )
		TArray<FWeaponDBInfo>		 GetWeaponInfoList()	;
	UFUNCTION(	BlueprintCallable	, Category = "BFConfigDBUtil" )
		TArray<FWeaponUpgradeDBList> GetWeaponUpgradeList( const FWeaponDBInfo& Info ) ;

	UFUNCTION(BlueprintCallable, Category = "BFConfigDBUtil")
		FWeaponDBInfo		 GetWeaponInfo(FName Name);
	UFUNCTION(BlueprintCallable, Category = "BFConfigDBUtil")
		FVehicleDBInfo		 GetVehicleInfo(FName Name);



	
	UFUNCTION(	BlueprintCallable	, Category = "BFConfigDBUtil" )
		TArray<FVehicleDBInfo>		  GetVehicleInfoList()	;
	UFUNCTION(	BlueprintCallable	, Category = "BFConfigDBUtil" )
		TArray<FVehicleUpgradeDBList> GetVehicleUpgradeList( const FVehicleDBInfo& Info ) ;

	UFUNCTION(BlueprintCallable, Category = "BFConfigDBUtil")
		FUpgradeItemDBInfo GetVehicleUpgradeItem(FName Name, EVehicleUpgrade UpgradeType , int32 UpgradeLevel );
	UFUNCTION(BlueprintCallable, Category = "BFConfigDBUtil")
		FUpgradeItemDBInfo GetWeaponUpgradeItem(FName Name, EAttachedWeaponUpgrade UpgradeType , int32 UpgradeLevel );


	
	UFUNCTION(BlueprintCallable, Category = "BFGameSaveData")
		FWeaponUpgradeDBList 			GetWeaponUpgrade(const FWeaponDBInfo& Info, int32 Index);
	UFUNCTION(BlueprintCallable, Category = "BFGameSaveData")
		FVehicleUpgradeDBList 			GetVehicleUpgrade(const FVehicleDBInfo& Info, int32 Index);

	UFUNCTION(	BlueprintCallable	, Category = "BFConfigDBUtil" )
		TArray<FChapterDBInfo>			GetChapterInfoList()	;
	UFUNCTION(	BlueprintCallable	, Category = "BFConfigDBUtil" )
		TArray<FMissionDBInfo>			GetMissionInfoForChapter(const FChapterDBInfo& ChapterInfo)	;

	UFUNCTION(BlueprintCallable, Category = "BFConfigDBUtil")
		FMissionDBInfo					GetMissionInfo(const FChapterDBInfo& ChapterInfo, int32 Index);
	UFUNCTION(BlueprintCallable, Category = "BFConfigDBUtil")
		FMissionDBInfo					GetMissionInfoForID(const FChapterDBInfo& ChapterInfo, int32 MissionID);
	UFUNCTION(BlueprintCallable, Category = "BFConfigDBUtil")
		FChapterDBInfo					GetChapterInfo(int32 ChapterID);

	UFUNCTION(	BlueprintCallable	, Category = "BFConfigDBUtil" )
		int32			GetWeaponsCount() ;
	UFUNCTION(	BlueprintCallable	, Category = "BFConfigDBUtil" )
		int32			GetVehiclesCount() ;
	
	UFUNCTION(	BlueprintCallable	, Category = "BFConfigDBUtil" )
		int32			GetChaptersCount() ;

	UFUNCTION(	BlueprintCallable	, Category = "BFConfigDBUtil" )
		int32			GetMissionsCount(const FChapterDBInfo& ChapterInfo) ; 

};
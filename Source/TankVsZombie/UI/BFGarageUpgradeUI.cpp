#include "TankVsZombie.h"
#include "BFGarageUpgradeUI.h"



UBFGarageUpgradeUI::UBFGarageUpgradeUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}
 
ABFGaragePlayerController* UBFGarageUpgradeUI::GetGaragePlayerController()
{
	return Cast<ABFGaragePlayerController>(GetPlayerController());
}
 
int32 UBFGarageUpgradeUI::GetAvailbleCredits()
{
	UBFLocalPlayer* Player = GetLocalPlayer();

	if (Player != NULL)
	{
		Player->GetAvailableCredits();
	}
	return 0;
}

bool UBFGarageUpgradeUI::NotEnoughCredits(int32 RequiredAmount)
{
	UBFLocalPlayer* Player = GetLocalPlayer();

	if (Player != NULL)
	{
		return Player->HasEnoughCredits(RequiredAmount);
	}
	return false;
}

FWeaponDBInfo  UBFGarageUpgradeUI::GetWeaponInfo(FName Name)
{
	FWeaponDBInfo Info;
	TArray<FWeaponDBInfo> InfoList = GetGameConfigDB()->GetWeaponInfoList();
	for (int32 i = 0; i < InfoList.Num(); i++)
	{
		if (InfoList[i].Name.IsEqual(Name))
			return Info;
	}
	return Info;
}

FVehicleDBInfo UBFGarageUpgradeUI::GetVehicleInfo(FName Name)
{
	FVehicleDBInfo Info;
	TArray<FVehicleDBInfo> InfoList = GetGameConfigDB()->GetVehicleInfoList();
	for (int32 i = 0; i < InfoList.Num(); i++)
	{
		if (InfoList[i].Name.IsEqual(Name))
			return Info;
	}
	return Info;
}

FWeaponDBInfo  UBFGarageUpgradeUI::GetWeaponInfoForIndex(int32 Index)
{
	FWeaponDBInfo Info;
	TArray<FWeaponDBInfo> InfoList = GetGameConfigDB()->GetWeaponInfoList();
	if (Index < InfoList.Num())
	{
		Info = InfoList[Index];
	}
	return Info;
}

FVehicleDBInfo UBFGarageUpgradeUI::GetVehicleInfoForIndex(int32 Index)
{
	FVehicleDBInfo Info;
	TArray<FVehicleDBInfo> InfoList = GetGameConfigDB()->GetVehicleInfoList();
	if (Index < InfoList.Num())
	{
		Info = InfoList[Index];
	}
	return Info;
}

FBFWeaponInfoUIData  UBFGarageUpgradeUI::GetWeaponInfoUIData(int32 Index)
{
	FBFWeaponInfoUIData UIData;

	TArray<FWeaponDBInfo> InfoList = GetGameConfigDB()->GetWeaponInfoList();
	if (Index < InfoList.Num())
	{
		FWeaponDBInfo Info = InfoList[Index];
		FSavedVehicleWeaponState State = GetGameSaveData()->GetWeaponStateByName(Info.Name); 

		UIData.Info			= Info;
		UIData.SavedState	= State;
		UIData.DisplayName	= Info.DisplayName;
		UIData.Name			= Info.Name;
		UIData.WeaponClass = Info.WeaponClass;
	}
	return UIData;
}

FBFVehicleInfoUIData UBFGarageUpgradeUI::GetVehicleInfoUIData(int32 Index)
{

	FBFVehicleInfoUIData UIData;

	TArray<FVehicleDBInfo> InfoList = GetGameConfigDB()->GetVehicleInfoList();
	if (Index < InfoList.Num())
	{
		FVehicleDBInfo Info = InfoList[Index];
		FSavedPlayerVehicleState State = GetGameSaveData()->GetVehicleStateByName(Info.Name);

		UIData.Info				  = Info;
		UIData.SavedState		  = State;
		UIData.DisplayName		  = Info.DisplayName;
		UIData.Name				  = Info.Name;
		UIData.GarageVehicleClass = Info.GarageVehicleClass;
	}

	return UIData;
} 

int32 UBFGarageUpgradeUI::GetMaxVehiclesCount()
{
	UBFConfigDBUtil* DBUtil = GetGameConfigDB();

	if (DBUtil != NULL)
	{
		return DBUtil->GetVehicleInfoList().Num();
	}
	
	return 0;
}

int32 UBFGarageUpgradeUI::GetMaxWeaponsCount()
{
	UBFConfigDBUtil* DBUtil = GetGameConfigDB();

	if (DBUtil != NULL)
	{
		return DBUtil->GetWeaponInfoList().Num();
	}

	return 0;
}

bool UBFGarageUpgradeUI::CanUpgradeWeaponLevel( FWeaponDBInfo Info , EAttachedWeaponUpgrade UpgradeType , int32 Level )
{
	UBFLocalPlayer* Player = GetLocalPlayer();
	if (Player != NULL)
	{

	}
	return false;
}

bool UBFGarageUpgradeUI::CanUpgradeVehicleLevel( FVehicleDBInfo Info , EVehicleUpgrade UpgradeType , int32 Level )
{
	UBFLocalPlayer* Player = GetLocalPlayer();
	if (Player != NULL)
	{

	}
	return false;
}

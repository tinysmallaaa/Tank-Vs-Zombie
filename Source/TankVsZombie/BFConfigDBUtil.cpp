#include "TankVsZombie.h"
#include "BFConfigDBUtil.h" 


UBFConfigDBUtil::UBFConfigDBUtil(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

} 


FWeaponUpgradeDBList  UBFConfigDBUtil::GetWeaponUpgrade(const FWeaponDBInfo& Info, int32 Index)
{
	TArray<FWeaponUpgradeDBList*> List;
	if (Info.Upgrades != NULL)
	{
		Info.Upgrades->GetAllRows(FString("FWeaponUpgradeDBList"), List);
		if (Index < List.Num())
		{
			return *List[Index];
		}
	}

	return FWeaponUpgradeDBList();
}

FVehicleUpgradeDBList  UBFConfigDBUtil::GetVehicleUpgrade(const FVehicleDBInfo& Info, int32 Index)
{
	TArray<FVehicleUpgradeDBList*> List;
	if (Info.Upgrades != NULL)
	{
		Info.Upgrades->GetAllRows(FString("FVehicleUpgradeDBList"), List);
		if (Index < List.Num())
		{
			return *List[Index];
		}
	}

	return FVehicleUpgradeDBList();
}

FUpgradeItemDBInfo UBFConfigDBUtil::GetVehicleUpgradeItem(FName Name , EVehicleUpgrade UpgradeType, int32 UpgradeLevel)
{
	FVehicleDBInfo Info = GetVehicleInfo(Name);

	TArray<FVehicleUpgradeDBList> UpgradesList = GetVehicleUpgradeList(Info);
	if (UpgradeLevel < UpgradesList.Num())
	{
		switch (UpgradeType)
		{
		case EVehicleUpgrade::EVEHICLE_UPGRADE_ALERTER:return UpgradesList[UpgradeLevel].Alerter;
		case EVehicleUpgrade::EVEHICLE_UPGRADE_DURABILITY:return UpgradesList[UpgradeLevel].Durability;
		case EVehicleUpgrade::EVEHICLE_UPGRADE_NITRO_CAP:return UpgradesList[UpgradeLevel].NitroCap;
		case EVehicleUpgrade::EVEHICLE_UPGRADE_SHELL_CLIP_SIZE:return UpgradesList[UpgradeLevel].ShellClipSize;
		case EVehicleUpgrade::EVEHICLE_UPGRADE_SHELL_DAMAGE:return UpgradesList[UpgradeLevel].ShellDamage;
		case EVehicleUpgrade::EVEHICLE_UPGRADE_SHELL_RELOAD:return UpgradesList[UpgradeLevel].ShellReload;
		}
	}

	return FUpgradeItemDBInfo();
}

FUpgradeItemDBInfo UBFConfigDBUtil::GetWeaponUpgradeItem(FName Name, EAttachedWeaponUpgrade UpgradeType, int32 UpgradeLevel)
{
	FWeaponDBInfo Info = GetWeaponInfo(Name);

	TArray<FWeaponUpgradeDBList> UpgradesList = GetWeaponUpgradeList(Info);
	
	if (UpgradeLevel < UpgradesList.Num())
	{
		switch (UpgradeType)
		{
		case EAttachedWeaponUpgrade::EATTACHEDWEAPON_UPGRADE_RELOAD:return UpgradesList[UpgradeLevel].Reload;
		case EAttachedWeaponUpgrade::EATTACHEDWEAPON_UPGRADE_POWER:return UpgradesList[UpgradeLevel].Power;
		case EAttachedWeaponUpgrade::EATTACHEDWEAPON_UPGRADE_FIRERATE:return UpgradesList[UpgradeLevel].FireRate;
		case EAttachedWeaponUpgrade::EATTACHEDWEAPON_UPGRADE_AMMOCAP:return UpgradesList[UpgradeLevel].AmmoCap;
		case EAttachedWeaponUpgrade::EATTACHEDWEAPON_UPGRADE_ACCURACY:return UpgradesList[UpgradeLevel].Accuracy;
		}
	} 

	return FUpgradeItemDBInfo();
}


TArray<FWeaponDBInfo> UBFConfigDBUtil::GetWeaponInfoList()
{

	TArray<FWeaponDBInfo*> InfoList;
	WeaponInfoTable->GetAllRows<FWeaponDBInfo>(FString("FWeaponDBInfo"), InfoList);

	TArray<FWeaponDBInfo> OutList;
	for (int32 i = 0; i < InfoList.Num(); i++)
	{
		OutList.Add( *InfoList[i] );
	}
	return OutList;

}

TArray<FWeaponUpgradeDBList> UBFConfigDBUtil::GetWeaponUpgradeList( const FWeaponDBInfo& Info )
{

	TArray<FWeaponUpgradeDBList*> InfoList;
	Info.Upgrades->GetAllRows<FWeaponUpgradeDBList>(FString("FUpgradeItemDBInfo"), InfoList);

	TArray<FWeaponUpgradeDBList> OutList;
	for (int32 i = 0; i < InfoList.Num(); i++)
	{
		OutList.Add(*InfoList[i]);
	}
	return OutList;

}

TArray<FVehicleDBInfo> UBFConfigDBUtil::GetVehicleInfoList()
{
	TArray<FVehicleDBInfo*> InfoList;
	VehicleInfoTable->GetAllRows<FVehicleDBInfo>(FString("FVehicleDBInfo"), InfoList);

	TArray<FVehicleDBInfo> OutList;
	for (int32 i = 0; i < InfoList.Num(); i++)
	{
		OutList.Add(*InfoList[i]);
	}
	return OutList;
}

TArray<FVehicleUpgradeDBList> UBFConfigDBUtil::GetVehicleUpgradeList( const FVehicleDBInfo& Info )
{
	TArray<FVehicleUpgradeDBList*> InfoList;
	Info.Upgrades->GetAllRows<FVehicleUpgradeDBList>(FString("FVehicleDBInfo"), InfoList);

	TArray<FVehicleUpgradeDBList> OutList;
	for (int32 i = 0; i < InfoList.Num(); i++)
	{
		OutList.Add(*InfoList[i]);
	}
	return OutList;
}

FWeaponDBInfo		 UBFConfigDBUtil::GetWeaponInfo(FName Name)
{
	FWeaponDBInfo Info;
	TArray<FWeaponDBInfo*> InfoList;
	WeaponInfoTable->GetAllRows<FWeaponDBInfo>(FString("FWeaponDBInfo"), InfoList);

	for (int32 i = 0; i < InfoList.Num(); i++)
	{
		if (InfoList[i]->Name == Name)
			Info = *InfoList[i];
	}

	return Info;
}

FVehicleDBInfo		 UBFConfigDBUtil::GetVehicleInfo(FName Name)
{
	FVehicleDBInfo Info;
	TArray<FVehicleDBInfo*> InfoList;
	WeaponInfoTable->GetAllRows<FVehicleDBInfo>(FString("FVehicleDBInfo"), InfoList);

	for (int32 i = 0; i < InfoList.Num(); i++)
	{
		if (InfoList[i]->Name == Name)
			Info = *InfoList[i];
	}

	return Info;
}

TArray<FChapterDBInfo> UBFConfigDBUtil::GetChapterInfoList()
{
	TArray<FChapterDBInfo*> InfoList;
	ChaptersTable->GetAllRows<FChapterDBInfo>(FString("FChapterDBInfo"), InfoList);

	TArray<FChapterDBInfo> OutList;
	for (int32 i = 0; i < InfoList.Num(); i++)
	{
		OutList.Add(*InfoList[i]);
	}
	return OutList;
}

TArray<FMissionDBInfo> UBFConfigDBUtil::GetMissionInfoForChapter(const FChapterDBInfo& ChapterInfo)
{

	TArray<FMissionDBInfo*> InfoList;
	ChapterInfo.MissionsTable->GetAllRows<FMissionDBInfo>(FString("FMissionDBInfo"), InfoList);

	TArray<FMissionDBInfo> OutList;
	for (int32 i = 0; i < InfoList.Num(); i++)
	{
		OutList.Add(*InfoList[i]);
	}

	return OutList;

}

FMissionDBInfo	UBFConfigDBUtil::GetMissionInfoForID(const FChapterDBInfo& ChapterInfo, int32 MissionID)
{
	FMissionDBInfo MissionInfo;

	TArray<FMissionDBInfo*> InfoList;
	ChapterInfo.MissionsTable->GetAllRows<FMissionDBInfo>(FString("FMissionDBInfo"), InfoList);
	if (InfoList.Num())
	{
		for (int32 i = 0; i < InfoList.Num(); i++)
		{
			if (InfoList[i]->MissionID == MissionID)
			{
				MissionInfo = *InfoList[i];
				break;
			}
		}
	}

	return MissionInfo;
}

FMissionDBInfo	UBFConfigDBUtil::GetMissionInfo(const FChapterDBInfo& ChapterInfo, int32 Index)
{
	FMissionDBInfo MissionInfo;

	TArray<FMissionDBInfo*> InfoList;
	ChapterInfo.MissionsTable->GetAllRows<FMissionDBInfo>(FString("FMissionDBInfo"), InfoList);
	if (InfoList.Num())
	{
		MissionInfo = *InfoList[Index];
	}

	return MissionInfo;
}

FChapterDBInfo UBFConfigDBUtil::GetChapterInfo(int32 ChapterID)
{
	FChapterDBInfo ChapterInfo;

	TArray<FChapterDBInfo*> InfoList;
	ChaptersTable->GetAllRows<FChapterDBInfo>(FString("FChapterDBInfo"), InfoList);

 
	for (int32 i = 0; i < InfoList.Num(); i++)
	{
		if (InfoList[i]->ChapterID == ChapterID)
		{
			ChapterInfo = *InfoList[i];
			break;
		}
	}

	return ChapterInfo;
}
int32 UBFConfigDBUtil::GetWeaponsCount()
{
	return WeaponInfoTable->GetRowNames().Num();
}

int32 UBFConfigDBUtil::GetVehiclesCount()
{
	return VehicleInfoTable->GetRowNames().Num();
}

int32 UBFConfigDBUtil::GetChaptersCount()
{
	return ChaptersTable->GetRowNames().Num();
}

int32 UBFConfigDBUtil::GetMissionsCount(const FChapterDBInfo& ChapterInfo)
{
	if (ChapterInfo.MissionsTable != NULL)
	{
		return ChapterInfo.MissionsTable->GetRowNames().Num();
	}

	return 0;
}
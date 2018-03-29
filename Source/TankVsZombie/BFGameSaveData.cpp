#include "TankVsZombie.h"
#include "BFLocalPlayer.h"
#include "BFGameSaveData.h"

UBFGameSaveData::UBFGameSaveData(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UBFGameSaveData::UpdateFromPlayer(UBFLocalPlayer* ProfilePlayer)
{

}

void UBFGameSaveData::PostInitProperties()
{
	Super::PostInitProperties(); 
}

void UBFGameSaveData::SetupDefault(UBFConfigDBUtil* Config)
{
	if (Config != NULL)
	{
		RefConfigDB = (Config);

		ChapterStates.Reset() ;

		for (int32 ChapterIndex = 0; ChapterIndex < Config->GetChapterInfoList().Num(); ChapterIndex++)
		{
			const FChapterDBInfo& Info = Config->GetChapterInfoList()[ChapterIndex];

			FSavedChapterState& State = ChapterStates[ChapterIndex];

			State.bUnlocked = false				;
			State.ChapterID = Info.ChapterID	;
			
			TArray<FMissionDBInfo> Missions = Config->GetMissionInfoForChapter(Info);

			for ( int32 MissionIndex = 0 ; MissionIndex < State.Missions.Num() ; MissionIndex++ )
			{
				State.Missions[MissionIndex].MissionID		= Missions[MissionIndex].MissionID	;
				State.Missions[MissionIndex].CreditsEarned	= 0		;
				State.Missions[MissionIndex].StarsEarned	= 0		;
			}

			if (ChapterIndex == 0)
			{
				State.bUnlocked = true	;
				if (State.Missions.Num() > 0)
					State.Missions[0].bUnlocked = true	;
			}
		}


		for( int32 VehicleIndex = 0 ; VehicleIndex < Config->GetVehiclesCount() ; VehicleIndex++ )
		{
			const FVehicleDBInfo& Info = Config->GetVehicleInfoList()[VehicleIndex]	;
		

			FVehicleUpgradeDBList& UpgradeList = Config->GetVehicleUpgradeList(Info)[0] ;
			FSavedPlayerVehicleState	  State ;
			State.bPurchased			= false	;
			State.bUnlocked				= false		;
			State.Name					= Info.Name		;
			State.GarageVehicleClass	= Info.GarageVehicleClass;
			State.VehicleClass			= Info.VehicleClass	;
			State.LightWeaponName		= Info.LightWeaponName ;

			State.Alerter				= UpgradeList.Alerter.UpgradeAmount ;
			State.AlerterLevel			= 0 ;

			State.Durability			= UpgradeList.Durability.UpgradeAmount	;
			State.DurabilityLevel		= 0 ;

			State.ShellReloadTime		= UpgradeList.ShellReload.UpgradeAmount ;
			State.ShellReloadLevel		= 0 ;

			State.ShellDamageAmount		= UpgradeList.ShellDamage.UpgradeAmount	;
			State.ShellDamageLevel		= 0 ;

			State.NitroCap				= UpgradeList.NitroCap.UpgradeAmount	;
			State.NitroCapLevel			= 0	;

			State.ShellClipSize			= UpgradeList.ShellClipSize.UpgradeAmount ;
			State.ShellClipSizeLevel	= 0;

			VehicleStates.Add(State);

		} 


		for (	int32 WeaponIndex = 0 ; WeaponIndex < Config->GetVehiclesCount() ; WeaponIndex++ )
		{
			const FWeaponDBInfo& Info = Config->GetWeaponInfoList()[WeaponIndex] ;
			
			FWeaponUpgradeDBList& UpgradeList = Config->GetWeaponUpgradeList(Info)[0] ;

			FSavedVehicleWeaponState  State ;
			State.Name				= Info.Name ;
			State.bPurchased		= false ;
			State.bUnlocked			= false ;
			State.ActiveWeaponClass = Info.WeaponClass ;
			
			State.Accuracy			= UpgradeList.Accuracy.UpgradeAmount ;
			State.AccurayLevel		= 0 ;

			State.AmmoClipSize		= UpgradeList.AmmoCap.UpgradeAmount ;
			State.AmmoClipSizeLevel = 0 ;

			State.FireRate			= UpgradeList.FireRate.UpgradeAmount ;
			State.FireRateLevel		= 0 ;

			State.Power				= UpgradeList.Power.UpgradeAmount ;
			State.PowerLevel		= 0 ;

			State.ReloadTime		= UpgradeList.Reload.UpgradeAmount ;
			State.ReloadTimeLevel	= 0 ;
 
			VehicleWeaponStates.Add(State) ;
		}  


	}
}

FSavedMissionState UBFGameSaveData::UnlockNextChapter(int32 ChapterID)
{
	for (int i = 0; i < ChapterStates.Num(); i++)
	{
		const FSavedChapterState& ChapterState = ChapterStates[i];

		if (ChapterState.ChapterID == ChapterID)
		{
			if (i < ChapterStates.Num() - 1)
			{
				ChapterStates[i + 1].bUnlocked = true;
				PlayerState.CurrentChapterID = ChapterStates[i + 1].ChapterID;

				if (ChapterStates[i + 1].Missions.Num() > 0)
				{
					ChapterStates[i + 1].Missions[0].bUnlocked = true;
					return ChapterStates[i + 1].Missions[0];
				}
			}
		}
	}

	return FSavedMissionState();
}


FSavedMissionState UBFGameSaveData::UnlockNextMission(int32 ChapterID, int32 MissionID)
{
	for (int i = 0; i < ChapterStates.Num(); i++)
	{
		  FSavedChapterState& ChapterState = ChapterStates[i];

		if (ChapterState.ChapterID == ChapterID)
		{
			for (int mIndex = 0; mIndex < ChapterState.Missions.Num(); mIndex++)
			{
				if (ChapterState.Missions[mIndex].MissionID == MissionID)
				{
					if (mIndex < ChapterState.Missions.Num() - 1)
					{
						ChapterState.Missions[mIndex + 1].bUnlocked = true;
						return ChapterState.Missions[mIndex + 1];
					}
					else if (mIndex == ChapterState.Missions.Num() - 1)
					{
						return UnlockNextChapter(ChapterState.ChapterID);
					}
				}
			}
		}
	}

	return FSavedMissionState();
}

FSavedMissionState  UBFGameSaveData::GetMissionForID( int32 ChapterID , int32 MissionID )
{ 

	for (int i = 0; i < ChapterStates.Num(); i++)
	{
		const FSavedChapterState& ChapterState = ChapterStates[i];

		if ( ChapterState.ChapterID == ChapterID )
		{
			for (int mIndex = 0; mIndex < ChapterState.Missions.Num(); mIndex++)
			{ 
				if(ChapterState.Missions[mIndex].MissionID == MissionID)
					return ChapterState.Missions[mIndex];
			}
		}
	} 

	return FSavedMissionState();
}

void UBFGameSaveData::ResetProfile_Implementation()
{
	VehicleStates		= ResetVehicleStates()	;
	PlayerState			= ResetPlayerState()	;
	ChapterStates		= ResetChapterStates()	;
	VehicleWeaponStates = ResetWeaponStates()	; 

	if (ChapterStates.Num())
	{
		FSavedChapterState& ChapterState = ChapterStates[0];
		if(ChapterState.Missions.Num())
			ChapterState.Missions[0].bUnlocked = true;
		
		ChapterState.bUnlocked = true;
		PlayerState.CurrentChapterID = ChapterState.ChapterID;
		
	}
}

 
void UBFGameSaveData::SaveMissionState( int32 ChapterID , FSavedMissionState NewState )
{ 
	for (int i = 0; i < ChapterStates.Num(); i++)
	{ 
		  FSavedChapterState& Chapter = ChapterStates[i];
		
		  if (Chapter.ChapterID == ChapterID)
		  {
			  for (int mIndex = 0; mIndex < Chapter.Missions.Num(); mIndex++)
			  {
				  FSavedMissionState& MissionState = Chapter.Missions[mIndex];

				  if (MissionState.MissionID == NewState.MissionID)
				  {
					  Chapter.Missions[mIndex] = NewState;
				  }
			  }
		  }
	}


	for (int i = 0; i < ChapterStates.Num(); i++)
	{
		FSavedChapterState& Chapter = ChapterStates[i];
		Chapter.StarsEarned = 0;
		for (int mIndex = 0; mIndex < Chapter.Missions.Num(); mIndex++)
		{
			Chapter.StarsEarned += Chapter.Missions[mIndex].StarsEarned;
		}
	}
} 

FSavedPlayerVehicleState UBFGameSaveData::GetPlayerVehicleState()
{
	return PlayerState.VehicleState;
}

void UBFGameSaveData::ChangeVehicleState(FSavedVehicleWeaponState WeaponState)
{
	for (int i = 0; i < VehicleWeaponStates.Num(); i++)
	{
		FSavedVehicleWeaponState& state = VehicleWeaponStates[i];

		if ( state.Name.IsEqual(WeaponState.Name))
		{
			VehicleWeaponStates[i] = WeaponState;
			
			break;
		}
	}
}

FSavedPlayerVehicleState UBFGameSaveData::GetVehicleStateByName(FName Name)
{ 
	for (int i = 0; i < VehicleWeaponStates.Num(); i++)
	{
		FSavedPlayerVehicleState& State = VehicleStates[i];
		if (State.Name.IsEqual(Name))
		{
			return State;
		}
	}

	return FSavedPlayerVehicleState();
} 

FSavedVehicleWeaponState UBFGameSaveData::GetWeaponStateByName(FName Name)
{
	for (int i = 0; i < VehicleWeaponStates.Num(); i++)
	{
		FSavedVehicleWeaponState& State = VehicleWeaponStates[i];
		if ( State.Name == Name )
		{
			return State;
		}
	}

	return FSavedVehicleWeaponState();
}

void UBFGameSaveData::UpgradeVehicle( int32 UpgradeLevel , float UpgradeAmount , TEnumAsByte<EVehicleUpgrade> VehicleUpgrade )
{ 
	switch (VehicleUpgrade)
	{
		case EVEHICLE_UPGRADE_SHELL_RELOAD:
		{
			PlayerState.VehicleState.ShellReloadTime  = UpgradeAmount	;
			PlayerState.VehicleState.ShellReloadLevel = UpgradeLevel;
		}break;
		case EVEHICLE_UPGRADE_SHELL_DAMAGE:
		{

			PlayerState.VehicleState.ShellDamageAmount = UpgradeAmount;
			PlayerState.VehicleState.ShellDamageLevel = UpgradeLevel;

		}break;
		case EVEHICLE_UPGRADE_ALERTER:
		{

			PlayerState.VehicleState.Alerter = UpgradeAmount;
			PlayerState.VehicleState.AlerterLevel = UpgradeLevel;

		}break;

		case EVEHICLE_UPGRADE_DURABILITY:
		{

			PlayerState.VehicleState.Durability = UpgradeAmount;
			PlayerState.VehicleState.DurabilityLevel = UpgradeLevel;

		}break;

		case EVEHICLE_UPGRADE_NITRO_CAP:
		{
			PlayerState.VehicleState.NitroCap = UpgradeAmount;
			PlayerState.VehicleState.NitroCapLevel = UpgradeLevel;
		}break;
		case EVEHICLE_UPGRADE_SHELL_CLIP_SIZE:
		{
			PlayerState.VehicleState.ShellClipSize = UpgradeAmount;
			PlayerState.VehicleState.ShellClipSizeLevel = UpgradeLevel;
		}break;
	}

	for (int i = 0; i < VehicleStates.Num(); i++)
	{
		if (VehicleStates[i].Name.IsEqual(PlayerState.VehicleState.Name))
		{
			VehicleStates[i] = PlayerState.VehicleState;
		}
	}

}


void UBFGameSaveData::UpgradeWeapon(FName WeaponName, int32 UpgradeLevel, float UpgradeAmount, TEnumAsByte<EAttachedWeaponUpgrade> WeaponUpgrade)
{ 
	for (int32 i = 0; i < VehicleWeaponStates.Num(); i++)
	{
		FSavedVehicleWeaponState& WeaponState = VehicleWeaponStates[i];
		if (WeaponName.IsEqual(WeaponState.Name) )
		{
			switch (WeaponUpgrade)
			{
			case EATTACHEDWEAPON_UPGRADE_ACCURACY:
			{
				WeaponState.Accuracy = UpgradeAmount;
				WeaponState.AccurayLevel = UpgradeLevel;
			}break;
			case EATTACHEDWEAPON_UPGRADE_FIRERATE:
			{
				WeaponState.FireRate = UpgradeAmount;
				WeaponState.FireRateLevel = UpgradeLevel;
			}break;
			case EATTACHEDWEAPON_UPGRADE_POWER:
			{
				WeaponState.Power = UpgradeAmount;
				WeaponState.PowerLevel = UpgradeLevel;
			}break;
			case EATTACHEDWEAPON_UPGRADE_AMMOCAP:
			{
				WeaponState.AmmoClipSize = UpgradeAmount;
				WeaponState.AmmoClipSizeLevel = UpgradeLevel;
			}break;
			case EATTACHEDWEAPON_UPGRADE_RELOAD:
			{

				WeaponState.ReloadTime = UpgradeAmount;
				WeaponState.ReloadTimeLevel = UpgradeLevel;

			}break;
			}
		}

	}

}

void UBFGameSaveData::SelectVehicleWeapon(int32 WeaponIndex)
{
	FSavedVehicleWeaponState& WeaponState = VehicleWeaponStates[WeaponIndex];
	PlayerState.VehicleState.LightWeaponName = WeaponState.Name;

	for (int i = 0; i < VehicleStates.Num(); i++)
	{ 
		if( VehicleStates[i].Name.IsEqual(PlayerState.VehicleState.Name) )
		{
			VehicleStates[i] = PlayerState.VehicleState;
		} 
	}

}

void UBFGameSaveData::SelectPrimaryVehicle(int32 VehicleIndex)
{
	if ( VehicleIndex < VehicleStates.Num() )
	{
		PlayerState.VehicleState = VehicleStates[VehicleIndex]; 
	}	
}
 
int32 UBFGameSaveData::GetVehicleStateIndex(FSavedPlayerVehicleState State)
{
	for (int i = 0; i < VehicleStates.Num(); i++)
	{
		if ( VehicleStates[i].Name.IsEqual(State.Name) )
		{
			return i;
		}
	}
	return 0;
}

int32 UBFGameSaveData::GetVehicleWeaponStateIndex(FSavedVehicleWeaponState State)
{
	for (int i = 0; i < VehicleWeaponStates.Num(); i++)
	{
		if (VehicleWeaponStates[i].Name.IsEqual(State.Name) )
		{
			return i;
		}
	}
	return 0;
}

int32  UBFGameSaveData::GetCurrentVehicleUpgradeLevel(EVehicleUpgrade Upgrade)
{
	switch (Upgrade)
	{
		case EVEHICLE_UPGRADE_ALERTER:
		{	
			return PlayerState.VehicleState.AlerterLevel;
		}break;
		case EVEHICLE_UPGRADE_DURABILITY:
		{
			return PlayerState.VehicleState.DurabilityLevel;
		}break;
		case EVEHICLE_UPGRADE_SHELL_RELOAD:
		{
			return PlayerState.VehicleState.ShellReloadLevel;
		}break;
		case EVEHICLE_UPGRADE_SHELL_DAMAGE:
		{
			return PlayerState.VehicleState.ShellDamageLevel;
		}break;
		case EVEHICLE_UPGRADE_SHELL_CLIP_SIZE:
		{
			return PlayerState.VehicleState.ShellClipSizeLevel;
		}break;
	}

	return 0; 
}

float UBFGameSaveData::GetCurrentVehicleUpgradeAmount(EVehicleUpgrade Upgrade)
{
	switch (Upgrade)
	{
		case EVEHICLE_UPGRADE_ALERTER:
		{
			return PlayerState.VehicleState.Alerter; 
		}break;
		case EVEHICLE_UPGRADE_DURABILITY:
		{
			return PlayerState.VehicleState.Durability;
		}break;
		case EVEHICLE_UPGRADE_SHELL_RELOAD:
		{
			return PlayerState.VehicleState.ShellReloadTime;
		}break;
		case EVEHICLE_UPGRADE_SHELL_DAMAGE:
		{
			return PlayerState.VehicleState.ShellDamageAmount;
		}break;
		case EVEHICLE_UPGRADE_SHELL_CLIP_SIZE:
		{
			return PlayerState.VehicleState.ShellClipSize;
		}break; 
	} 

	return 0.0f;
}
 
int32  UBFGameSaveData::GetCurrentWeaponUpgradeLevel(EAttachedWeaponUpgrade Upgrade)
{
	FName Name = GetPrimaryWeaponName();

	for (int32 i = 0; i < VehicleWeaponStates.Num(); i++)
	{
		FSavedVehicleWeaponState& WeaponState = VehicleWeaponStates[i];

		if ( WeaponState.Name.IsEqual(Name) )
		{
			switch (Upgrade)
			{
				case EATTACHEDWEAPON_UPGRADE_ACCURACY:
				{
					return WeaponState.AccurayLevel;
				}break;
				case EATTACHEDWEAPON_UPGRADE_AMMOCAP:
				{
					return WeaponState.AmmoClipSizeLevel;
				}break;
				case EATTACHEDWEAPON_UPGRADE_FIRERATE:
				{
					return WeaponState.FireRateLevel;
				}break;
				case EATTACHEDWEAPON_UPGRADE_POWER:
				{
					return WeaponState.PowerLevel;
				}break;
				case EATTACHEDWEAPON_UPGRADE_RELOAD:
				{
					return WeaponState.ReloadTimeLevel;
				}break;

			}
		}
	}

	return 0;
}

float  UBFGameSaveData::GetCurrentWeaponUpgradeAmount(EAttachedWeaponUpgrade Upgrade)
{

	FName Name = GetPrimaryWeaponName();

	for (int32 i = 0; i < VehicleWeaponStates.Num(); i++)
	{
		FSavedVehicleWeaponState& WeaponState = VehicleWeaponStates[i]; 

		if ( WeaponState.Name.IsEqual(Name) )
		{
			switch (Upgrade)
			{ 
				case EATTACHEDWEAPON_UPGRADE_ACCURACY:
				{
					return WeaponState.Accuracy;
				}break;

				case EATTACHEDWEAPON_UPGRADE_AMMOCAP:
				{
					return WeaponState.AmmoClipSize;
				}break;

				case EATTACHEDWEAPON_UPGRADE_FIRERATE:
				{
					return WeaponState.FireRate;
				}break;

				case EATTACHEDWEAPON_UPGRADE_POWER:
				{
					return WeaponState.Power;
				}break;

				case EATTACHEDWEAPON_UPGRADE_RELOAD:
				{
					return WeaponState.ReloadTime;
				}break; 
			}
		} 
	}

	return 0;
}

int32 UBFGameSaveData::GetVehicleIndex(FName Name)
{
	for ( int32 index = 0 ; index < VehicleStates.Num() ; index++ )
	{
		FSavedPlayerVehicleState& state = VehicleStates[index];
		if ( state.Name.IsEqual(Name))
		{
			return index;
		}
	}
	return 0;
}

int32 UBFGameSaveData::GetWeaponIndex(FName Name)
{
	for (int32 index = 0; index < VehicleWeaponStates.Num(); index++)
	{
		FSavedPlayerVehicleState& state = VehicleStates[index];
		if (state.Name.IsEqual(Name) )
		{
			return index;
		}
	}
	return 0;
}

int32  UBFGameSaveData::GetPrimaryWeaponIndex()
{
	return GetWeaponIndex(PlayerState.VehicleState.LightWeaponName);
}

int32  UBFGameSaveData::GetPrimaryVehicleIndex()
{
	return GetWeaponIndex(PlayerState.VehicleState.Name);
}
 
FName UBFGameSaveData::GetPrimaryWeaponName()
{	
	return PlayerState.VehicleState.LightWeaponName;
}


FName UBFGameSaveData::GetPrimaryVehicleName()
{	
	return PlayerState.VehicleState.Name; 
}

int32 UBFGameSaveData::GetCurrentChapterIndex()
{
	for (int i = 0; i < ChapterStates.Num(); i++)
	{
		if (ChapterStates[i].ChapterID == PlayerState.CurrentChapterID)
			return i;
	}
	return 0;
}


FSavedChapterState UBFGameSaveData::GetCurrentChapterState()
{
	for (int i = 0; i < ChapterStates.Num(); i++)
	{
		if (ChapterStates[i].ChapterID == PlayerState.CurrentChapterID)
			return ChapterStates[i];
	}

	return FSavedChapterState();
}

void UBFGameSaveData::UnlockWeapon(TSubclassOf<ABFWeapon> WeaponClass)
{
	for (int i = 0; i < VehicleWeaponStates.Num(); i++)
	{
		if (VehicleWeaponStates[i].ActiveWeaponClass == WeaponClass)
		{
			VehicleWeaponStates[i].bUnlocked = true; 
		}
	}
}

void UBFGameSaveData::UnlockVehicle(TSubclassOf<ABFTankVehicle> VehicleClass)
{
	for (int i = 0; i < VehicleStates.Num(); i++)
	{
		if (VehicleStates[i].VehicleClass == VehicleClass)
		{
			VehicleStates[i].bUnlocked = true;  
		}
	}
}

void UBFGameSaveData::MarkVehiclePurchased(TSubclassOf<ABFTankVehicle> VehicleClass)
{
	for (int i = 0; i < VehicleStates.Num(); i++)
	{
		if (VehicleStates[i].VehicleClass == VehicleClass)
		{
			VehicleStates[i].bPurchased = true; 
		}
	}
}

void UBFGameSaveData::MarkWeaponPurchased(TSubclassOf<ABFWeapon> WeaponClass)
{
	for (int i = 0; i < VehicleWeaponStates.Num(); i++)
	{
		if (VehicleWeaponStates[i].ActiveWeaponClass == WeaponClass)
		{
			VehicleWeaponStates[i].bPurchased = true; 
		}
	}
}

FBFAchievementLevelSavedState UBFGameSaveData::GetAchievementSavedState(int32 AchievementIndex)
{ 
	return SavedAchievementStates[AchievementIndex];
}
 
void UBFGameSaveData::SaveAchievementState(FBFAchievementLevelSavedState State)
{
	SavedAchievementStates[State.Type] = State;
}
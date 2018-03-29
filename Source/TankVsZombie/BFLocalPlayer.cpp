#include "TankVsZombie.h"
#include "BFGameInstance.h"
#include "BFGameSingleton.h"
#include "BFLocalPlayer.h"

#define MAX_ARMOR  1000.0f
#define MAX_ATTACK 10.0f
#define MAX_HEALTH 1000.0f
#define MAX_FIRE_SPEED 1.0f
#define MAX_MOVEMENT 4.0f

UBFLocalPlayer::UBFLocalPlayer(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultSaveGameSlot = "TankVsZombies" ;

}

UBFLocalPlayer::~UBFLocalPlayer()
{

} 

void UBFLocalPlayer::PostInitProperties()
{
	Super::PostInitProperties();
}

void UBFLocalPlayer::CloseAllUI(bool bExceptDialogs)
{

} 

bool UBFLocalPlayer::IsMenuGame()
{
	return false;
}

UBFGameSaveData* UBFLocalPlayer::CreateProfileSettingsObject(const TArray<uint8>& Buffer)
{

	UBFGameSaveData* ProfileSettings = NewObject<UBFGameSaveData>(GetTransientPackage(), UBFGameSaveData::StaticClass());
	 
	FMemoryReader MemoryReader(Buffer, true);
	FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);

	Ar.Seek(0);
 
	ProfileSettings->Serialize(Ar);

	return ProfileSettings;

}

void UBFLocalPlayer::InitSaveGameSlot()
{

	const FString SaveSlotName = GetSaveSlotName();

	UBFGameSingleton* Singleton = UBFGameSingleton::Get();

	if ( Singleton && Singleton->SaveGameClass && !UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0) )
	{ 
		if ( UGameplayStatics::DoesSaveGameExist( DefaultSaveGameSlot , 0 ) )
		{
			UGameplayStatics::DeleteGameInSlot( DefaultSaveGameSlot , 0 );
		}
	
		if (SaveData == nullptr)
		{
		 	SaveData = Cast<UBFGameSaveData>(UGameplayStatics::CreateSaveGameObject( Singleton->SaveGameClass ) );
			SaveData->ResetProfile();
		}  

		UGameplayStatics::SaveGameToSlot( SaveData , SaveSlotName , 0 ); 
	}
	else
	{
		SaveData = Cast<UBFGameSaveData>( UGameplayStatics::LoadGameFromSlot( SaveSlotName , 0 ) );
	 
	}

	if (Singleton->ConfigDBClass != NULL)
	{
		ConfigDB = NewObject<UBFConfigDBUtil>( this , Singleton->ConfigDBClass , "GameConfigDatabase" );
	}

	checkSlow(SaveData==NULL && ConfigDB == NULL);
}

void UBFLocalPlayer::SelectChapter(int32 ChapterIndex)
{ 

	if (SaveData != NULL)
	{
		SaveData->PlayerState.CurrentChapterID = SaveData->ChapterStates[ChapterIndex].ChapterID;
		SaveGameData();
	}

}

FSavedChapterState UBFLocalPlayer::GetChapterState(int32 ChapterIndex)
{  

	if (SaveData != NULL)
	{
		if (ChapterIndex < SaveData->ChapterStates.Num())
		{
			return SaveData->ChapterStates[ChapterIndex];
		}
	}

	return FSavedChapterState();
}

void UBFLocalPlayer::AdvanceMissionAndSave()
{
	checkSlow(	ActiveMission == NULL	);

	if (SaveData != NULL)
	{ 
		ABFPlayerState* PlayerState =  UBFGameplayStatics::GetPlayerState(GetWorld());

		if (PlayerState != NULL && ActiveMission!=NULL)
		{
			CurrentMissionState.StarsEarned		=	PlayerState->CalculateStarsEarned(ActiveMission->ObjectiveList);
			CurrentMissionState.CreditsEarned	=	PlayerState->CreditsEarned;
			SaveData->AddCredits(PlayerState->CreditsEarned); 
			SaveData->SaveMissionState( CurrentChapterID, CurrentMissionState);
			SaveData->UnlockNextMission( CurrentChapterID, CurrentMissionState.MissionID);
			
			CurrentChapterID = SaveData->PlayerState.CurrentChapterID;
			SaveGameData();
		} 
	} 
}

bool UBFLocalPlayer::ShouldDisplayAd()
{ 

	if (SaveData != NULL)
	{
		return !SaveData->bAdFreeInAppPurchaseMade;
	}

	return true; 

}

FString UBFLocalPlayer::GetSaveSlotName() const
{ 
	return DefaultSaveGameSlot; 
}

UBFGameSaveData* UBFLocalPlayer::GetGameSaveData()
{ 

	UBFGameSingleton* Singleton = UBFGameSingleton::Get();

	if ( SaveData == NULL &&  Singleton != NULL && Singleton->SaveGameClass != NULL )
	{  
		InitSaveGameSlot();
	}

	return SaveData; 

}

void  UBFLocalPlayer::SaveGameData()
{ 
 
	UGameplayStatics::SaveGameToSlot( SaveData , GetSaveSlotName() , 0 );

}

void  UBFLocalPlayer::ReadGameSaveData()
{
	
	SaveData = Cast<UBFGameSaveData> ( UGameplayStatics::LoadGameFromSlot( GetSaveSlotName() , 0 ) );

}

 

 
void UBFLocalPlayer::HideMenu()
{ 	

	UBFGameInstance* GI = Cast<UBFGameInstance>(GetGameInstance());
	
	if ( GI != NULL )
	{

	}

}

void UBFLocalPlayer::AddCredits(int32 Count)
{ 

	if (SaveData != NULL)
	{

		SaveData->AddCredits(Count);
		SaveGameData();

	}  
}
  
int32 UBFLocalPlayer::TakeCredits(int32 Count)
{ 

	if (SaveData != NULL)
	{

		if (SaveData->PlayerState.CreditsEarned >= Count)
		{
			SaveData->PlayerState.CreditsEarned -= Count;
		} 

		SaveGameData();
		return SaveData->PlayerState.CreditsEarned;

	}

	return 0;
}

int32 UBFLocalPlayer::GetAvailableCredits()
{ 

	if (SaveData != NULL)
	{ 

		return SaveData->PlayerState.CreditsEarned;

	}

	return 0;  
}

bool UBFLocalPlayer::HasEnoughCredits(int32 Count)
{ 

	if (SaveData != NULL)
	{

		if (SaveData->PlayerState.CreditsEarned >= Count)
		{
			return true;
		}

	}

	return false; 
}

void UBFLocalPlayer::UpgradeVehicle(int32 UpgradeLevel, float UpgradeAmount, TEnumAsByte<EVehicleUpgrade> VehicleUpgrade)
{ 

	if (SaveData != NULL)
	{
		SaveData->UpgradeVehicle(UpgradeLevel, UpgradeAmount, VehicleUpgrade);
		SaveGameData();
	} 
}
 
void UBFLocalPlayer::UpgradeWeapon(FName WeaponName, int32 UpgradeLevel, float UpgradeAmount, TEnumAsByte<EAttachedWeaponUpgrade> WeaponUpgrade)
{

	if (SaveData != NULL)   
	{ 
		SaveData->UpgradeWeapon(WeaponName, UpgradeLevel, UpgradeAmount, WeaponUpgrade);
		SaveGameData(); 
	}

}

FSavedVehicleWeaponState UBFLocalPlayer::GetVehicleWeaponState(UClass* WeaponClass)
{

	if (SaveData != NULL)
	{

		for (int i = 0; i < SaveData->VehicleWeaponStates.Num(); i++)
		{ 
			FSavedVehicleWeaponState& WeaponState = SaveData->VehicleWeaponStates[i];
			if (WeaponState.ActiveWeaponClass==WeaponClass)
			{ 
				return WeaponState;
			}  
		}  
	}

	return FSavedVehicleWeaponState(); 
}

FSavedPlayerVehicleState UBFLocalPlayer::GetPrimaryVehicleState()
{ 
	if (SaveData != NULL)
	{
		return SaveData->PlayerState.VehicleState;
	}

	return FSavedPlayerVehicleState();
}

FSavedVehicleWeaponState UBFLocalPlayer::GetPrimaryWeaponState()
{
	if (SaveData != NULL)
	{
		for (int i = 0; i < SaveData->VehicleWeaponStates.Num(); i++)
		{
			FSavedVehicleWeaponState& WeaponState = SaveData->VehicleWeaponStates[i];

			if ( WeaponState.Name.Compare(SaveData->PlayerState.VehicleState.LightWeaponName) == 0 )
			{
				return WeaponState;
			}
		}
	}

	return FSavedVehicleWeaponState();
}

FSavedPlayerVehicleState UBFLocalPlayer::GetVehicleState(UClass* VehicleClass)
{ 
	if (SaveData != NULL)
	{
		for (int i = 0; i < SaveData->VehicleStates.Num(); i++)
		{
			if (SaveData->VehicleStates[i].VehicleClass == VehicleClass)
			{
				return SaveData->VehicleStates[i];
			}
		}
	}

	return FSavedPlayerVehicleState();
}

FSavedPlayerVehicleState UBFLocalPlayer::GetVehicleStateByIndex(int32 Index)
{
	if (SaveData != NULL)
	{
		if (Index < SaveData->VehicleStates.Num())
		{
			return SaveData->VehicleStates[Index];
		}
	}

	return FSavedPlayerVehicleState();
}

FSavedVehicleWeaponState UBFLocalPlayer::GetVehicleWeaponStateByIndex(int32 Index)
{
	if (SaveData != NULL)
	{
		if (Index < SaveData->VehicleWeaponStates.Num())
		{
			return SaveData->VehicleWeaponStates[Index];
		}
	}

	return FSavedVehicleWeaponState();
}

FSavedMissionState UBFLocalPlayer::GetMissionForChapter( int32 ChapterID , int32 MissionID )
{
	if (SaveData != NULL)
	{
		return SaveData->GetMissionForID( ChapterID , MissionID );
	}

	return FSavedMissionState();
}

void UBFLocalPlayer::SaveMissionState( int32 ChapterID , FSavedMissionState NewState)
{

	if (SaveData != NULL)
	{
		SaveData->SaveMissionState( ChapterID , NewState );
		SaveGameData();
	}

}

void  UBFLocalPlayer::SetupMission()
{	
	ActiveMission->ObjectiveList.Objective0.DisplayText = GetDisplayTextForObjective( ActiveMission->ObjectiveList.Objective0 );
	ActiveMission->ObjectiveList.Objective1.DisplayText = GetDisplayTextForObjective( ActiveMission->ObjectiveList.Objective1 );
	ActiveMission->ObjectiveList.Objective2.DisplayText = GetDisplayTextForObjective( ActiveMission->ObjectiveList.Objective2 );
}


int32   UBFLocalPlayer::GetGrenadesCount()
{
	if (SaveData != NULL)
	{
		return SaveData->PlayerState.GrenadesCount;
	}

	return 0;
}

int32 UBFLocalPlayer::GetLivesCount()
{
	if (SaveData != NULL)
	{
		return SaveData->PlayerState.LivesCount;
	}

	return 0;
}

int32   UBFLocalPlayer::GetMinesCount()
{
	if (SaveData != NULL)
	{
		return SaveData->PlayerState.MinesCount;
	}

	return 0;
}

int32   UBFLocalPlayer::AddGrenades(int32 Count)
{
	if (SaveData != NULL)
	{
		SaveData->PlayerState.GrenadesCount += Count;

		if (SaveData->PlayerState.GrenadesCount < 0)
		{
			SaveData->PlayerState.GrenadesCount = 0;
		}

		SaveGameData();

		return SaveData->PlayerState.GrenadesCount;
	}

	return 0;
}

int32   UBFLocalPlayer::AddMines(int32 Count)
{

	if (SaveData != NULL)
	{

		SaveData->PlayerState.MinesCount += Count;
		if (SaveData->PlayerState.MinesCount < 0)
		{
			SaveData->PlayerState.MinesCount = 0;
		}

		SaveGameData();

		return SaveData->PlayerState.MinesCount;
	}

	return 0;

}

int32 UBFLocalPlayer::AddLives(int32 Count)
{

	if (SaveData != NULL)
	{

		SaveData->PlayerState.LivesCount += Count;
		if (SaveData->PlayerState.LivesCount < 0)
		{
			SaveData->PlayerState.LivesCount = 0;
		}
		SaveGameData();

		return SaveData->PlayerState.LivesCount;

	}

	return 0;
}

void UBFLocalPlayer::SelectVehicleWeapon(int32 WeaponIndex)
{

	if (SaveData != NULL)
	{

		SaveData->SelectVehicleWeapon(WeaponIndex);
		SaveGameData();

	}

}

void UBFLocalPlayer::SelectPrimaryVehicle(int32 VehicleIndex)
{

	if (SaveData != NULL)
	{

		SaveData->SelectPrimaryVehicle(VehicleIndex);
		SaveGameData();

	}

}

void  UBFLocalPlayer::DisableAd()
{

	if (SaveData != NULL)
	{
		
		SaveData->bAdFreeInAppPurchaseMade = true ;		
		SaveGameData();
	}

}

int32 UBFLocalPlayer::GetActiveMissionIndex()
{

	if (SaveData != NULL)
	{
		for ( int32 i = 0 ; i < SaveData->ChapterStates.Num() ; i++ )
		{
			if ( SaveData->ChapterStates[i].ChapterID == SaveData->PlayerState.CurrentChapterID )
			{
				for (int32 mIndex = 0; mIndex < SaveData->ChapterStates[i].Missions.Num(); mIndex++)
				{

					if (SaveData->ChapterStates[i].Missions[mIndex].bUnlocked == false)
						return mIndex;
				}
			}
		}
	}

	return 0;
}
 
int32  UBFLocalPlayer::GetVehicleStateIndex(FSavedPlayerVehicleState State)
{

	if (SaveData != NULL)
	{
		return SaveData->GetVehicleStateIndex( State );
	}

	return 0;
}

int32  UBFLocalPlayer::GetVehicleWeaponStateIndex(FSavedVehicleWeaponState State)
{

	if (SaveData != NULL)
	{
		return SaveData->GetVehicleWeaponStateIndex( State );
	}
	return 0;
}

void  UBFLocalPlayer::UnlockWeapon( TSubclassOf<ABFWeapon> WeaponClass )
{

	if (SaveData != NULL)
	{
		SaveData->UnlockWeapon( WeaponClass );
		SaveGameData();
	}
}

void  UBFLocalPlayer::UnlockVehicle( TSubclassOf<ABFTankVehicle> VehicleClass )
{

	if (SaveData != NULL)
	{
		SaveData->UnlockVehicle( VehicleClass );
		SaveGameData();
	}

 }


FSavedPlayerVehicleState  UBFLocalPlayer::MarkVehiclePurchased(TSubclassOf<ABFTankVehicle> VehicleClass)
{
	if (SaveData != NULL)
	{
		for (int i = 0; i < SaveData->VehicleStates.Num(); i++)
		{
			if (SaveData->VehicleStates[i].VehicleClass == VehicleClass)
			{

				SaveData->VehicleStates[i].bPurchased = true	;
				SaveData->VehicleStates[i].bUnlocked = true		;
				SaveGameData();
				return SaveData->VehicleStates[i];
			}
		}
	}

	return FSavedPlayerVehicleState();
}

FSavedVehicleWeaponState  UBFLocalPlayer::MarkWeaponPurchased(TSubclassOf<ABFWeapon> WeaponClass)
{
	if (SaveData != NULL)
	{
		for (int i = 0; i < SaveData->VehicleWeaponStates.Num(); i++)
		{
			if (SaveData->VehicleWeaponStates[i].ActiveWeaponClass == WeaponClass)
			{
				SaveData->VehicleWeaponStates[i].bPurchased = true	;
				SaveData->VehicleWeaponStates[i].bUnlocked = true	;
				SaveGameData();
				return SaveData->VehicleWeaponStates[i];
			}
		}
	}

	return FSavedVehicleWeaponState(); 
}

FSavedVehicleWeaponState UBFLocalPlayer::MarkVehicleWeaponPurchasedByName(FName Name)
{
	if ( SaveData != NULL )
	{
		for (int i = 0; i < SaveData->VehicleWeaponStates.Num(); i++)
		{
			if ( SaveData->VehicleWeaponStates[i].Name.IsEqual(Name) )
			{
				SaveData->VehicleWeaponStates[i].bPurchased = true	;
				SaveData->VehicleWeaponStates[i].bUnlocked = true	;	 
			
				SaveGameData();
				return SaveData->VehicleWeaponStates[i];
			}
		}
	}

	return FSavedVehicleWeaponState();
}

FSavedPlayerVehicleState UBFLocalPlayer::MarkVehiclePurchasedByName(FName Name)
{
	if ( SaveData != NULL )
	{
		for (int i = 0; i < SaveData->VehicleStates.Num(); i++)
		{
			if ( SaveData->VehicleStates[i].Name.IsEqual(Name))
			{
				SaveData->VehicleStates[i].bPurchased = true	;
				SaveData->VehicleStates[i].bUnlocked = true		;

				SaveGameData();
				return SaveData->VehicleStates[i];
			}
		}
	}

	return FSavedPlayerVehicleState();
}

FBFAchievementLevelSavedState UBFLocalPlayer::GetAchievementSavedState(EAchievementType Type)
{

	int32 Index = (int32)Type;
	FBFAchievementLevelSavedState State;
	if (SaveData != NULL)
	{
		State = SaveData->GetAchievementSavedState( Type );
	}

	return State; 
}

void UBFLocalPlayer::SaveAchievementState(FBFAchievementLevelSavedState State)
{

	if (SaveData != NULL)
	{
		SaveData->SaveAchievementState( State );
	}

}

FBFAchievementUIData UBFLocalPlayer::GetAchievementUIData(EAchievementType Type, UBFLocalPlayer* Player)
{
	return AchievementManager->GetAchievementUIData(Type, Player);
}

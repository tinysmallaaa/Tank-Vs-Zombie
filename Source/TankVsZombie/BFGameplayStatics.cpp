#include "TankVsZombie.h"
#include "BFGameplayStatics.h" 

bool UBFGameplayStatics::IsMobilePlatfom()
{
	if ( GetPlatformName().Compare(TEXT("Android")) == 0)
		return true;

	else if ( GetPlatformName().Compare(TEXT("IOS")) == 0)
		return true;

	return false;

}

UBFLocalPlayer* UBFGameplayStatics::GetLocalPlayerFromPC(APlayerController* PlayerController)
{
 
	return Cast<UBFLocalPlayer>( PlayerController->GetLocalPlayer() );
 
}

UBFLocalPlayer*  UBFGameplayStatics::GetLocalPlayer()
{
	if (GEngine->GetWorld() != NULL)
	{
		APlayerController* PC = GEngine->GetWorld()->GetFirstPlayerController();

		if (PC != NULL)
		{
			return Cast<UBFLocalPlayer>(PC->GetLocalPlayer());
		}
	}

	return nullptr;

}

ABFPlayerState* UBFGameplayStatics::GetPlayerState(UWorld* World)
{

	APlayerController* PC = World->GetFirstPlayerController();

	if (PC != NULL)
	{
		return Cast<ABFPlayerState>(PC->PlayerState);
	}

	return NULL;

}

ABFTankPlayerController* UBFGameplayStatics::GetTankPlayerController()
{

	return Cast<ABFTankPlayerController>( GEngine->GetWorld()->GetFirstPlayerController() );

}

FCultureInfo UBFGameplayStatics::GetCurrentCultureInfo()
{	
	UBFGameUserSettings* Settings = Cast<UBFGameUserSettings>(GEngine->GetGameUserSettings());

	if (Settings != NULL)
	{ 

		TArray<FCultureInfo> CulturesList = GetAvailbleCultures();
		for (int32 i = 0; i < CulturesList.Num(); i++)
		{
			if (CulturesList[i].Name.Compare(Settings->CultureName) == 0)
				return CulturesList[i];
		}

	}

	return FCultureInfo();
}

bool UBFGameplayStatics::ChangeCurrentCultureInGame(FCultureInfo Info)
{

	TArray<FCultureRef> LocalizedCulturesForGame;

	FInternationalization::Get().GetCulturesWithAvailableLocalization(FPaths::GetGameLocalizationPaths(), LocalizedCulturesForGame, true);
 
	int32 numCultures = LocalizedCulturesForGame.Num();

	for (int32 i = 0; i < numCultures; ++i)
	{
		if ( !LocalizedCulturesForGame[i].Get().GetEnglishName().Compare(Info.EnglishName) )
		{
			UBFGameUserSettings* Settings = Cast<UBFGameUserSettings>(GEngine->GetGameUserSettings());

			if (Settings != NULL)
			{

				Settings->CultureName = Info.Name;
			}
			if(GEngine->IsEditor()==false)
			FInternationalization::Get().SetCurrentCulture(Info.Name);
			Settings->SaveConfig();
			return true;
		}
	} 

	return false;
}

TArray<FCultureInfo> UBFGameplayStatics::GetAvailbleCultures()
{
	
	TArray<FCultureRef> LocalizedCulturesForGame ; 
	FInternationalization::Get().GetCulturesWithAvailableLocalization(FPaths::GetGameLocalizationPaths(), LocalizedCulturesForGame, false);
	
	TArray<FCultureInfo> InfoList ;	
	int32 numCultures = LocalizedCulturesForGame.Num() ;

	for ( int32 i = 0 ; i < numCultures ; ++i ) 
	{
		FCultureInfo Info; 
		
		Info.Name		 = LocalizedCulturesForGame[i].Get().GetName(); 
		Info.EnglishName = LocalizedCulturesForGame[i].Get().GetEnglishName(); 
		Info.DisplayName = LocalizedCulturesForGame[i].Get().GetDisplayName();

		InfoList.Add(Info); 
	}

	return InfoList;
	 
}

void UBFGameplayStatics::SaveGameUserSettings()
{
	UBFGameUserSettings* Settings = Cast<UBFGameUserSettings>(GEngine->GetGameUserSettings());

	if (Settings != NULL)
	{
		Settings->SaveConfig();
	}
}
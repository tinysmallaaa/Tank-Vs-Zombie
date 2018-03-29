#include "TankVsZombie.h" 
#include "BFGameUserSettings.h"

UBFGameUserSettings::UBFGameUserSettings(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	MasterVolume = 1.0f;
	EffectsVolume = 1.0f;
	MusicVolume = 1.0f;

}


 void UBFGameUserSettings::SetSoundClassVolumeByName(const FString& SoundClassName, float NewVolume)
{
 
	if (!GIsEditor)
	{
		FAudioDevice* AudioDevice = GEngine->GetMainAudioDevice();
		if (AudioDevice != NULL)
		{
			 
			FAudioThreadSuspendContext AudioThreadSuspend;

			PRAGMA_DISABLE_DEPRECATION_WARNINGS
				for (TMap<USoundClass*, FSoundClassProperties>::TConstIterator It(AudioDevice->GetSoundClassPropertyMap()); It; ++It)
					PRAGMA_ENABLE_DEPRECATION_WARNINGS
				{
					USoundClass* ThisSoundClass = It.Key();
					
					if (ThisSoundClass != NULL && (ThisSoundClass->GetName().Compare(SoundClassName)==0))
					{ 
						ThisSoundClass->Properties.Volume = NewVolume;
					}
			
				}
		}
	}
}

 
float UBFGameUserSettings::GetCrosshairSpeed()
{
	return CrosshairSpeed;
}
 
void UBFGameUserSettings::SetCrosshairSpeed(float Speed)
{
	CrosshairSpeed = Speed;
 
}
void UBFGameUserSettings::SetMasterVolume(float NewVolume)
{
	MasterVolume = NewVolume;
  
	SetSoundClassVolumeByName(MasterSoundClassName, NewVolume);
}

float UBFGameUserSettings::GetMasterVolume()
{
	return MasterVolume;
}

void UBFGameUserSettings::SetMusicVolume(float NewVolume)
{
	MusicVolume = NewVolume;
	 
	SetSoundClassVolumeByName(MusicSoundClassName, NewVolume);
}

float UBFGameUserSettings::GetMusicVolume()
{
	return MusicVolume;
}

void UBFGameUserSettings::SetEffectsVolume(float NewVolume)
{
	EffectsVolume = NewVolume;
	 
	SetSoundClassVolumeByName(EffectsSoundClassName, NewVolume);
}

float UBFGameUserSettings::GetEffectsVolume()
{
	return EffectsVolume;
}


void UBFGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();
 
	FullscreenMode = EWindowMode::Windowed;
	ScreenPercentage = 1.0f; 
	MasterVolume = 1.0f;
	EffectsVolume = 1.0f;
	MusicVolume = 1.0f; 
	ShadowQuality		= 1.0f;
	ScreenPercentage	= 1.0f;
	EffectsQuality		= 1.0f;
}

void UBFGameUserSettings::SetShadowsQuality(float ShadowPercentage)
{
	ShadowQuality = ShadowPercentage;

	int32 Level = ShadowQuality*5.0f;

	auto ShadowQualityCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ShadowQuality"));
	ShadowQualityCVar->ClearFlags(EConsoleVariableFlags::ECVF_SetBySystemSettingsIni);
	ShadowQualityCVar->Set(Level, ECVF_SetByGameSetting); 
}

void UBFGameUserSettings::SetEffectsQuality(float QualityPercentage)
{
	EffectsQuality = QualityPercentage ;
	int32 Level  = EffectsQuality*5.0f ;

	auto AAModeCVar = IConsoleManager::Get().FindConsoleVariable( TEXT("r.PostProcessAAQuality") );
	AAModeCVar->ClearFlags( EConsoleVariableFlags::ECVF_SetBySystemSettingsIni ) ;
	AAModeCVar->Set( Level , ECVF_SetByGameSetting );

	auto SharpenCVar = IConsoleManager::Get().FindConsoleVariable( TEXT("r.Tonemapper.Sharpen") );
	SharpenCVar->ClearFlags( EConsoleVariableFlags::ECVF_SetBySystemSettingsIni );
	if (Level == 4)
	{
		SharpenCVar->Set( 1 , ECVF_SetByGameSetting );
	}
	else
	{
		SharpenCVar->Set(0, ECVF_SetByGameSetting);
	}
	SetShadowsQuality(QualityPercentage);

} 
 
void UBFGameUserSettings::SetContentScaleFactor(float NewScreenPercentage)
{ 
	ScreenPercentage = NewScreenPercentage;

#if (PLATFORM_ANDROID) || (PLATFORM_IOS)    

	//auto MobileContentScaleFactorCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileContentScaleFactor"));
	//MobileContentScaleFactorCVar->Set( ScreenPercentage*2.0f , ECVF_SetByDeviceProfile  );

#endif

} 

void UBFGameUserSettings::SetQualityLevel(int32 Level)
{

	QualityLevel = Level ; 
	auto CVarMaterialQualityLevel = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MaterialQualityLevel"));
		
	CVarMaterialQualityLevel->Set( Level , ECVF_SetByScalability );  
	if ( Level == 2 )			Level = 1	;
	else if ( Level == 1 )		Level = 2	;
	SetOverallScalabilityLevel( Level )	;

}

void UBFGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);

	SetContentScaleFactor( ScreenPercentage ) ;
	SetQualityLevel		( QualityLevel ) ;
	SetMasterVolume		( MasterVolume ) ;
	SetEffectsVolume	( EffectsVolume	) ;
	SetMusicVolume		( MusicVolume )	;
	
	UBFGameplayStatics::ChangeCurrentCultureInGame(UBFGameplayStatics::GetCurrentCultureInfo());
}

void UBFGameUserSettings::UpdateVersion()
{

}

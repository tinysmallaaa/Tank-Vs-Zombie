#pragma once
#include "BFGameUserSettings.Generated.h"


UCLASS(Blueprintable,config = GameUserSettings, configdonotcheckdefaults)
class TANKVSZOMBIE_API UBFGameUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY( EditDefaultsOnly , Category = "BFGameUserSettings" )
		FString MasterSoundClassName  ;
	UPROPERTY( EditDefaultsOnly , Category = "BFGameUserSettings" )
		FString EffectsSoundClassName ;
	UPROPERTY( EditDefaultsOnly , Category = "BFGameUserSettings" )
		FString MusicSoundClassName   ;

	UPROPERTY( config , BlueprintReadWrite )
		FString CultureName		;
	UPROPERTY( config , BlueprintReadWrite )
		float EffectsVolume		;
	UPROPERTY( config , BlueprintReadWrite )
		float MasterVolume		;

	UPROPERTY( config , BlueprintReadWrite )
		float MusicVolume		;	 
	UPROPERTY( config , BlueprintReadWrite )
		float CrosshairSpeed	;
	UPROPERTY( globalconfig , BlueprintReadWrite)
		float ScreenPercentage	;

	UPROPERTY( config , BlueprintReadWrite )
		float EffectsQuality	;
	UPROPERTY( config , BlueprintReadWrite )
		float ShadowQuality		;
	UPROPERTY( config , BlueprintReadWrite )
		int32 QualityLevel;

	UFUNCTION( BlueprintCallable , Category = "BFGameUserSettings" )
		float GetCrosshairSpeed();
	UFUNCTION( BlueprintCallable , Category = "BFGameUserSettings" )
		void SetCrosshairSpeed(float Speed);
	UFUNCTION( BlueprintCallable , Category = "BFGameUserSettings")
		void SetMasterVolume(float NewVolume);


	UFUNCTION( BlueprintCallable, Category = "BFGameUserSettings" )
		float GetMasterVolume();
	UFUNCTION( BlueprintCallable , Category = "BFGameUserSettings" )
		float GetMusicVolume();
	UFUNCTION( BlueprintCallable , Category = "BFGameUserSettings" )
		void SetMusicVolume(float NewVolume);

	UFUNCTION( BlueprintCallable , Category = "BFGameUserSettings" )
		float GetEffectsVolume();
	UFUNCTION( BlueprintCallable , Category = "BFGameUserSettings" )
		void SetEffectsVolume(float NewVolume);

	void  SetSoundClassVolumeByName(const FString& SoundClassName, float NewVolume);
	virtual void SetToDefaults();
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;

	virtual void UpdateVersion();
	UFUNCTION( BlueprintCallable , Category = "BFGameUserSetttings" )
		void SetShadowsQuality(float ShadowPercentage);

	UFUNCTION( BlueprintCallable , Category = "BFGameUserSettings" )
	  void SetEffectsQuality(float QualityPercentage); 
	UFUNCTION( BlueprintCallable , Category = "BFGameUserSettings" )
	  void SetContentScaleFactor(float NewScreenPercentage);
	UFUNCTION( BlueprintCallable , Category = "BFGameUserSettings" )
		void SetQualityLevel(int32 Level);

};
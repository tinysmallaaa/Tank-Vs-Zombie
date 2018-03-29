#pragma once
#include "BFGameplayStatics.Generated.h"

USTRUCT(BlueprintType)
struct FCultureInfo
{
	GENERATED_USTRUCT_BODY()
	FCultureInfo()
		:EnglishName("En") 
	{

	}
	UPROPERTY(BlueprintReadOnly, Category = "CultureInfo")
		FString EnglishName;
	UPROPERTY(BlueprintReadOnly, Category = "CultureInfo")
		FString DisplayName;
	UPROPERTY(BlueprintReadOnly, Category = "CultureInfo")
		FString Name;
};

UCLASS(Blueprintable)
class UBFGameplayStatics : public UGameplayStatics
{
	GENERATED_BODY()
public:  
	
	UFUNCTION(BlueprintCallable, Category = "BFGameplayStatics")
		static bool IsMobilePlatfom();

	UFUNCTION(BlueprintCallable, Category = "BFGameplayStatics")
		static UBFLocalPlayer* GetLocalPlayerFromPC(APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, Category = "BFGameplayStatics")
		static UBFLocalPlayer* GetLocalPlayer();

	UFUNCTION(BlueprintCallable, Category = "BFGameplayStatics")
		static ABFPlayerState* GetPlayerState(UWorld* World); 
	 
	UFUNCTION(BlueprintCallable, Category = "BFGameplayStatics")
		static ABFTankPlayerController* GetTankPlayerController(); 

	UFUNCTION(BlueprintCallable, Category = "BFGameplayStatics")
		static bool ChangeCurrentCultureInGame( FCultureInfo Info );

	UFUNCTION(BlueprintCallable, Category = "BFGameplayStatics")
		static FCultureInfo GetCurrentCultureInfo();

	UFUNCTION(BlueprintCallable, Category = "BFGameplayStatics")
		static TArray<FCultureInfo> GetAvailbleCultures();
	UFUNCTION(BlueprintCallable, Category = "BFGameplayStatics")
		static void SaveGameUserSettings();


};
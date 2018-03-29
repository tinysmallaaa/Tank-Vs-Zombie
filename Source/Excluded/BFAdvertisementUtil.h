#pragma once
#include "BFAdvertisementUtil.Generated.h"
 
#define USE_APPODEAL_AD

UCLASS(Blueprintable)
class UBFAdvertisementUtil : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "BFAdvertisementUtil")
		void Initialize(FString Key, bool INTERSTITIAL, bool NON_SKIPPABLEE_VIDEO, bool REWARDED_VIDEO, bool BANNER);

	UFUNCTION(BlueprintCallable, Category = "Advertising|Appodeal")
		bool Show(FString PlacementName, bool INTERSTITIAL, bool NON_SKIPPABLEE_VIDEO, bool REWARDED_VIDEO, bool BANNER_BOTTOM, bool BANNER_TOP);

	UFUNCTION(BlueprintCallable, Category = "Advertising|Appodeal")
		void Cache(bool INTERSTITIAL, bool NON_SKIPPABLEE_VIDEO, bool REWARDED_VIDEO, bool BANNER);

	UFUNCTION(BlueprintCallable, Category = "Advertising|Appodeal")
		void HideBanner();

	UFUNCTION(BlueprintCallable, Category = "Advertising|Appodeal")
		bool IsLoaded(bool INTERSTITIAL, bool NON_SKIPPABLEE_VIDEO, bool REWARDED_VIDEO, bool BANNER);

	UFUNCTION(BlueprintCallable, Category = "Advertising|Appodeal")
		void DisableNetwork(FString Network, bool INTERSTITIAL, bool NON_SKIPPABLEE_VIDEO, bool REWARDED_VIDEO, bool BANNER);

	UFUNCTION(BlueprintCallable, Category = "Advertising|Appodeal")
		void TrackInAppPurchase(float amount, FString currency);

	UFUNCTION(BlueprintCallable, Category = "Advertising|Appodeal")
		void DisableAutoCache(bool INTERSTITIAL, bool NON_SKIPPABLEE_VIDEO, bool REWARDED_VIDEO, bool BANNER);

	UFUNCTION(BlueprintCallable, Category = "Advertising|Appodeal")
		void SetTestMode(bool Testing);

	UFUNCTION(BlueprintCallable, Category = "Advertising|Appodeal")
		void SetLogging(bool Logging);

	UFUNCTION(BlueprintCallable, Category = "Advertising|Appodeal")
		void DisableSmartBanners(bool Disable);

	UFUNCTION(BlueprintCallable, Category = "Advertising|Appodeal")
		void DisableBannerAnimation(bool Disable);

	UFUNCTION(BlueprintCallable, Category = "Advertising|Appodeal")
		void Disable728x90Banners(bool Disable);

	UFUNCTION(BlueprintCallable, Category = "Advertising|Appodeal")
		void DisableWriteExternalStoragePermissionCheck();

	UFUNCTION(BlueprintCallable, Category = "Advertising|Appodeal")
		 void DisableLocationPermissionCheck();
	UPROPERTY()
		UObject* InternalAdObject;

};
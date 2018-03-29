#include "TankVsZombie.h"
#include <Appodeal/public/AppodealComponent.h>
#include "BFAdvertisementUtil.h"

UBFAdvertisementUtil::UBFAdvertisementUtil(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

#if defined(USE_APPODEAL_AD)
#if defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)		
	InternalAdObject = ObjectInitializer.CreateDefaultSubobject<UAppodealComponent>(this, TEXT("Appodeal"));
#endif
#endif 

}

void UBFAdvertisementUtil::Initialize(FString Key, bool INTERSTITIAL, bool NON_SKIPPABLEE_VIDEO, bool REWARDED_VIDEO, bool BANNER)
{
	if (InternalAdObject != NULL)
	{
#if defined(USE_APPODEAL_AD)

#if  defined(PLATFORM_IOS)		
		(Cast<UAppodealComponent>(InternalAdObject))->AppodealInitializeWithParameters( "" , Key, INTERSTITIAL , NON_SKIPPABLEE_VIDEO , REWARDED_VIDEO , BANNER );
#elif defined(PLATFORM_ANDROID)
		(Cast<UAppodealComponent>(InternalAdObject))->AppodealInitializeWithParameters(Key, "" , INTERSTITIAL , NON_SKIPPABLEE_VIDEO , REWARDED_VIDEO , BANNER );
#endif

#endif
	}
}

bool UBFAdvertisementUtil::Show(FString PlacementName, bool INTERSTITIAL, bool NON_SKIPPABLEE_VIDEO, bool REWARDED_VIDEO, bool BANNER_BOTTOM, bool BANNER_TOP)
{
	if (InternalAdObject != NULL)
	{

#if defined(USE_APPODEAL_AD) && (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)	)
		return (Cast<UAppodealComponent>(InternalAdObject))->AppodealShow( PlacementName , INTERSTITIAL , NON_SKIPPABLEE_VIDEO , REWARDED_VIDEO , BANNER_BOTTOM,  BANNER_TOP);
#endif 

	}
	return false;

}

void UBFAdvertisementUtil::Cache(bool INTERSTITIAL, bool NON_SKIPPABLEE_VIDEO, bool REWARDED_VIDEO, bool BANNER)
{
	if (InternalAdObject != NULL)
	{

#if defined(USE_APPODEAL_AD) && (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)	)
		(Cast<UAppodealComponent>(InternalAdObject))->AppodealCache( INTERSTITIAL, NON_SKIPPABLEE_VIDEO, REWARDED_VIDEO, BANNER);
#endif 

	}

}

void UBFAdvertisementUtil::HideBanner()
{
	if (InternalAdObject != NULL)
	{

#if defined(USE_APPODEAL_AD) && (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)	)
		(Cast<UAppodealComponent>(InternalAdObject))->AppodealHideBanner();
#endif

	}
}

bool UBFAdvertisementUtil::IsLoaded(bool INTERSTITIAL, bool NON_SKIPPABLEE_VIDEO, bool REWARDED_VIDEO, bool BANNER)
{
	
	if (InternalAdObject != NULL)
	{

#if defined(USE_APPODEAL_AD) && (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)	)
		(Cast<UAppodealComponent>(InternalAdObject))->AppodealIsLoaded( INTERSTITIAL , NON_SKIPPABLEE_VIDEO , REWARDED_VIDEO , BANNER );
#endif 

	}

	return false;
}

void UBFAdvertisementUtil::DisableNetwork( FString Network , bool INTERSTITIAL , bool NON_SKIPPABLEE_VIDEO , bool REWARDED_VIDEO , bool BANNER )
{
	if (InternalAdObject != NULL)
	{

#if defined(USE_APPODEAL_AD) && (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)	)
		(Cast<UAppodealComponent>(InternalAdObject))->AppodealDisableNetwork( Network , INTERSTITIAL , NON_SKIPPABLEE_VIDEO , REWARDED_VIDEO , BANNER );
#endif 

	}
}

void UBFAdvertisementUtil::TrackInAppPurchase(float amount, FString currency)
{

#if defined(USE_APPODEAL_AD) && (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)	)
	(Cast<UAppodealComponent>(InternalAdObject))->AppodealTrackInAppPurchase( amount , currency );
#endif 

}

void UBFAdvertisementUtil::DisableAutoCache(bool INTERSTITIAL, bool NON_SKIPPABLEE_VIDEO, bool REWARDED_VIDEO, bool BANNER)
{

#if defined(USE_APPODEAL_AD) && (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)	)
	(Cast<UAppodealComponent>(InternalAdObject))->AppodealDisableAutoCache(INTERSTITIAL, NON_SKIPPABLEE_VIDEO, REWARDED_VIDEO, BANNER);
#endif 

}

void UBFAdvertisementUtil::SetTestMode(bool Testing)
{
#if defined(USE_APPODEAL_AD) && (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)	)
	(Cast<UAppodealComponent>(InternalAdObject))->AppodealSetTesting(Testing);
#endif 
}

void UBFAdvertisementUtil::SetLogging(bool Logging)
{
#if defined(USE_APPODEAL_AD) && (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)	)
	(Cast<UAppodealComponent>(InternalAdObject))->AppodealSetLogging(Logging);
#endif 
}

void UBFAdvertisementUtil::DisableSmartBanners(bool Disable)
{
#if defined(USE_APPODEAL_AD) && (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)	)
	(Cast<UAppodealComponent>(InternalAdObject))->AppodealDisableSmartBanners(Disable);
#endif 
}

void UBFAdvertisementUtil::DisableBannerAnimation(bool Disable)
{
#if defined(USE_APPODEAL_AD) && (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)	)
	(Cast<UAppodealComponent>(InternalAdObject))->AppodealDisableBannerAnimation(Disable);
#endif 
}

void UBFAdvertisementUtil::Disable728x90Banners(bool Disable)
{
#if defined(USE_APPODEAL_AD) && (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)	)
	(Cast<UAppodealComponent>(InternalAdObject))->AppodealDisable728x90Banners(Disable);
#endif 
}

void UBFAdvertisementUtil::DisableWriteExternalStoragePermissionCheck()
{
#if defined(USE_APPODEAL_AD) && (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)	)
	(Cast<UAppodealComponent>(InternalAdObject))->AppodealDisableWriteExternalStoragePermissionCheck();
#endif 
}

void UBFAdvertisementUtil::DisableLocationPermissionCheck()
{
#if defined(USE_APPODEAL_AD) && (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)	)
	(Cast<UAppodealComponent>(InternalAdObject))->AppodealDisableLocationPermissionCheck();
#endif 
}

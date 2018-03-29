#pragma once
#include "BFGameAchievementManager.Generated.h"


UENUM(BlueprintType)
enum EAchievementType
{
	ACHIEVEMENT_TYPE_EAGLE_EYE,
	ACHIEVEMENT_TYPE_ZOMBIE_ATE_MY_GIRLFRIEND,
	ACHIEVEMENT_TYPE_ZOMBIE_OVERLORD,
	ACHIEVEMENT_TYPE_SHOT_LIKE_A_BOSS,
	ACHIEVEMENT_TYPE_THE_WORLD_DOESNT_END_TODAY,
	ACHIEVEMENT_TYPE_I_NEED_MORE_AMMO,
	ACHIEVEMENT_TYPE_NO_ONE_TOUCH_ME,
	ACHIEVEMENT_TYPE_TURRET_I_CHOOSE_YOU,
	ACHIEVEMENT_TYPE_QUADRA_KILL,
	ACHIEVEMENT_TYPE_HOW_TO_EXPLODE_YOUR_BOSS,
	ACHIEVEMENT_TYPE_I_HATE_RELOAD,
	ACHIEVEMENT_TYPE_NIGHTMARE_ON_THE_ZOMBIE_STREET,
	ACHIEVEMENT_TYPE_PAINKILLER,
	ACHIEVEMENT_TYPE_KNEEL_BEFORE_ME, 
	ACHIEVEMENT_TYPE_KA_BOOM,
	ACHIEVEMENT_TYPE_TURRET_LEVEL,
	ACHIEVEMENT_TYPE_MINIGUN_LEVEL,
	ACHIEVEMENT_TYPE_COIL_LEVEL,
	ACHIEVEMENT_TYPE_LASER_LEVEL,
	ACHIEVEMENT_TYPE_DUAL_MINIGUN_LEGEND,
	ACHIEVEMENT_TYPE_NONE,
};

USTRUCT(BlueprintType)
struct FBFAchievementUpdateResult
{
	GENERATED_USTRUCT_BODY()

	FBFAchievementUpdateResult()
		:Type(ACHIEVEMENT_TYPE_NONE),
		Level(0),
		CreditsEarned(0)
	{

	}

	UPROPERTY( BlueprintReadOnly , Category = "BFAchievementInfo")
		TEnumAsByte<EAchievementType> Type ; 

	UPROPERTY(BlueprintReadOnly, Category = "BFAchievementInfo")
		int32 Level	;

	UPROPERTY(BlueprintReadOnly, Category = "BFAchievementResult")
		int32 CreditsEarned	; 

	UPROPERTY(BlueprintReadOnly, Category = "BFAchievementResult")
		bool bShowNotification ; 



};

USTRUCT(BlueprintType)
struct FBFAchievementLevel
{
	GENERATED_USTRUCT_BODY()
	
	FBFAchievementLevel()
		:Description(""),
		RequiredAmount(0),
		RewardsAmount(0)
	{

	}
	UPROPERTY(	BlueprintReadWrite	, Category = "BFAchievementLevel"	)
		FString	 Description	;

	UPROPERTY(	BlueprintReadWrite	, Category = "BFAchievementLevel"	)
		int32    RequiredAmount	;

	UPROPERTY(	BlueprintReadWrite	, Category = "BFAchievementLevel"	)
		int32    RewardsAmount	;
};

USTRUCT(BlueprintType)
struct FBFAchievementInfo
{
	GENERATED_USTRUCT_BODY()
	FBFAchievementInfo()
		:Name(""),
		Type(ACHIEVEMENT_TYPE_NONE)
	{

	}
	UPROPERTY(	BlueprintReadWrite	, Category = "BFAchievementInfo")
		FString Name;

	UPROPERTY(  BlueprintReadWrite	, Category = "BFAchievementInfo" )
		TEnumAsByte<EAchievementType> Type ; 

	UPROPERTY(	BlueprintReadWrite	, Category = "BFAchievementInfo" )
		TArray<FBFAchievementLevel> Levels ;  


};

USTRUCT(BlueprintType)
struct FBFAchievementUIData
{
	GENERATED_USTRUCT_BODY()
	FBFAchievementUIData()
		:Name(""),Description(""),
		ProgressAmountMax(0.0f),
		CurrentProgress(0.0f),
		CurrentLevel(0)
	{

	}
	UPROPERTY(	BlueprintReadOnly	, Category = "BFAchievementInfo"	)
		FString Name;

	UPROPERTY(	BlueprintReadOnly	, Category = "BFAchievementInfo"	)
		FString Description;

	UPROPERTY(	BlueprintReadOnly	, Category = "BFAchievementInfo"	)
		float	ProgressAmountMax;

	UPROPERTY(	BlueprintReadOnly	, Category = "BFAchievementInfo"	)
		float	CurrentProgress;

	UPROPERTY(	BlueprintReadOnly	, Category = "BFAchievementInfo"	)
		int32	CurrentLevel;  
};

UCLASS(Blueprintable)
class UBFGameAchievementManager : public UObject
{
public:
	GENERATED_UCLASS_BODY()

	UPROPERTY( BlueprintReadWrite , Category = "BFGameAchievementManager" )
		TArray<FBFAchievementInfo> AchievementInfoList;

	UFUNCTION( BlueprintCallable  , Category = "BFGameAchievementManager" )
		FBFAchievementUpdateResult UpdateAchievement( EAchievementType Type , int32 Amount , UBFLocalPlayer* Player) ;

	UFUNCTION(BlueprintNativeEvent, Category = "BFGameAchievementManager")
		void InitAchievement( UBFLocalPlayer* LocalPlayer );

	FBFAchievementUIData GetAchievementUIData(EAchievementType Type,class UBFLocalPlayer* Player);

};

#pragma once
#include "BFLevelSummery.Generated.h" 



USTRUCT(BlueprintType)
struct FBFTestMissionSetup
{
	GENERATED_USTRUCT_BODY()


 
};
UCLASS(Blueprintable)
class TANKVSZOMBIE_API UBFLevelSummary : public UObject
{
	 GENERATED_UCLASS_BODY()
public: 

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = LevelSummery)
		FBFTestMissionSetup	TestMission;

	UPROPERTY(	EditInstanceOnly	, AssetRegistrySearchable, Category = LevelSummery)
		FString LevelName;
	UPROPERTY(	EditInstanceOnly	, AssetRegistrySearchable, Category = LevelSummery)
		FString LevelPath;
	UPROPERTY(	EditInstanceOnly	,	AssetRegistrySearchable	, Category = LevelSummary	)
		FString Title;
 	UPROPERTY(	EditInstanceOnly	,	AssetRegistrySearchable	, Category = LevelSummary	)
		int32	OptimalPlayerCount;
 	UPROPERTY(	EditInstanceOnly	,	AssetRegistrySearchable	, Category = LevelSummary	)
		int32	OptimalTeamPlayerCount;
 	UPROPERTY(	EditInstanceOnly	,	AssetRegistrySearchable	, Category = LevelSummary	)
		FString Author;
 	UPROPERTY(	EditInstanceOnly	,	AssetRegistrySearchable	, Category = LevelSummary	)
		FText	Description;
 	UPROPERTY(	EditInstanceOnly	, Category = LevelSummary	)
	UTexture2D* Screenshot;
 	UPROPERTY(AssetRegistrySearchable)
		FString ScreenshotReference;
 
	virtual void PreSave(const class ITargetPlatform* TargetPlatform)
	{
		if (Screenshot)
		{
			ScreenshotReference = Screenshot->GetPathName();
		}
		else
		{
			ScreenshotReference = TEXT("");
		}
	}
};
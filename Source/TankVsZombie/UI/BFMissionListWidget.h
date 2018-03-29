#pragma once
#include "BFUserWidget.h"
#include "BFMissionListWidget.Generated.h"

USTRUCT(BlueprintType)
struct FBFMissionItemUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "MissionItemUIData")
		bool bIsLocked;
	UPROPERTY(BlueprintReadOnly, Category = "MissionItemUIData")
		FText DisplayName;
	UPROPERTY(BlueprintReadOnly, Category = "MissionItemUIData")
		UTexture2D* ScreenShot;
	UPROPERTY(BlueprintReadOnly, Category = "MissionItemUIData")
		int32	StarRating;
};

UCLASS(Blueprintable,Abstract)
class UBFMissionListWidget : public UBFUserWidget
{
	GENERATED_UCLASS_BODY()
public:

	UFUNCTION( BlueprintCallable , Category = "MissionListWidget" )
		TArray<FBFMissionItemUIData> GetMissionItems(int32 ChapterID, bool bIsIndex);
 
};
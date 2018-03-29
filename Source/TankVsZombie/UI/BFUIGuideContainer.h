#pragma once
#include "BFUserWidget.h"
#include "BFUIGuideContainer.Generated.h"



USTRUCT(BlueprintType)
struct FGuideInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "FocusedWidget")
		UBFUserWidget* TargetWidget; 

	UPROPERTY(BlueprintReadWrite, Category = "FocusedWidget")
		UBFUserWidget* GuideWidget;

	UPROPERTY(BlueprintReadWrite, Category = "FocusedWidget")
		TEnumAsByte<EBFUIFeedbackType> FeedbackType; 
};
   
UCLASS(Blueprintable, Abstract)
class UBFUIGuideContainer : public UBFUserWidget 
{
	GENERATED_UCLASS_BODY()

public: 

	void ExecuteFeedback(FGuideInfo Guide);
 
private:

	int32				GuideItr;
	TArray<FGuideInfo>  Guides;
};
#pragma once  
#include "UserWidget.h"
#include "BFLoadingWidget.Generated.h"


UCLASS(Blueprintable)
class UBFLoadingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBFLoadingWidget(const FObjectInitializer& ObjectInitializer)
		:Super(ObjectInitializer)
	{

	}
	 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BFLoadingWidget")
		float PreLoadingAnimTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BFLoadingWidget")
		float PostLoadingAnimTime;

	UFUNCTION(BlueprintImplementableEvent, Category = "BFLoadingWidget")
		void BeginLoadingAnimation();
	UFUNCTION(BlueprintImplementableEvent, Category = "BFLoadingWidget")
		void EndLoadingAnimation();
};

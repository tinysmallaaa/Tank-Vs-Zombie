#pragma once 
#include <UserWidget.h>
#include "BFConfigDBUtil.h"
#include "BFGameSaveData.h"
#include "BFUserWidget.Generated.h"


UENUM(BlueprintType)
enum EBFUIFeedbackType
{
	UI_FEEDBACK_COMMAND		 ,
	UI_FEEDBACK_LIST_SCROLLED  ,
	UI_FEEDBACK_CREDIT_BOUGHT,
	UI_FEEDBACK_SPIN_SLOT	 ,
	UI_FEEDBACK_TANK_SPEED_UPGRADED,
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUIFeedbackDelegate, UBFUserWidget*, Widget);

UCLASS(Blueprintable,Abstract)
class UBFUserWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()

public: 
	int32 ZOrder;

	UPROPERTY( BlueprintReadWrite , Category = "BFUserWidget" )
		FUIFeedbackDelegate Delegate;

	UFUNCTION( BlueprintCallable , Category = "BFUserWidget" )
		FAnchors GetBound() const; 
	int32 GetParentZOrder(ULocalPlayer* LocalPlayer) ;
	 
	UFUNCTION(BlueprintCallable, Category = "BFUserWidget")
		APlayerController* GetPlayerController();
	UFUNCTION(BlueprintCallable, Category = "BFUserWidget")
		UBFLocalPlayer* GetLocalPlayer();

	UFUNCTION(BlueprintCallable, Category = "BFUserWidget")
		UBFGameSaveData* GetGameSaveData();
	UFUNCTION(BlueprintCallable, Category = "BFUserWidget")
		UBFConfigDBUtil* GetGameConfigDB();

	UFUNCTION(BlueprintCallable, Category = "BFUserWidget")
		void PushToViewport( UBFUserWidget* Parent , int32 UIZOrder = 0 );
};

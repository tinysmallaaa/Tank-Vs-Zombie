#pragma once
#include "BFPlaceableItemController.Generated.h"


UCLASS(Blueprintable)
class ABFPlaceableItemController : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "PlaceableItemController")
		TSubclassOf<AActor> PlaceableItemClass; 	
	UPROPERTY()
		AActor* PlaceableItem; 

	void CreatePlaceableItem(); 

	virtual void OnDragStart(APlayerController* PC, float ScreenX, float ScreenY)  ;
	virtual void OnDragEnd(APlayerController* PC, float ScreenX, float ScreenY)  ;
	virtual void OnDragMove(APlayerController* PC, float ScreenX, float ScreenY)  ;

	UFUNCTION(BlueprintImplementableEvent, Category = "PlaceableItemController")
		void OnInteractionStarted();

	UFUNCTION(BlueprintImplementableEvent, Category = "PlaceableItemController")
		void PromptForInteraction();

	IBFPlaceableGadgetInterface* GetGadgetItem()
	{
		return Cast<IBFPlaceableGadgetInterface>(PlaceableItem);
	}

private:

	bool bInteractionStarted ;
	FVector LastBestPosition ;
};

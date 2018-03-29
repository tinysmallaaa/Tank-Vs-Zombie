#pragma once
#include <AIController.h>
#include "BFMiniTankBotAIController.Generated.h"


UCLASS(Blueprintable,Abstract)
class ABFMiniTankBotAIController : public AAIController 
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(BlueprintReadWrite , Category = "MiniTankBotAIController")
		ABFMiniTankBot* Bot	;

	UPROPERTY(BlueprintReadWrite, Category = "MiniTankBotAIController")
		TWeakObjectPtr<class ABFAIBot> ActiveTarget	;
	
	virtual void Tick(float DeltaSeconds) override;

		void  FirePressed(bool Pressed)	;

	UFUNCTION(BlueprintCallable, Category = "MiniTankBotAIController")
		FVector GetFireDirection() ; 
private:
	bool bFirePressed;

};

#pragma once
#include "BFEncounterVolumeTarget.Generated.h"

UCLASS(Blueprintable)
class TANKVSZOMBIE_API ABFEncounterVolumeTarget : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = "EncounterVolumeTarget")
		void Activate();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "EncounterVolumeTarget")
		void Deactivate(); 

};

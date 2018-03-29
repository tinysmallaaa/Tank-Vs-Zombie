#pragma once
#include "AnimNotify_SwitchCamera.Generated.h"


UCLASS(Blueprintable)
class UAnimNotify_SwitchCamera : public UAnimNotify 
{
	GENERATED_BODY()
public:
	UAnimNotify_SwitchCamera();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "SwitchCamera" , meta = (ExposeOnSpawn = true) )
		bool bCameraComponent ;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "SwitchCamera" , meta = (ExposeOnSpawn = true) )
		FName CameraName ;
 
};

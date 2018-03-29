#include "TankVsZombie.h"
#include "AnimNotify_SwitchCamera.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Vehicle/BFPlayerTank.h"

/////////////////////////////////////////////////////
// UAnimNotify_PlaySound

UAnimNotify_SwitchCamera::UAnimNotify_SwitchCamera()
	: Super()
{ 

#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(196, 142, 255, 255);
#endif // WITH_EDITORONLY_DATA

}

void UAnimNotify_SwitchCamera::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	ABFPlayerTank* TankPawn = Cast<ABFPlayerTank>(MeshComp->GetOwner());

	if (TankPawn != NULL)
	{
		TankPawn->FireCannon();
	}
}

 

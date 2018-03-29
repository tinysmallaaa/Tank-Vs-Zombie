#include "TankVsZombie.h"
#include "AnimNotify_PawnCMD.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Vehicle/BFPlayerTank.h"

/////////////////////////////////////////////////////
// UAnimNotify_PlaySound

UAnimNotify_PawnCMD::UAnimNotify_PawnCMD()
	: Super()
{ 

#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(196, 142, 255, 255);
#endif // WITH_EDITORONLY_DATA

}

void UAnimNotify_PawnCMD::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	switch (Command)
	{
		case PAWN_CMD_FIRE_SHELL:
		{
			checkSlow( Parameters.Num() >= 1 );
			ABFPlayerTank* TankPawn = Cast<ABFPlayerTank>(MeshComp->GetOwner());

			if (TankPawn != NULL)
			{
				TankPawn->Fire(Parameters[0].BooleanFlag);
			}

		}break;

		case PAWN_CMD_FIRE_GUN:
		{
			ABFPlayerTank* TankPawn = Cast<ABFPlayerTank>(MeshComp->GetOwner());

			if (TankPawn != NULL)
			{
				TankPawn->FireMountedWeapon();
			}

		}break;

		case PAWN_CMD_FIRE_GUN2:
		{
			ABFPlayerTank* TankPawn = Cast<ABFPlayerTank>(MeshComp->GetOwner());

			if (TankPawn != NULL)
			{
				TankPawn->FireCannon();
			}

		}break;

		case PAWN_CMD_FIRE_SHELL_STOP:
		{
			ABFPlayerTank* TankPawn = Cast<ABFPlayerTank>(MeshComp->GetOwner());

			if (TankPawn != NULL)
			{
				TankPawn->FireCannon();
			}

		}break;

		case PAWN_CMD_FIRE_GUN_STOP:
		{
			ABFPlayerTank* TankPawn = Cast<ABFPlayerTank>(MeshComp->GetOwner());

			if (TankPawn != NULL)
			{
				TankPawn->StopWeaponFiring();
			}

		}break; 
	} 
}

FString UAnimNotify_PawnCMD::GetNotifyName_Implementation() const
{ 

	return Super::GetNotifyName_Implementation(); 

}

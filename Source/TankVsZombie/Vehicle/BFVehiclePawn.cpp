#include "TankVsZombie.h"
#include "BFVehiclePawn.h"  
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextRenderComponent.h"
#include "Materials/Material.h"
#include "GameFramework/Controller.h"
#include "BFTankMovementComponent.h"



const FName ABFVehiclePawn::LookUpBinding("LookUp");
const FName ABFVehiclePawn::LookRightBinding("LookRight");

#define LOCTEXT_NAMESPACE "VehiclePawn"

ABFVehiclePawn::ABFVehiclePawn(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UBFTankMovementComponent>(Super::VehicleMovementComponentName))
{
	// Simulation
	TankVehicleComp = CastChecked<UBFTankMovementComponent>(GetVehicleMovement());
 
	
	// Colors for the incar gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	// Colors for the in-car gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	bInReverseGear = false;
}
 

void ABFVehiclePawn::MoveForward(float Val)
{
	TankVehicleComp->SetThrottleInput(Val);
}

void ABFVehiclePawn::MoveRight(float Val)
{
	TankVehicleComp->SetSteeringInput(Val);
}

void ABFVehiclePawn::OnHandbrakePressed()
{
	TankVehicleComp->SetHandbrakeInput(true);
}

void ABFVehiclePawn::OnHandbrakeReleased()
{
	TankVehicleComp->SetHandbrakeInput(false);
}

void ABFVehiclePawn::OnToggleCamera()
{

}



void ABFVehiclePawn::Tick(float Delta)
{
	Super::Tick(Delta);

	// Setup the flag to say we are in reverse gear
	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;

	// Update the strings used in the hud (incar and onscreen)
	UpdateHUDStrings();

	// Set the string in the incar hud
	SetupInCarHUD();

	bool bHMDActive = false;

	if (bHMDActive == false)
	{
		if ((InputComponent) && (bInCarCameraActive == true))
		{
			//	FRotator HeadRotation = InternalCamera->RelativeRotation;
			//	HeadRotation.Pitch += InputComponent->GetAxisValue(LookUpBinding);
			//	HeadRotation.Yaw += InputComponent->GetAxisValue(LookRightBinding);
			//	InternalCamera->RelativeRotation = HeadRotation;
		}
	}
}

void ABFVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	bool bEnableInCar = false;
} 
 
void ABFVehiclePawn::UpdateHUDStrings()
{
	float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	int32 KPH_int = FMath::FloorToInt(KPH);

	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));

	if (bInReverseGear == true)
	{
		GearDisplayString = FText(LOCTEXT("ReverseGear", "R"));
	}
	else
	{
		int32 Gear = GetVehicleMovement()->GetCurrentGear();
		GearDisplayString = (Gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(Gear);
	}
}

void ABFVehiclePawn::SetupInCarHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ((PlayerController != nullptr))
	{

	}
}

#undef LOCTEXT_NAMESPACE

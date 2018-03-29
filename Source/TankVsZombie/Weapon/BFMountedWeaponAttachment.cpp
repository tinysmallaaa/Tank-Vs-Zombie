#include "TankVsZombie.h"
#include "BFProjectile.h"
#include "BFMountedWeapon.h"


ABFMountedWeapon::ABFMountedWeapon(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>( this , FName("Camera") );
	CameraComponent->AttachToComponent(	Mesh , FAttachmentTransformRules(	EAttachmentRule::KeepRelative , false	),TEXT("CameraSocket")); 
	CameraJitterStrength = 0.1f;

}

void ABFMountedWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (CameraMountSocket != NAME_None)
	{
		CameraComponent->AttachToComponent(Mesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false), CameraMountSocket);
	}
}



void ABFMountedWeapon::UpdateViewRotation(float Yaw, float Pitch)
{ 
	WeaponPitch += Pitch	;
	WeaponYaw	+= Yaw		;

	WeaponPitch = FMath::Clamp(	WeaponPitch	, WeaponPitchMin	, WeaponPitchMax	);
	WeaponYaw	= FMath::Clamp(	WeaponYaw	, WeaponYawMin		, WeaponYawMax		);
}

void ABFMountedWeapon::Tick(	float DeltaSeconds	)
{
	Super::Tick(DeltaSeconds);

	UBFAnimInstance* Instance = Cast<UBFAnimInstance>(	Mesh->GetAnimInstance()	);
	if (	Instance != NULL	)
	{
		Instance->AnimInstanceOwner = this	;
	}
}

void ABFMountedWeapon::PostFireEffects()
{
	ABFPlayerTank* OwnerTank = Cast<ABFPlayerTank>(WeaponOwner)	;
	if (	OwnerTank != NULL	)
	{
		OwnerTank->TriggerCameraJitter( CameraJitterStrength , 0.2f ) ;
	}
}



void ABFMountedWeapon::ControlCamera(float DeltaSeconds, float Yaw, float Pitch)
{

}
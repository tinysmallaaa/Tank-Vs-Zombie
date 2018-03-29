#include "TankVsZombie.h"
#include "BFAnimInstance.h"


UBFAnimInstance::UBFAnimInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

ABFWeapon* UBFAnimInstance::GetMountedWeaponOwner()
{ 
	if (AnimInstanceOwner != NULL)
	{
		return Cast<ABFWeapon>(AnimInstanceOwner);
	} 

	return  Cast<ABFWeapon>( GetOwningActor() ) ;
}
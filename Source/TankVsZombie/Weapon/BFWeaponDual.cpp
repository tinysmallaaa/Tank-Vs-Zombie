#include "TankVsZombie.h"
#include "AI/BFAIBot.h"
#include "BFWeaponDual.h"

ABFWeaponDual::ABFWeaponDual(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	 
}

void ABFWeaponDual::BeginPlay()
{
	Super::BeginPlay();  
}
 

void ABFWeaponDual::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

 
FVector ABFWeaponDual::CalculateFireLocation(FVector Direction)
{

	FVector Location = GetActorLocation();

	if (FireLocationItr == 0)
	{
		FireLocationItr = 1;

		if (Mesh->GetSocketByName(FireLocationSocket))
			Location = Mesh->GetSocketLocation(FireLocationSocket);

	}
	else if (FireLocationItr == 1)
	{
		FireLocationItr = 0;

		if (Mesh->GetSocketByName(FireLocationSocket2))
			Location = Mesh->GetSocketLocation(FireLocationSocket2);
	}

	return ( Location + Direction * FireOffset ); 

}


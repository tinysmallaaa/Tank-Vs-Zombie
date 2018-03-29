#include "TankVsZombie.h"
#include "BFGameSingleton.h"


UBFGameSingleton* UBFGameSingleton::Get()
{
	return Cast<UBFGameSingleton>( GEngine->GameSingleton );
}

UBFGameSingleton::UBFGameSingleton(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

 
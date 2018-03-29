#include "TankVsZombie.h"
#include "BFGameModeMayhem.h"


ABFGameModeMayhem::ABFGameModeMayhem(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{


}

void ABFGameModeMayhem::BeginMissionPlay()
{

}

void ABFGameModeMayhem::EndMissionPlay()
{

}

void  ABFGameModeMayhem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void  ABFGameModeMayhem::BeginPlay()
{
	Super::BeginPlay();
}

void  ABFGameModeMayhem::ScoreKill(AController* Killed, AController* KilledBy, const FBotDamageParamEx& DamageParam)
{

}
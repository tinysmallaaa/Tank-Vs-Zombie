// Fill out your copyright notice in the Description page of Project Settings.

#include "TankVsZombie.h"
#include "BFProjectileImpactEffect.h"


 ABFProjectileImpactEffect::ABFProjectileImpactEffect(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	PrimaryActorTick.bCanEverTick = true;

}

 void ABFProjectileImpactEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

 void ABFProjectileImpactEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

}

void  ABFProjectileImpactEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

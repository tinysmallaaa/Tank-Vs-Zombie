// Fill out your copyright notice in the Description page of Project Settings.

#include "TankVsZombie.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, TankVsZombie, "TankVsZombie" );

 

FCollisionResponseParams WorldResponseParams = []()
{
	FCollisionResponseParams Result(ECR_Ignore);
	Result.CollisionResponse.WorldStatic = ECR_Block;
	Result.CollisionResponse.WorldDynamic = ECR_Block;
	return Result;
}();


#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleModuleRequired.h"
#include "Particles/ParticleLODLevel.h"

bool IsLoopingParticleSystem(const UParticleSystem* PSys)
{
	for (int32 i = 0; i < PSys->Emitters.Num(); i++)
	{
		if (PSys->Emitters[i]->GetLODLevel(0)->RequiredModule->EmitterLoops <= 0 && PSys->Emitters[i]->GetLODLevel(0)->RequiredModule->bEnabled)
		{
			return true;
		}
	}
	return false;
}

void UnregisterComponentTree(USceneComponent* Comp)
{
	if (Comp != NULL)
	{
		TArray<USceneComponent*> Children;
		Comp->GetChildrenComponents(true, Children);
		Comp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Comp->UnregisterComponent();
		for (USceneComponent* Child : Children)
		{
			Child->UnregisterComponent();
		}
	}
}

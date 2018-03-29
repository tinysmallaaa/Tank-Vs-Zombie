// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h" 
#include "Materials/Material.h"
#include "ParticleDefinitions.h"
#include "SoundDefinitions.h" 
#include "Net/UnrealNetwork.h" 
#include "Particles/ParticleSystemComponent.h"
#include "BFTypes.h"

#define TOVEC2D(v) FVector2D(v.X,v.Y)

class UMaterial;
UENUM(BlueprintType)
enum EBFPhysMaterialType
{
	BF_Default,
	BF_Concrete,
	BF_Dirt,
	BF_Water,
	BF_Metal,
	BF_Wood,
	BF_Grass,
	BF_Glass,
	BF_Flesh,
};

#define COLLISION_PROJECTILE			ECC_GameTraceChannel1
#define COLLISION_TRACE_WEAPON			ECC_GameTraceChannel2
#define COLLISION_PROJECTILE_SHOOTABLE	ECC_GameTraceChannel3

#define BATTLE_SURFACE_Default		SurfaceType_Default
#define BATTLE_SURFACE_Concrete		SurfaceType1
#define BATTLE_SURFACE_Dirt			SurfaceType2
#define BATTLE_SURFACE_Water		SurfaceType3
#define BATTLE_SURFACE_Metal		SurfaceType4
#define BATTLE_SURFACE_Wood			SurfaceType5
#define BATTLE_SURFACE_Grass		SurfaceType6
#define BATTLE_SURFACE_Glass		SurfaceType7
#define BATTLE_SURFACE_Flesh		SurfaceType8 

TANKVSZOMBIE_API DECLARE_LOG_CATEGORY_EXTERN( BFLOG , Log , All );

/** handy response params for world-only checks */
extern FCollisionResponseParams WorldResponseParams ;

/** utility to find out if a particle system loops */
extern bool IsLoopingParticleSystem( const UParticleSystem* PSys );

/** utility to detach and unregister a component and all its children */
extern void UnregisterComponentTree( USceneComponent* Comp );


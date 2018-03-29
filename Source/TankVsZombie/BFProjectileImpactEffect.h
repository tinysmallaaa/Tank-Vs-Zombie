// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BFImpactEffect.h"
#include "BFProjectileImpactEffect.generated.h"

UCLASS(Blueprintable)
class TANKVSZOMBIE_API ABFProjectileImpactEffect : public ABFImpactEffect
{
	GENERATED_UCLASS_BODY()
public:
 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	
	virtual void Tick(float DeltaTime) override; 
 
	virtual void PostInitializeComponents() override;


};

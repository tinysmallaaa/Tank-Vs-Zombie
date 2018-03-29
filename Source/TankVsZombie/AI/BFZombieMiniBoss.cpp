#include "TankVsZombie.h"
#include "BFWorldSettings.h"
#include "BFZombieMiniBoss.h"


ABFZombieMiniBoss::ABFZombieMiniBoss(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bAllowHeadShot = false;
	HeadShotDamageMultiplier = 2.0f;
}

void ABFZombieMiniBoss::BeginPlay()
{

	Super::BeginPlay();

	for (int32 i = 0; i < BloodDecals.Num(); i++)
	{
		FBFBloodDecalInfo& DecalInfo = BloodDecals[i];
		if (DecalInfo.Material != NULL)
		{
			DecalInfo.DynamicInstance = UMaterialInstanceDynamic::Create(DecalInfo.Material, this);
		}
	}

}

void  ABFZombieMiniBoss::SpawnHeavyDamageDacals( float Radius , int32 Count )
{	

	ABFWorldSettings* Settings = Cast<ABFWorldSettings>(GetWorldSettings());

	if (Settings != NULL)
	{
		if (BloodDecals.Num() > 0)
		{
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			TArray<UPrimitiveComponent*> ChildComponents;
			this->GetComponents(ChildComponents);


			Params.AddIgnoredComponents(ChildComponents);

			for (int i = 0; i < Count; i++)
			{
				const FBFBloodDecalInfo& DecalInfo = BloodDecals[FMath::RandHelper(BloodDecals.Num())];
				DecalInfo.DynamicInstance->SetScalarParameterValue("Frame", FMath::FRandRange(0.0f, DecalInfo.MaxFrames - 1));

				FVector TraceStart = GetActorLocation() + FMath::RandPointInBox(FBox(FVector(-Radius, -Radius, 0.0f), FVector(Radius, Radius, 0.0f)));
				FVector TraceEnd = TraceStart + FVector(0.0f, 0.0f, -1.0f) * 10000.0f;
				if (DecalInfo.Material != NULL)
				{
					FHitResult Hit;
					
					

					if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params ))
					{
						static FName NAME_BloodDecal(TEXT("BloodDecal"));
					 
						if (Hit.Component->bReceivesDecals)
						{
							UDecalComponent* Decal = NewObject<UDecalComponent>(GetWorld());

							if (Hit.Component.Get() != NULL && Hit.Component->Mobility == EComponentMobility::Movable)
							{
								Decal->SetAbsolute(false, false, true);
								Decal->AttachToComponent(Hit.Component.Get(), FAttachmentTransformRules::KeepRelativeTransform);
							}
							else
							{
								Decal->SetAbsolute(true, true, true);
							}

							FVector2D DecalScale = DecalInfo.BaseScale * FMath::FRandRange(DecalInfo.ScaleMultRange.X, DecalInfo.ScaleMultRange.Y);

							Decal->DecalSize = FVector(50.0f, DecalScale.X, DecalScale.Y);
							Decal->SetWorldLocation(Hit.Location);
							Decal->SetWorldRotation((-Hit.Normal).Rotation() + FRotator(0.0f, 0.0f, 360.0f * FMath::FRand()));
							Decal->SetDecalMaterial(DecalInfo.Material);
							Decal->RegisterComponentWithWorld(GetWorld());

							Settings->AddImpactEffect(Decal);
						}
					}
				}
			}
		}
	}

}

void ABFZombieMiniBoss::PlayReactAnim()
{

	if (ReactAnimMontages.Num() > 0)
	{
		int32 Attempts = ReactAnimMontages.Num();
		UAnimMontage* Montage = NULL;
		while (Attempts >= 0)
		{
			Montage = ReactAnimMontages[FMath::RandRange(0, ReactAnimMontages.Num() - 1)];

			if (Montage != NULL)
				break;

			Attempts--;
		}
		if (Montage != NULL && GetCurrentMontage() != Montage)
		{
			PlayAnimMontage(Montage);

			bAttackWasCanceled = true;

			SpawnHeavyDamageDacals(100.0f , 2 );
		}
	}

}

void ABFZombieMiniBoss::CancelAttackAttempt()
{

}
float ABFZombieMiniBoss::ModifyDamageTaken(float Damage, FBotDamageParamEx DamageParams)
{

	float ActualDamage = Damage;
	if (DamageParams.bHeadShot)
	{
		ActualDamage *= 1.0f / HeadShotDamageMultiplier;
	}

	return ActualDamage;

}

float ABFZombieMiniBoss::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	float DamageTaken = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (DamageTaken > 0.0f)
	{
		UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();

		if (Cast<UBFDamageType_Shell>(DamageTypeCDO) != NULL)
		{
			PlayReactAnim();
		}
	}

	return DamageTaken;

}

void ABFZombieMiniBoss::DamageReceived()
{

	Super::DamageReceived();
	bDrawHealthBar = true;

}
 
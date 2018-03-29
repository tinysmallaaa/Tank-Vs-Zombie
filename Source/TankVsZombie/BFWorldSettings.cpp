#include "TankVsZombie.h"
#include "BFLocalPlayer.h"
#include "BFWorldSettings.h"

ABFWorldSettings::ABFWorldSettings(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	MaxImpactEffectVisibleLifetime = 5.0f;
	MaxImpactEffectInvisibleLifetime = 1.0f;
	ImpactEffectFadeTime = 1.0f;
	ImpactEffectFadeSpeed = 1.0f;
}

void ABFWorldSettings::LevelActorDestroyed(AActor* TheActor)
{
	new(DestroyedLevelActors) FDestroyedActorInfo(TheActor);
}

 static FName NAME_LevelSummary(TEXT("LevelSummary"));
void ABFWorldSettings::PostLoad()
{
	Super::PostLoad();
	CreateLevelSummary();
}

void ABFWorldSettings::PostDuplicate(bool bDuplicateForPIE)
{
	Super::PostDuplicate(bDuplicateForPIE);

	if (LevelSummary != nullptr && !LevelSummary->IsIn(GetOutermost()))
	{
		LevelSummary = DuplicateObject<UBFLevelSummary>(LevelSummary, GetOutermost(), NAME_LevelSummary);
	}

}

void ABFWorldSettings::PostInitProperties()
{
	Super::PostInitProperties();
	if (!HasAnyFlags(RF_NeedLoad))
	{
		CreateLevelSummary();
	}
}

void ABFWorldSettings::PreSave(const class ITargetPlatform* TargetPlatform)
{
	Super::PreSave(TargetPlatform);
	if (LevelSummary == nullptr || !LevelSummary->IsIn(GetOutermost()))
	{
		LevelSummary = nullptr;
		CreateLevelSummary();
	}
}

void ABFWorldSettings::CreateLevelSummary()
{
	if (!IsTemplate())
	{
		if (LevelSummary == NULL)
		{
			LevelSummary = FindObject<UBFLevelSummary>(GetOutermost(), *NAME_LevelSummary.ToString());
			if (LevelSummary == NULL)
			{
				LevelSummary = NewObject<UBFLevelSummary>(GetOutermost(), NAME_LevelSummary, RF_Standalone);
			}
		}
		else if (LevelSummary->GetFName() != NAME_LevelSummary)
		{
 			LevelSummary = DuplicateObject<UBFLevelSummary>(LevelSummary, GetOutermost(), *NAME_LevelSummary.ToString());
		}
	}
}

void ABFWorldSettings::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	 
	FadeImpactEffects(DeltaTime);

	for (int32 i = 0; i < MaterialParamCurves.Num(); i++)
	{
		if (!MaterialParamCurves[i].MI.IsValid() || MaterialParamCurves[i].ParamCurve == NULL)
		{
			MaterialParamCurves.RemoveAt(i--, 1);
		}
		else
		{
			MaterialParamCurves[i].ElapsedTime += DeltaTime;
			bool bAtEnd = false;
			{
				float MinTime, MaxTime;
				MaterialParamCurves[i].ParamCurve->GetTimeRange(MinTime, MaxTime);
				bAtEnd = MaterialParamCurves[i].ElapsedTime > MaxTime;
			}
			UCurveFloat* FloatCurve = Cast<UCurveFloat>(MaterialParamCurves[i].ParamCurve);
			if (FloatCurve != NULL)
			{
				if (bAtEnd && MaterialParamCurves[i].bClearOnComplete && MaterialParamCurves[i].MI->Parent != NULL)
				{
 					float ParentValue = 0.0f;
					MaterialParamCurves[i].MI->Parent->GetScalarParameterValue(MaterialParamCurves[i].ParamName, ParentValue);
					MaterialParamCurves[i].MI->SetScalarParameterValue(MaterialParamCurves[i].ParamName, ParentValue);
				}
				else
				{
					MaterialParamCurves[i].MI->SetScalarParameterValue(MaterialParamCurves[i].ParamName, FloatCurve->GetFloatValue(MaterialParamCurves[i].ElapsedTime));
				}
			}
			else
			{
				UCurveLinearColor* ColorCurve = Cast<UCurveLinearColor>(MaterialParamCurves[i].ParamCurve);
				if (ColorCurve != NULL)
				{
					if (bAtEnd && MaterialParamCurves[i].bClearOnComplete && MaterialParamCurves[i].MI->Parent != NULL)
					{
						// there's no clear single parameter in UMaterialInstance...
						FLinearColor ParentValue = FLinearColor::Black;
						MaterialParamCurves[i].MI->Parent->GetVectorParameterValue(MaterialParamCurves[i].ParamName, ParentValue);
						MaterialParamCurves[i].MI->SetVectorParameterValue(MaterialParamCurves[i].ParamName, ParentValue);
					}
					else
					{
						MaterialParamCurves[i].MI->SetVectorParameterValue(MaterialParamCurves[i].ParamName, ColorCurve->GetLinearColorValue(MaterialParamCurves[i].ElapsedTime));
					}
				}
			}
			if (bAtEnd)
			{
				MaterialParamCurves.RemoveAt(i--, 1);
			}
		}
	}
	for (int32 i = 0; i < LightParamCurves.Num(); i++)
	{
		if (!LightParamCurves[i].Light.IsValid() || LightParamCurves[i].ParamCurve == NULL)
		{
			LightParamCurves.RemoveAt(i--, 1);
		}
		else
		{
			LightParamCurves[i].ElapsedTime += DeltaTime;
			bool bAtEnd = false;
			{
				float MinTime, MaxTime;
				LightParamCurves[i].ParamCurve->GetTimeRange(MinTime, MaxTime);
				bAtEnd = LightParamCurves[i].ElapsedTime > MaxTime;
			}
			switch (LightParamCurves[i].Param)
			{
			case TLP_Color:
			{
				UCurveLinearColor* ColorCurve = Cast<UCurveLinearColor>(LightParamCurves[i].ParamCurve);
				if (ColorCurve != NULL)
				{
					LightParamCurves[i].Light->SetLightColor(ColorCurve->GetLinearColorValue(LightParamCurves[i].ElapsedTime));
				}
				break;
			}
			case TLP_Intensity:
			{
				UCurveFloat* FloatCurve = Cast<UCurveFloat>(LightParamCurves[i].ParamCurve);
				if (FloatCurve != NULL)
				{
					LightParamCurves[i].Light->SetIntensity(FloatCurve->GetFloatValue(LightParamCurves[i].ElapsedTime));
				}
				break;
			}
			default:
				break;
			}
			if (bAtEnd)
			{
 				if (LightParamCurves[i].Light->Intensity <= 0.0f)
				{
					for (int32 j = 0; j < TimedEffects.Num(); j++)
					{
						if (TimedEffects[j].EffectComp == LightParamCurves[i].Light)
						{
							TimedEffects[j].EffectComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
							TimedEffects[j].EffectComp->DestroyComponent();
							TimedEffects.RemoveAt(j--);
							break;
						}
					}
				}

				LightParamCurves.RemoveAt(i--, 1);
			}
		}
	}

}

void ABFWorldSettings::NotifyBeginPlay()
{
	UWorld* World = GetWorld();
	if (!World->bBegunPlay)
	{
		World->bBegunPlay = true;
 		TArray<AActor*> FullActorList;
		TArray<AActor*> LevelActorList;
		FullActorList.Reserve(World->PersistentLevel->Actors.Num());
		for (FActorIterator It(World); It; ++It)
		{
 			if (It->IsA(ALevelScriptActor::StaticClass()))
			{
				LevelActorList.Add(*It);
			}
			else
			{
				FullActorList.Add(*It);
			}
		}
		FullActorList.Append(LevelActorList);
		for (AActor* Actor : FullActorList)
		{
 			if (Actor->bNetStartup)
			{
 				Actor->OnDestroyed.AddDynamic(this, &ABFWorldSettings::LevelActorDestroyed);
			}
			Actor->DispatchBeginPlay();
		}
	}
}

void ABFWorldSettings::BeginPlay()
{
	if (Music != NULL && GetNetMode() != NM_DedicatedServer)
	{
		bool bPlayMusic = true;

		UBFLocalPlayer* LP = Cast<UBFLocalPlayer>(GEngine->GetFirstGamePlayer(GetWorld()));
	//	if (LP && LP->GetKillcamPlaybackManager() && LP->GetKillcamPlaybackManager()->GetKillcamWorld() == GetWorld())
		{
		//	bPlayMusic = false;
		}

		if (bPlayMusic)
		{
			MusicComp = NewObject<UAudioComponent>(this);
			MusicComp->bAllowSpatialization = false;
			MusicComp->bShouldRemainActiveIfDropped = true;
			MusicComp->SetSound(Music);
			MusicComp->Play();
		}
	}

	Super::BeginPlay();

	if (!IsPendingKillPending())
	{
		FTimerHandle TempHandle;
		GetWorldTimerManager().SetTimer(TempHandle, this, &ABFWorldSettings::ExpireImpactEffects, 0.5f, true);
	}
}

void ABFWorldSettings::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void ABFWorldSettings::Reset()
{
	for (int32 i = 0; i < FadingEffects.Num(); i++)
	{
		if (FadingEffects[i].EffectComp != nullptr)
		{
			FadingEffects[i].EffectComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
			FadingEffects[i].EffectComp->DestroyComponent();
		}
	}
	FadingEffects.Empty();
}

void ABFWorldSettings::AddImpactEffect(USceneComponent* NewEffect, float LifeScaling)
{
	bool bNeedsTiming = true;

 	UParticleSystemComponent* PSC = Cast <UParticleSystemComponent>(NewEffect);
	if (PSC != NULL)
	{
		if (PSC->bAutoDestroy && PSC->Template != NULL && !IsLoopingParticleSystem(PSC->Template))
		{
			bNeedsTiming = false;
		}
	}
	else
	{
		UAudioComponent* AC = Cast<UAudioComponent>(NewEffect);
		if (AC != NULL)
		{
			if (AC->bAutoDestroy && AC->Sound != NULL && AC->Sound->GetDuration() < INDEFINITELY_LOOPING_DURATION)
			{
				bNeedsTiming = false;
			}
		}
	}

	if (bNeedsTiming)
	{
 		new(TimedEffects)FTimedImpactEffect(NewEffect, GetWorld()->TimeSeconds, LifeScaling);
	}
}

void ABFWorldSettings::FadeImpactEffects(float DeltaTime)
{
	float WorldTime = GetWorld()->TimeSeconds;

	for (int32 i = 0; i < FadingEffects.Num(); i++)
	{
		float LastRenderTime = WorldTime;
		UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(FadingEffects[i].EffectComp);
		if (Prim != NULL)
		{
			LastRenderTime = Prim->LastRenderTime;
		}

		float DesiredTimeout = (WorldTime - LastRenderTime < 1.0f) ? MaxImpactEffectVisibleLifetime : MaxImpactEffectInvisibleLifetime;
		DesiredTimeout *= FadingEffects[i].LifetimeScaling;

		float TimeLived = WorldTime - FadingEffects[i].CreationTime;

		UDecalComponent* Decal = Cast<UDecalComponent>(FadingEffects[i].EffectComp);
		if (Decal)
		{
			Decal->FadeScreenSize += ImpactEffectFadeSpeed * DeltaTime * FadingEffects[i].FadeMultipllier;
		}

		if (TimeLived > DesiredTimeout)
		{
			FadingEffects[i].EffectComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			FadingEffects[i].EffectComp->DestroyComponent();
			FadingEffects.RemoveAt(i--);
		}
	}
}

void ABFWorldSettings::ExpireImpactEffects()
{
	float WorldTime = GetWorld()->TimeSeconds;
	 
	if (MaxImpactEffectInvisibleLifetime > 0.0f)
	{
		int32 NumToKill = TimedEffects.Num() - FMath::TruncToInt(MaxImpactEffectInvisibleLifetime * 3.0f);
		if (NumToKill > 0)
		{
			for (int32 i = NumToKill - 1; i >= 0; i--)
			{
				if (TimedEffects[i].EffectComp != NULL && TimedEffects[i].EffectComp->IsRegistered())
				{
					TimedEffects[i].CreationTime = WorldTime - MaxImpactEffectVisibleLifetime * TimedEffects[i].LifetimeScaling + ImpactEffectFadeTime;
					UDecalComponent* Decal = Cast<UDecalComponent>(TimedEffects[i].EffectComp);
					if (Decal)
					{
						TimedEffects[i].FadeMultipllier = Decal->FadeScreenSize / 0.01f;
					}
					FadingEffects.Add(TimedEffects[i]);
				}
			}
			TimedEffects.RemoveAt(0, NumToKill);
		}
	}
	for (int32 i = 0; i < TimedEffects.Num(); i++)
	{
		if (TimedEffects[i].EffectComp == NULL || !TimedEffects[i].EffectComp->IsRegistered())
		{
			TimedEffects.RemoveAt(i--);
		}
		else
		{
		 
			float LastRenderTime = WorldTime;
			UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(TimedEffects[i].EffectComp);
			if (Prim != NULL)
			{
				LastRenderTime = Prim->LastRenderTime;
			}
			float DesiredTimeout = (WorldTime - LastRenderTime < 1.0f) ? MaxImpactEffectVisibleLifetime : MaxImpactEffectInvisibleLifetime ;
			DesiredTimeout *= TimedEffects[i].LifetimeScaling ;
			if (DesiredTimeout > 0.0f)
			{
				float TimeLived = WorldTime - TimedEffects[i].CreationTime;
				if (TimeLived > DesiredTimeout)
				{
					TimedEffects[i].EffectComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
					TimedEffects[i].EffectComp->DestroyComponent();
					TimedEffects.RemoveAt(i--);
				}
				else if (TimeLived > DesiredTimeout - ImpactEffectFadeTime)
				{
					UDecalComponent* Decal = Cast<UDecalComponent>(TimedEffects[i].EffectComp);
					if (Decal)
					{
						TimedEffects[i].FadeMultipllier = Decal->FadeScreenSize / 0.01f;
					}

					FadingEffects.Add(TimedEffects[i]);
					TimedEffects.RemoveAt(i--);
				}
			}
		}
	}
}

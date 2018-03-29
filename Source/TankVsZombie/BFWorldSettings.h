#pragma once
#include "BFLevelSummery.h"
#include "BFWorldSettings.Generated.h"
 
USTRUCT(BlueprintType)
struct FTimedImpactEffect
{
	GENERATED_USTRUCT_BODY()

 	UPROPERTY()
		USceneComponent* EffectComp;
 	UPROPERTY()
		float CreationTime;
 	UPROPERTY()
		float LifetimeScaling;
	UPROPERTY()
		float FadeMultipllier;

	FTimedImpactEffect()
	{}
	FTimedImpactEffect(USceneComponent* InComp, float InTime, float InScaling)
		: EffectComp(InComp), CreationTime(InTime), LifetimeScaling(InScaling), FadeMultipllier(1.0f)
	{}
};

 USTRUCT(BlueprintType)
struct FTimedMaterialParameter
{
	GENERATED_USTRUCT_BODY()

 		UPROPERTY()
		TWeakObjectPtr<class UMaterialInstanceDynamic> MI;
 	UPROPERTY()
		FName ParamName;
 	UPROPERTY()
		UCurveBase* ParamCurve;
 	UPROPERTY()
		float ElapsedTime;
 	UPROPERTY()
		bool bClearOnComplete;

	FTimedMaterialParameter()
		: MI(NULL)
	{}
	FTimedMaterialParameter(UMaterialInstanceDynamic* InMI, FName InParamName, UCurveBase* InCurve, bool bInClearOnComplete = true)
		: MI(InMI), ParamName(InParamName), ParamCurve(InCurve), ElapsedTime(0.0f), bClearOnComplete(bInClearOnComplete)
	{}
};

UENUM()
enum ETimedLightParameter
{
	TLP_Intensity,
	TLP_Color,
};

 USTRUCT(BlueprintType)
struct FTimedLightParameter
{
	GENERATED_USTRUCT_BODY()

 		UPROPERTY()
		TWeakObjectPtr<class ULightComponent> Light;
 	UPROPERTY()
		TEnumAsByte<ETimedLightParameter> Param;
 	UPROPERTY()
		UCurveBase* ParamCurve;
 	UPROPERTY()
		float ElapsedTime;

	FTimedLightParameter()
		: Light(NULL)
	{}
	FTimedLightParameter(ULightComponent* InLight, ETimedLightParameter InParam, UCurveBase* InCurve)
		: Light(InLight), Param(InParam), ParamCurve(InCurve), ElapsedTime(0.0f)
	{}
};



UCLASS(Blueprintable)
class TANKVSZOMBIE_API ABFWorldSettings : public AWorldSettings
{
	GENERATED_UCLASS_BODY()
public: 

	UPROPERTY(globalconfig)
		float MaxImpactEffectVisibleLifetime;
 
	UPROPERTY(globalconfig)
		float MaxImpactEffectInvisibleLifetime;

 	UPROPERTY()
		TArray<FTimedImpactEffect> TimedEffects;

	UPROPERTY()
		TArray<FTimedImpactEffect> FadingEffects;
 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LevelSettings)
		USoundBase* Music;

	UPROPERTY()
		UAudioComponent* MusicComp;
 
 	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Instanced, Category = LevelSummary)
	class UBFLevelSummary* LevelSummary;

	UPROPERTY()
		float ImpactEffectFadeTime;

	UPROPERTY()
		float ImpactEffectFadeSpeed;

	UPROPERTY()
		TArray<FTimedMaterialParameter> MaterialParamCurves;

	UPROPERTY()
		TArray<FTimedLightParameter> LightParamCurves;

	struct FDestroyedActorInfo
	{
		TWeakObjectPtr<ULevel> Level;
		FName ActorName;
		FString FullName;
		FDestroyedActorInfo(AActor* InActor)
			: Level(InActor->GetLevel()), ActorName(InActor->GetFName()), FullName(InActor->GetFullName())
		{

		}
	};
	 
protected:
 
	TArray<FDestroyedActorInfo> DestroyedLevelActors;

	UFUNCTION()
		void LevelActorDestroyed(AActor* TheActor);
public:
	inline const TArray<FDestroyedActorInfo>& GetDestroyedLevelActors() const
	{
		return DestroyedLevelActors;
	}  
	virtual void CreateLevelSummary(); 
	virtual void PostLoad() override;
	virtual void PostDuplicate(bool bDuplicateForPIE) override;
	virtual void PostInitProperties() override;
	virtual void PreSave(const class ITargetPlatform* TargetPlatform) override;

public:

	inline const UBFLevelSummary* GetLevelSummary() const
	{ 
		return LevelSummary; 
	}

	virtual void NotifyBeginPlay() override; 
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 
 	virtual void AddImpactEffect(class USceneComponent* NewEffect, float LifeScaling = 1.f); 
 	virtual void ExpireImpactEffects(); 
	virtual void FadeImpactEffects(float DeltaTime); 
	virtual void Reset() override; 
	virtual void Tick(float DeltaSeconds) override; 

};
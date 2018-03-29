#pragma once
#include "BFGibSkeletalMesh.Generated.h"
USTRUCT(BlueprintType)
struct FBFBloodDecalInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BloodDecalInfo)
		UMaterialInterface* Material ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BloodDecalInfo)
		FVector2D BaseScale ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BloodDecalInfo)
		FVector2D ScaleMultRange ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BloodDecalInfo)
		float MaxFrames	;
	UPROPERTY()
		UMaterialInstanceDynamic* DynamicInstance ;

	FBFBloodDecalInfo()
		: Material(NULL), 
		BaseScale(32.0f, 32.0f), 
		ScaleMultRange(0.8f, 1.2f),
		MaxFrames(3)
	{

	}
};


UCLASS(Abstract)
class TANKVSZOMBIE_API ABFGibSkeletalMesh : public AActor
{
	GENERATED_UCLASS_BODY()

 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gib )
		USkeletalMeshComponent* Mesh;
 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gib )
		float InvisibleLifeSpan;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gib)
		TArray<FName> BloodEffectSocketNames;

 	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = Gib )
		TArray<UParticleSystem*> BloodEffects;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gib )
		TArray<FBFBloodDecalInfo> BloodDecals;
 	UPROPERTY(BlueprintReadWrite, Category = Gib )
		float LastBloodTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gib )
		float BloodTrailLifeSpanMin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gib )
		float BloodTrailLifeSpanMax;
	UPROPERTY(BlueprintReadWrite, Category = Gib )
		float FadeOutAlpha;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gib )
		TArray<USkeletalMesh*> MeshChoices;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gib )
		bool bUseLocalRadialForce;
	UPROPERTY( EditDefaultsOnly ,	Category = Gib )
		float MomentumMultiplier;

	UPROPERTY( EditDefaultsOnly ,	Category = Gib )
		float LinearVelocityMultiplier;
	UPROPERTY( EditDefaultsOnly ,	Category = Gib )
		float AngularVelocityMultiplier;

	UFUNCTION( BlueprintCallable , Category = Gib )
		void Explode( FVector Origin , FVector Momentum );
	void SetMaterial( int32 ElemIndex , UMaterialInterface* Material );
	void TriggerFadeOut();


	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
 	virtual void CheckGibVisibility();

	inline USkeletalMeshComponent* GetMesh() {return Mesh;}	
	void TraceResult(const FTraceHandle& TraceHandle, FTraceDatum& TraceData);
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION()
		virtual void OnPhysicsCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:

	FTraceHandle			  DecalTraceHandle	;
	bool					  bExploded			;
	float					  FadeStartTimer	;
	bool					  bFadeTriggered	;
	FBFBloodDecalInfo		  BloodDecalInfo	;
	UMaterialInstanceDynamic* MaterialDynamic	;

};


#pragma once
#include "BFTypes.h"
#include "BFImpactEffect.Generated.h"


USTRUCT(Blueprintable)
struct FBFImpactVisualFX
{
	GENERATED_USTRUCT_BODY()
		FBFImpactVisualFX()
		:WorldScale(1.0f)
	{

	}
	UPROPERTY(EditDefaultsOnly, Category = Visual)
		UParticleSystem* ParticleFX ;
	UPROPERTY(EditDefaultsOnly, Category = Visual)
		float			 WorldScale  ;
	UPROPERTY(EditDefaultsOnly, Category = Visual)
		USoundCue*		 SoundFX ; 
};

UCLASS(Blueprintable)
class TANKVSZOMBIE_API ABFImpactEffect : public AActor
{
	GENERATED_UCLASS_BODY()
public:
	/** default impact FX used when material specific override doesn't exist */
	UPROPERTY( EditDefaultsOnly, Category = Defaults )
		FBFImpactVisualFX DefaultFX;
	UPROPERTY(EditDefaultsOnly, Category = Visual)
		float UniformScaleOverride;

	/** impact FX on concrete */
	UPROPERTY( EditDefaultsOnly, Category = Visual)
		FBFImpactVisualFX ConcreteFX;

	/** impact FX on dirt */
	UPROPERTY( EditDefaultsOnly, Category = Visual )
		FBFImpactVisualFX DirtFX;

	/** impact FX on water */
	UPROPERTY( EditDefaultsOnly, Category = Visual )
		FBFImpactVisualFX WaterFX;

	/** impact FX on metal */
	UPROPERTY( EditDefaultsOnly, Category = Visual )
		FBFImpactVisualFX MetalFX;

	/** impact FX on wood */
	UPROPERTY( EditDefaultsOnly, Category = Visual )
		FBFImpactVisualFX WoodFX;

	/** impact FX on glass */
	UPROPERTY( EditDefaultsOnly, Category = Visual )
		FBFImpactVisualFX GlassFX;

	/** impact FX on grass */
	UPROPERTY( EditDefaultsOnly, Category = Visual )
		FBFImpactVisualFX GrassFX;

	/** impact FX on flesh */
	UPROPERTY( EditDefaultsOnly, Category = Visual )
		FBFImpactVisualFX FleshFX;

	/** default decal when material specific override doesn't exist */
	UPROPERTY( EditDefaultsOnly, Category = Defaults )
		FDecalData DefaultDecal;

	/** surface data for spawning */
	UPROPERTY( BlueprintReadWrite , Category = Surface )
		FHitResult SurfaceHit;

	/** spawn effect */
	virtual void PostInitializeComponents() override; 

	/** get FX for material type */
	const FBFImpactVisualFX& GetImpactFX( TEnumAsByte<EPhysicalSurface> SurfaceType ) const;
 
};
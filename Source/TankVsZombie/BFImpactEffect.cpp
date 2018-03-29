#include "TankVsZombie.h"
#include "BFImpactEffect.h"

ABFImpactEffect::ABFImpactEffect(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	bAutoDestroyWhenFinished = true;
	UniformScaleOverride = 1.0f;
}

void ABFImpactEffect::PostInitializeComponents()
{

	Super::PostInitializeComponents();
	
	UPhysicalMaterial* HitPhysMat	= SurfaceHit.PhysMaterial.Get();
	EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitPhysMat);
	
	FRotator Orientation = SurfaceHit.ImpactNormal.ToOrientationRotator();
	Orientation.Pitch -= 90;

	SetActorRotation(Orientation);
	// show particles
	const  FBFImpactVisualFX&  ImpactFX = GetImpactFX(HitSurfaceType) ;
	if ( ImpactFX.ParticleFX != NULL )
	{  
		UParticleSystemComponent* ParticleFX =  UGameplayStatics::SpawnEmitterAtLocation( this , ImpactFX.ParticleFX , this->GetActorLocation() , Orientation, true );
		ParticleFX->SetWorldScale3D( FVector( UniformScaleOverride*ImpactFX.WorldScale ) ); 
	}
	else if (DefaultFX.ParticleFX != NULL)
	{ 
		UParticleSystemComponent* ParticleFX = UGameplayStatics::SpawnEmitterAtLocation(this, DefaultFX.ParticleFX, this->GetActorLocation(), Orientation , true);
		ParticleFX->SetWorldScale3D(FVector(UniformScaleOverride*DefaultFX.WorldScale));
	}
	 
	if ( ImpactFX.SoundFX != NULL )
	{
		UGameplayStatics::PlaySoundAtLocation( this , ImpactFX.SoundFX , this->GetActorLocation() );
	}
	else if ( DefaultFX.SoundFX != NULL )
	{
		UGameplayStatics::PlaySoundAtLocation(this, DefaultFX.SoundFX, this->GetActorLocation());
	}

	if ( DefaultDecal.DecalMaterial )
	{ 
		 
		FRotator RandomDecalRotation = SurfaceHit.ImpactNormal.Rotation();
		RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);

		UGameplayStatics::SpawnDecalAttached( DefaultDecal.DecalMaterial , FVector( DefaultDecal.DecalSize , DefaultDecal.DecalSize , 1.0f ),
			SurfaceHit.Component.Get() , SurfaceHit.BoneName,SurfaceHit.ImpactPoint ,
			RandomDecalRotation , EAttachLocation::KeepWorldPosition , DefaultDecal.LifeSpan );  

	}

	SetActorRotation(Orientation);

}

const  FBFImpactVisualFX& ABFImpactEffect::GetImpactFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{ 
	switch ( SurfaceType )
	{
	case BATTLE_SURFACE_Concrete:	return  ConcreteFX; break;
	case BATTLE_SURFACE_Dirt:		return  DirtFX; break;
	case BATTLE_SURFACE_Water:		return  WaterFX; break;
	case BATTLE_SURFACE_Metal:		return  MetalFX; break;
	case BATTLE_SURFACE_Wood:		return  WoodFX; break;
	case BATTLE_SURFACE_Grass:		return  GrassFX; break;
	case BATTLE_SURFACE_Glass:		return  GlassFX; break;
	case BATTLE_SURFACE_Flesh:		return  FleshFX; break; 
	}

	return DefaultFX;
} 
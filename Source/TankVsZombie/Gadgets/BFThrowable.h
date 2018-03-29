#pragma once
#include "BFPlaceableGadgetInterface.h"
#include "BFThrowable.Generated.h"

UCLASS(Blueprintable)
class ABFThrowable : public AActor , public IBFPlaceableGadgetInterface
{
	GENERATED_UCLASS_BODY()
public: 

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite, Category = "Throwable")
		TSubclassOf<UDamageType>	DamageType;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite, Category = "Throwable")
		float						Momentum;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite, Category = "Throwable")
		FRadialDamageParams			DamageParams;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite, Category = "Throwable")
		bool						bExplodeOnPlacement;
	UPROPERTY(EditDefaultsOnly, Category = "Throwable")
		float Height; 

	virtual void AdjustOnDrag( FVector NewLocation , FVector ImpactNormal ,bool DragOverEvent ) override; 
	virtual void ReleaseOnLocation( FVector Location ) override;

	UFUNCTION( BlueprintImplementableEvent , Category = "Throwable" )
		void OnPlaced( FVector NewLocation );

	UFUNCTION(  BlueprintNativeEvent , BlueprintCallable  , Category= "Throwable" )
		void Explode( const FHitResult& HitInfo , const FVector& Location , UPrimitiveComponent* HitComp );

	UFUNCTION( BlueprintImplementableEvent , Category = "Throwable" )
		void OnExploded();
	UFUNCTION( BlueprintCallable , Category="Throwable" )
	void ShutDown();

	bool HurtRadius( UObject* WorldContextObject , float BaseDamage , float MinimumDamage , float BaseMomentumMag , const FVector& Origin ,  float DamageInnerRadius , float DamageOuterRadius , float DamageFalloff ,
		TSubclassOf<class UDamageType> DamageTypeClass , const TArray<AActor*>& IgnoreActors , AActor* DamageCauser , AController* InstigatedByController ,
		float CollisionFreeRadius , const TArray<FVector>* AltVisibilityOrigins );

	UFUNCTION( BlueprintCallable , Category = "Throwable" )
		float GetOuterRadius() ;
	virtual  void SetEnableCollision( bool flag ) override ;
	bool bInteractionStarted	;
	bool bInOverlap				;
	bool bExploded				;
};

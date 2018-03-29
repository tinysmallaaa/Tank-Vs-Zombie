#pragma once
#include "BFPlaceableGadgetInterface.h"
#include "BFGameSaveData.h"
#include "BFMiniTankBot.Generated.h"

class ABFMiniTankBotAIController;
UCLASS( Blueprintable , Abstract )
class ABFMiniTankBot : public ACharacter , public IBFPlaceableGadgetInterface 
{
	GENERATED_UCLASS_BODY()
public:  
	UPROPERTY( EditDefaultsOnly	, Category = "BFMiniTankBot" )
		TArray<FName>	 FireLocationSocketList;
	UPROPERTY(EditDefaultsOnly, Category = "Throwable")
		float			 Height; 
	UPROPERTY( Category = "BFMiniTankBot" , VisibleAnywhere , BlueprintReadOnly , meta = ( AllowPrivateAccess = "true" ) )
		UAudioComponent* FiringSoundComp ;

	virtual void AdjustOnDrag( FVector NewLocation , FVector ImpactNormal , bool DragOverEvent ) override ;
	virtual void ReleaseOnLocation(FVector Location) override;

	UPROPERTY()
		ABFMiniTankBotAIController* BotAIController ;

	UPROPERTY( EditDefaultsOnly , Category = "BFMiniTankBot" )
		float FireRate ;
	UPROPERTY( EditDefaultsOnly , Category = "BFMiniTankBot" )
		float DamagePower ;
	UPROPERTY(EditDefaultsOnly, Category = "BFMiniTankBot" )
		float DamageMultiplier ;
	UPROPERTY( BlueprintReadWrite , Category = "BFMiniTankBot" )
		float Health ;

	UPROPERTY( EditDefaultsOnly , Category = "BFMiniTankBot" )
		float HealthMax	;
	UPROPERTY( EditDefaultsOnly , Category = "BFMiniTankBot" )
		float FireOffset ;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = "BFMiniTankBot" )
		TSubclassOf<class ABFProjectile> ProjectileClass ;
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = "BFMiniTankBot" )
		UParticleSystem* InstantFireFX ;

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = "BFMiniTankBot" )
		TSubclassOf<class ABFImpactEffect>		InstantHitImpactFXClass ;
	UPROPERTY( BlueprintReadWrite , Category = "BFMiniTankBot" )
		bool bPlaced; 

	virtual void PossessedBy( AController* AIController ) override ; 
	virtual void UnPossessed() override ; 
	UFUNCTION(BlueprintCallable, Category = "BFMiniTankBot" )
		void StopFire() ; 
	UFUNCTION()
		virtual void Fire() ;


	virtual bool FireWeapon(const FVector Direction);
	virtual bool FireWeapon( AActor* Target , FVector TargetLocation );
	virtual void FireProjectile( const FVector FireLocation , const FVector Direction );
	virtual void FireInstantHit( const FVector FireLocation , const FVector Direction );

	virtual void BeginPlay()	override;
	virtual void Tick(float DeltaSeconds)	override;  
	virtual void PostInitializeComponents()	;
	UFUNCTION(BlueprintNativeEvent, Category = "BFMiniTankBot" )
		void StartFiringFX() ;

	UFUNCTION(BlueprintNativeEvent, Category = "BFMiniTankBot" )
		void StopFiringFX() ;
	UFUNCTION(BlueprintNativeEvent, Category = "BFMiniTankBot" )
		void PlayFiringSoundFX() ;
	virtual bool CanFire();	
	virtual UCameraComponent* GetAttachedCamera() { return NULL ; }

	UFUNCTION(BlueprintImplementableEvent, Category = "BFMiniTankBot")
		void SpawnInstantBeamFX(FVector StartLocation, FVector EndLocation);	
	virtual FVector CalculateFireLocation(FVector Direction) ;
	FVector CalculateFireDirection();
	FVector CalculateFireDirection(FVector FireLocation, FVector FireDirection);

	void	SetupBot(FSavedMiniTankBotState& State) ;
	UFUNCTION(BlueprintNativeEvent, Category = "BFMiniTankBot")
		FVector GetFireLocation();
	virtual void PostFireEffects() {}
	UFUNCTION(BlueprintImplementableEvent, Category = "Throwable")
		void OnPlaced(FVector NewLocation);

	virtual void SetEnableCollision(bool flag) override; 

 
private:

	int32 FireLocationItr;
	FTimerHandle NextFireTimer;
	FTimerHandle ClipReloadingTimer;
	float LastFireTime;
	bool  bFirePressed;
	float FireDelay;
	bool  bClipReloading;
	bool  bHasPendingReload;
};


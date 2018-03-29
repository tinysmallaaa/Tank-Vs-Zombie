#pragma once 
#include "DetourCrowdAIController.h"
#include "BFAIBot.h"
#include "BFTeamInterface.h"
#include "BFAIController.Generated.h"

class ABFEncounterVolume;

UCLASS( Blueprintable , Abstract )
class TANKVSZOMBIE_API ABFAIController : public AAIController , public IBFTeamInterface
{
	 GENERATED_UCLASS_BODY()
public:  

	UPROPERTY( BlueprintReadWrite , Category = "AIController")
		FBotAIParams AIParams			;

	UPROPERTY( BlueprintReadWrite , Category = "AIController")
		uint8 TeamIndex					;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "AIController" )
		bool ActivateAIOnSpawn			; 
	UPROPERTY(BlueprintReadWrite, Category = "AIController")
		AActor* CurrentEnemy	;

	UFUNCTION( BlueprintImplementableEvent , BlueprintCallable, Category = "AIController" )
		void ActivateAI(); 

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override; 	
	virtual float GetCurrentMontageTime() { return 0.0f; };	
	UAnimMontage* GetActiveMontage() { return NULL; }

	UFUNCTION(BlueprintCallable , Category = "AIController")
	virtual void ShutdownAI(); 
	virtual uint8 GetTeamNum() const override;
	virtual void SetTeamNum(uint8 TeamNum) override
	{
		TeamIndex = TeamNum;
	}

	void DisallowFiring() { bFiringAllowed = false; }  
	void AllowFiring() { bFiringAllowed = true; } 
	FORCEINLINE bool IsFiringAllowed() 
	{
		return bFiringAllowed;
	}

	UFUNCTION(BlueprintCallable, Category = "AIController")
		FVector FindNextCoverLocation(float Radius, FVector CurrentLocation);

	UFUNCTION(BlueprintCallable, Category = "AIController")
		  bool GetPawnMobility();
	virtual bool IsPawnMovable() { return false; }

	virtual void SetAIParams(const FBotAIParams& Params);
	virtual float GetDamageMultiplier()
	{
		return  AIParams.DamageMultiplier;
	}

public: 
		
	bool bFiringAllowed;
};



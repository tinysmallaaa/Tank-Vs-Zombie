#pragma once
#include "BFGameEngine.Generated.h"


UCLASS()
class TANKVSZOMBIE_API UBFGameEngine : public UGameEngine
{
	GENERATED_UCLASS_BODY()
public: 
	UPROPERTY(globalconfig)
		bool bFirstRun;
	void InitializeObjectReferences();
	virtual void Init(IEngineLoop* InEngineLoop);
	void OnLoadingMoviePlaybackFinished();
	virtual void Tick(float DeltaSeconds, bool bIdleMode);
	virtual void PreExit();

 
};

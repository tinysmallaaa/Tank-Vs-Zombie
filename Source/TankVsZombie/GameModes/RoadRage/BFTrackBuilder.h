#pragma once
#include "BFTrackVisualPart.h"
#include "BFTrackBuilder.Generated.h"

UCLASS(Blueprintable)
class ABFTrackBuilder : public AActor
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY( EditAnywhere , Category = "TrackBuilder" )
		TArray<ABFTrackVisualPart*> VisualParts ; 
	UPROPERTY( EditAnywhere , Category = "TrackBuilder" )
		float TrackLengthInKM ;
	UPROPERTY( EditAnywhere , Category = "TrackBuilder" )
		TArray<ABFTrackVisualPart*> TracksTemplateList ; 
	UPROPERTY( EditAnywhere , Category = "TrackBuilder" )
		TArray< TSubclassOf<class ABFZombie> > ZombieClasses ;
	
	UPROPERTY(EditAnywhere, Category = "TrackBuilder")
		float ZombieSpawnDistance ;
	
	UPROPERTY(EditAnywhere, Category = "TrackBuilder")
		float ZombieCullDistance;

	virtual void Tick(float DeltaSeconds) override  ;

	void UpdateTracks( APawn* PlayerPawn )			;
	void UpdateZombieSpawn( APawn* PlayerPawn )		;
	void UpdateZombieCull(APawn* PlayerPawn)		;

private:

	TArray<ABFTrackVisualPart*> ActiveTracks;

};
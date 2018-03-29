

#pragma once
#include "BFEncounterTarget.Generated.h"



class ABFEncounterVolume;

#define TEAM_NONE 0
#define TEAM_PLAYER 10
#define TEAM_ENEMY 20

UINTERFACE(MinimalAPI,Blueprintable)
class UBFEncounterTarget : public UInterface
{

	GENERATED_BODY()

public: 

}; 
class IBFEncounterTarget
{

	GENERATED_IINTERFACE_BODY() 


		  
	virtual void Activate( bool Hidden	 )	  = 0;
	virtual void Deactivate( bool Hidden )	  = 0;
	virtual void SetEncounterVolume(ABFEncounterVolume* Volume) = 0; 

};


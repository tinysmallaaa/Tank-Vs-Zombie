#pragma once
#include "BFTeamInterface.Generated.h"


#define TEAM_NONE 0
#define TEAM_PLAYER 10
#define TEAM_ENEMY 20

UINTERFACE(MinimalAPI)
class UBFTeamInterface : public UInterface
{
	GENERATED_BODY()
public:



};

class IBFTeamInterface
{
	GENERATED_IINTERFACE_BODY()

 	virtual bool IsFriendlyToAll() const
	{
		return false;
	}

 	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "GetTeamNum"))
		uint8 ScriptGetTeamNum();
 	
	virtual uint8 GetTeamNum() const PURE_VIRTUAL(IBFTeamInterface::GetTeamNum, return 255;);	
	virtual void SetTeamNum(uint8 TeamNum)=0;

};


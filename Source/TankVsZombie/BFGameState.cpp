#include "TankVsZombie.h"
#include "BFGameState.h"


ABFGameState::ABFGameState(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

} 

bool ABFGameState::OnSameTeam(const AActor* Actor1, const AActor* Actor2)
{
	const IBFTeamInterface* TeamInterface1 = Cast<IBFTeamInterface>( Actor1 );
	const IBFTeamInterface* TeamInterface2 = Cast<IBFTeamInterface>( Actor2 );

	if (TeamInterface1 == NULL)
	{
		TeamInterface1 = Cast<IBFTeamInterface>(Cast<APawn>(Actor1)->GetController());
	}

	if (TeamInterface2 == NULL)
	{
		TeamInterface2 = Cast<IBFTeamInterface>(Cast<APawn>(Actor2)->GetController());
	}

	if ( TeamInterface1 == NULL || TeamInterface2 == NULL )
	{
		return false;

	}else if ( TeamInterface1->IsFriendlyToAll() || TeamInterface2->IsFriendlyToAll() )
	{
		return true;

	}else
	{
		uint8 TeamNum1 = TeamInterface1->GetTeamNum();
		uint8 TeamNum2 = TeamInterface2->GetTeamNum();

		if ( TeamNum1 == 255 || TeamNum2 == 255 )
		{
			return false;
		}
		else
		{
			return TeamNum1 == TeamNum2 ;
		}

	}

	return false;
}

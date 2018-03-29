#include "TankVsZombie.h"
#include "BFGameSaveData.h"
#include "BFGameAchievementManager.h"


UBFGameAchievementManager::UBFGameAchievementManager( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	
}

FBFAchievementUIData UBFGameAchievementManager::GetAchievementUIData(EAchievementType Type, UBFLocalPlayer* Player)
{
	FBFAchievementUIData UIData	;
	FBFAchievementInfo& Info = AchievementInfoList[(int32)Type]	;
	FBFAchievementLevelSavedState  State = Player->GetAchievementSavedState(Type) ;
	
	UIData.Name					= Info.Name	;
	UIData.Description			= Info.Levels[State.CurrentLevel].Description ;
	UIData.ProgressAmountMax	= Info.Levels[State.CurrentLevel].RequiredAmount ;
	UIData.CurrentProgress		= State.ProgressAmount ;
	UIData.CurrentLevel			= State.CurrentLevel ;
	
	return UIData; 
}

FBFAchievementUpdateResult UBFGameAchievementManager::UpdateAchievement( EAchievementType Type, int32 Amount, UBFLocalPlayer* Player )
{	
	FBFAchievementUpdateResult Result ;

	FBFAchievementInfo& Info = AchievementInfoList[ (int32) Type] ;
	FBFAchievementLevelSavedState  State = Player->GetAchievementSavedState( Type ) ;

	State.ProgressAmount = Amount ;
	int32 PrevLevel = State.CurrentLevel ;
	if ( Info.Levels[State.CurrentLevel].RequiredAmount > 0 )
	{
		if ( Amount >= Info.Levels[State.CurrentLevel].RequiredAmount )
		{
			State.CurrentLevel = FMath::Clamp( State.CurrentLevel + 1 ,0, Info.Levels.Num() ) ; 

			if( PrevLevel != State.CurrentLevel )
				Result.CreditsEarned = Info.Levels[State.CurrentLevel].RewardsAmount ; 
		}
	} 

	Result.Level	=	State.CurrentLevel ;
	Result.Type		=	Type ;

	switch (Type)
	{
		case ACHIEVEMENT_TYPE_EAGLE_EYE:
		{ 
			
		}break;

		case ACHIEVEMENT_TYPE_ZOMBIE_ATE_MY_GIRLFRIEND:
		{

		}break;

		case ACHIEVEMENT_TYPE_ZOMBIE_OVERLORD:
		{

		}break;

		case ACHIEVEMENT_TYPE_SHOT_LIKE_A_BOSS:
		{

		}break;

		case ACHIEVEMENT_TYPE_THE_WORLD_DOESNT_END_TODAY:
		{

		}break;

		case ACHIEVEMENT_TYPE_I_NEED_MORE_AMMO:
		{

		}break;

		case ACHIEVEMENT_TYPE_NO_ONE_TOUCH_ME:
		{

		}break;

		case ACHIEVEMENT_TYPE_TURRET_I_CHOOSE_YOU:
		{

		}break;
		case ACHIEVEMENT_TYPE_QUADRA_KILL:
		{

		}break;

		case ACHIEVEMENT_TYPE_HOW_TO_EXPLODE_YOUR_BOSS:
		{

		}break;

		case ACHIEVEMENT_TYPE_I_HATE_RELOAD:
		{

		}break;

		case ACHIEVEMENT_TYPE_NIGHTMARE_ON_THE_ZOMBIE_STREET:
		{

		}break;

		case ACHIEVEMENT_TYPE_PAINKILLER:
		{

		}break;

		case ACHIEVEMENT_TYPE_KNEEL_BEFORE_ME:
		{

		}break;

		case ACHIEVEMENT_TYPE_KA_BOOM:
		{

		}break;

		case ACHIEVEMENT_TYPE_TURRET_LEVEL:
		{

		}break;

		case ACHIEVEMENT_TYPE_MINIGUN_LEVEL:
		{

		}break;
		case ACHIEVEMENT_TYPE_COIL_LEVEL:
		{

		}break;

		case ACHIEVEMENT_TYPE_LASER_LEVEL:
		{

		}break;

		case ACHIEVEMENT_TYPE_DUAL_MINIGUN_LEGEND:
		{

		}break;

	 }

	return Result;
}

void UBFGameAchievementManager::InitAchievement_Implementation( UBFLocalPlayer* LocalPlayer )
{

}
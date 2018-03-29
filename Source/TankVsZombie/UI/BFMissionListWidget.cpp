#include "TankVsZombie.h"
#include "BFConfigDBUtil.h"
#include "BFGameSaveData.h"
#include "BFMissionListWidget.h"


UBFMissionListWidget::UBFMissionListWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

TArray<FBFMissionItemUIData> UBFMissionListWidget::GetMissionItems(int32 ChapterID,bool bIsIndex)
{
	TArray<FBFMissionItemUIData> Items;

	 
	UBFConfigDBUtil* ConfigDB = GetGameConfigDB();
	UBFGameSaveData* SaveData = GetGameSaveData();

	if ( ConfigDB != NULL && SaveData != NULL )
	{
		

		TArray<FChapterDBInfo> Chapters = ConfigDB->GetChapterInfoList();

		FChapterDBInfo SelectedChapter;
		if (bIsIndex && ChapterID<Chapters.Num())
		{			
			SelectedChapter = Chapters[ChapterID];
		}
		else
		{
			for (int32 i = 0; i < Chapters.Num(); i++)
			{
				if (Chapters[i].ChapterID == ChapterID)
				{
					SelectedChapter = Chapters[i];
				}
			}
		}

		TArray<FMissionDBInfo*> Missions = SelectedChapter.GetMissions();

		for (int32 i = 0; i < Missions.Num(); i++)
		{
			const FMissionDBInfo MissionInfo = *Missions[i];
			const FSavedMissionState MissionState = SaveData->GetMissionForID(ChapterID, MissionInfo.MissionID);

			FBFMissionItemUIData UIData;
			UIData.bIsLocked   = !MissionState.bUnlocked;
			UIData.ScreenShot  = MissionInfo.Screenshot;
			UIData.StarRating  = MissionState.StarsEarned;
			UIData.DisplayName = MissionInfo.DisplayName; 

			Items.Add(UIData);
		}
	} 

	return Items;
}


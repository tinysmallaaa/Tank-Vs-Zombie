#pragma once
#include "BFGameLeaderboardManager.Generated.h"
 
USTRUCT(BlueprintType)
struct FLeaderboardEntry
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY( BlueprintReadWrite , Category = "LeaderboardEntry" )
		FString  NickName	;
	UPROPERTY( BlueprintReadWrite , Category = "LeaderboardEntry" )
		int32	 Points		;
	UPROPERTY( BlueprintReadWrite , Category = "LeaderboardEntry" )
		int32	 Rank		;
};
 

UCLASS(Blueprintable)
class UBFGameLeaderboardManager : public  UObject
{
	GENERATED_UCLASS_BODY()
public:
	 
	UPROPERTY(BlueprintReadWrite , Category = "GameLeaderboard" )
		TArray<FLeaderboardEntry> Entries;

	UPROPERTY(BlueprintReadWrite , Category = "GameLeaderboard" )
		FString		BoardName;

	void LoadLeaderboard( FString Name ); 

};



#include "TankVsZombie.h"
#include "BFTrackBuilder.h"
 
ABFTrackBuilder::ABFTrackBuilder(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void ABFTrackBuilder::UpdateTracks(APawn* PlayerPawn)
{
	FVector Location = PlayerPawn->GetActorLocation();

	for (int32 i = 0; i < ActiveTracks.Num(); i++)
	{
		ABFTrackVisualPart* Track = ActiveTracks[i];

		if (IsValid(Track))
		{
		
		}
	}
	 
}

void ABFTrackBuilder::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds); 
} 
 
void ABFTrackBuilder::UpdateZombieCull(APawn* PlayerPawn)
{

	TArray<AActor*> Zombies ;
	UGameplayStatics::GetAllActorsOfClass( GetWorld() , TSubclassOf<ABFZombie>()  , Zombies );
	for ( int32 i = 0 ; i < Zombies.Num() ; i++ )
	{
		ABFZombie* Zombie =Cast<ABFZombie>( Zombies[i] ) ;

		if ( Zombie != NULL )
		{
			FVector Offset = PlayerPawn->GetActorLocation() - Zombie->GetActorLocation() ;

			float Length = Offset.Size() ;

			if (Length < ZombieCullDistance ) 
			{

			}
		}
	}
}
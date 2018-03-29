#include "TankVsZombie.h"
#include "BFTrackVisualPart.h"


ABFTrackVisualPart::ABFTrackVisualPart(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ZoneVolume = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Volume"));
	RootComponent = ZoneVolume;

	TrackStartAnchor = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this, TEXT("StartAnchor"));
	TrackStartAnchor->AttachToComponent(ZoneVolume, FAttachmentTransformRules( EAttachmentRule::KeepRelative , false ) );

	TrackEndAnchor = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this, TEXT("EndAnchor"));
	TrackEndAnchor->AttachToComponent(ZoneVolume, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false) );


	TrackSpline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("TrackSpline"));
	TrackSpline->AttachToComponent(ZoneVolume, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	LeftTrackSpline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("LeftTrackSpline"));
	LeftTrackSpline->AttachToComponent(ZoneVolume, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	RightTrackSpline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("RightTrackSpline"));
	RightTrackSpline->AttachToComponent(ZoneVolume, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

}

FVector ABFTrackVisualPart::GetStartAnchorPosition() const 
{
	return TrackStartAnchor->GetComponentLocation();
}

FVector ABFTrackVisualPart::GetEndAnchorPosition() const
{
	return TrackEndAnchor->GetComponentLocation();
}

float ABFTrackVisualPart::GetTrackLength() const
{
	return TrackSpline->GetSplineLength();
}

float ABFTrackVisualPart::GetRelativeOffset(APawn* Pawn)const
{
	FVector Offset = Pawn->GetActorLocation() - TrackStartAnchor->GetComponentLocation();

	return Offset.Size(); 
}

void ABFTrackVisualPart::BeginPlay()
{
	Super::BeginPlay();

	 TArray<UActorComponent*> Components ;
	 GetComponents(Components, false);
	
	for (int32 i = 0; i < Components.Num(); i++)
	{
		UBFTrackZombieSpawnSlot* SpawnSlot = Cast<UBFTrackZombieSpawnSlot>( Components[i] );

		if ( SpawnSlot != NULL )
		{
			DynamicSpawnSlots.Add(SpawnSlot);
		}
	}

}

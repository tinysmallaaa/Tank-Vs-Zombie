#include "TankVsZombie.h"
#include "BFSpawnVolume.h"


ABFSpawnVolume::ABFSpawnVolume(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	BoxComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>( this , FName("SpawnBox") ) ;
	BoxComponent->SetBoxExtent( FVector(100.0f , 100.0f , 100.0f) ) ;
	RootComponent = BoxComponent ;

	SpawnDelay = 0.2f;
	
}

void ABFSpawnVolume::SpawnZombies(int32 Count)
{ 
	Spawn( ZombieClasses , Count , 0 ) ; 
}

 
void ABFSpawnVolume::Spawn( TArray< TSubclassOf<ABFZombie> >& Classes , int32 Count , int32 GroupID )
{

	FSpawnGroup Group;

	Group.SpawnCount		= Count	;
	Group.SpawnItr			= 0	;
	Group.SpawnClasses.Append(Classes);
	Group.SpawnGroupID		= GroupID ;

	ActiveSpawnGroups.Add(Group);

	if(Count>0)
	GetWorldTimerManager().SetTimer(Group.SpawnTimerHandle , this , &ABFSpawnVolume::SpawnNextZombie , SpawnDelay , false) ;

} 

void ABFSpawnVolume::SpawnNextZombie()
{ 

	if (ActiveSpawnGroups.Num() > 0)
	{ 

		FSpawnGroup& SpawnGroup = ActiveSpawnGroups[0];

		if ( SpawnGroup.SpawnClasses.Num() > 0 )
		{

			int32 RandomIndex = FMath::RandRange(0, SpawnGroup.SpawnClasses.Num() - 1);

			SpawnZombie(SpawnGroup.SpawnClasses[RandomIndex], SpawnGroup.SpawnGroupID);

			SpawnGroup.SpawnItr++ ;

			if (SpawnGroup.SpawnItr < SpawnGroup.SpawnCount)
			{
				GetWorldTimerManager().SetTimer(SpawnGroup.SpawnTimerHandle, this, &ABFSpawnVolume::SpawnNextZombie, SpawnDelay, false);
			}
			else
			{
				ActiveSpawnGroups.RemoveAt(0);
			}

		}

	}

}

ABFZombie*  ABFSpawnVolume::SpawnZombie( UClass* ZombieClass , int32 GroupID )
{
	if (ZombieClass == NULL)
		return NULL ;

	FVector   SpawnPosition	 ;
	FRotator  SpawnRotation  ;
	FVector   TargetPosition ;

	FindValidSpot( ZombieClass , SpawnPosition  , SpawnRotation ) ; 
 
	FActorSpawnParameters spawnParam ;
	
	spawnParam.Instigator	= NULL ;
	spawnParam.Owner		= this ;
	
	FTransform spawnTransform ;
	spawnTransform.SetIdentity() ;
	spawnTransform.SetTranslation(SpawnPosition) ;

	ABFZombie* Zombie = Cast<ABFZombie>( UGameplayStatics::BeginDeferredActorSpawnFromClass( this , ZombieClass , spawnTransform ,ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn) ) ;

	UGameplayStatics::FinishSpawningActor(Zombie, spawnTransform ) ; 

	Zombie->GetCharacterMovement()->MaxWalkSpeed *= FMath::RandRange(0.6f, 1.0f) ;

	ABFGameModeBase*  GameMode = Cast<ABFGameModeBase>( UGameplayStatics::GetGameMode( GetWorld() ) ) ;

	if (GameMode != NULL)
	{
		GameMode->OnZombieSpawned( Zombie , GroupID) ;
	}

	return Zombie ;

}

void ABFSpawnVolume::FindValidSpot(UClass* BotClass, FVector& SrcPosition , FRotator& Rotator)
{  
	float CapsuleHeight =	TSubclassOf<ABFZombie>(BotClass).GetDefaultObject()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() ;
	float CapsuleRadius =	TSubclassOf<ABFZombie>(BotClass).GetDefaultObject()->GetCapsuleComponent()->GetScaledCapsuleRadius()	 ;
	float HalfRadius = CapsuleRadius / 2 ;

	FBox box = this->GetComponentsBoundingBox()	;

	bool	bFoundValidSpot = false	;  

	FVector Extent = BoxComponent->GetScaledBoxExtent()		;
	FVector Origin = BoxComponent->GetComponentLocation()	;

	FBox SrcBox = FBox(Origin - Extent, Origin + Extent)	;
	
 
	SrcPosition = FMath::RandPointInBox(SrcBox)	;
	SrcPosition.Z = Origin.Z+ CapsuleHeight;
	Rotator = FRotator(0.0f, 0.0f, 0.0f)	; 

	
	TArray<FHitResult> Hits	; 

	FCollisionObjectQueryParams ObjectQueryParams		;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn)	; 

	FVector startTrace	 = SrcPosition	; 
	FVector endTrace	 = SrcPosition	;
	endTrace.Z			-= 1000.0f		;

	FHitResult out	;
	FCollisionObjectQueryParams queryParams	; 
	queryParams.AddObjectTypesToQuery(ECC_Visibility) ;

	FCollisionQueryParams collisionQueryParams	;
	collisionQueryParams.AddIgnoredActor(this)	;

	if (GetWorld()->LineTraceSingleByObjectType(out, startTrace, endTrace, queryParams, collisionQueryParams))
	{
	//	SrcPosition = out.ImpactPoint			;
	//	SrcPosition.Z += CapsuleHeight+50.0f	;
	} 

}
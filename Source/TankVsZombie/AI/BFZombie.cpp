#include "TankVSZombie.h"
#include "BFZombie.h"

ABFZombie::ABFZombie(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)

{
	
	HeadBlastFXComp = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("HeadBlastFX"))	;
	HeadBlastFXComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("HeadBlastFXSocket")) ;
	
	HeadBlastFXComp->SetAutoActivate(false)	;
	
	AudioComponent = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("Zombie3DAudio"))	;
	AudioComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false)) ;

	DamageAmount = 5.0f	 ;
	bFirstDamageReceived = false ;

	bAllowHeadShot = true ;
	bDrawHealthBar = false ;

}

void ABFZombie::PlaySound3D(USoundCue* Cue)
{

	if (Cue && Active3DSound == Cue && AudioComponent->IsPlaying())return; 

	Active3DSound = Cue;
	if (Active3DSound != NULL)
	{
		AudioComponent->Stop();
		AudioComponent->SetSound(Active3DSound);
		AudioComponent->bIsUISound = false;
		AudioComponent->bAllowSpatialization = true;
		AudioComponent->Play();
	} 

}

void ABFZombie::StopSound3D()
{

	AudioComponent->Stop()	;
	Active3DSound = NULL	;
}

FAttackMontage  ABFZombie::GetRandomAttackMontage()
{
	if (GetCurrentMontage() != NULL)
		return FAttackMontage()		;

	if (AttackMontageList.Num() > 0)
	{

		if (Health < HealthMax / 9)
		{

			int32 heavyIndex = 0		;
			float HighestDamage = 0.0f	;
			for ( int32 i = 0	; i < AttackMontageList.Num() ; i++ )
			{
				const FAttackMontage& Montage = AttackMontageList[i]	;
				if ( Montage.DamageScale > HighestDamage)
				{
					heavyIndex = i	;
					HighestDamage = Montage.DamageScale	;
				}
			}

			return AttackMontageList[heavyIndex] ;

		}else 
		{
			int32 MontageIndex = FMath::RandRange( 0 , AttackMontageList.Num() - 1 ) ;
			return AttackMontageList[MontageIndex] ;
		}

	}

	return FAttackMontage() ;
}

void ABFZombie::CauseDamage(AActor* Target)
{

	AActor* TargetToDamage = Target == NULL ? CurrentTarget : Target;

	if (TargetToDamage != NULL)
	{
		FVector Direction = (TargetToDamage->GetActorLocation() - GetActorLocation()).GetSafeNormal() ;
		FBFPointDamageEvent DamageEvent;

		DamageEvent.Damage			= DamageAmount*DamageMultiplier;
		DamageEvent.ShotDirection	= FVector_NetQuantizeNormal(Direction);
		DamageEvent.Momentum		= Direction;

		TargetToDamage->TakeDamage( DamageAmount , DamageEvent , GetController() , this );
	}

}

void ABFZombie::Kill(FVector Momentum, const FBotDamageParamEx& DamageParams)
{

	if ( bIsDead ) return ;

	StopAnimMontage( NULL );
	StopSound3D() ;
	bIsDead = true ;
	GetCharacterMovement()->MaxWalkSpeed = 0.0f ;
	OnKilled( DamageParams.bHeadShot );

	if ( DamageParams.bHeadShot && bAllowHeadShot )
	{
		
		bWasHeadShot = true;
		StartRagdoll( Momentum * 0.1f ) ;

	}
	else if (bRagdollOnDeath)
	{
		
		bWasHeadShot = false ;
		StartRagdoll( Momentum ) ;

	}
	else if ( ZombieGibsClass != NULL )
	{
		
		bWasHeadShot = false ;
		Explode( SavedLastDamageOrigin , SavedLastMomentumVec );
		DestroyBot() ;

	}
	else
	{

		bWasHeadShot = false ;
		if ( DeathMontage )
		{
			float PlayLength = DeathMontage->GetPlayLength() ;

			PlayAnimMontage( DeathMontage , 1.0f );
			SetLifeSpan( PlayLength ) ;
		}
		else if ( LifeSpanPostDeath > 0.0f )
		{
			SetLifeSpan( LifeSpanPostDeath ) ; 
		}	
		else
		{
			ExecuteDeath_BP() ;
		}
	}

	ABFAIController* AIController = Cast<ABFAIController>( GetController() ) ;
	if ( AIController != NULL )
	{
		AIController->ShutdownAI() ;
	}

}

void ABFZombie::Explode(FVector Origin, FVector Momentum)
{

	if ( ZombieGibsClass != NULL )
	{
		FTransform SpawnTransform = GetActorTransform() ;
		ABFGibSkeletalMesh* Gibs = Cast<ABFGibSkeletalMesh>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ZombieGibsClass, SpawnTransform));

		for ( int32 i = 0 ; i < GetMesh()->GetMaterials().Num() ; i++ )
		{
			Gibs->SetMaterial( i , GetMesh()->GetMaterials()[i] ) ;
		}

		
		UGameplayStatics::FinishSpawningActor( Gibs , SpawnTransform );
		Gibs->Explode( Origin , Momentum );
		ResetDamageFrac() ;
	}

}

void ABFZombie::ResetDamageFrac()
{

	if (MaterialInstance != NULL)
	{
		MaterialInstance->SetScalarParameterValue(TEXT("Damage"), 0.0f);
	}

}

void ABFZombie::DamageReceived()
{

	bFirstDamageReceived = true;

	if (WalkSpeedOnDamageMultiplier > 0.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeedOnDamageMultiplier;
	}

	bDrawHealthBar = true;
	if (MaterialInstance != NULL)
	{
		DamageFrac = 1.0f - (float(Health) / float(HealthMax));
		MaterialInstance->SetScalarParameterValue(TEXT("DamageFrac"), DamageFrac);
		MaterialInstance->SetScalarParameterValue(TEXT("Damage"), 0.2f);
		GetWorldTimerManager().SetTimer(DamageFracTimer, this, &ABFZombie::ResetDamageFrac, 0.05f, false);
	}

}

bool  ABFZombie::CloseEnoughForMelee(AActor* OtherActor )
{

	if (OtherActor != NULL)
	{

		const float Radius = GetCapsuleComponent()->GetScaledCapsuleRadius();
		const  FVector Origin = GetActorLocation();
		const  FVector TraceDir = this->GetActorForwardVector();

		FVector StartTrace = Origin + (TraceDir*GetCapsuleComponent()->GetScaledCapsuleRadius());
		FVector EndTrace = StartTrace + TraceDir * Radius*2.0f ;

		FCollisionQueryParams CollisionParams ;

		CollisionParams.AddIgnoredActor(this);
		CollisionParams.AddIgnoredComponent( GetCapsuleComponent() );

  
		FHitResult Out;
		if( GetWorld()->LineTraceSingleByChannel( Out , StartTrace , EndTrace , ECollisionChannel::ECC_Pawn , CollisionParams ) )
		{
			if ( Out.Actor == OtherActor )
			{
			
			
				FVector Direction = (OtherActor->GetActorLocation() -  GetActorLocation()).GetSafeNormal() ;
				
				float Yaw = Direction.ToOrientationRotator().Yaw ;
				float Diff = GetActorRotation().Yaw - Yaw;
				if (Diff > -45.0f && Diff < 45.0f )
					return true;
			}
		}
		 
		{

			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

			FCollisionShape Shape;
			Shape.SetSphere(GetCapsuleComponent()->GetScaledCapsuleRadius());
			 
			StartTrace		= GetActorLocation();
			EndTrace		= StartTrace + TraceDir * Radius * 2.0f;
			 
			if (GetWorld()->SweepSingleByObjectType( Out , StartTrace , EndTrace , FQuat(1.0f, 1.0f, 1.0f, 1.0f) , ObjectQueryParams , Shape , CollisionParams ))
			{
				if (Out.IsValidBlockingHit() && Out.Actor == OtherActor)
				{

					FVector Direction = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

					float Yaw = Direction.ToOrientationRotator().Yaw;
					if (Yaw > -45.0f && Yaw < 45.0f)
						return true;
				}
			}
		}

	}

	return false;

}
void  ABFZombie::NotifyNavigationFailed()
{ 
	float Time = GetWorld()->GetTimeSeconds();
	if (LastNavigationFailedTime == 0.0f)
	{
		LastNavigationFailedTime = Time;
	}
	if ( Time - LastNavigationFailedTime > 30.0f )
	{

		ABFGameModeBase* GameMode = Cast<ABFGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode != NULL)
			GameMode->OnZombieKilled(this);
		DestroyBot();
	} 
}
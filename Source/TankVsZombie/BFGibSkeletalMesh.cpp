#include "TankVsZombie.h"
#include "BFWorldSettings.h"
#include "BFGibSkeletalMesh.h"

 
ABFGibSkeletalMesh::ABFGibSkeletalMesh(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	 
	Mesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, FName(TEXT("Mesh")));
	if (Mesh != NULL)
	{
		Mesh->bReceivesDecals = false;
		Mesh->SetCollisionProfileName(FName(TEXT("CharacterMesh")));
		Mesh->SetCollisionObjectType(ECC_PhysicsBody);
		Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetNotifyRigidBodyCollision(true);
	//	Mesh->OnComponentHit.AddDynamic( this , &ABFGibSkeletalMesh::OnPhysicsCollision );
		RootComponent = Mesh;
	}

	InvisibleLifeSpan = 3.f;
	InitialLifeSpan = 10.0f;
	MomentumMultiplier = 0.5f;
	LinearVelocityMultiplier = 0.5f;
	AngularVelocityMultiplier = 0.5f;
}

void ABFGibSkeletalMesh::PreInitializeComponents()
{
	USkeletalMeshComponent* SKComp = Cast<USkeletalMeshComponent>(Mesh);
	if (MeshChoices.Num() > 0 && SKComp != NULL)
	{
		if (SKComp->SkeletalMesh != NULL)
		{
			MeshChoices.AddUnique(SKComp->SkeletalMesh);
		}
		SKComp->SetSkeletalMesh(MeshChoices[FMath::RandHelper(MeshChoices.Num())]);
	}

	LastBloodTime = GetWorld()->TimeSeconds;

	Super::PreInitializeComponents();

}

void ABFGibSkeletalMesh::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < BloodDecals.Num(); i++)
	{
		FBFBloodDecalInfo& DecalInfo = BloodDecals[i];
		if (DecalInfo.Material != NULL)
		{
			DecalInfo.DynamicInstance = UMaterialInstanceDynamic::Create(DecalInfo.Material, this);
		}
	}


}

void ABFGibSkeletalMesh::Explode( FVector Origin , FVector Momentum)
{
	if (IsActorBeingDestroyed() || !GetMesh()->IsRegistered())
	{
		return;
	}

	if (RootComponent == GetMesh() && GetMesh()->IsSimulatingPhysics())
	{
		return;
	}

	if (!bExploded)
	{
		TGuardValue<bool> RagdollGuard(bExploded, true);

		if (!GetMesh()->ShouldTickPose())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance == nullptr || !AnimInstance->IsPostUpdatingAnimation())
			{
				GetMesh()->TickAnimation(0.0f, false);
				GetMesh()->RefreshBoneTransforms();
				GetMesh()->UpdateComponentToWorld();
			}
		} 

		if (GetMesh()->GetBodyInstance() != nullptr)
		{
			GetMesh()->GetBodyInstance()->bUseCCD = true;
		}

		GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetAllBodiesNotifyRigidBodyCollision(true);
		GetMesh()->UpdateKinematicBonesToAnim(GetMesh()->GetComponentSpaceTransforms(), ETeleportType::TeleportPhysics, true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->RefreshBoneTransforms();
		GetMesh()->SetAllBodiesPhysicsBlendWeight(1.0f);
		//GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//RootComponent = GetMesh();
		GetMesh()->bGenerateOverlapEvents = true;
		GetMesh()->bShouldUpdatePhysicsVolume = true;
		GetMesh()->RegisterClothTick(false);
		//GetMesh()->SetAllPhysicsLinearVelocity(Momentum, false);
 
		FVector FinalMomentum = Momentum * FMath::FRandRange( MomentumMultiplier * 0.5f , MomentumMultiplier );

		FVector Center = GetMesh()->GetSocketLocation("Root");


		for (FBodyInstance* Body : GetMesh()->Bodies)
		{
			if (Body != NULL)
			{
				FVector Delta = Body->GetUnrealWorldTransform(true).GetTranslation() - Center ; 

				Delta *= FMath::FRandRange( LinearVelocityMultiplier*0.5f , LinearVelocityMultiplier );
				FVector AngularVelocity = Momentum * FMath::FRandRange( AngularVelocityMultiplier * 0.5f , AngularVelocityMultiplier );

				Body->SetAngularVelocityInRadians(FMath::DegreesToRadians(AngularVelocity) , true );
//				Body->SetAngularVelocity((AngularVelocity), true);
				Body->SetLinearVelocity(  FinalMomentum + Delta , true );
				Body->SetEnableGravity(true);
			
			}
		} 
		 
		if (BloodEffects.Num() > 0)
		{
			UParticleSystem* BloodEffect = BloodEffects[FMath::RandHelper(BloodEffects.Num())];

		 

			if ( BloodEffect != NULL )
			{
				for (int i = 0; i < BloodEffectSocketNames.Num(); i++)
				{
					
					FName	SocketName		= BloodEffectSocketNames[i]; 
					FRotator  SocketRotation	= GetMesh()->GetSocketRotation( SocketName );

					UParticleSystemComponent* ParticleComp = UGameplayStatics::SpawnEmitterAttached( BloodEffect , GetMesh() , SocketName , FVector(0.0f), SocketRotation  );
					 
					if (ParticleComp != NULL)
					{

					}
					 
				}
			}
		}
	
		if ( BloodDecals.Num() > 0 )
		{
			BloodDecalInfo = BloodDecals[ FMath::RandHelper( BloodDecals.Num() ) ] ;
			BloodDecalInfo.DynamicInstance->SetScalarParameterValue("Frame", FMath::FRandRange(0.0f, BloodDecalInfo.MaxFrames - 1));
			if ( BloodDecalInfo.Material != NULL )
			{
				static FName NAME_BloodDecal(TEXT("BloodDecal"));

				FCollisionQueryParams QueryParams( NAME_BloodDecal , true , this );

				FTraceDelegate TraceDelegate;
				TraceDelegate.BindUObject( this , &ABFGibSkeletalMesh::TraceResult );

				FCollisionResponseParams ResponseParams;
				FVector TraceStart  = GetActorLocation();
				FVector TraceEnd	= GetActorLocation() + FVector( 0.0f , 0.0f , -1.0f ) * 1000.0f ;

				DecalTraceHandle = GetWorld()->AsyncLineTraceByChannel( EAsyncTraceType::Single , TraceStart , TraceEnd , ECC_Visibility , QueryParams , ResponseParams , &TraceDelegate );
 
			}
		}
		
		 
		if (!IsPendingKillPending())
		{
			FTimerHandle TempHandle;
			GetWorldTimerManager().SetTimer(TempHandle, this, &ABFGibSkeletalMesh::CheckGibVisibility, 5.f, false);
		}  



	}
}

void  ABFGibSkeletalMesh::TriggerFadeOut()
{
	bFadeTriggered = true;
 
}

void ABFGibSkeletalMesh::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bFadeTriggered)
	{

		float Frac = GetWorld()->GetTimeSeconds() - FadeStartTimer ;
		Frac /= (0.8f) ;   
		FadeOutAlpha = 1.0f - Frac ; 
		
		if (MaterialDynamic != NULL)
		{
			MaterialDynamic->SetScalarParameterValue("FadeOutAlpha", FadeOutAlpha);
		}

		if (Frac > 1.0f)
		{
			Destroy(false, false);
		}
	}

}

void ABFGibSkeletalMesh::TraceResult(const FTraceHandle& TraceHandle, FTraceDatum & TraceData)
{
	if (IsPendingKillPending())return;

	ABFWorldSettings* Settings = Cast<ABFWorldSettings>(GetWorldSettings());

	if (Settings != NULL)
	{
		if (TraceData.OutHits.Num() > 0)
		{
			const FHitResult DecalHit = TraceData.OutHits[0];

			if (DecalHit.Component->bReceivesDecals)
			{
				UDecalComponent* Decal = NewObject<UDecalComponent>(GetWorld());

				{
					Decal->SetAbsolute(true, true, true);
				}

				FVector2D DecalScale = BloodDecalInfo.BaseScale * FMath::FRandRange(BloodDecalInfo.ScaleMultRange.X, BloodDecalInfo.ScaleMultRange.Y);

				Decal->DecalSize = FVector(50.0, DecalScale.X, DecalScale.Y);
				Decal->SetWorldLocation(DecalHit.Location);
				Decal->SetWorldRotation((-DecalHit.Normal).Rotation() + FRotator(0.0f, 0.0f, 360.0f * FMath::FRand()));
			 	
			
				Decal->SetDecalMaterial(BloodDecalInfo.DynamicInstance);

			
				Decal->RegisterComponentWithWorld(GetWorld());

				Settings->AddImpactEffect(Decal,10.0f);
			}
		}
	}
}

void ABFGibSkeletalMesh::SetMaterial( int32 ElemIndex , UMaterialInterface* Material)
{ 
	MaterialDynamic = UMaterialInstanceDynamic::Create( Material->GetBaseMaterial() , this );

	if ( MaterialDynamic != NULL ) 
	{
		GetMesh()->SetMaterial( ElemIndex , MaterialDynamic );
	} 

}
 
void ABFGibSkeletalMesh::CheckGibVisibility()
{
	if (GetWorld()->GetTimeSeconds() - GetLastRenderTime() > 1.f)
	{
		Destroy();
	}
}

void ABFGibSkeletalMesh::OnPhysicsCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

#if !UE_SERVER

	// if we landed on a mover, attach to it
	if (OtherComp != NULL && OtherComp->Mobility == EComponentMobility::Movable && (Hit.Normal.Z > 0.7f))
	{
		//RootComponent->AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);
	}

	// maybe spawn blood as we smack into things
	if (OtherComp != NULL && Cast<ABFGibSkeletalMesh>(OtherActor) == NULL && GetWorld()->TimeSeconds - LastBloodTime > 0.5f && GetWorld()->TimeSeconds - GetLastRenderTime() < 0.5f)
	{
		if (BloodEffects.Num() > 0)
		{
			UParticleSystem* Effect = BloodEffects[FMath::RandHelper(BloodEffects.Num())];
			if (Effect != NULL)
			{
			//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, Hit.Location, Hit.Normal.Rotation(), true);
			}
		}

		ABFWorldSettings* Settings = Cast<ABFWorldSettings>(GetWorldSettings());

		if (Settings != NULL)
		{
			if (BloodDecals.Num() > 0)
			{
				const FBFBloodDecalInfo& DecalInfo = BloodDecals[FMath::RandHelper(BloodDecals.Num())];
				if (DecalInfo.Material != NULL)
				{
					static FName NAME_BloodDecal(TEXT("BloodDecal"));

					FHitResult DecalHit;
					if ( GetWorld()->LineTraceSingleByChannel( DecalHit , GetActorLocation() , GetActorLocation() - Hit.Normal * 200.0f , ECC_Visibility , FCollisionQueryParams(NAME_BloodDecal, false, this)) && Hit.Component->bReceivesDecals )
					{
						UDecalComponent* Decal = NewObject<UDecalComponent>( GetWorld() ) ;

						if ( Hit.Component.Get() != NULL && Hit.Component->Mobility == EComponentMobility::Movable )
						{ 
							Decal->SetAbsolute( false , false , true );
							Decal->AttachToComponent( Hit.Component.Get() , FAttachmentTransformRules::KeepRelativeTransform ); 
						}
						else
						{ 
							Decal->SetAbsolute(true, true, true); 
						}

						FVector2D DecalScale = DecalInfo.BaseScale * FMath::FRandRange( DecalInfo.ScaleMultRange.X , DecalInfo.ScaleMultRange.Y );

						Decal->DecalSize = FVector( 1.0f , DecalScale.X , DecalScale.Y );
						Decal->SetWorldLocation( DecalHit.Location );
						Decal->SetWorldRotation(( -DecalHit.Normal ).Rotation() + FRotator( 0.0f , 0.0f , 360.0f * FMath::FRand() ) );
						Decal->SetDecalMaterial( DecalInfo.Material );
						Decal->RegisterComponentWithWorld( GetWorld() );

						Settings->AddImpactEffect(Decal);
					}
				}
			}
		}

		LastBloodTime = GetWorld()->TimeSeconds;
	}
#endif

}
#include "TankVsZombie.h"
#include "AI/BFAIBot.h"
#include "Vehicle/BFTankVehicle.h"
#include "Vehicle/BFTankPlayerController.h"
#include "BFGameHUD.h"

ABFGameHUD::ABFGameHUD(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CrossHairSize = 50.0f;
	CrossHairClipOffset = CrossHairSize*3.0f;
	TextInterpTime = 2.0f;
	BonusTextColor = FLinearColor::Blue;
	TextScale = 2.0f;
	HUDWidgetColor = FLinearColor::Green;
	HUDTextColor = FLinearColor::Green;
	SelectionCrossHairColor = FLinearColor::Red; 
	RadarSizeRelative = 100.0f;
	RadarRange = 5000.0f;
	RadarSignalStrength = 45.0f;
	RadarPositionRelative = FVector2D(0.0f, 0.0f);

	CreditIconSize = 80.0f;
	CreditsAnimTime = 2.0f;
	HeadShotAnimTime = 2.0f;
	HeadShotIconSize = 200.0f;

	bDrawRadar = true;
	bDrawHealthMeter = false;
	BonusTextSize = 3.0f;
}

void ABFGameHUD::HandleCrossHair(float X, float Y)
{
	CrossHairPos.X = X;
	CrossHairPos.Y = Y;
}

void ABFGameHUD::DrawHUD()
{

	Super::DrawHUD();
	int32 ScreenWidth, ScreenHeight;
	APlayerController* PC = GetOwningPlayerController();
	if (PC != NULL)
	{
		PC->GetViewportSize(ScreenWidth, ScreenHeight);
		ViewportSizeX = ScreenWidth;
		ViewportSizeY = ScreenHeight;
	}

	DrawAnimatedTexts();
	DrawAnimatedIcons();
	DrawRadar();
	DrawBotsHealthMeter();
	DrawCrossHairs();
	DrawDamageScratch();
	DrawAlerts(); 

}

void ABFGameHUD::DrawCrossHairs()
{
	ABFTankPlayerController* PC = Cast<ABFTankPlayerController>( GetOwningPlayerController() );

	if ( PC != NULL	)
	{
		int32 ViewportWidth;
		int32 ViewportHeight;

		const FLinearColor FocusedColor = PC->HasFocusedTarget() ? SelectionCrossHairColor : FLinearColor::White;

		PC->GetViewportSize( ViewportWidth , ViewportHeight );
		
		CrossHairPos.X = ViewportWidth  / 2;
		CrossHairPos.Y = ViewportHeight / 2;

		CrossHairPos.X = CrossHairPos.X - CrossHairSize*0.5f;
		CrossHairPos.Y = CrossHairPos.Y - CrossHairSize*0.5f;

		float CrossHairWidth	= CrossHairSize;
		float CrossHairHeight	= CrossHairSize;

		if (bShowCrossHair && CrossHairTexture != NULL)
		{ 
			if (bReloadingShell)
			{ 
				float Rotation = GetWorld()->GetTimeSeconds()*360.0f; 

				DrawTexture( ReloadingAnimTex , CrossHairPos.X , CrossHairPos.Y , CrossHairWidth, CrossHairHeight, 0.0f , 0.0f , 1.0f , 1.0f , FLinearColor(1.0f, 1.0f, 1.0f, 0.5f), BLEND_Translucent , 1.0f , false , Rotation ,  FVector2D(0.5f, 0.5f));
			}
			else
			{
				DrawTexture( CrossHairTexture , CrossHairPos.X , CrossHairPos.Y , CrossHairWidth, CrossHairHeight, 0.0f , 0.0f , 1.0f , 1.0f , FocusedColor );
			}

		}else if (bShowTurretCrosshair && TurretCrossHairTexture != NULL)
		{
  
			if (bReloadingClip)
			{
				float Rotation = GetWorld()->GetTimeSeconds()*360.0f;

				DrawTexture( ReloadingAnimTex , CrossHairPos.X , CrossHairPos.Y , CrossHairWidth, CrossHairHeight, 0.0f , 0.0f , 1.0f , 1.0f , FLinearColor(1.0f,1.0f,1.0f,0.5f) , BLEND_Translucent , 1.0f , false , Rotation ,FVector2D(0.5f,0.5f));
		 	}
			else
			{
				DrawTexture(TurretCrossHairTexture, CrossHairPos.X, CrossHairPos.Y, CrossHairWidth, CrossHairHeight, 0.0f, 0.0f, 1.0f, 1.0f, FocusedColor);
			}
		}

	}
	
}

void ABFGameHUD::DrawZoomOptics()
{
	 

}

void ABFGameHUD::DrawAnimatedIcons()
{
	int32 i;
	for ( i = 0; i < AnimatedIcons.Num(); i++)
	{
		FBonusIconAnim& Anim = AnimatedIcons[i]; 

		if (Anim.IconTexture == NULL)continue;

		FVector  ScreenPos = Project(Anim.StartPosition);

		float Frac = (CurrentTime - Anim.StartTime) / Anim.AnimTime;

		float yPos = 0.0f ;
		float xPos = 0.0f ;

		float Aspect = Anim.IconTexture->GetSurfaceWidth() / Anim.IconTexture->GetSurfaceHeight();

		float Width  = Anim.IconSize*Aspect ;
		float Height = Anim.IconSize ;
	
		float scale = 1.0f;
		if ( (Anim.AnimType & EICON_ANIM_ALPHA) != 0)
		{
			Anim.Color.A = 1.0f - Frac;
		}

		if ( (Anim.AnimType & EICON_ANIM_SCALE) != 0)
		{
			if (HeadShotZoomAnimCurve != NULL)
			{
				Width  *= HeadShotZoomAnimCurve->GetFloatValue(Frac);
				Height *= HeadShotZoomAnimCurve->GetFloatValue(Frac);
			}
		}

		if ( (Anim.AnimType & EICON_ANIM_Y_ANIM)!=0 )
		{
			if (CreditsAnimCurve != NULL)
			{
				yPos = CreditsAnimCurve->GetFloatValue(Frac);
			}
		}
		 
		float LocationX = ScreenPos.X - xPos;
		float LocationY = ScreenPos.Y - yPos;


		if (Frac >= 1.0f )
		{
			Anim.bDone = true;
		}
		
		DrawTexture(  Anim.IconTexture , LocationX - Width*0.5f , LocationY - Height*0.5f , Width , Height , 0.0f , 0.0f , 1.0f , 1.0f , Anim.Color , BLEND_Translucent ); 

	}

	for ( i = 0 ; i < AnimatedIcons.Num() ; i++ )
	{
		FBonusIconAnim& Anim = AnimatedIcons[i] ;

		if ( Anim.bDone )
		{
			AnimatedIcons.RemoveAt( i );
		}
	}
}

void ABFGameHUD::DrawAnimatedTexts()
{
	for ( int i = 0 ; i < AnimatedTexts.Num() ; i++ )
	{
		FBonusTextAnim& Anim = AnimatedTexts[i] ;
		FVector ScreenPos = Project( Anim.StartPosition ) ;

		float Frac		= ( CurrentTime - Anim.StartTime ) / TextInterpTime ;
		Anim.Color.A	= 1.0f - Frac;
		float yPos		= ScreenPos.Y;

		if (CreditsAnimCurve != NULL)
		{ 
			yPos = CreditsAnimCurve->GetFloatValue(Frac); 
		}
		
		Anim.CurrentPosition = FVector2D(ScreenPos.X , ScreenPos.Y - yPos  ) ;
		
		if (Frac >= 1.0f )
		{
			Anim.bDone = true ;
		}

		DrawText( Anim.Text , Anim.Color , Anim.CurrentPosition.X , Anim.CurrentPosition.Y , NULL , BonusTextSize );
	}

	for ( int i = 0 ; i < AnimatedTexts.Num() ; i++ )
	{

		FBonusTextAnim& Anim = AnimatedTexts[i];

		if ( Anim.bDone )
		{
			AnimatedTexts.RemoveAt( i );
		}
	}
}

void ABFGameHUD::DrawRadar()
{
	APlayerController* OwningPC = GetOwningPlayerController() ;
	SignalRotation  += GetWorld()->GetDeltaSeconds() * RadarSignalStrength ;
	float ColorFrac  = FMath::Clamp( FMath::Sin( GetWorld()->GetTimeSeconds()*0.05f*180.0f ) , 0.0f , 1.0f ) ;
	ABFPlayerTank* TankPawn = Cast<ABFPlayerTank>(OwningPC->GetPawn());
	if (TankPawn!=NULL && OwningPC != NULL && bDrawRadar)
	{
		int32 SizeX, SizeY;
		OwningPC->GetViewportSize(SizeX, SizeY) ; 
		float RadarRadius = RadarSizeRelative*SizeX;
		FVector2D RadarScreenPos	= FVector2D( RadarPositionRelative.X * SizeX	 , RadarPositionRelative.Y * SizeY ) ;
		FVector2D RadarScreenCenter = FVector2D( RadarScreenPos.X + RadarRadius*0.5f , RadarScreenPos.Y + RadarRadius*0.5f );
	   

	
		FVector   PlayerCenter		= TankPawn->GetActorLocation() ;
		FVector2D PlayerScreenPos	= TOVEC2D( Project(PlayerCenter) );
		float Rotation = TankPawn->TargetCannonRotationYaw ;
		
		OutPawns.Reset() ;
		PawnPositions.Reset() ;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABFAIBot::StaticClass(), OutPawns) ;
	 
		for(int i=0;i<OutPawns.Num();i++)
		{
			ABFAIBot* Pawn = Cast<ABFAIBot>(OutPawns[i]) ;
			
			if (IsValid(Pawn) && Pawn->IsAlive())
			{
				FVector PawnPos = Pawn->GetActorLocation() ;

				FVector Offset = PawnPos - PlayerCenter ;
				float Distance = Offset.Size2D() ;

				if (Distance < RadarRange)
				{
					PawnPositions.Add(PawnPos);
				}
			}

		} 
		if(RadarBackground)
		DrawTexture(RadarBackground, RadarScreenPos.X , RadarScreenPos.Y , RadarRadius , RadarRadius , 0.0f , 0.0f , 1.0f , 1.0f , FLinearColor::White , BLEND_Translucent );
		
		if (BotPositionTexture != NULL)
		{

			float InvRangeFrac	= 1.0f / RadarRange;
			float HalfRadius	= RadarRadius * 0.5f ;

			FMatrix ParentRotMatrix = FRotationMatrix(FRotator(0.0f, Rotation + 90.0f ,0.0f));
			ParentRotMatrix = ParentRotMatrix.ConcatTranslation(PlayerCenter).Inverse();


			for (int i = 0; i < PawnPositions.Num(); i++)
			{
				const FVector	PawnLocation  = PawnPositions[i] ;
				const FLinearColor  RedInterp = FLinearColor( 0.8f + ColorFrac , 0.0f , 0.0f , 0.6f );
		
				FVector LocalOffset = ParentRotMatrix.TransformPosition( FVector( PawnLocation ) );

				float DistanceFrac = LocalOffset.Size2D() * InvRangeFrac ;

				FVector2D Direction = TOVEC2D( LocalOffset.GetSafeNormal2D() );

				if (DistanceFrac > 1.0f)
				{
					DistanceFrac = 1.0f;
				}

				FVector2D ScreenPos = RadarScreenCenter + Direction * ( DistanceFrac * HalfRadius );

				DrawTexture( BotPositionTexture , ScreenPos.X , ScreenPos.Y , 5.0f , 5.0f , 0.0f , 0.0f , 1.0f , 1.0f , RedInterp );
			}

		}

		if(RadarSignal)
		DrawTexture(RadarSignal, RadarScreenPos.X , RadarScreenPos.Y , RadarRadius , RadarRadius , 0.0f , 0.0f , 1.0f , 1.0f , FLinearColor(0.0f,1.0f,0.0f,0.8f) , BLEND_Translucent , 1.0f , false , SignalRotation , FVector2D(0.5f,0.5f) );
		
		if(RadarForeground)
		DrawTexture(RadarForeground, RadarScreenPos.X , RadarScreenPos.Y , RadarRadius , RadarRadius , 0.0f , 0.0f , 1.0f, 1.0f , FLinearColor::White , BLEND_Translucent , 1.0f , false , Rotation  , FVector2D(0.5f,0.5f) );
		
		if(RadarDirArrow)
		DrawTexture(RadarDirArrow, RadarScreenPos.X-20.0f, RadarScreenPos.Y - 20.0f, RadarRadius + 40.0f, RadarRadius + 40.0f, 0.0f, 0.0f, 1.0f, 1.0f, FLinearColor::White, BLEND_Translucent, 1.0f, false, Rotation, FVector2D(0.5f, 0.5f));

		
	}
}

void ABFGameHUD::DrawBotsHealthMeter()
{ 
	APlayerController* OwningPC = GetOwningPlayerController();

	if ( OwningPC != NULL && bDrawHealthMeter )
	{ 
		int32 SizeX, SizeY;
		OwningPC->GetViewportSize( SizeX , SizeY );
		UWorld* world = GetWorld();

		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass( world , APawn::StaticClass() , OutActors );		 

		for ( int i = 0 ; i < OutActors.Num() ; i++ )
		{
			ABFAIBot* Bot = Cast<ABFAIBot>(OutActors[i]); 

			if ( IsValid(Bot) )
			{ 
				FVector HealthBarPosWorld = Bot->GetActorLocation() + FVector(0.0f, 0.0f, Bot->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
				FVector ScreenPos = Project(HealthBarPosWorld)	;
 
				float Health = 100.0f ;
				float HealthMax = 100.0f ;
				float HealthBarDrawOffsetY = 50.0f ;
				bool bVisible = false ;
				 

				if (  Bot->IsAlive() && Bot->bDrawHealthBar )
				{

					bVisible		= Bot->GetMesh()->IsVisible()		;
					HealthBarDrawOffsetY = Bot->HealthBarDrawOffsetY	;
					Health			= Bot->Health						;
					HealthMax		= Bot->HealthMax					; 
				} 

				if ( bVisible && Health > 0.0f )
				{ 
					float Frac = Health / (float)(HealthMax <= 0.0f ? 1.0f : HealthMax) ; 
					
					float Offset		= BorderWidth ;
					float Width			= BotHealthBarDim.X ;
					float Height		= BotHealthBarDim.Y ;
					DrawRect( BotHealthBarBackground , ScreenPos.X , ScreenPos.Y - HealthBarDrawOffsetY , Width , Height );
					DrawRect( BotHealthBarProgress , ScreenPos.X + Offset , ( ScreenPos.Y - HealthBarDrawOffsetY ) + Offset , ( Width * Frac ) - Offset * 2 , Height - Offset * 2 );
				}  
			}

		}

	}

}

void ABFGameHUD::Tick(float DeltaSeconds)
{ 
	Super::Tick(DeltaSeconds); 
	CurrentTime += DeltaSeconds; 
}

void ABFGameHUD::SpawnBonusText(FString  BonusText, FVector SpawnPositionWorld)
{
	FBonusTextAnim Anim;

	Anim.Color			= BonusTextColor	 ;
	Anim.StartPosition  = SpawnPositionWorld ;
	Anim.StartTime		= CurrentTime		 ;
	Anim.EndTime		= CurrentTime + 2.0f ;
	Anim.Text			= BonusText			 ;
	Anim.bDone			= false				 ;
	Anim.AnimCurve		= CreditsAnimCurve	 ;

	AnimatedTexts.Add(Anim); 
}

void ABFGameHUD::SpawnHeadShotAnim( FLinearColor Color, FVector SpawnPositionWorld, float Time)
{
	if (HeadShotCreditTex != NULL)
	{ 
		FBonusIconAnim	Anim;

		Anim.AnimType		 = EICON_ANIM_ALPHA | EICON_ANIM_SCALE ;
		Anim.Color			 = Color	;
		Anim.StartPosition	 = SpawnPositionWorld	; 
		Anim.StartTime		 = CurrentTime	;
		Anim.EndTime		 = Anim.StartTime + HeadShotAnimTime ;
		Anim.IconTexture	 = HeadShotCreditTex ;
		Anim.bDone			 = false ;
		Anim.AnimCurve		 = HeadShotZoomAnimCurve ;
		Anim.AnimTime		 = HeadShotAnimTime	;
		Anim.IconSize		 = HeadShotIconSize	;

		AnimatedIcons.Add(Anim);
	}
}
 
void ABFGameHUD::SpawnAnimatedCredits( ECreditType CreditType , FLinearColor Color , FVector SpawnPositionWorld , float Time )
{
	UTexture2D* Texture = NULL;

	for (int i = 0; i < CreditIcons.Num(); i++)
	{
		if (CreditIcons[i].CreditType == CreditType)
		{
			Texture = CreditIcons[i].IconTexture;
		}
	}
 
	if (Texture == NULL)return;
	  
	FBonusIconAnim	Anim;
	Anim.AnimType		 = EICON_ANIM_ALPHA | EICON_ANIM_Y_ANIM ;
	Anim.Color			 = Color ;
	Anim.StartPosition   = SpawnPositionWorld ;
	Anim.StartTime		 = CurrentTime ; 
	Anim.EndTime		 = Anim.StartTime + CreditsAnimTime;
	Anim.IconTexture	 = Texture ;
	Anim.bDone			 = false ;
	Anim.AnimCurve		 = HeadShotZoomAnimCurve ; 
	Anim.AnimTime		 = CreditsAnimTime ;
	Anim.IconSize		 = CreditIconSize ;

	AnimatedIcons.Add( Anim );
}

void ABFGameHUD::PlayReloadAnim(EWeaponMode Mode, float ReloadTime)
{
	UWorld* World = GetWorld();
	ReloadAnimStartTime = World->GetTimeSeconds();
	ReloadAnimTime		= ReloadTime;

	if ( Mode == EWEAPONMODE_MOUNTEDGUN ){ 
		bReloadingClip = true ; 
	}else if ( Mode == EWEAPONMODE_SHELL ){ 
		bReloadingShell = true ; 
	}
}

void ABFGameHUD::StopReloading()
{
	bReloadingClip  = false;
	bReloadingShell = false; 
}

void  ABFGameHUD::AddScratchMark( FVector2D RelativePosition , float RotationInDegree , float ScratchSize , FVector ActorLocation )
{ 
	ABFPlayerTank* PlayerTank = Cast<ABFPlayerTank>(GetOwningPlayerController()->GetPawn());
	FVector PlayerLocation;

	if (PlayerTank != NULL)
	{
		PlayerLocation = PlayerTank->GetActorLocation();
	}

	FVector DamageAlertDir = FVector(ActorLocation - PlayerLocation);
	FRotator Rotation = DamageAlertDir.ToOrientationRotator();
	float DamageAlertAngle = Rotation.Yaw;

	float UpperBodyAngle = PlayerTank->UpperBody->GetComponentRotation().Yaw;

	float DeltaAngle = FMath::Abs(UpperBodyAngle - (DamageAlertAngle - 90.0f));

	if (DeltaAngle < 60.0f)
	{
		float Frac = FMath::Clamp(DeltaAngle / 60.0f,0.2f,1.0f); 

		float Size = ScratchSize*(1.0f - Frac)*2.0f;
		FDamageScratch Scratch;

		Scratch.StartTime = GetWorld()->GetTimeSeconds();
		Scratch.ScreenLocation = RelativePosition - FVector2D(Size*0.5f, Size*0.5f);
		Scratch.Texture = DamageScratchTextures[FMath::RandRange(0, DamageScratchTextures.Num() - 1)];
		Scratch.TimeSpan = 3.0f;
		Scratch.ScratchSize = ViewportSizeY*Size;
		Scratch.FadeOutTime = 1.5f;

		DamageScratches.Add(Scratch);
	}
}



EDamageAlertSide ABFGameHUD::GetAlertSide( FVector DamageAlertLocation, float ActorRotationYaw)
{ 
	ABFPlayerTank* PlayerTank = Cast<ABFPlayerTank>(GetOwningPlayerController()->GetPawn());

	if (PlayerTank != NULL)
	{   
		FVector DamageAlertDir  = FVector(DamageAlertLocation - PlayerTank->VehicleBody->GetComponentLocation()) ; 
		
		FRotator Rotation		= DamageAlertDir.ToOrientationRotator();
		float DamageAlertAngle  = Rotation.Yaw ; 


		FString AngleStr = "DamageAngle : "  ; 
		AngleStr.AppendInt(DamageAlertAngle)	 ;
		 

		DamageAlertAngle = DamageAlertAngle - PlayerTank->GetActorRotation().Yaw;
		
		if (DamageAlertAngle < 0.0f)
			DamageAlertAngle = 360.0f + DamageAlertAngle;

		if (DamageAlertAngle<45.0f || DamageAlertAngle>(360.0f - 45.0f))
		{
			return EALERT_FRONT;
		}
		else if (DamageAlertAngle > 45.0f && DamageAlertAngle < (45.0f+90.0f))
		{
			return EALERT_RIGHT;
		}
		else if (DamageAlertAngle > 135.0f && DamageAlertAngle < 135.0f + 90.0f)
		{
			return EALERT_BACK;
		}
		else if (DamageAlertAngle > (135.0f + 90.0f) && DamageAlertAngle < (360.0f - 45.0f) )
		{
			return EALERT_LEFT;
		}

		AngleStr += TEXT ( "Normalized : " ) ;
		AngleStr.AppendInt(DamageAlertAngle)	 ;

		DrawText( AngleStr , FLinearColor::Green , 200.0f , 300.0f ) ;

	}

	return EALERT_FRONT;
}

void  ABFGameHUD::AddDamageAlert( FVector DamageAlertLocation , float ActorRotationYaw , float TriggerTime )
{ 
	ABFPlayerTank* PlayerTank = Cast<ABFPlayerTank>(GetOwningPlayerController()->GetPawn());
	FVector PlayerLocation;
	if (PlayerTank != NULL)
	{
		PlayerLocation = PlayerTank->GetActorLocation();
	}
	FVector DamageAlertDir = FVector(DamageAlertLocation - PlayerLocation);
	FRotator Rotation = DamageAlertDir.ToOrientationRotator();
	float DamageAlertAngle = Rotation.Yaw;

	float UpperBodyAngle = PlayerTank->UpperBody->GetComponentRotation().Yaw; 
	
	float DeltaAngle = FMath::Abs(UpperBodyAngle - (DamageAlertAngle - 90.0f));
	 
	if (DeltaAngle < 45.0f)
		return;

	if (DamageAlertAngle < 0.0f)
		DamageAlertAngle = 360.0f + DamageAlertAngle;


	FRadarAlert Alert;
	
	Alert.StartTime				= GetWorld()->GetTimeSeconds()	;
	Alert.TriggerTime			= TriggerTime			;	 
	Alert.DamageAlertLocation	= DamageAlertLocation	;
	Alert.bFadeOut				= false		;
	Alert.bTriggered			= true		;  
	Alert.AlertAngle			= DamageAlertAngle	; 
	Alert.Side					= GetAlertSide(DamageAlertLocation, ActorRotationYaw);
	int32 LocationIndex			= (int32)Alert.Side ; 
	
	RadarAlerts[LocationIndex]  = Alert ; 
}

void ABFGameHUD::DrawDamageScratch()
{ 
	float TimeSeconds = GetWorld()->GetTimeSeconds();
	
	if (ScratchMatDynamic == NULL)
	{
		ScratchMatDynamic = UMaterialInstanceDynamic::Create(ScratchMaterialInstance, this, FName("ScratchMaterialDynamic"));
	}
	if (ScratchMatDynamic!=NULL && IsValid(ScratchMatDynamic))
	{  
	
		for (int i = 0; i < DamageScratches.Num(); i++)
		{ 
			FDamageScratch& Scratch = DamageScratches[i];
			float Alpha = 1.0f;
			
			float DeltaTime = TimeSeconds - Scratch.StartTime;
			
			if (DeltaTime > Scratch.FadeOutTime)
			{
				float Frac = (DeltaTime-Scratch.FadeOutTime) / (Scratch.TimeSpan-Scratch.FadeOutTime);
		
				if (Frac > 1.0f)
				{
					Frac  = 1.0f;
					 
				}
				Alpha = 1.0f - Frac;
			}
			
			FLinearColor Color(1.0f, 0.0f, 0.0f,  Alpha);
			ScratchMatDynamic->SetTextureParameterValue( TEXT("ScratchTexture")  , Scratch.Texture ); 
			ScratchMatDynamic->SetVectorParameterValue ( TEXT("ScratchColor")	, Color );
			DrawMaterial(ScratchMatDynamic, ViewportSizeX*Scratch.ScreenLocation.X , ViewportSizeY*Scratch.ScreenLocation.Y , Scratch.ScratchSize , Scratch.ScratchSize , 0.0f , 0.0f , 1.0f , 1.0f  );
			
			if(Alpha == 0.0f )
				DamageScratches.RemoveAt(i);

		}

	} 

}

void ABFGameHUD::DrawAlerts()
{  

	ABFPlayerTank* PlayerTank = Cast<ABFPlayerTank>( GetOwningPlayerController()->GetPawn() );

	if ( PlayerTank != NULL )
	{ 
		float UpperBodyAngle	=	PlayerTank->UpperBody->GetComponentRotation().Yaw ;
		float TankBodyAngle		=	PlayerTank->GetActorTransform().Rotator().Yaw	; 

		if (UpperBodyAngle < 0.0f)
		{
			UpperBodyAngle = 360.0f + UpperBodyAngle;
		} 

		FString AngleText = "UpperBody : ";
		AngleText.AppendInt(UpperBodyAngle);

		AngleText += " Tank : ";
		AngleText.AppendInt(TankBodyAngle);

		//DrawText(AngleText, FLinearColor(0.0f, 1.0f, 0.0f, 1.0f), 200, 200, NULL); 
	
		float StartX	= (ViewportSizeX*0.5f) - (ViewportSizeY*0.3f) ;
		float StartY	= (ViewportSizeY*0.5f) - (ViewportSizeY*0.3f) ;

		float Width		= (ViewportSizeY*0.6f) ;
		float Height	= (ViewportSizeY*0.6f) ;    

		if ( PlayerTank != NULL )
		{
			float TimeSeconds = GetWorld()->GetTimeSeconds() ;

			for (int i = 0; i < 4; i++)
			{ 
				FRadarAlert& Alert = RadarAlerts[i]	;

				float Delta = TimeSeconds - Alert.StartTime ;

				if ( Alert.bTriggered && Delta > Alert.TriggerTime )
				{ 
				 
					float RenderAngle	= 0.0f;
					float RenderAlpha	= 1.0f;
					 

					RenderAngle = Alert.AlertAngle - UpperBodyAngle   ;

					if (RenderAngle < 0.0f )
					{
						RenderAngle = 360.0f + RenderAngle;
					}
					RenderAngle -= 90.0f; 

					if (!Alert.bFadeOut && Delta-Alert.TriggerTime > 1.0f)
					{
						Alert.bFadeOut = true;
						Alert.FadeStartTime = TimeSeconds;
					}

					if ( Alert.bFadeOut )
					{    
						float Frac = ( TimeSeconds - Alert.FadeStartTime ) / 0.5f ;      

						if ( Frac > 1.0f )
						{ 
							Frac = 1.0f;
							Alert.bFadeOut		= false ;
							Alert.bTriggered	= false ;
						} 

						RenderAlpha = 1.0f - Frac;  
					}

					DrawTexture( AlertnessTexture , StartX , StartY , Width , Height , 0.0f , 0.0f , 1.0f , 1.0f , FLinearColor( 1.0f , 1.0f , 1.0f , RenderAlpha ) , BLEND_Translucent , 1.0f , false , RenderAngle , FVector2D( 0.5f , 0.5f ) );

 				}
			}
		}
	}
}

#pragma once
#include "Vehicle/BFTankPlayerController.h"
#include "BFGameHUD.Generated.h"
 

UENUM(BlueprintType)
enum EDamageAlertSide
{
	EALERT_LEFT,
	EALERT_RIGHT,

	EALERT_FRONT,
	EALERT_BACK,
}; 

UENUM(BlueprintType)
enum ECreditType
{
	ECREDIT_5,
	ECREDIT_10,
	ECREDIT_15,
	ECREDIT_20,
}; 
 
#define 	EICON_ANIM_Y_ANIM	  1 << 1	 
#define 	EICON_ANIM_SCALE	  1 << 2	 
#define 	EICON_ANIM_ALPHA	  1 << 3	 
 
struct FBonusIconAnim
{
	int32		AnimType		   ;
	bool		   bDone           ;
	UTexture	 * IconTexture     ;

	FVector		   StartPosition   ;
	FVector2D      CurrentPosition ;
	float		   IconSize		   ;

	FLinearColor   Color		   ;
	float		   StartTime	   ;

	float		   EndTime		   ;
	float		   AnimTime		   ;
	UCurveFloat*   AnimCurve	   ;
}; 

struct FBonusTextAnim
{
	bool		  bDone			   ;
	FString		  Text			   ;

	FVector		  StartPosition	   ;
	FVector2D     CurrentPosition  ;
	
	FLinearColor  Color			   ;
	float		  StartTime		   ;
	
	float		  EndTime		   ;
	UCurveFloat*  AnimCurve		   ;
}; 

USTRUCT(BlueprintType)
struct FCreditType
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditDefaultsOnly, Category = "CreditType")
		TEnumAsByte<ECreditType> CreditType;
	UPROPERTY(EditDefaultsOnly, Category = "CreditType")
		UTexture2D*				 IconTexture;
};

struct FRadarAlert
{ 
	EDamageAlertSide Side		;
	bool  bTriggered			; 
	float StartTime				;
	float TriggerTime			; 
	bool  bFadeOut				; 
	float FadeStartTime			; 
	float AlertAngle			;
	FVector DamageAlertLocation ;
}; 

struct FDamageScratch
{
	float	  StartTime;
	float	  TimeSpan; 
	FVector2D ScreenLocation;
	UTexture* Texture;
	float	  ScratchSize;
	float	  FadeOutTime;
};

UCLASS(Blueprintable)
class TANKVSZOMBIE_API ABFGameHUD : public AHUD
{

	GENERATED_UCLASS_BODY()

	UPROPERTY(  EditDefaultsOnly , BlueprintReadWrite	, Category = "GameHUD" )
		FLinearColor		SelectionCrossHairColor ;
	UPROPERTY(	EditDefaultsOnly , BlueprintReadWrite	, Category = "GameHUD" )
		float				CrossHairMovementSpeed	;
	UPROPERTY(	EditDefaultsOnly , BlueprintReadWrite	, Category = "GameHUD" )
		float				CrossHairSize			;
	
	UPROPERTY(	EditDefaultsOnly , BlueprintReadWrite	, Category = "GameHUD" )
		float				CrossHairClipOffset		;

	UPROPERTY(	EditDefaultsOnly , BlueprintReadWrite	, Category = "GameHUD" )
		bool				bShowCrossHair			;
	UPROPERTY(	EditDefaultsOnly , BlueprintReadWrite	, Category = "GameHUD" )
		UTexture*			CrossHairTexture		; 

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite, Category = "GameHUD" )
		UTexture*			TurretCrossHairTexture	;
	UPROPERTY(	EditDefaultsOnly , BlueprintReadWrite	, Category = "GameHUD" )
		bool				bShowZoomOptics			;
	
 

	UPROPERTY( BlueprintReadWrite , Category = "GameHUD" )
		bool				bShowZoomOpticsMaxed	;
	UPROPERTY( BlueprintReadWrite , Category = "GameHUD" )
		float				TextScale				;
	UPROPERTY( BlueprintReadWrite , Category = "GameHUD" )
		bool				bShellCrossHair			;

	UPROPERTY( BlueprintReadWrite , Category = "GameHUD" )
		bool				bMachineGunCrossHair	;
	UPROPERTY( EditDefaultsOnly , Category = "GameHUD" )
		FLinearColor		HUDWidgetColor			;
	UPROPERTY( EditDefaultsOnly , Category = "GameHUD" )
		FLinearColor		HUDTextColor			;
	 

	UPROPERTY( BlueprintReadWrite , Category = "GameHUD" )
		bool				bShellReloading			;
	UPROPERTY( BlueprintReadWrite , Category = "GameHUD" )
		float				CannonRotation			;
	UPROPERTY(	BlueprintReadWrite , Category = "GameHUD" )
		bool				bShowTurretCrosshair	;

 
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = "GameHUD" )
		UTexture*			ZoomOpticsMaxedTexture	;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "GameHUD" )
		float				TextInterpTime			;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "GameHUD" )
		FLinearColor		BonusTextColor			;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameHUD")
		float				BonusTextSize ;

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = "GameHUD" )
		bool				bDrawHealthMeter		;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameHUD")
		bool				bDrawRadar;
	UPROPERTY( EditDefaultsOnly , Category = "GameHUD|Debug" )
		float				BorderWidth				;
	UPROPERTY( EditDefaultsOnly , Category = "GameHUD|Debug" )
		FVector2D			BotHealthBarDim			;

	UPROPERTY( EditDefaultsOnly , Category = "GameHUD|Debug" )
		FLinearColor		BotHealthBarBackground  ;
	UPROPERTY( EditDefaultsOnly , Category = "GameHUD|Debug" )
		FLinearColor		BotHealthBarProgress	;
	UPROPERTY( EditDefaultsOnly , Category = "GameHUD" )
		TArray<FCreditType> CreditIcons				;
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD")
		float				CreditIconSize;

	UPROPERTY(EditDefaultsOnly, Category = "GameHUD" )
		UCurveFloat		   *HeadShotZoomAnimCurve	; 
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD" )
		UCurveFloat		   *CreditsAnimCurve		;
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD")
		float				CreditsAnimTime			;

	UPROPERTY( EditDefaultsOnly , Category = "GameHUD" )
		UTexture*		    ReloadingAnimTex  ;

	UPROPERTY( EditDefaultsOnly , Category = "GameHUD" )
		UTexture*			HeadShotCreditTex ;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD")
		float				HeadShotAnimTime  ;
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD")
		float				HeadShotIconSize;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD|Radar")
		UMaterialInstanceDynamic* RadarMaterialInstance;

	UPROPERTY(EditDefaultsOnly, Category = "GameHUD|Radar")
		UTexture*			RadarBackground;
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD|Radar")
		UTexture*			RadarForeground;
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD|Radar")
		UTexture*			RadarSignal;
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD|Radar")
		UTexture*			RadarDirArrow;

	UPROPERTY(EditDefaultsOnly, Category = "GameHUD|Radar")
		UTexture*			BotPositionTexture;

	UPROPERTY(EditDefaultsOnly, Category = "GameHUD|Radar")
		float				RadarRange;
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD|Radar")
		FVector2D			RadarPositionRelative;
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD|Radar")
		float				RadarSizeRelative;
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD|Radar")
		float				RadarSignalStrength;


	UPROPERTY(EditDefaultsOnly, Category = "GameHUD|Waves")
		UTexture*			WaveStartTexture; 
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD|Waves")
		UTexture*			WaveEndingTexture; 


	UPROPERTY(EditDefaultsOnly , Category	= "GameHUD|Damage")
		TArray<UTexture*>   DamageScratchTextures; 
	UPROPERTY(EditDefaultsOnly, Category	= "GameHUD|Damage")
		UTexture*			AlertnessTexture;
	UPROPERTY(EditDefaultsOnly, Category = "GameHUD|Damage")
		UMaterialInstance* ScratchMaterialInstance;

public:

	UFUNCTION(BlueprintCallable, Category = "GameHUD")
		void DrawRadar();

	UFUNCTION( BlueprintCallable , Category = "GameHUD" )
		void HandleCrossHair( float X , float Y ) ; 

	UFUNCTION( BlueprintCallable , Category = "GameHUD" )
		virtual void DrawHUD() override ;
	  
	UFUNCTION( BlueprintCallable , Category = "GameHUD" )
		void DrawBotsHealthMeter();
	UFUNCTION( BlueprintCallable , Category = "GameHUD" )
		void DrawAnimatedTexts();
	UFUNCTION(BlueprintCallable, Category = "GameHUD" )
		void DrawAnimatedIcons();
	UFUNCTION( BlueprintCallable , Category = "GameHUD" )
		void DrawZoomOptics();
	UFUNCTION(BlueprintCallable, Category = "GameHUD" )
		void DrawCrossHairs();
	UFUNCTION(BlueprintCallable, Category = "GameHUD")
		void DrawDamageScratch();
	UFUNCTION(BlueprintCallable, Category = "GameHUD")
		void DrawAlerts();


	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION( BlueprintCallable , Category = "GameHUD" )
		void SpawnBonusText(FString  BonusText , FVector SpawnPositionWorld );

	UFUNCTION( BlueprintCallable , Category = "GameHUD" )
		void SpawnAnimatedCredits( ECreditType CreditType   , FLinearColor Color , FVector SpawnPositionWorld ,  float Time );
 

	UFUNCTION( BlueprintCallable , Category = "GameHUD" )
		void SpawnHeadShotAnim(   FLinearColor Color , FVector SpawnPositionWorld , float Time );

	UFUNCTION( BlueprintCallable , Category = "GameHUD" )
		void PlayReloadAnim( EWeaponMode Mode , float ReloadTime );

	UFUNCTION(BlueprintCallable, Category = "GameHUD")
		void StopReloading(); 

	UFUNCTION(BlueprintCallable, Category = "GameHUD")
		void AddScratchMark(FVector2D RelativePosition, float RotationInDegree,float RelativeSize, FVector ActorLocation);
	UFUNCTION(BlueprintCallable, Category = "GameHUD")
		void AddDamageAlert( FVector DamageAlertLocation , float ActorRotationYaw , float TriggerTime );

	//void DrawTextureAtlas(UVtaSlateTexture* TextureAtlas, float ScreenX, float ScreenY, float ScreenW, float ScreenH, FLinearColor Color, EBlendMode BlendMode, float Scale, bool bScalePosition, float Rotation, FVector2D RotPivot);
	EDamageAlertSide GetAlertSide( FVector DamageAlertLocation , float ActorRotationYaw );
	UPROPERTY()
		UMaterialInstanceDynamic* ScratchMatDynamic;

private:

	TArray<FBonusTextAnim>  AnimatedTexts		;
	TArray<FBonusIconAnim>  AnimatedIcons		;
	float					CurrentTime			;

	FVector2D				LastCrossHairPos	;
	FVector2D				CrossHairPos		;	
	bool					bReloadingShell		;

	float					ReloadAnimFrac		;
	float					ReloadAnimTime		; 
	float					ReloadAnimStartTime ;

	float					SignalRotation	; 
	bool					bReloadingClip  ;
	TArray<AActor*>			OutPawns		;

	TArray<FVector>			PawnPositions	; 
	FRadarAlert				RadarAlerts[4]	;
	TArray<FDamageScratch>  DamageScratches	; 

	float					ViewportSizeX	;
	float					ViewportSizeY	;

};


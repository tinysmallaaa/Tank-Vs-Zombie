#pragma once 
#include "BFWeaponAttachment.h"
#include "BFMountedWeaponAttachment.Generated.h"
 
UCLASS( Blueprintable , Abstract )	
class TANKVSZOMBIE_API UBFMountedWeaponAttachment : public UBFWeaponAttachment
{  
	GENERATED_UCLASS_BODY()  
public: 
	
	UPROPERTY( EditAnywhere , Category = "MountedWeapon" )
		FName				CameraMountSocket		;
	UPROPERTY( EditAnywhere , Category = "MountedWeapon" )
		float				CameraJitterStrength	;

	UPROPERTY( EditAnywhere	, Category = "MountedWeapon" )
		float				WeaponYawMin			;
	UPROPERTY( EditAnywhere , Category = "MountedWeapon" )
		float				WeaponYawMax			;
	UPROPERTY( EditAnywhere	, Category = "MountedWeapon" )
		float				WeaponPitchMin			;

	UPROPERTY( EditAnywhere		  , Category = "MountedWeapon" )
		float				WeaponPitchMax			;
	UPROPERTY( BlueprintReadWrite , Category = "MountedWeapon" )
		float				WeaponPitch				;
	UPROPERTY( BlueprintReadWrite , Category = "MountedWeapon" )
		float				WeaponYaw				;

	UPROPERTY( EditAnywhere		  , Category = "MountedWeapon" )
		bool				bCreateChildCamera	; 
 
	UFUNCTION( BlueprintImplementableEvent , Category = "MountedWeapon" )
		void OnEquipped()	;
	UFUNCTION( BlueprintImplementableEvent , Category = "MountedWeapon" )
		void OnUnequipped() ;
	void UpdateViewRotation( float DeltaYaw , float DeltaPitch ) ;

	virtual void Tick( float DeltaSeconds ) override	;
	virtual void BeginPlay() override	; 	  
	virtual void ControlCamera( float DeltaSeconds , float Yaw , float Pitch ) ;

	virtual UCameraComponent* GetAttachedCamera() { return GetAttachedWeapon()->CameraComponent ; } 
	virtual void PostFireEffects() override ;

};

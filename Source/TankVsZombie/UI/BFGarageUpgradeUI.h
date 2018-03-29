#pragma once
#include "BFUserWidget.h"
#include "BFGarageUpgradeUI.Generated.h"


USTRUCT(BlueprintType)
struct FBFWeaponUpgradeUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "WeaponUpgradeUIData")
		FText DisplayName;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponUpgradeUIData")
		FText Decription;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponUpgradeUIData")
		TEnumAsByte<EAttachedWeaponUpgrade> UpgradeType;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponUpgradeUIData")
		FText DisplayAmount;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponUpgradeUIData")
		FText DisplayMaxAmount; 

};
 
USTRUCT(BlueprintType)
struct FBFWeaponInfoUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		FName	Name;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		FText	DisplayName;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		FText	Description; 
	
	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		TSubclassOf<ABFWeapon> WeaponClass;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		TArray<FBFWeaponUpgradeUIData> UpgradeUIDataList;
	
	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		FWeaponDBInfo Info;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		FSavedVehicleWeaponState SavedState;

};

USTRUCT(BlueprintType)
struct FBFVehicleUpgradeUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "WeaponUpgradeUIData")
		FText DisplayName;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponUpgradeUIData")
		FText Decription;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponUpgradeUIData")
		TEnumAsByte<EVehicleUpgrade> UpgradeType;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponUpgradeUIData")
		FText DisplayAmount;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponUpgradeUIData")
		FText DisplayMaxAmount;  

};

USTRUCT(BlueprintType)
struct FBFVehicleInfoUIData
{
	GENERATED_USTRUCT_BODY()	

	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		FName	Name ;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		TSubclassOf<ABFGarageTank> GarageVehicleClass;


	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		FText	DisplayName ;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		FText	Description ;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		bool    blocked ;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		FText	UnlockText ;
	
	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		bool    bPurchased;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		FText   PurchaseText; 

	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		TArray<FBFVehicleUpgradeUIData> UpgradeUIDataList ;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		FVehicleDBInfo Info ;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponInfoUIData")
		FSavedPlayerVehicleState SavedState ; 
};
 



UCLASS(Blueprintable)
class UBFGarageUpgradeUI : public UBFUserWidget
{
	GENERATED_UCLASS_BODY()
public:

	UFUNCTION( BlueprintCallable , Category = "BFGarageUpgradeUI" )
		ABFGaragePlayerController* GetGaragePlayerController();

	UFUNCTION( BlueprintCallable , Category = "BFGarageUpgradeUI" )
		int32 GetAvailbleCredits();
	UFUNCTION(BlueprintCallable, Category = "BFGarageUpgradeUI")
		bool NotEnoughCredits(int32 RequiredAmount);

	UFUNCTION( BlueprintCallable , Category = "BFGarageUpgradeUI")
		FWeaponDBInfo  GetWeaponInfo( FName Name );
	UFUNCTION( BlueprintCallable, Category = "BFGarageUpgradeUI")
		FVehicleDBInfo GetVehicleInfo( FName Name );

	UFUNCTION( BlueprintCallable , Category = "BFGarageUpgradeUI")
		FWeaponDBInfo  GetWeaponInfoForIndex( int32 Index );
	UFUNCTION( BlueprintCallable , Category = "BFGarageUpgradeUI")
		FVehicleDBInfo GetVehicleInfoForIndex( int32 Index );

	UFUNCTION( BlueprintCallable , Category = "BFGarageUpgradeUI")
		FBFWeaponInfoUIData  GetWeaponInfoUIData( int32 Index );
	UFUNCTION( BlueprintCallable , Category = "BFGarageUpgradeUI")
		FBFVehicleInfoUIData GetVehicleInfoUIData( int32 Index );
 
	UFUNCTION(BlueprintCallable, Category = "BFGarageUpgradeUI")
		int32 GetMaxVehiclesCount() ;
	UFUNCTION(BlueprintCallable, Category = "BFGarageUpgradeUI")
		int32 GetMaxWeaponsCount()  ; 

	UFUNCTION( BlueprintCallable , Category = "BFGarageUpgradeUI")
		bool CanUpgradeWeaponLevel( FWeaponDBInfo Info , EAttachedWeaponUpgrade UpgradeType , int32 Level ) ;
	UFUNCTION( BlueprintCallable , Category = "BFGarageUpgradeUI")
		bool CanUpgradeVehicleLevel( FVehicleDBInfo Info , EVehicleUpgrade UpgradeType , int32 Level ) ; 


};

#pragma once
#include "BFUserWidget.h"
#include "BFCasinoSlotWidget.Generated.h" 

UENUM(BlueprintType)
enum ESlotType
{
	ESLOT_HEALTH_KIT		,
	ESLOT_CREDIT_LARGE		,
	ESLOT_CREDIT_SMALL		,
	ESLOT_GRENADE			,
	ESLOT_MINE				,
	ESLOT_SHELL_CARTRIDGE	,
	ESLOT_BOOSTER			,
};

UCLASS(Blueprintable, Abstract)
class UBFCasinoSlotWidget : public UBFUserWidget
{
	GENERATED_UCLASS_BODY()
public: 

	UPROPERTY( EditDefaultsOnly  , Category = "CasinoSlotWidget" )
		UMaterialInstanceDynamic* Material ;
	UPROPERTY( EditDefaultsOnly  , Category = "CasinoSlotWidget" )
		float Speed	;
	UPROPERTY( EditDefaultsOnly  , Category = "CasinoSlotWidget" )
		float Friction ;
	UPROPERTY( EditDefaultsOnly	 , Category = "CasinoSlotWidget" )
		float Height ;

	UFUNCTION( BlueprintCallable , Category = "CasinoSlotWidget" )
		void UpdateSpin( float Dir , float DeltaSeconds ) ;

	
private:

	float VOrigin;
	float VCenter;
	float Velocity;
	float Deccleration;

};

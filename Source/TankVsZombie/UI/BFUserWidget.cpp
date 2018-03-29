#include "TankVsZombie.h"
#include <Slate/SGameLayerManager.h>
#include <Widgets/SOverlay.h>
#include "BFUserWidget.h"

UBFUserWidget::UBFUserWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

APlayerController* UBFUserWidget::GetPlayerController()
{
	return Cast<APlayerController>( GetWorld()->GetFirstPlayerController() ); 
}

UBFGameSaveData* UBFUserWidget::GetGameSaveData()
{
	UBFLocalPlayer* LocalPlayer = GetLocalPlayer();

	if (LocalPlayer != NULL)
	{
		return LocalPlayer->GetGameSaveData();
	}

	return NULL;
}

UBFConfigDBUtil* UBFUserWidget::GetGameConfigDB()
{
	UBFLocalPlayer* LocalPlayer = GetLocalPlayer();

	if (LocalPlayer != NULL)
	{
		return LocalPlayer->ConfigDB;
	}

	return NULL;
}

UBFLocalPlayer* UBFUserWidget::GetLocalPlayer()
{
	return Cast<UBFLocalPlayer>(GetPlayerController()->GetLocalPlayer());
}

int32 UBFUserWidget::GetParentZOrder(ULocalPlayer* LocalPlayer)
{
	SGameLayerManager* GameLayerManager = (SGameLayerManager*)( GEngine->GameViewport->GetGameLayerManager().Get() );

	if ( GameLayerManager  && LocalPlayer && GameLayerManager->PlayerLayers.Num() > 0 )
	{
		// not for split screen 
		TSharedPtr<SGameLayerManager::FPlayerLayer> LayerPtr = GameLayerManager->PlayerLayers[LocalPlayer];

		if ( LayerPtr.IsValid() && LayerPtr->Widget.IsValid() )
		{
			TPanelChildren<SOverlay::FOverlaySlot>* SlotPanel =   ( TPanelChildren<SOverlay::FOverlaySlot>* )( LayerPtr->Widget->GetChildren() )  ;
			if ( SlotPanel->Num() > 0 )
			{
				const  SOverlay::FOverlaySlot& OverlaySlot =  ((*SlotPanel)[SlotPanel->Num() - 1 ]);

				return OverlaySlot.ZOrder ;
			} 
		}
	}

	return 1 ;

}
 
void UBFUserWidget::PushToViewport( UBFUserWidget* Parent , int32 UIZOrder )
{	 
	ULocalPlayer* LocalPlayer = GetWorld()->GetFirstPlayerController()->GetLocalPlayer()  ;

	if ( Parent != NULL )
	{
		ZOrder = UIZOrder + Parent->ZOrder + 1	; 
	}else
	{ 
		ZOrder = UIZOrder + GetParentZOrder( LocalPlayer ) + 1	; 
	}

	SetInputActionPriority( ZOrder ) ;
	Super::AddToViewport( ZOrder ) ; 
}

FAnchors UBFUserWidget::GetBound() const 
{ 
	const FGeometry& Geom = this->GetCachedGeometry() ;

	FVector2D min =  Geom.GetAbsolutePosition()   ;
	FVector2D max =  min + Geom.GetAbsoluteSize() ;

	return FAnchors( min.X , min.Y , max.X , max.Y ) ; 
}


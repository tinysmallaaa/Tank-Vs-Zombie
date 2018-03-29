#include "TankVsZombie.h"
#include "BFPlaceableItemController.h"

ABFPlaceableItemController::ABFPlaceableItemController( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{

}

void ABFPlaceableItemController::OnDragStart( APlayerController* PC , float ScreenX , float ScreenY )
{
	if ( PC != NULL )
	{

		FVector StartPosition  ;
		FVector WorldDirection ;
		UGameplayStatics::DeprojectScreenToWorld( PC , FVector2D( ScreenX , ScreenY ) , StartPosition , WorldDirection );

		FVector EndPosition ;
		EndPosition = StartPosition + WorldDirection * 20000.0f ;

		FHitResult OutResult ;
		FCollisionQueryParams QueryParams ;
		QueryParams.bTraceComplex = true  ;

		FCollisionResponseParams	ResponseParams( ECR_Block );

		if (GetWorld()->LineTraceSingleByChannel( OutResult , StartPosition , EndPosition , ECollisionChannel::ECC_Visibility , QueryParams , ResponseParams ))
		{
			if ( OutResult.IsValidBlockingHit() )
			{

			 
				if ( !bInteractionStarted  )
				{
					CreatePlaceableItem()	;
					OnInteractionStarted()	;
					bInteractionStarted = true ;

					if ( GetGadgetItem() )
					{
						GetGadgetItem()->AdjustOnDrag( OutResult.ImpactPoint , OutResult.ImpactNormal , false );
					}
				}
			}
		}

	}
}

void ABFPlaceableItemController::CreatePlaceableItem()
{
	if (PlaceableItemClass != NULL)
	{
	 
		PlaceableItem = UGameplayStatics::BeginDeferredActorSpawnFromClass( this , PlaceableItemClass , GetTransform() );
			
		UGameplayStatics::FinishSpawningActor( PlaceableItem , GetTransform() );
	}
}

void ABFPlaceableItemController::OnDragEnd( APlayerController* PC , float ScreenX , float ScreenY )
{
	
	if ( PC != NULL )
	{

		FVector StartPosition	;
		FVector WorldDirection	;
		UGameplayStatics::DeprojectScreenToWorld( PC , FVector2D( ScreenX , ScreenY ) , StartPosition , WorldDirection ) ;

		FVector EndPosition	;
		EndPosition = StartPosition + WorldDirection * 20000.0f ;

		FHitResult OutResult ;
		FCollisionQueryParams QueryParams ;
		QueryParams.bTraceComplex = true ;

		FCollisionResponseParams ResponseParams( ECR_Block );

		if ( GetWorld()->LineTraceSingleByChannel( OutResult , StartPosition , EndPosition , ECollisionChannel::ECC_Visibility , QueryParams , ResponseParams ) )
		{
			if ( OutResult.IsValidBlockingHit() )
			{ 
				if (GetGadgetItem())
				{
					GetGadgetItem()->AdjustOnDrag( OutResult.ImpactPoint , OutResult.ImpactNormal , true );
				}
			}
		}

	}

}

void ABFPlaceableItemController::OnDragMove( APlayerController* PC , float ScreenX , float ScreenY )
{

	if ( PC != NULL )
	{

		FVector StartPosition  ;
		FVector WorldDirection ;
		UGameplayStatics::DeprojectScreenToWorld( PC , FVector2D( ScreenX , ScreenY ) , StartPosition , WorldDirection );

		FVector EndPosition	 ;
		EndPosition = StartPosition + WorldDirection * 20000.0f ;

		FHitResult OutResult ;
		FCollisionQueryParams QueryParams ;
		QueryParams.bTraceComplex = true  ;

		FCollisionResponseParams	ResponseParams( ECR_Block ) ;

		if ( GetWorld()->LineTraceSingleByChannel( OutResult, StartPosition , EndPosition , ECollisionChannel::ECC_Visibility , QueryParams , ResponseParams ))
		{
			if ( OutResult.IsValidBlockingHit() )
			{
				if ( GetGadgetItem() )
				{
					GetGadgetItem()->AdjustOnDrag( OutResult.ImpactPoint , OutResult.ImpactNormal , false );
				}
			}
		}

	}

}
	
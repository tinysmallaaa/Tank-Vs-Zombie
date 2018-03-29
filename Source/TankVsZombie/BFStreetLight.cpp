#include "TankVsZombie.h"
#include "BFStreetLight.h"


ABFStreetLight::ABFStreetLight(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	MeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, FName("StreetPole"));
	RootComponent = MeshComponent;

	BillboardComponent = ObjectInitializer.CreateDefaultSubobject<UMaterialBillboardComponent>(this, FName("Corona"));
	BillboardComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

}

void ABFStreetLight::BeginPlay()
{
	Super::BeginPlay();
	

	GetWorldTimerManager().SetTimer(VisibilityTimer, this, &ABFStreetLight::CheckVisibility, 0.5f, false);
}

void ABFStreetLight::CheckVisibility()
{
	
	FVector EyePosition = GetWorld()->GetFirstPlayerController()->GetViewTarget()->GetActorLocation(); 
	FVector BillboardPosition = BillboardComponent->GetComponentLocation();  

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FTraceDelegate TraceDelegate;
	TraceDelegate.BindUObject( this , &ABFStreetLight::TraceResult );

	FCollisionResponseParams ResponseParams;
	CurTraceHandle = GetWorld()->AsyncLineTraceByChannel( EAsyncTraceType::Single ,  BillboardPosition , EyePosition , ECC_Visibility , QueryParams , ResponseParams , &TraceDelegate );

}

void ABFStreetLight::TraceResult(const FTraceHandle& TraceHandle, FTraceDatum & TraceData)
{
	if (TraceData.OutHits.Num() > 0)
	{
		if (IsValid(BillboardComponent))
		{
			BillboardComponent->SetVisibility(false);
		}
	}
	else
	{
		if (IsValid(BillboardComponent))
		{
			BillboardComponent->SetVisibility(true);
		} 
	}

	CurTraceHandle = FTraceHandle();
	GetWorldTimerManager().SetTimer( VisibilityTimer , this , &ABFStreetLight::CheckVisibility , 1.0f , false );
}

void ABFStreetLight::BeginDestroy()
{
	Super::BeginDestroy(); 
	 
}
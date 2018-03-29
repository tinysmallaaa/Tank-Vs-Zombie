#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PawnCMD.generated.h"

class UAnimSequenceBase;
class USkeletalMeshComponent;


USTRUCT(BlueprintType)
struct FPawnCMDParam
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = PawnCMDParam )
		FName Name; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PawnCMDParam)
		int32  Integer		;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PawnCMDParam)
		bool  BooleanFlag	;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = PawnCMDParam )
		float Scalar		; 

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = PawnCMDParam )
		FVector Vector		;
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PawnCMDParam )
		FColor Color		; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PawnCMDParam )
		class UMaterialInterface* Material	;

	FPawnCMDParam()
		: Name(NAME_None)
		, Integer(0)
		, BooleanFlag(false)
		, Scalar(0) 
		, Vector(ForceInit) 
		, Color(ForceInit) 
		, Material(NULL)  
	{

	}
	 

};


UENUM(BlueprintType)
enum EPawnCMD
{
	PAWN_CMD_FIRE_SHELL,
	PAWN_CMD_FIRE_GUN,
	PAWN_CMD_FIRE_GUN2, 

	PAWN_CMD_FIRE_SHELL_STOP,
	PAWN_CMD_FIRE_GUN_STOP,
	PAWN_CMD_FIRE_GUN2_STOP,

	PAWN_CMD_ZOMBIE_SPIT,
};


UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "PawnCMD"))
class  UAnimNotify_PawnCMD : public UAnimNotify
{
	GENERATED_BODY()

public:

	UAnimNotify_PawnCMD();

	// Begin UAnimNotify interface
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	// End UAnimNotify interface

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
		TEnumAsByte<EPawnCMD> Command;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particles)
		TArray<FPawnCMDParam> Parameters;
};

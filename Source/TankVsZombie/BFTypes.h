#pragma once
#include "BFTypes.Generated.h"


static FName NAME_MapInfo_Title(TEXT("Title"));
static FName NAME_MapInfo_Author(TEXT("Author"));
static FName NAME_MapInfo_Description(TEXT("Description"));
static FName NAME_Vignettes(TEXT("Vignettes"));
static FName NAME_FullScreenMovieModule(TEXT("FullScreenMovie"));

USTRUCT(BlueprintType)
struct FTextureUVs
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureUVs")
		float U;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureUVs")
		float V;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureUVs")
		float UL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureUVs")
		float VL;

	FTextureUVs()
		: U(0.0f)
		, V(0.0f)
		, UL(0.0f)
		, VL(0.0f)
	{
	}

	FTextureUVs(float inU, float inV, float inUL, float inVL)
	{
		U = inU; V = inV; UL = inUL;  VL = inVL;
	}

};

USTRUCT(BlueprintType)
struct FDecalData
{
	GENERATED_USTRUCT_BODY()

		/** material */
		UPROPERTY(EditDefaultsOnly, Category = "Decal")
		UMaterial* DecalMaterial;

	/** quad size (width & height) */
	UPROPERTY(EditDefaultsOnly, Category = "Decal")
		float DecalSize;

	/** lifespan */
	UPROPERTY(EditDefaultsOnly, Category = "Decal")
		float LifeSpan;

	/** defaults */
	FDecalData()
		: DecalSize(256.f)
		, LifeSpan(10.f)
	{
	}
};


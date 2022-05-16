#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HexWorldDataAsset.generated.h"

UCLASS()
class HEXWORLDRUNTIME_API UHexWorldDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Hexagon location"))
	FIntVector Location;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Hexagon Type"))
	FString Type = TEXT("0000-0000-0000-0000");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Hexagon Data"))
	TMap<FString, FString> Data;
	
};

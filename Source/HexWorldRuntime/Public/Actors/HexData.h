#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HexData.generated.h"

/**
 * 
 */
UCLASS()
class HEXWORLDRUNTIME_API UHexData : public UObject
{
	GENERATED_BODY()

public:
	UHexData() {};

	UPROPERTY(EditAnywhere, Category = HexWorld, meta = (DisplayName = "Type"))
	FString Type;

	UPROPERTY(EditAnywhere, Category = HexWorld, meta = (DisplayName = "Type"))
	FIntVector Location;
	
	UPROPERTY(EditAnywhere, Category = HexWorld, meta = (DisplayName = "Data"))
	TMap<FString, FString> Data;
	
};

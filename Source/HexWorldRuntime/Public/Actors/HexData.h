#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HexData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HEXWORLDRUNTIME_API UHexData : public UObject
{
	GENERATED_BODY()

public:
	UHexData() {};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = HexWorld, meta = (DisplayName = "Type"))
	FString Type;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = HexWorld, meta = (DisplayName = "Type"))
	FIntVector Location;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = HexWorld, meta = (DisplayName = "Data"))
	TMap<FString, FString> GlobalData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = HexWorld, meta = (DisplayName = "Data"))
	TMap<FString, FString> LocalData;
};

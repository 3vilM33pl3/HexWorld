// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = HexWorld, meta = (DisplayName = "Location"))
	FIntVector Location;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = HexWorld, meta = (DisplayName = "Global Data"))
	TMap<FString, FString> GlobalData;

	// TODO: Multimap can not be used in blueprints, so we need to find a way to make this work
	// UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = HexWorld, meta = (DisplayName = "Local Data"))
	TMultiMap<FString, FString> LocalData;
};

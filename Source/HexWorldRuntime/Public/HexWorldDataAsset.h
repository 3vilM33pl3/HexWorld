// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HexWorldDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FLocalHexagon
{
	GENERATED_BODY()

public:
	// TODO: MultiMap can not be used in blueprint, so we need to find a way to make this work
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Meta Data"))
	TMultiMap<FString, FString> Data;	
};

UCLASS(BlueprintType)
class HEXWORLDRUNTIME_API UHexWorldDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Hexagon location"))
	FIntVector Location;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Hexagon Type"))
	FString Type = TEXT("0000-0000-0000-0000");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Global Data"))
	TMap<FString, FString> GlobalData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Local Data"))
	TMap<FGuid, FLocalHexagon> LocalData;

	UFUNCTION(BlueprintCallable)
	static int CombinePair(int X, int Y)
	{
		const int XX = X >= 0 ? X*2 : X * -2 - 1;
		const int YY = Y >= 0 ? Y * 2 : Y * -2 * -1;
		return (XX >= YY ? (XX * XX + XX + YY) : (YY * YY + XX));
	}  

	UFUNCTION(BlueprintCallable)
	FVector2D SplitToPair(int z) 
	{
		float Sqrtz = floor(sqrt(z));
		const float Sqz = Sqrtz * Sqrtz;
		std::pair<int, int> result = ((z - Sqz) >= Sqrtz) ? std::pair(Sqrtz, z - Sqz - Sqrtz) : std::pair(z - Sqz, Sqrtz);
		float Xx = result.first % 2 == 0 ? result.first / 2 : (result.first + 1) / -2;
		float Yy = result.second % 2 == 0 ? result.second / 2 : (result.second + 1) / -2;
		return FVector2D{Xx, Yy};
	} 
	
};

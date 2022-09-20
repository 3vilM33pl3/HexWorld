#pragma once

#include "CoreMinimal.h"
#include "Pairing.generated.h"

UCLASS(BlueprintType)
class HEXWORLDRUNTIME_API UPairing final: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Pairing", DisplayName="Signed Pairing")
	static int64 Pair(int64 X, int64 Y);

	UFUNCTION(BlueprintCallable, Category = "Pairing", DisplayName="Signed UnPairing")
	static void UnPair(int64 S, int64& X, int64& Y);
	
private:
	static uint64 SignedTransform(int64 X);

	static int64 UnsignedTransform(uint64 X);
	
	static uint64 SzudzikPairing(uint64 X, uint64 Y);
	
	static void SzudzikUnPairing(uint64 S, int64& X, int64& Y);
	
};

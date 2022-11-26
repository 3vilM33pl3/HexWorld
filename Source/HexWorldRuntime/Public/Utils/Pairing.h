// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pairing.generated.h"

/**
 * Class with pairing functions which maps two signed integers to a single, unique integer.
 * It uses the Szudzik function to map two integers to a single integer. The Szudzik function
 * can only map positive integers, so we first map the signed integers to unique unsigned integers
 * and vice versa using zigzag encoding. Zigzag encoding maps negative integers to even numbers and
 * positive integers to odd numbers. 
 */
UCLASS(BlueprintType)
class HEXWORLDRUNTIME_API UPairing final: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Maps two signed integers to a single, unique integer.
	 * @param X The first integer.
	 * @param Y The second integer.
	 * @return The unique integer.
	 */
	UFUNCTION(BlueprintCallable, Category = "Pairing", DisplayName="Signed Pairing")
	static int64 Pair(int64 X, int64 Y);

	/**
	 * Maps a single, unique integer to two signed integers.
	 * @param S The unique integer.
	 * @param X The first integer.
	 * @param Y The second integer.
	 */
	UFUNCTION(BlueprintCallable, Category = "Pairing", DisplayName="Signed UnPairing")
	static void UnPair(int64 S, int64& X, int64& Y);
	
private:
	// maps signed integer to a single, unique integer
	static uint64 SignedTransform(int64 X);

	// maps unsigned integer to a unique signed integer
	static int64 UnsignedTransform(uint64 X);

	// pairs two unsigned integers to a single, unique unsigned integer
	static uint64 SzudzikPairing(uint64 X, uint64 Y);

	// maps a single, unique unsigned integer to two unsigned integers
	static void SzudzikUnPairing(uint64 S, int64& X, int64& Y);
	
};

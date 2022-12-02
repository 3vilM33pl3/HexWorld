// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#include "Utils/Pairing.h"


uint64 UPairing::SzudzikPairing(const uint64 X, const uint64 Y)
{
	return X >= Y ? X * X + X + Y : X + Y * Y;
}

void UPairing::SzudzikUnPairing(const int64 S, int64& X, int64& Y)
{
	const int64 b = static_cast<int64>(floor(sqrt(static_cast<float>(S))));
	const int64 a = S - b * b;

	if(a < b)
	{
		X = a;
		Y = b;
	}
	else
	{
		X = b;
		Y = a - b;
	}
	
}

int64 UPairing::Pair(const int64 X, const int64 Y)
{
	const uint64 x = SignedTransform(X);
	const uint64 y = SignedTransform(Y);

	return SzudzikPairing(x, y);
	
}

void UPairing::UnPair(const int64 S, int64& X, int64& Y)
{
	int64 x, y;
	SzudzikUnPairing(S, x, y);

	X = UnsignedTransform(x);
	Y = UnsignedTransform(y);
}

uint64 UPairing::SignedTransform(const int64 X)
{
	return (X < 0) ? static_cast<uint64>((-2 * X) - 1) : static_cast<uint64>(X * 2);	
}

int64 UPairing::UnsignedTransform(const uint64 X)
{
	return (X & 1) ? static_cast<int64>((X / -2) - 1) : static_cast<int64>(X / 2);
}

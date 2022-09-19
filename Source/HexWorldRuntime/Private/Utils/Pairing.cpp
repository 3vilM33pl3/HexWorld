#include "Utils/Pairing.h"


uint64 UPairing::SzudzikPairing(const uint64 X, const uint64 Y)
{
	return X >= Y ? X * X + X + Y : X + Y * Y;
}

void UPairing::SzudzikUnPairing(const uint64 S, int64& X, int64& Y)
{
	const int64 b = (int64)floor(sqrt(S));
	const int64 a = S - b * b;

	if(a > b)
	{
		X = a - b;
		Y = b;
	}
	else
	{
		X = b;
		Y = a;
	}
	
}

int64 UPairing::Pair(int64 X, int64 Y)
{
	uint64 x = SignedTransform(X);
	uint64 y = SignedTransform(Y);

	return SzudzikPairing(x, y);
	
}

void UPairing::UnPair(int64 S, int64& X, int64& Y)
{
	int64 x, y;
	SzudzikUnPairing(S, x, y);

	X = SignedTransform(x);
	Y = SignedTransform(y);
}

uint64 UPairing::SignedTransform(int64 X)
{
	return (X < 0) ? (uint64)(-X) * 2 - 1 : (uint64)X * 2;	
}

int64 UPairing::UnsignedTransform(uint64 X)
{
	return (X & 1) ? -(int64)(X / 2) - 1 : (int64)(X / 2);
}

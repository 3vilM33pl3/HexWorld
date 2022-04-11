#pragma once

#include "CoreMinimal.h"
#include "hexworld/hex_lib.h"

class HEXWORLDEDITOR_API FHexWorldClient
{
public:
	FHexWorldClient();
	~FHexWorldClient();

	TArray<Hexagon> GetHexagonRing(Hexagon* center, const int64_t Radius, bool bFill);
	
};

// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HexagonMap.h"
#include "UObject/Object.h"
#include "UHexWorldSubsysten.generated.h"

/**
 * 
 */
UCLASS()
class HEXWORLDRUNTIME_API UUHexWorldSubsysten : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void DestroyAllHexagonActors();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UHexagonMap* GetHexagonMap() const { return HexagonMap; }

private:
	UHexagonMap* HexagonMap;

};

// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HexagonMap.h"
#include "UHexWorldSubsysten.generated.h"

/**
 * Unreal Engine subsystem for HexagonMap. 
 */
UCLASS()
class HEXWORLDRUNTIME_API UUHexWorldSubsysten : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Destroys all Actors with tag "Hexagon".
	 */
	void DestroyAllHexagonActors();

	/**
	 * Initializes the subsystem. Creates a HexagonMap and retrieves the map data
	 * from the backend server. 
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * Cleanup of the subsystem before it is removed.
	 */
	virtual void Deinitialize() override;

	/**
	 * Returns the HexagonMap.
	 */
	UHexagonMap* GetHexagonMap() const { return HexagonMap; }

private:
	UHexagonMap* HexagonMap;

};

// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WaterBodyRiverActor.h"
#include "Actors/HexData.h"
#include "Containers/CircularQueue.h"
#include "hexworld/hex_lib.h"
#include "HexagonMap.generated.h"

class HexagonClient;
UCLASS()
class HEXWORLDRUNTIME_API UHexagonMap : public UObject
{
	GENERATED_BODY()

public:
	UHexagonMap();

	/**
	 * Get hexagon map data from server and put them on a circular queue.
	 * @param deprecated bClearMap remove all assets with a tag "Hexagon" from the map.
	 */
	UFUNCTION(BlueprintCallable, Category = Options)
	void RetrieveMap(bool bClearMap);

	/**
	 * Get hexagon map data from circular queue and use it tp populate the map.
	 */ 
	UFUNCTION(BlueprintCallable, Category = Options)
	void PopulateMap();

	/**
	 * Add a river to the map, folowing the path set by the gates. 
	 */
	UFUNCTION(BlueprintCallable, Category = Options)
	void AddRiver(FString Name, int PointPosition);

	UPROPERTY(EditAnywhere, Category = Options, meta = (DisplayName = "Use this location as center of the map"))
	FIntVector CenterLocation = FIntVector{0,0,0};

	UPROPERTY(EditAnywhere, Category = Options, meta = (DisplayName = "Diameter of retrieved area"))
	int Diameter = 1;

	UPROPERTY(EditAnywhere, Category = Connection, meta = (DisplayName = "Backend server address"))
	FString Address = "localhost:8080";
	
	UPROPERTY(EditAnywhere, Category = Connection, meta = (DisplayName = "Secure connection over SSL"))
	bool bSecure = false;

	TCircularQueue<UHexData*>* HexCoordData = new TCircularQueue<UHexData*>(120);

	static FVector HexToLocation(const FIntVector* Location, const int Size);

protected:
	TSoftObjectPtr<UMaterialInterface> WaterMaterial;
	
private:
	HexagonClient* HexagonClient;
	AWaterBodyRiver* RiverActor;
};

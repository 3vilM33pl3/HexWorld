// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Navigation/NavigationGate.h"
#include "Hexagon.generated.h"

UENUM()
enum class EHexagonDirection : uint8
{
	CENTER = 0 UMETA(DisplayName "Center"),
	N = 1 UMETA(DisplayName = "North"),
	NE = 2 UMETA(DisplayName = "NorthEast"),
	E = 3 UMETA(DisplayName = "East"),
	SE = 4 UMETA(DisplayName = "SouthEast"),
	S = 5 UMETA(DisplayName = "South"),
	SW = 6 UMETA(DisplayName = "SouthWest"),
	W = 7 UMETA(DisplayName = "West"),
	NW = 8 UMETA(DisplayName = "NorthWest")
	
};

UCLASS(BlueprintType)
class HEXWORLDRUNTIME_API AHexagon : public AActor
{
	GENERATED_BODY()

public:
	AHexagon();

	UPROPERTY(BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Static mesh component"))
	UStaticMeshComponent* HexagonMeshComponent;

	UPROPERTY(BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Static mesh"))
	UStaticMesh* HexMesh;

	UPROPERTY(BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Hexagon location"))
	FIntVector Location;

	UPROPERTY(BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Hexagon coordinates"))
	FIntVector Coordinates;
	
	UPROPERTY(BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Hexagon Direction"))
	EHexagonDirection Direction;

	UPROPERTY(BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Hexagon Type"))
	FString Type = TEXT("0000-0000-0000-0000");

	UPROPERTY(BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Hexagon Data"))
	TMap<FString, FString> Data;

	UPROPERTY(BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Hexagon Navigation Gate"))
	ANavigationGate* NavigationGate;
	
	void SetTypeAndDirection(FString Type, EHexagonDirection NewDirection);

	static EHexagonDirection ConvertDirection(FString* Direction);
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	FIntVector CubeToWorld(FIntVector CubeLocation, const int Size) const;
};

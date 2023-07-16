// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Navigation/NavigationGate.h"
#include "Hexagon.generated.h"

UENUM()
enum class EHexagonDirection : uint8
{
	CENTER UMETA(DisplayName = "Center"),
	N UMETA(DisplayName = "North"),
	NE UMETA(DisplayName = "NorthEast"),
	E UMETA(DisplayName = "East"),
	SE UMETA(DisplayName = "SouthEast"),
	S UMETA(DisplayName = "South"),
	SW UMETA(DisplayName = "SouthWest"),
	W UMETA(DisplayName = "West"),
	NW UMETA(DisplayName = "NorthWest"),
	
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

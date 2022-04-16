#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "hexworld/hex_lib.h"
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
class HEXWORLDEDITOR_API AHexagon : public AActor
{
	GENERATED_BODY()

public:
	AHexagon();

	UPROPERTY(EditAnywhere, Category = HexWorld, meta = (DisplayName = "Static mesh component"))
	UStaticMeshComponent* HexagonMeshComponent;

	UPROPERTY(EditAnywhere, Category = HexWorld, meta = (DisplayName = "Static mesh"))
	UStaticMesh* HexMesh;

	UPROPERTY(EditAnywhere, Category = HexWorld, meta = (DisplayName = "Hexagon location"))
	FIntVector Location;

	UPROPERTY(BlueprintReadOnly, Category = HexWorld, meta = (DisplayName = "Hexagon coordinates"))
	FVector Coordinates;
	
	UPROPERTY(EditAnywhere, Category = HexWorld, meta = (DisplayName = "Hexagon Direction"))
	EHexagonDirection Direction;

	UPROPERTY(EditAnywhere, Category = HexWorld, meta = (DisplayName = "Hexagon Type"))
	FString Type = TEXT("0000-0000-0000-0000");

	void SetTypeAndDirection(FString Type, EHexagonDirection NewDirection);

	static EHexagonDirection ConvertDirection(::Direction Direction);
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	FIntVector CubeToWorld(FIntVector CubeLocation, const int Size) const;
};

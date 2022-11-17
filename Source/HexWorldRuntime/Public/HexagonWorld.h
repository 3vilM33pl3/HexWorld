// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UHexWorldSubsysten.h"
#include "GameFramework/Actor.h"
#include "HexagonWorld.generated.h"

UCLASS()
class HEXWORLDRUNTIME_API AHexagonWorld : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHexagonWorld();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector HexToLocation(const FIntVector* Location, int Size) const;
	UUHexWorldSubsysten* HexWorldSubsysten;
	UHexagonMap* HexagonMap;
};

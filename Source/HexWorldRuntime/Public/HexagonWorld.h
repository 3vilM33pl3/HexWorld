// Fill out your copyright notice in the Description page of Project Settings.

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
	FVector HexToLocation(const UHexData* Hex, int Size) const;
	UUHexWorldSubsysten* HexWorldSubsysten;
	UHexagonMap* HexagonMap;
};

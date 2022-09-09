// Fill out your copyright notice in the Description page of Project Settings.

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
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	AHexagonMap* GetHexagonMap() const { return HexagonMap; }

private:
	AHexagonMap* HexagonMap;

};

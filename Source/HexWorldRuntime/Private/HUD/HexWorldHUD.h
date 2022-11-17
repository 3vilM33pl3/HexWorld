// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UObject/Object.h"
#include "HexWorldHUD.generated.h"

/**
 * 
 */
UCLASS()
class AHexWorldHUD : public AHUD
{
	GENERATED_BODY()

public:
	AHexWorldHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

private:
	UTexture2D* Logo3vilM33pl3;

};

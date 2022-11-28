// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Navigation/NavigationGate.h"
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
	
	void SetNextGateLabel(ATextRenderActor* ALocationLabel);

private:
	UTexture2D* Logo3vilM33pl3;
	UTexture2D* NarrowboatTexture;
	ATextRenderActor* LocationLabel;

};

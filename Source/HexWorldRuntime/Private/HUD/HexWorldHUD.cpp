// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.


#include "HexWorldHUD.h"

#include "CanvasItem.h"
#include "Engine/Canvas.h"
#include "UObject/ConstructorHelpers.h"

AHexWorldHUD::AHexWorldHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> LogoAsset(TEXT("/HexWorld/Textures/Hexagon_Jungle_T"));
	Logo3vilM33pl3 = LogoAsset.Object;
	
}

void AHexWorldHUD::DrawHUD()
{
	Super::DrawHUD();
	
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	const FVector2D CrosshairDrawPosition( (Center.X), (Center.Y + 20.0f));

	FCanvasTileItem TileItem( CrosshairDrawPosition, Logo3vilM33pl3->Resource, FLinearColor::White);
    TileItem.BlendMode = SE_BLEND_Translucent;
    Canvas->DrawItem( TileItem );
    
}

void AHexWorldHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AHexWorldHUD::SetNextGateLabel(ATextRenderActor* LocationLabel)
{
	this->LocationLabel = LocationLabel;
}

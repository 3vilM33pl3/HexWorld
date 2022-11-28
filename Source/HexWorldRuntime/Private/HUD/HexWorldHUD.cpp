// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.


#include "HexWorldHUD.h"

#include "CanvasItem.h"
#include "Engine/Canvas.h"
#include "UObject/ConstructorHelpers.h"

AHexWorldHUD::AHexWorldHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> LogoAsset(TEXT("/HexWorld/Textures/Hexagon_Jungle_T"));
	Logo3vilM33pl3 = LogoAsset.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> NarrowboatAsset(TEXT("/HexWorld/Textures/NarrowboatHUD"));
	NarrowboatTexture = NarrowboatAsset.Object;
	
}

void AHexWorldHUD::DrawHUD()
{
	Super::DrawHUD();
	
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	const FVector2D LogoDrawPosition( 0.0f, 0.0f);

	FCanvasTileItem TileItem( LogoDrawPosition, Logo3vilM33pl3->Resource, FLinearColor::White);
    TileItem.BlendMode = SE_BLEND_Translucent;
	TileItem.Size = FVector2D( Logo3vilM33pl3->GetSizeX() / 4.0f , Logo3vilM33pl3->GetSizeY() / 4.0f );
	TileItem.
    Canvas->DrawItem( TileItem );

	float Sizing = 2.0f;
	const FVector2D NarrowboatDrawPosition( Canvas->ClipX * 0.5f - (NarrowboatTexture->GetSizeX() / 2) / Sizing, Canvas->ClipY - NarrowboatTexture->GetSizeY() / Sizing );
	FCanvasTileItem NarrowboatTileItem( NarrowboatDrawPosition, NarrowboatTexture->Resource, FLinearColor::White);
	NarrowboatTileItem.BlendMode = SE_BLEND_Translucent;
	NarrowboatTileItem.Size = FVector2D( NarrowboatTexture->GetSizeX() / Sizing , NarrowboatTexture->GetSizeY() / Sizing );
	Canvas->DrawItem( NarrowboatTileItem);     
}

void AHexWorldHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AHexWorldHUD::SetNextGateLabel(ATextRenderActor* ALocationLabel)
{
	this->LocationLabel = ALocationLabel;
}

// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#include "HexWorldEditHexTool.h"
#include "InteractiveToolManager.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "HexWorldEditHexTool"

UInteractiveTool* UHexWorldEditHexToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UHexWorldEditHexTool* NewTool = NewObject<UHexWorldEditHexTool>(SceneState.ToolManager);
	
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}

UHexWorldEditHexTool::UHexWorldEditHexTool()
{
}

void UHexWorldEditHexTool::Setup()
{
	Super::Setup();

	Properties = NewObject<UHexWorldEditMapProperties>(this);
	AddToolPropertySource(Properties);
}

void UHexWorldEditHexTool::SetWorld(UWorld* World)
{
	this->TargetWorld = World;	
}

#undef LOCTEXT_NAMESPACE
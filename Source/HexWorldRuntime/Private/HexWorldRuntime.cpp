// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#include "HexWorldRuntime.h"

#include "Engine/RendererSettings.h"

#define LOCTEXT_NAMESPACE "HexWorldRuntime"



void FHexWorldRuntime::StartupModule()
{
	IModuleInterface::StartupModule();

	UE_LOG(LogTemp, Log, TEXT("Enabling Virtual Textures (UDIM)"));
	URendererSettings* Settings = GetMutableDefault<URendererSettings>();
	Settings->bVirtualTextures = true;
	Settings->SaveConfig();
	
}

void FHexWorldRuntime::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FHexWorldRuntime, HexWorldRuntime)
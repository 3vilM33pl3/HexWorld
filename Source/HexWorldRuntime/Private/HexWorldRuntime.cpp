// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#include "HexWorldRuntime.h"

#define LOCTEXT_NAMESPACE "HexWorldRuntime"



void FHexWorldRuntime::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FHexWorldRuntime::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FHexWorldRuntime, HexWorldRuntime)
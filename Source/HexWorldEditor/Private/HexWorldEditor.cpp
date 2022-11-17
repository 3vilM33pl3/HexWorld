// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#include "HexWorldEditor.h"

#include "HexWorldCommands.h"


#define LOCTEXT_NAMESPACE "HexWorldEditorModule"

void FHexWorldEditor::StartupModule()
{
	FHexWorldCommands::Register();	
}

void FHexWorldEditor::ShutdownModule()
{
	FHexWorldCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FHexWorldEditor, HexWorldEditorEdMode)
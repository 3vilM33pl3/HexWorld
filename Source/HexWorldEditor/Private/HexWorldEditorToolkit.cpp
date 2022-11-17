// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#include "HexWorldEditorToolkit.h"

#define LOCTEXT_NAMESPACE "HexWorldToolkit"

FHexWorldEditorToolkit::FHexWorldEditorToolkit()
{
}

void FHexWorldEditorToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

FName FHexWorldEditorToolkit::GetToolkitFName() const
{
	return FName("HexWorldEditorMode");	
}

FText FHexWorldEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "HexWorldEditorMode Toolkit");
}

void FHexWorldEditorToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}

#undef LOCTEXT_NAMESPACE
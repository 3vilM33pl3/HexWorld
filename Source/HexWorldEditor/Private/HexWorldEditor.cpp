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
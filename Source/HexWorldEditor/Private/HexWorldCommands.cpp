#include "HexWorldCommands.h"


#define LOCTEXT_NAMESPACE "FHexWorldCommands"

FHexWorldCommands::FHexWorldCommands() : TCommands<FHexWorldCommands>("HexWorld",
		LOCTEXT("ContextDescription", "HexWorld Editor"), 
		NAME_None, 
		GetStyleSetName()
		)
{
}

void FHexWorldCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);
	
	UI_COMMAND(HexWorldRetrieveMap, "Retrieve Map", "Retrieves map info from server", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(HexWorldRetrieveMap);

	UI_COMMAND(HexWorldEditHex, "Edit Hexagon", "Retrieves map info from server", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(HexWorldEditHex);
	
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FHexWorldCommands::GetCommands()
{
	return FHexWorldCommands::Get().Commands;
}


#undef LOCTEXT_NAMESPACE

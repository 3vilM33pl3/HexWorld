#include "HexWorldEditorMode.h"
#include "HexWorldCommands.h"
#include "HexWorldEditorToolkit.h"
#include "InteractiveToolManager.h"

#include "Tools/HexWorldEditHexTool.h"
#include "Tools/HexWorldRetrieveMapTool.h"

#define LOCTEXT_NAMESPACE "HexWorldEdMode"

const FEditorModeID UHexWorldEditorMode::EM_HexWorldModeID = TEXT("EM_HexWorldModeID");

FString UHexWorldEditorMode::RetrieveMapToolName = TEXT("HexWorldt_RetrieveMapTool");
FString UHexWorldEditorMode::EditHexToolName = TEXT("HexWorld_EditHexTool");


UHexWorldEditorMode::UHexWorldEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	Info = FEditorModeInfo(UHexWorldEditorMode::EM_HexWorldModeID,
		LOCTEXT("ModeName", "HexWorld"),
		FSlateIcon(),
		true);
}

void UHexWorldEditorMode::Enter()
{
	Super::Enter();

	const FHexWorldCommands& ToolCommands = FHexWorldCommands::Get();

	RegisterTool(ToolCommands.HexWorldRetrieveMap, RetrieveMapToolName, NewObject<UHexWorldRetrieveMapToolBuilder>(this));
	RegisterTool(ToolCommands.HexWorldEditHex, EditHexToolName, NewObject<UHexWorldEditHexToolBuilder>(this));

	GetToolManager()->SelectActiveToolType(EToolSide::Left, TEXT("HexWorldt_RetrieveMapTool"));
	GetToolManager()->ActivateTool(EToolSide::Left);
	
}

void UHexWorldEditorMode::Exit()
{
	Super::Exit();
}

void UHexWorldEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FHexWorldEditorToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UHexWorldEditorMode::GetModeCommands() const
{
	return FHexWorldCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE


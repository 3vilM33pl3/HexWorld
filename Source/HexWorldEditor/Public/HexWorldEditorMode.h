#pragma once

#include "CoreMinimal.h"
#include "Tools/UEdMode.h"
#include "HexWorldEditorMode.generated.h"

UCLASS()
class HEXWORLDEDITOR_API UHexWorldEditorMode : public UEdMode
{
	GENERATED_BODY()

public:
	const static FEditorModeID EM_HexWorldModeID;

	UHexWorldEditorMode();
	
	static FString RetrieveMapToolName;
	static FString EditHexToolName;
	
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	virtual void CreateToolkit() override;
public:
	virtual TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> GetModeCommands() const override;
	
};

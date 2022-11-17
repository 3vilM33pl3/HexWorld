// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseTools/SingleClickTool.h"
#include "Components/Image.h"
#include "HexWorldEditHexTool.generated.h"

UCLASS()
class UHexWorldEditHexToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()
public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; };
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};


UCLASS(Transient)
class HEXWORLDEDITOR_API UHexWorldEditMapProperties : public UInteractiveToolPropertySet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Options, meta = (DisplayName = "Hexagon location"))
	FIntVector HexLocation;

	UPROPERTY(EditAnywhere, Category = Options, meta = (DisplayName = "Hexagon type"))
	FString Type;
};


UCLASS()
class HEXWORLDEDITOR_API UHexWorldEditHexTool : public USingleClickTool
{
	GENERATED_BODY()

public:
	UHexWorldEditHexTool();

	virtual void Setup() override;
	virtual void SetWorld(UWorld* World);

protected:
	UPROPERTY()
	TObjectPtr<UHexWorldEditMapProperties> Properties;
	
	UWorld* TargetWorld;
};

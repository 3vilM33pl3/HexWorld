#pragma once

#include "CoreMinimal.h"
#include "InteractiveToolBuilder.h"
#include "BaseTools/SingleClickTool.h"
#include "Containers/CircularQueue.h"
#include "hexworld/hex_lib.h"
#include "HexWorldRetrieveMapTool.generated.h"


UCLASS()
class HEXWORLDEDITOR_API UHexWorldRetrieveMapToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; };
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};

UCLASS(Transient)
class HEXWORLDEDITOR_API UHexWorldRetrieveMapProperties : public UInteractiveToolPropertySet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = Options, meta = (DisplayName = "Use this location as center of the map"))
	FIntVector CenterLocation = FIntVector{0,0,0};

	UPROPERTY(EditAnywhere, Category = Options, meta = (DisplayName = "Diameter of retrieved area"))
	int Diameter = 1;

	UPROPERTY(EditAnywhere, Category = Options, meta = (DisplayName = "Clear existing map before retrieving"))
	bool bClearMap;

	UPROPERTY(EditAnywhere, Category = Connection, meta = (DisplayName = "Backend server address"))
	FString Address = "localhost:8080";
	
	UPROPERTY(EditAnywhere, Category = Connection, meta = (DisplayName = "Secure connection over SSL"))
	bool bSecure = false;

	UFUNCTION(CallInEditor, Category = Options)
	void RetrieveMap();
	
	TCircularQueue<Hexagon>* HexCoordData = new TCircularQueue<Hexagon>(12);

};

UCLASS()
class HEXWORLDEDITOR_API UHexWorldRetrieveMapTool : public USingleClickTool
{
	GENERATED_BODY()

public:
	UHexWorldRetrieveMapTool();

	virtual void Setup() override;
	virtual void SetWorld(UWorld* World);
	
	
protected:
	UPROPERTY()
	TObjectPtr<UHexWorldRetrieveMapProperties> Properties;
	
	UWorld* TargetWorld;

private:
	virtual void OnTick(float DeltaTime) override;
	FVector HexToLocation(const Hexagon* Hex, const int Size) const;
	
};


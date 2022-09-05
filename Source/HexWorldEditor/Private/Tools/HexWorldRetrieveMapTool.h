#pragma once

#include "CoreMinimal.h"
#include "InteractiveToolBuilder.h"
#include "Actors/Hexagon.h"
#include "Actors/HexData.h"
#include "BaseTools/SingleClickTool.h"
#include "Containers/CircularQueue.h"
#include "hexworld/hex_client.h"
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

	UFUNCTION(CallInEditor, Category = Options)
	void RetrieveMap();
	
	UPROPERTY(EditAnywhere, Category = Connection, meta = (DisplayName = "Backend server address"))
	FString Address = "localhost:8080";
	
	UPROPERTY(EditAnywhere, Category = Connection, meta = (DisplayName = "Secure connection over SSL"))
	bool bSecure = false;

	UFUNCTION(CallInEditor, Category= Connection)
	void TestConnection();
	
	TCircularQueue<UHexData*>* HexCoordData = new TCircularQueue<UHexData*>(24);

private:
	HexagonClient* HexagonClient;

};

UCLASS()
class HEXWORLDEDITOR_API UHexWorldRetrieveMapTool : public UInteractiveTool
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
	FVector HexToLocation(const UHexData* Hex, const int Size) const;
	
};


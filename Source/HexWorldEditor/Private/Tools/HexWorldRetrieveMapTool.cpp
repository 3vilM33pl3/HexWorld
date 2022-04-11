#include "HexWorldRetrieveMapTool.h"
#include "InteractiveToolManager.h"
#include "Comms/HexWorldRunnable.h"
#include "Engine/World.h"
#include "hexworld/hex_client.h"

#define LOCTEXT_NAMESPACE "HexWorldRetrieveMapTool"

UInteractiveTool* UHexWorldRetrieveMapToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UHexWorldRetrieveMapTool* NewTool = NewObject<UHexWorldRetrieveMapTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;

}

void UHexWorldRetrieveMapProperties::RetrieveMap()
{
	UE_LOG(LogTemp, Log, TEXT("Retrieve map called"));
	

	FHexWorldRunnable::RunLambdaOnBackgroundThread(HexCoordData, [&]
	{
		HexagonClient* HexagonClient = new ::HexagonClient(std::string(TCHAR_TO_UTF8(*Address)), bSecure);
		const auto Status = HexagonClient->ConnectToServer();

		if(Status != HEXWORLD_CONNECTION_READY)
		{
			UE_LOG(LogTemp, Warning, TEXT("Error connecting to server"));
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Connected to server"));

		Hexagon* Center = new Hexagon(CenterLocation.X, CenterLocation.Y, CenterLocation.Z, "", Direction::N);
		const std::vector<Hexagon> HexCV = HexagonClient->GetHexagonRing(Center, Diameter, true);
		for(int i=0; i< HexCV.size(); i++)
		{
			HexCoordData->Enqueue(HexCV[i]);
		}
		
	});
}

UHexWorldRetrieveMapTool::UHexWorldRetrieveMapTool()
{
}

void UHexWorldRetrieveMapTool::Setup()
{
	Super::Setup();

	Properties = NewObject<UHexWorldRetrieveMapProperties>(this);
	AddToolPropertySource(Properties);
}

void UHexWorldRetrieveMapTool::SetWorld(UWorld* World)
{
	this->TargetWorld = World;
}

void UHexWorldRetrieveMapTool::OnTick(float DeltaTime)
{
	if(!Properties->HexCoordData->IsEmpty())
	{
		Hexagon* Hex = new Hexagon(0,0,0,"", Direction::N);
		if(Properties->HexCoordData->Dequeue(*Hex))
		{
			UE_LOG(LogTemp, Display, TEXT("[%d, %d, %d ]"), Hex->X, Hex->Y, Hex->Z);
		}
	}	
}

#undef LOCTEXT_NAMESPACE

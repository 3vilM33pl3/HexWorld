#include "HexWorldRetrieveMapTool.h"
#include "InteractiveToolManager.h"
#include "Actors/Hexagon.h"
#include "Comms/HexWorldRunnable.h"
#include "Engine/World.h"
#include "hexworld/hex_client.h"
#include "Kismet/GameplayStatics.h"

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

	if(bClearMap)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHexagon::StaticClass(), FoundActors);

		for(auto& Hex: FoundActors)
		{
			Hex->Destroy();
		}
		
	}
	
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
			const FVector Location = HexToLocation(Hex, 1500);
			const FRotator Rotation(0.0f, 0.0f, 0.0f);

			
			AHexagon* HexTemplate = NewObject<AHexagon>(this);
			HexTemplate->Coordinates = Location;
			HexTemplate->Location = FIntVector(Hex->X, Hex->Y, Hex->Z);
			HexTemplate->Type = UTF8_TO_TCHAR(Hex->Type.c_str());

			FActorSpawnParameters Parameters;
			Parameters.Template = HexTemplate;
			GetWorld()->SpawnActor<AHexagon>(Location, Rotation, Parameters);

		}
	}	
}

FVector UHexWorldRetrieveMapTool::HexToLocation(const Hexagon* Hex, const int Size) const
{
	double x = Size * (sqrt(3.0) * Hex->X + sqrt(3.0)/2.0 * Hex->Y);
	double y = Size * (3.0/2.0 * Hex->Y);
	return FVector(x, y, 0);	
}

#undef LOCTEXT_NAMESPACE

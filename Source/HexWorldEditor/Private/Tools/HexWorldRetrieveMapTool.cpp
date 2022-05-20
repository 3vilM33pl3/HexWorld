#include "HexWorldRetrieveMapTool.h"

#include "FileHelpers.h"
#include "HexWorldDataAsset.h"
#include "InteractiveToolManager.h"
#include "Actors/Hexagon.h"
#include "Actors/HexData.h"
#include "Comms/HexWorldRunnable.h"
#include "Engine/World.h"
#include "hexworld/hex_client.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

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
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Hexagon"), FoundActors);

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

		Hexagon* Center = new Hexagon(CenterLocation.X, CenterLocation.Y, CenterLocation.Z, "");
		const std::vector<Hexagon> HexCV = HexagonClient->GetHexagonRing(Center, Diameter, true);
		for(int i=0; i< HexCV.size(); i++)
		{
			UHexData* HexData = NewObject<UHexData>();
			HexData->Location.X = HexCV[i].X;
			HexData->Location.Y = HexCV[i].Y;
			HexData->Location.Z = HexCV[i].Z;
			HexData->Type = FString(HexCV[i].Type.c_str());
			for(auto const&[ key, value]: HexCV[i].LocalData)
			{
				HexData->LocalData.Add(FString(key.c_str()), FString(value.c_str()));
			}
			for(auto const&[ key, value]: HexCV[i].GlobalData)
			{
				HexData->GlobalData.Add(FString(key.c_str()), FString(value.c_str()));
			}
			UE_LOG(LogTemp, Display, TEXT("Enqueue %d [%d, %d, %d ] %s"), i, HexCV[i].X, HexCV[i].Y, HexCV[i].Z, *FString(HexCV[i].Type.c_str()));
			HexCoordData->Enqueue(HexData);
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
		UHexData* HexData;
		if(Properties->HexCoordData->Dequeue(HexData))
		{
			const FString Type(HexData->Type);
			EHexagonDirection Direction = EHexagonDirection::N;
			if(HexData->LocalData.Contains("direction"))
			{
				Direction = AHexagon::ConvertDirection(HexData->LocalData.Find("direction"));
			} 
			
			const FVector Location = HexToLocation(HexData, 1500);
			//const FRotator Rotation(0.0f, 60.0f * (static_cast<std::underlying_type_t<EHexagonDirection>>(Direction) - 1), 0.0f);

			UE_LOG(LogTemp, Display, TEXT("[%d, %d, %d ] %s"), HexData->Location.X, HexData->Location.Y, HexData->Location.Z, *Type);

			FString BluePrintName("/HexWorld/");
			
			BluePrintName = BluePrintName.Append(Type).Append("/BP_").Append(Type).Append(".BP_").Append(Type);
			
			UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(),NULL, *BluePrintName));

			UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
						
			if (!SpawnActor)
			{
				UE_LOG(LogTemp, Display, TEXT("Blueprint %s not found"), *BluePrintName);
				return;
			}
			
			UClass* SpawnClass = SpawnActor->StaticClass();
			if (SpawnClass == NULL)
			{
				UE_LOG(LogTemp, Display, TEXT("Blueprint %s StaticClass == NULL"), *BluePrintName);
				return;
			}

			FString DataAssetName("/HexWorld/");
			DataAssetName = DataAssetName.Append(Type).Append("/DA_").Append(Type).Append(".DA_").Append(Type);
			UHexWorldDataAsset* HexDataAsset = Cast<UHexWorldDataAsset>(StaticLoadObject(UObject::StaticClass(),NULL, *DataAssetName));
			
//			HexDataAsset->LocalData.Add(UHexWorldDataAsset::CombinePair(HexData->Location.X, HexData->Location.Y), FLocalHexagon{HexData->LocalData}); 
			
			for(auto data : HexData->GlobalData)
			{
				HexDataAsset->GlobalData.Add(data.Key, data.Value);	
			}
			HexDataAsset->Location = HexData->Location;
			HexDataAsset->Type = HexData->Type;
						
			FActorSpawnParameters SpawnParameters;
			FGuid guid = FGuid::NewGuid();
			
			SpawnParameters.OverrideActorGuid = guid;
			HexDataAsset->LocalData.Add(guid, FLocalHexagon{HexData->LocalData}); 
			
			AActor* HexActor = GetWorld()->SpawnActor<AActor>(GeneratedBP->GeneratedClass, Location, FRotator{}, SpawnParameters);
			HexActor->Modify(true);
			HexActor->Tags.Add("Hexagon");
			
			
		}
	}	
}

FVector UHexWorldRetrieveMapTool::HexToLocation(const UHexData* Hex, const int Size) const
{
	const double x = Size * (sqrt(3.0) * Hex->Location.X + sqrt(3.0)/2.0 * Hex->Location.Y);
	const double y = Size * (3.0/2.0 * Hex->Location.Y);
	return FVector(x, y, 0);	
}

#undef LOCTEXT_NAMESPACE

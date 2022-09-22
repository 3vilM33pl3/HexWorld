#include "HexWorldRetrieveMapTool.h"

#include "HexWorldDataAsset.h"
#include "InteractiveToolManager.h"
#include "UHexWorldSubsysten.h"
#include "Actors/Hexagon.h"
#include "Actors/HexData.h"
#include "Comms/HexWorldRunnable.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Engine/World.h"
#include "hexworld/hex_client.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/NavigationGate.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#include "Utils/Pairing.h"

#define LOCTEXT_NAMESPACE "HexWorldRetrieveMapTool"

UInteractiveTool* UHexWorldRetrieveMapToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UHexWorldRetrieveMapTool* NewTool = NewObject<UHexWorldRetrieveMapTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;

}

void UHexWorldRetrieveMapProperties::RetrieveMap()
{
	if(bClearMap)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Hexagon"), FoundActors);

		for(AActor* Hex: FoundActors)
		{
			Hex->Destroy();
		}
		
	}
	
	HexagonMap->RetrieveMap(bClearMap);

}

void UHexWorldRetrieveMapProperties::ClearMap()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Hexagon"), FoundActors);

	for(AActor* Hex: FoundActors)
	{
		Hex->Destroy();
	}
}

void UHexWorldRetrieveMapProperties::TestConnection()
{
	// if(!HexagonClient)
	// {
	// 	HexagonClient = new ::HexagonClient(std::string(TCHAR_TO_UTF8(*Address)), bSecure);
	// }
	//
	// auto State = HexagonClient->GetConnectionState();
	//
	// switch (State)
	// {
	// case hw_conn_state::HEXWORLD_CONNECTION_IDLE:
	// 	UE_LOG(LogTemp, Warning, TEXT("Connection idle"));
	// 	break;
	// case hw_conn_state::HEXWORLD_CONNECTION_READY:
	// 	UE_LOG(LogTemp, Warning, TEXT("Connection ready"));
	// 	break;
	// }
}

void UHexWorldRetrieveMapProperties::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if(PropertyChangedEvent.Property != nullptr)
	{
		if(PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UHexWorldRetrieveMapProperties, bShowLabels))
		{
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATextRenderActor::StaticClass(), FoundActors);

			for(AActor* Actor: FoundActors)
			{
				ATextRenderActor* TextLabel = Cast<ATextRenderActor>(Actor);
				if(TextLabel->ActorHasTag(FName("HexagonLabel")))
				{
					TextLabel->GetTextRender()->SetVisibility(bShowLabels);
				}
			}
		}
	}
}

UHexWorldRetrieveMapTool::UHexWorldRetrieveMapTool()
{
}

void UHexWorldRetrieveMapTool::Setup()
{
	Super::Setup();

	Properties = NewObject<UHexWorldRetrieveMapProperties>(this);
	Properties->HexagonMap = NewObject<UHexagonMap>(this);
	
	AddToolPropertySource(Properties);
	
}

void UHexWorldRetrieveMapTool::SetWorld(UWorld* World)
{
	this->TargetWorld = World;
}

void UHexWorldRetrieveMapTool::AddLabel(const FIntVector* Location) const
{
	const FVector WorldLocation = HexToLocation(Location, 1500.0f);

	const FVector TextLocation = FVector(WorldLocation.X, WorldLocation.Y, WorldLocation.Z + 1000);
	ATextRenderActor* Text = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), TextLocation, FRotator(0.f, 90.f, 0.f));

	const FString LocationStr = FString::Printf(TEXT("(%d) - [%d,%d,%d]"), UPairing::Pair(Location->X, Location->Y),Location->X, Location->Y, Location->Z);
	Text->GetTextRender()->SetText(FText::FromString(LocationStr));
	Text->GetTextRender()->SetTextRenderColor(FColor::White);
	Text->SetActorScale3D(FVector(5.f, 5.f, 5.f));
	Text->Tags.Add(FName("HexagonLabel"));
	Text->Tags.Add(FName("Hexagon"));
	
}

void UHexWorldRetrieveMapTool::OnTick(float DeltaTime)
{
		
	if(!Properties->HexagonMap->HexCoordData->IsEmpty())
	{
		UHexData* HexData;
		if(Properties->HexagonMap->HexCoordData->Dequeue(HexData))
		{
			const FString Type(HexData->Type);
			EHexagonDirection Direction = EHexagonDirection::N;
			if(HexData->LocalData.Contains("direction"))
			{
				Direction = AHexagon::ConvertDirection(HexData->LocalData.Find("direction"));
			} 
			
			const FVector WorldLocation = HexToLocation(&HexData->Location, 1500);
			const FRotator Rotation(0.0f, 60.0f * (static_cast<std::underlying_type_t<EHexagonDirection>>(Direction) - 1), 0.0f);

			UE_LOG(LogTemp, Display, TEXT("[%d, %d, %d ] %s"), HexData->Location.X, HexData->Location.Y, HexData->Location.Z, *Type);

			FString BluePrintName("/HexWorld/");
			
			BluePrintName = BluePrintName.Append(Type).Append("/BP_").Append(Type).Append(".BP_").Append(Type);
			
			UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), nullptr, *BluePrintName));

			UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
						
			if (!SpawnActor)
			{
				UE_LOG(LogTemp, Display, TEXT("Blueprint %s not found"), *BluePrintName);
				return;
			}

			if (UClass* SpawnClass = SpawnActor->StaticClass(); SpawnClass == nullptr)
			{
				UE_LOG(LogTemp, Display, TEXT("Blueprint %s StaticClass == NULL"), *BluePrintName);
				return;
			}

			FString DataAssetName("/HexWorld/");
			DataAssetName = DataAssetName.Append(Type).Append("/DA_").Append(Type).Append(".DA_").Append(Type);
			UHexWorldDataAsset* HexDataAsset = Cast<UHexWorldDataAsset>(StaticLoadObject(UObject::StaticClass(),NULL, *DataAssetName));
			if (!HexDataAsset)
			{
				UE_LOG(LogTemp, Display, TEXT("Data Asset %s not found"), *DataAssetName);
				return;
			}
			
			for(auto data : HexData->GlobalData)
			{
				HexDataAsset->GlobalData.Add(data.Key, data.Value);	
			}
			HexDataAsset->Location = HexData->Location;
			HexDataAsset->Type = HexData->Type;
						
			FActorSpawnParameters SpawnParameters;
			FGuid guid = FGuid::NewGuid();

			// Setting GUID before spawn so that it can be saved in the data asset before the Blueprints construction scripts runs
			SpawnParameters.OverrideActorGuid = guid; 
			HexDataAsset->LocalData.Add(guid, FLocalHexagon{HexData->LocalData}); 
			
			AActor* HexActor = GetWorld()->SpawnActor<AActor>(GeneratedBP->GeneratedClass, WorldLocation, Rotation, SpawnParameters);
			HexActor->Modify(true);
			HexActor->Tags.Add("Hexagon");

			const FString DirectionStr = FString("direction:").Append(ToCStr(*HexData->LocalData.Find("direction"))); 
			const FString LocationStr = FString::Printf(TEXT("location:%d:%d"), HexData->Location.X, HexData->Location.Y);
			
			HexActor->Tags.Add(ToCStr(DirectionStr));
			HexActor->Tags.Add(ToCStr(LocationStr));


			
			if(HexData->LocalData.Contains("link"))
			{
				FActorSpawnParameters GateSpawnParameters;
				GateSpawnParameters.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
				FString Gatename = FString::Printf(TEXT("Gate_%d"), UPairing::Pair(HexData->Location.X, HexData->Location.Y));
				GateSpawnParameters.Name = FName(*Gatename);
								
				int64 X, Y;
				FString LocSingleValue = *HexData->LocalData.Find("link");
				UPairing::UnPair(FCString::Atoi64(*LocSingleValue), X, Y);
				FIntVector* LinkLocation = new FIntVector(X, Y, 0);
				FVector LookAt = HexToLocation(LinkLocation, 1500);
				FRotator GateRot = UKismetMathLibrary::FindLookAtRotation(WorldLocation, LookAt);
				ANavigationGate* NavigationGate = GetWorld()->SpawnActor<ANavigationGate>(ANavigationGate::StaticClass(), (WorldLocation + LookAt) / 2, GateRot, GateSpawnParameters);
				DrawDebugLine(GetWorld(), FVector{WorldLocation.X, WorldLocation.Y, 500}, FVector{LookAt.X, LookAt.Y, 500}, FColor::Emerald, true, -1, 0, 10);
				
				NavigationGate->NextGateNameTag = FString::Printf(TEXT("Gate_%d"), FCString::Atoi64(*LocSingleValue));
				
				
				NavigationGate->Tags.Add(FName(*Gatename));

				NavigationGate->SetOwner(HexActor);
				NavigationGate->Tags.Add("Hexagon");
				NavigationGate->Tags.Add("HexagonGate");
			}
			
			AddLabel(&HexData->Location);
			
		}
			
	}

	TArray<AActor*> HexagonLabels;	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATextRenderActor::StaticClass(), HexagonLabels);

	for(AActor* HexagonLabel : HexagonLabels)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		UUnrealEditorSubsystem* EditorSubsysten = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>();
		EditorSubsysten->GetLevelViewportCameraInfo(CameraLocation, CameraRotation);

		FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(HexagonLabel->GetActorLocation(), CameraLocation);
		HexagonLabel->SetActorRotation(LookAtRotator);
	}
	
}

FVector UHexWorldRetrieveMapTool::HexToLocation(const FIntVector* Location, const int Size) const
{
	const double x = Size * (sqrt(3.0) * Location->X + sqrt(3.0)/2.0 * Location->Y);
	const double y = Size * (3.0/2.0 * Location->Y);
	return FVector(x, y, 0);	
}

#undef LOCTEXT_NAMESPACE

// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#include "HexWorldRetrieveMapTool.h"

#include "InteractiveToolManager.h"
#include "UHexWorldSubsysten.h"
#include "WaterBodyRiverActor.h"
#include "WaterBodyRiverComponent.h"
#include "WaterSplineComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/UnrealEditorSubsystem.h"

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

void UHexWorldRetrieveMapProperties::AddRiver()
{
	HexagonMap->AddRiver("Gate_6",0);
	HexagonMap->AddRiver("Gate_4",1);
	HexagonMap->AddRiver("Gate_5",2);
	HexagonMap->AddRiver("Gate_2",3);
	HexagonMap->AddRiver("Gate_1",4);
	HexagonMap->AddRiver("Gate_7",5);
	HexagonMap->AddRiver("Gate_6",6);
	HexagonMap->AddRiver("Gate_4",7);
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

void UHexWorldRetrieveMapTool::OnTick(float DeltaTime)
{
		
    Properties->HexagonMap->PopulateMap();
	
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

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
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Gate_6"), FoundActors);

	ANavigationGate *Gate = Cast<ANavigationGate>(FoundActors[0]);
	if(!Gate)
	{
		return;
	}

	const FVector GateLocation = Gate->GetActorLocation();
	FVector GateForward = Gate->GetActorForwardVector();
	// FVector GateBackward = Gate->GetActorForwardVector() * -100.f;

	FVector ArriveTangent = GateForward;
	// FVector LeaveTangent = GateLocation + ;
	
	AWaterBodyRiver* RiverActor = GetWorld()->SpawnActor<AWaterBodyRiver>(AWaterBodyRiver::StaticClass());

	UWaterBodyRiverComponent* RiverComponents = Cast<UWaterBodyRiverComponent>(RiverActor->GetWaterBodyComponent());
	UWaterSplineComponent* WaterSplineComponent = RiverComponents->GetWaterSpline();
	USplineMetadata* SplineMetadata = WaterSplineComponent->GetSplinePointsMetadata();
	// WaterSplineComponent->SetSplinePointType(0, ESplinePointType::CurveCustomTangent);
	WaterSplineComponent->SplineCurves.Position.Points[0].OutVal = GateLocation;
	WaterSplineComponent->SplineCurves.Position.Points[0].ArriveTangent = ArriveTangent;
	WaterSplineComponent->SplineCurves.Position.Points[0].LeaveTangent = ArriveTangent;

	
	// WaterSplineComponent->SplineCurves.Position.Points[0].

	RiverComponents->CurveSettings.bUseCurveChannel = true;
	UCurveFloat* Curve = NewObject<UCurveFloat>();

	FKeyHandle NewKeyHandle = Curve->FloatCurve.AddKey(0, 0);

	Curve->FloatCurve.SetKeyInterpMode(NewKeyHandle, RCIM_Cubic);
	Curve->FloatCurve.SetKeyTangentMode(NewKeyHandle, RCTM_Auto);
	Curve->FloatCurve.SetKeyTangentWeightMode(NewKeyHandle, RCTWM_WeightedNone);

	Curve->FloatCurve.AddKey(1, 1);
	
	
	RiverComponents->CurveSettings.ElevationCurveAsset = Curve; 
	RiverComponents->CurveSettings.ChannelEdgeOffset = 0.0f;
	RiverComponents->CurveSettings.ChannelDepth = 256.0f;
	RiverComponents->CurveSettings.CurveRampWidth= 512.0f;
	
	FWaterBodyHeightmapSettings HeightmapSettings;
	HeightmapSettings.FalloffSettings.FalloffAngle = 45.0f;
	HeightmapSettings.FalloffSettings.FalloffWidth = 512.0f;
	HeightmapSettings.FalloffSettings.EdgeOffset = 128.0f;
	HeightmapSettings.FalloffSettings.ZOffset = 8.0f;

	RiverComponents->WaterHeightmapSettings = HeightmapSettings;

	WaterMaterial = FSoftObjectPath(TEXT("/Water/Materials/WaterSurface/Water_Material_River.Water_Material_River"));
	RiverComponents->SetWaterMaterial(WaterMaterial.LoadSynchronous());
	
	UWaterSplineMetadata* Data = RiverActor->GetWaterSplineMetadata();
	UWaterSplineComponent* Component = RiverActor->GetWaterSpline();


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

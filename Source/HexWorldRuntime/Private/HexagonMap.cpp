// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#include "HexagonMap.h"

#include "HexWorldDataAsset.h"
#include "WaterBodyRiverActor.h"
#include "WaterBodyRiverComponent.h"
#include "WaterSplineComponent.h"
#include "Actors/Hexagon.h"
#include "Comms/HexWorldRunnable.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "hexworld/hex_client.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/NavigationGate.h"
#include "Utils/Pairing.h"

UHexagonMap::UHexagonMap()
{
	
}


void UHexagonMap::RetrieveMap(bool bClearMap)
{
	UE_LOG(LogTemp, Log, TEXT("Retrieve map called"));


	
	FHexWorldRunnable::RunLambdaOnBackgroundThread(HexCoordData, [&]
	{
		if(!HexagonClient)
		{
			this->HexagonClient = new ::HexagonClient(std::string(TCHAR_TO_UTF8(*Address)), bSecure);
		}
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
			const FIntVector HexLocation = FIntVector(HexCV[i].X, HexCV[i].Y, HexCV[i].Z);
			HexData->Location = HexLocation;
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

void UHexagonMap::PopulateMap()
{
	if(!HexCoordData)
	{
		UE_LOG(LogTemp, Warning, TEXT("HexWorldData is null"));
		return;
	}
	
	if(HexCoordData || HexCoordData->IsEmpty())
	{
		UHexData* HexData = NewObject<UHexData>();
		
		if(HexCoordData->Dequeue(HexData))
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
			// SpawnParameters.OverrideActorGuid = guid; 
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
				// DrawDebugLine(GetWorld(), FVector{WorldLocation.X, WorldLocation.Y, 500}, FVector{LookAt.X, LookAt.Y, 500}, FColor::Emerald, true, -1, 0, 10);
				
				NavigationGate->NextGateNameTag = FString::Printf(TEXT("Gate_%d"), FCString::Atoi64(*LocSingleValue));
				
				
				NavigationGate->Tags.Add(FName(*Gatename));

				NavigationGate->SetOwner(HexActor);
				NavigationGate->Tags.Add("Hexagon");
				NavigationGate->Tags.Add("HexagonGate");
				NavigationGate->AddLabel(&HexData->Location);
			}
		}
	}
}


void UHexagonMap::AddRiver(FString Name, const int PointPosition)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(Name), FoundActors);

	if (FoundActors.IsEmpty())
	{
		return;
	}
	
	ANavigationGate *Gate = Cast<ANavigationGate>(FoundActors[0]);
	if(!Gate)
	{
		return;
	}

	const FVector GateLocation = Gate->GetActorLocation();
	const FRotator GateRotation = Gate->GetActorRotation();
	FVector GateForward = Gate->GetActorForwardVector();
	// FVector GateBackward = Gate->GetActorForwardVector() * -100.f;

	FVector ArriveTangent = GateForward;
	// FVector LeaveTangent = GateLocation + ;

	if(!RiverActor)
	{
		RiverActor = GetWorld()->SpawnActor<AWaterBodyRiver>(AWaterBodyRiver::StaticClass());	
	}
	
	UWaterBodyRiverComponent* RiverComponents = Cast<UWaterBodyRiverComponent>(RiverActor->GetWaterBodyComponent());
	UWaterSplineComponent* WaterSplineComponent = RiverComponents->GetWaterSpline();
	USplineMetadata* SplineMetadata = WaterSplineComponent->GetSplinePointsMetadata();

	if(WaterSplineComponent->SplineCurves.Position.Points.Num() < PointPosition + 1)
	{
		WaterSplineComponent->SplineCurves.Position.Points.Add(FInterpCurvePoint<FVector>{static_cast<float>(PointPosition), GateLocation, ArriveTangent, ArriveTangent, CIM_CurveAuto});
		WaterSplineComponent->SplineCurves.Rotation.Points.Add(FInterpCurvePoint<UE::Math::TQuat<double>>{static_cast<float>(PointPosition),UE::Math::TQuat<double>{0,0,0,1}, UE::Math::TQuat<double>{0,0,0,1}, UE::Math::TQuat<double>{0,0,0,1}, CIM_CurveAuto});
		WaterSplineComponent->SplineCurves.Scale.Points.Add(FInterpCurvePoint<FVector>{static_cast<float>(PointPosition), FVector{2048.f, 150.f, 1.f}, FVector{0.f, 0.f, 0.f}, FVector{0.f, 0.f, 0.f}, CIM_CurveAuto});
	}

	// WaterSplineComponent->SetSplinePointType(0, ESplinePointType::CurveCustomTangent);
	WaterSplineComponent->SplineCurves.Position.Points[PointPosition].OutVal = GateLocation;
	WaterSplineComponent->SplineCurves.Position.Points[PointPosition].ArriveTangent = ArriveTangent;
	WaterSplineComponent->SplineCurves.Position.Points[PointPosition].LeaveTangent = ArriveTangent;
	
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

	// RiverComponents->WaterHeightmapSettings = HeightmapSettings;

	WaterMaterial = FSoftObjectPath(TEXT("/Water/Materials/WaterSurface/Water_Material_River.Water_Material_River"));
	RiverComponents->SetWaterMaterial(WaterMaterial.LoadSynchronous());
	
	UWaterSplineMetadata* Data = RiverActor->GetWaterSplineMetadata();
	UWaterSplineComponent* Component = RiverActor->GetWaterSpline();

	RiverComponents->UpdateAll(true);

}

FVector UHexagonMap::HexToLocation(const FIntVector* Location, const int Size) 
{
	const double x = Size * (sqrt(3.0) * Location->X + sqrt(3.0)/2.0 * Location->Y);
	const double y = Size * (3.0/2.0 * Location->Y);
	return FVector(x, y, 0);	
}

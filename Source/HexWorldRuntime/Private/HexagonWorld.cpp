// Fill out your copyright notice in the Description page of Project Settings.


#include "HexagonWorld.h"

#include "EditorScriptingHelpers.h"
#include "HexWorldDataAsset.h"
#include "UHexWorldSubsysten.h"
#include "Actors/Hexagon.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#include "Utils/Pairing.h"


// Sets default values
AHexagonWorld::AHexagonWorld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHexagonWorld::BeginPlay()
{
	Super::BeginPlay();

	HexWorldSubsysten = GetWorld()->GetGameInstance()->GetSubsystem<UUHexWorldSubsysten>();
	HexagonMap = HexWorldSubsysten->GetHexagonMap();
	
}

// Called every frame
void AHexagonWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!HexagonMap->HexCoordData->IsEmpty())
	{
		UHexData* HexData;
		if(HexagonMap->HexCoordData->Dequeue(HexData))
		{
			const FString Type(HexData->Type);
			EHexagonDirection Direction = EHexagonDirection::N;
			if(HexData->LocalData.Contains("direction"))
			{
				Direction = AHexagon::ConvertDirection(HexData->LocalData.Find("direction"));
			} 
			
			const FVector Location = HexToLocation(&HexData->Location, 1500);
			const FRotator Rotation(0.0f, 60.0f * (static_cast<std::underlying_type_t<EHexagonDirection>>(Direction) - 1), 0.0f);

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
			
			AActor* HexActor = GetWorld()->SpawnActor<AActor>(GeneratedBP->GeneratedClass, Location, Rotation, SpawnParameters);
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
				FRotator GateRot = UKismetMathLibrary::FindLookAtRotation(Location, LookAt);
				ANavigationGate* NavigationGate = GetWorld()->SpawnActor<ANavigationGate>(ANavigationGate::StaticClass(), (Location + LookAt) / 2, GateRot, GateSpawnParameters);
				DrawDebugLine(GetWorld(), FVector{Location.X, Location.Y, 500}, FVector{LookAt.X, LookAt.Y, 500}, FColor::Emerald, true, -1, 0, 10);
				
				NavigationGate->NextGateNameTag = FString::Printf(TEXT("Gate_%d"), FCString::Atoi64(*LocSingleValue));
				
				
				NavigationGate->Tags.Add(FName(*Gatename));

				NavigationGate->SetOwner(HexActor);
				NavigationGate->Tags.Add("Hexagon");
				NavigationGate->Tags.Add("HexagonGate");
			}
		}
	}
}

FVector AHexagonWorld::HexToLocation(const FIntVector* Location, const int Size) const
{
	const double x = Size * (sqrt(3.0) * Location->X + sqrt(3.0)/2.0 * Location->Y);
	const double y = Size * (3.0/2.0 * Location->Y);
	return FVector(x, y, 0);	
}


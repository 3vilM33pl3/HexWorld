﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "HexagonMap.h"

#include "Actors/Hexagon.h"
#include "Comms/HexWorldRunnable.h"
#include "hexworld/hex_client.h"
#include "Kismet/GameplayStatics.h"

AHexagonMap::AHexagonMap()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHexagonMap::BeginPlay()
{
	Super::BeginPlay();
	RetrieveMap();
	
}

void AHexagonMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!HexCoordData->IsEmpty())
	{
		if(Hexagon* Hex = new Hexagon(0,0,0,"", Direction::N); HexCoordData->Dequeue(*Hex))
		{
			const FString Type(Hex->Type.c_str());
			const EHexagonDirection Direction = AHexagon::ConvertDirection(Hex->Direction);
			const FVector Location = HexToLocation(Hex, 1500);
			const FRotator Rotation(0.0f, 60.0f * (static_cast<std::underlying_type_t<::Direction>>(Direction) - 1), 0.0f);

			UE_LOG(LogTemp, Display, TEXT("[%d, %d, %d ] %s"), Hex->X, Hex->Y, Hex->Z, *Type);

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

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AActor* HexActor = GetWorld()->SpawnActor<AActor>(GeneratedBP->GeneratedClass, Location, Rotation, SpawnParameters);
			HexActor->Tags.Add(FName("Hexagon"));
			HexActor->Modify(true);

			const FString StringPrintf = FString(TEXT("{0},{1},{2}"));
			const FString StringFormatted = FString::Format(*StringPrintf, {Hex->X, Hex->Y, Hex->Z});
			HexActor->Tags.Add(FName(StringFormatted));
			HexActor->Tags.Add(FName(Type));

			//bool bSaved = FEditorFileUtils::SaveLevel(GetWorld()->GetLevel(0));
			
		}
	}	
}

void AHexagonMap::RetrieveMap()
{
	UE_LOG(LogTemp, Log, TEXT("Retrieve map called"));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Hexagon"), FoundActors);

	for(auto& Hex: FoundActors)
	{
		Hex->Destroy();
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
			UE_LOG(LogTemp, Display, TEXT("Enqueue %d [%d, %d, %d ] %s"), i, HexCV[i].X, HexCV[i].Y, HexCV[i].Z, *FString(HexCV[i].Type.c_str()));
			HexCoordData->Enqueue(HexCV[i]);
		}
		
	});	
}

FVector AHexagonMap::HexToLocation(const Hexagon* Hex, const int Size) const
{
	double x = Size * (sqrt(3.0) * Hex->X + sqrt(3.0)/2.0 * Hex->Y);
	double y = Size * (3.0/2.0 * Hex->Y);
	return FVector(x, y, 0);	
}

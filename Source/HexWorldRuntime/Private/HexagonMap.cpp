// Fill out your copyright notice in the Description page of Project Settings.


#include "HexagonMap.h"

#include "HexWorldDataAsset.h"
#include "Actors/Hexagon.h"
#include "Comms/HexWorldRunnable.h"
#include "hexworld/hex_client.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/NavigationGate.h"

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

FVector UHexagonMap::HexToLocation(const UHexData* Hex, const int Size) const
{
	const double x = Size * (sqrt(3.0) * Hex->Location.X + sqrt(3.0)/2.0 * Hex->Location.Y);
	const double y = Size * (3.0/2.0 * Hex->Location.Y);
	return FVector(x, y, 0);	
}

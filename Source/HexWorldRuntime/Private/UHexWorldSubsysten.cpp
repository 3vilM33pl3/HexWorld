// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#include "UHexWorldSubsysten.h"

#include "GameFramework/GameModeBase.h"
#include "HUD/HexWorldHUD.h"
#include "Kismet/GameplayStatics.h"

void UUHexWorldSubsysten::DestroyAllHexagonActors()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Hexagon"), FoundActors);

	for(AActor* Hex: FoundActors)
	{
		Hex->Destroy();
	}
}

void UUHexWorldSubsysten::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("UUHexWorldSubsysten::Initialize"));

	// DestroyAllHexagonActors();

	// HexagonMap = NewObject<UHexagonMap>(this);
	// HexagonMap->RetrieveMap(true);


}

void UUHexWorldSubsysten::Deinitialize()
{
	Super::Deinitialize();

	// DestroyAllHexagonActors();

	UE_LOG(LogTemp, Warning, TEXT("UUHexWorldSubsysten::Deinitialize"));
	
}


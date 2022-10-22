// Fill out your copyright notice in the Description page of Project Settings.


#include "UHexWorldSubsysten.h"

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

	DestroyAllHexagonActors();

	HexagonMap = NewObject<UHexagonMap>(this);
	HexagonMap->RetrieveMap(true);
}

void UUHexWorldSubsysten::Deinitialize()
{
	Super::Deinitialize();

	DestroyAllHexagonActors();

	UE_LOG(LogTemp, Warning, TEXT("UUHexWorldSubsysten::Deinitialize"));
	
}


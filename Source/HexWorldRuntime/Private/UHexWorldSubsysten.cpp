// Fill out your copyright notice in the Description page of Project Settings.


#include "UHexWorldSubsysten.h"

#include "Kismet/GameplayStatics.h"

void UUHexWorldSubsysten::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("UUHexWorldSubsysten::Initialize"));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Hexagon"), FoundActors);

	for(AActor* Hex: FoundActors)
	{
		Hex->Destroy();
	}

	HexagonMap = NewObject<AHexagonMap>(this);
	HexagonMap->RetrieveMap(true);
}

void UUHexWorldSubsysten::Deinitialize()
{
	Super::Deinitialize();

	UE_LOG(LogTemp, Warning, TEXT("UUHexWorldSubsysten::Deinitialize"));
	
}


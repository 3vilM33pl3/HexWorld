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
	HexagonMap->PopulateMap();

}

FVector AHexagonWorld::HexToLocation(const FIntVector* Location, const int Size) const
{
	const double x = Size * (sqrt(3.0) * Location->X + sqrt(3.0)/2.0 * Location->Y);
	const double y = Size * (3.0/2.0 * Location->Y);
	return FVector(x, y, 0);	
}


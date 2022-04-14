#include "Hexagon.h"


AHexagon::AHexagon()
{
	PrimaryActorTick.bCanEverTick = true;
	HexagonPlain = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HexagonPlain"));
	RootComponent = HexagonPlain;

	HexMesh = Cast<UStaticMesh>(StaticLoadObject( UStaticMesh::StaticClass(), nullptr, *FName("/HexWorld/0000-0000-0000-0000.0000-0000-0000-0000").ToString() ));
	HexagonPlain->SetStaticMesh(HexMesh);
}

void AHexagon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHexagon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FIntVector AHexagon::CubeToWorld(const FIntVector CubeLocation, const int Size) const
{
	const double x = Size * (3.0 / 2.0 * CubeLocation.X);
	const double y = Size * (sqrt(3.0)/2.0 * CubeLocation.X + sqrt(3.0) * CubeLocation.Y);
	return FIntVector(x, y, 0);	
}


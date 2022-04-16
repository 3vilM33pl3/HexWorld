#include "Hexagon.h"


AHexagon::AHexagon()
{
	PrimaryActorTick.bCanEverTick = true;
	HexagonMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HexagonPlain"));
	RootComponent = HexagonMeshComponent;

	
}

void AHexagon::SetTypeAndDirection(FString TypeName,  EHexagonDirection NewDirection)
{
	this->Type = TypeName;
	FString MeshName("/HexWorld/");
	MeshName = MeshName.Append(Type).Append(".").Append(Type);
	HexMesh = Cast<UStaticMesh>(StaticLoadObject( UStaticMesh::StaticClass(), nullptr, *FName(MeshName).ToString()));
	HexagonMeshComponent->SetStaticMesh(HexMesh);

	HexagonMeshComponent->SetWorldRotation(FRotator(0.0f,60.0f * (static_cast<std::underlying_type_t<::Direction>>(NewDirection) - 1), 0.0f));
	Direction = NewDirection;

}

EHexagonDirection AHexagon::ConvertDirection(::Direction Direction)
{
	switch (Direction)
	{
	case Direction::N:
		return EHexagonDirection::N;
	case Direction::NE:
		return EHexagonDirection::NE;
	case Direction::E:
		return EHexagonDirection::E;
	case Direction::SE:
		return EHexagonDirection::SE;
	case Direction::S:
		return EHexagonDirection::S;
	case Direction::SW:
		return EHexagonDirection::SW;
	case Direction::W:
		return EHexagonDirection::W;
	case Direction::NW:
		return EHexagonDirection::NW;
	default:
		return EHexagonDirection::CENTER;
			
	}
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


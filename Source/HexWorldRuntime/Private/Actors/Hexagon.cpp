#include "Actors/Hexagon.h"


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

	HexagonMeshComponent->SetWorldRotation(FRotator(0.0f,60.0f * (static_cast<std::underlying_type_t<EHexagonDirection>>(NewDirection) - 1), 0.0f));
	Direction = NewDirection;

}

EHexagonDirection AHexagon::ConvertDirection(std::string Direction)
{
	if(Direction ==  "N")
		return EHexagonDirection::N;
	if(Direction ==  "NE")
		return EHexagonDirection::NE;
	if(Direction ==  "E")
		return EHexagonDirection::E;
	if(Direction ==  "SE")
		return EHexagonDirection::SE;
	if(Direction ==  "S")
		return EHexagonDirection::S;
	if(Direction ==  "SW")
		return EHexagonDirection::SW;
	if(Direction ==  "W")
		return EHexagonDirection::W;
	if(Direction ==  "NW")
		return EHexagonDirection::NW;
	return EHexagonDirection::CENTER;
	
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


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

EHexagonDirection AHexagon::ConvertDirection(FString* Direction)
{
	if(Direction->Equals("N"))
		return EHexagonDirection::N;
	if(Direction->Equals("NE"))
		return EHexagonDirection::NE;
	if(Direction->Equals("E"))
		return EHexagonDirection::E;
	if(Direction->Equals("SE"))
		return EHexagonDirection::SE;
	if(Direction->Equals("S"))
		return EHexagonDirection::S;
	if(Direction->Equals("SW"))
		return EHexagonDirection::SW;
	if(Direction->Equals("W"))
		return EHexagonDirection::W;
	if(Direction->Equals("NW"))
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


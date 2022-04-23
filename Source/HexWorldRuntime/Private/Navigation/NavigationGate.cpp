#include "Navigation/NavigationGate.h"

#include "Editor.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/BezierCurveFunctions.h"
#include "UObject/ConstructorHelpers.h"


ANavigationGate::ANavigationGate()
{
	PrimaryActorTick.bCanEverTick =  true;
	if constexpr (WITH_EDITOR)
	{
		PrimaryActorTick.bCanEverTick = true;
		PrimaryActorTick.bStartWithTickEnabled = true;
	}

	USceneComponent* EmptyComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = EmptyComponent;
	
	// Set default gate static mesh
	GateVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	GateVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateVisualAsset(TEXT("/HexWorld/Narrowboat/SM_Gate.SM_Gate"));
	if (GateVisualAsset.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Gate static mesh loaded"));

		Gate = GateVisualAsset.Object;
		GateVisual->SetStaticMesh(Gate);
		GateVisual->SetCollisionProfileName(TEXT("NoCollision"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load gate static mesh"));
	}

	NextGate = this; // If there is no next gate point to itself (default)
}

void ANavigationGate::BeginPlay()
{
	CalculateControlPointsCubicBezier();
	Super::BeginPlay();
	
}

void ANavigationGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANavigationGate::OnConstruction(const FTransform& Transform)
{
	if constexpr (WITH_EDITOR)
	{
		CalculateControlPointsCubicBezier();
		const TSubclassOf<ANavigationGate> ClassToFind = ANavigationGate::StaticClass();
		TArray<AActor*> FoundGates;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundGates);

		for (AActor* ActorGate : FoundGates)
		{
			auto NavigationGate = Cast<ANavigationGate>(ActorGate);
			NavigationGate->CalculateControlPointsCubicBezier();
		}
		
	}
	Super::OnConstruction(Transform);
}

#if WITH_EDITOR
void ANavigationGate::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	TArray<UActorComponent*> MyComponents;
	GetComponents(MyComponents);
	
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if ((PropertyName == GET_MEMBER_NAME_CHECKED(ANavigationGate, Gate)))
	{
		FMultiComponentReregisterContext ReregisterContext(MyComponents);

		for (UActorComponent* Comp : MyComponents)
		{
			if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Comp))
			{
				GateVisual->SetStaticMesh(Gate); // Update the component to the new mesh
				GEditor->EditorUpdateComponents();
			}
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

bool ANavigationGate::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ANavigationGate::CalculateControlPointsCubicBezier()
{
	P0 = GetActorLocation();

	P1 = GetActorForwardVector();
	P1 = P1.ForwardVector;
	P1.X = P1.X * ForwardBezierStrength / sqrt(pow(P1.X,2) + pow(P1.Y,2)); // Forward vector is a unit vector
	P1 = GetTransform().TransformPosition(P1);

	if(NextGate != this && NextGate != nullptr)
	{
		P3 = NextGate->GetActorLocation();
		
		P2 = NextGate->GetActorForwardVector();
		P2 = P2.BackwardVector;
		P2.X = P2.X * NextGate->BackwardBezierStrength / sqrt(pow(P2.X,2)+pow(P2.Y,2));
		P2 = NextGate->GetTransform().TransformPosition(P2);

		Length = BezierCurveFunctions::CubicBezierLengthEstimate(P0, P1, P2, P3, 50);
		CubicBezierCurvePoints = BezierCurveFunctions::CubicBezierCurve(P0, P1, P2, P3, (1.0 / Length) * DebugPointsDistance);

	}	
}


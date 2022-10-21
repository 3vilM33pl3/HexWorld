#include "Navigation/HexWorldPawn.h"

#include "Kismet/GameplayStatics.h"
#include "Navigation/BezierCurveFunctions.h"
#include "UObject/ConstructorHelpers.h"


AHexWorldPawn::AHexWorldPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup carrier
	USceneComponent* EmptyComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = EmptyComponent;

	CarrierVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	CarrierVisual->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CarrierVisualAsset(TEXT("/HexWorld/Narrowboat/SM_Narrowboat_v4_0.SM_Narrowboat_v4_0"));
	if (CarrierVisualAsset.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Carrier static mesh loaded"));
		Carrier = CarrierVisualAsset.Object;
		CarrierVisual->SetStaticMesh(Carrier);
		CarrierVisual->SetRelativeRotation(FRotator{0,90,0});
		CarrierVisual->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load boat static mesh"));
	}

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
}

void AHexWorldPawn::BeginPlay()
{
	Super::BeginPlay();

	StartNavigation();


		
}

void AHexWorldPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(NextGate == nullptr)
	{
		StartNavigation();
		return;
	}
	
	if (ProgressAlongCurve < 1.0f)
	{
		ProgressAlongCurve = ProgressAlongCurve + DeltaTime * CarrierSpeed / Length;
		if (ProgressAlongCurve > 1.0f)
		{
			ProgressAlongCurve = 1.0f;
		}
		const FVector Location = BezierCurveFunctions::CubicBezierLocation(P0, P1, P2, P3, ProgressAlongCurve);
		this->SetActorLocation(Location);
		UE_LOG(LogTemp, Verbose, TEXT("Progress: %f"), ProgressAlongCurve);

		FVector Rotation = BezierCurveFunctions::CubicBezierCurveDerivative(P0, P1, P2, P3, ProgressAlongCurve);
		Rotation.Z = 0;
		this->SetActorRotation(Rotation.Rotation());
	} else
	{
		if (NextGate != nullptr && NextGate->NextGate != nullptr)
		{
			auto const CurrentGate = NextGate;
			UE_LOG(LogTemp, Log, TEXT("This gate name tag: %s"), *CurrentGate->Tags[0].ToString());
			UE_LOG(LogTemp, Log, TEXT("Next gate name tag: %s"), *CurrentGate->NextGateNameTag);
			Length = CurrentGate->Length;
			ForwardBezierStrength = CurrentGate->ForwardBezierStrength;
			BackwardBezierStrength = CurrentGate->NextGate->BackwardBezierStrength;
			NextGate = CurrentGate->NextGate;
			ProgressAlongCurve = 0.0;
			CalculateControlPointsCubicBezier();
		}
	}
}

void AHexWorldPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);  
}

void AHexWorldPawn::CalculateControlPointsCubicBezier()
{
	if (NextGate != nullptr)
	{
		P0 = this->GetActorLocation();
		P1 = this->GetActorForwardVector();
		//BezierStrength = NextGate->BezierStrength;

		P1 = P1.ForwardVector;
		P1.X = P1.X * ForwardBezierStrength / sqrt(pow(P1.X, 2) + pow(P1.Y, 2)); // Forward vector is a unit vector
		P1 = this->GetTransform().TransformPosition(P1);

		P3 = NextGate->GetActorLocation();

		P2 = NextGate->GetActorForwardVector();
		P2 = P2.BackwardVector;
		P2.X = P2.X * NextGate->BackwardBezierStrength / sqrt(pow(P2.X, 2) + pow(P2.Y, 2));
		P2 = NextGate->GetTransform().TransformPosition(P2);

	}	
}

void AHexWorldPawn::StartNavigation()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(*FirstGateNameTag), FoundActors);

	if(FoundActors.Num() > 0)
	{
		NextGate = Cast<ANavigationGate>(FoundActors[0]);
		CalculateControlPointsCubicBezier();
		ForwardBezierStrength = InitialBezierStrength;
		if(NextGate != nullptr)
		{
			BackwardBezierStrength = NextGate->BackwardBezierStrength;
		}
	
		Length = BezierCurveFunctions::CubicBezierLengthEstimate(P0, P1, P2, P3, SegmentInterval);
		this->SetActorLocation(this->GetActorLocation());
		this->SetActorEnableCollision(true);
	}
}


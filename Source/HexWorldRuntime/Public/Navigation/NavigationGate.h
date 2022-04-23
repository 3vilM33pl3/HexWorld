#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include "NavigationGate.generated.h"

UCLASS()
class HEXWORLDRUNTIME_API ANavigationGate : public AActor
{
	GENERATED_BODY()

public:
	ANavigationGate();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual bool ShouldTickIfViewportsOnly() const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Magic Bezier")
	FVector P0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	FVector P1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	FVector P2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	FVector P3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	float Length = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	float ForwardBezierStrength = 4000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	float BackwardBezierStrength = 4000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	float DebugPointsDistance = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	ANavigationGate* NextGate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	UStaticMesh* Gate;

	UStaticMeshComponent* GateVisual;

private:
	std::vector<FVector> CubicBezierCurvePoints;
	void CalculateControlPointsCubicBezier();
};

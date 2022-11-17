// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NavigationGate.h"
#include "GameFramework/Pawn.h"
#include "HUD/HexWorldHUD.h"
#include "HexWorldPawn.generated.h"

UCLASS()
class HEXWORLDRUNTIME_API AHexWorldPawn : public APawn
{
	GENERATED_BODY()

public:
	AHexWorldPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	ANavigationGate* NextGate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	FString FirstGateNameTag;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
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
	float InitialBezierStrength = 1000;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	float ForwardBezierStrength = 1000;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic Bezier")
	float BackwardBezierStrength = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	float CarrierSpeed = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	UStaticMesh* Carrier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Bezier")
	float SegmentInterval = 50;

	UStaticMeshComponent* CarrierVisual;

	float ProgressAlongCurve = 0.0f;

private:
	void CalculateControlPointsCubicBezier();

	void StartNavigation();

	AHexWorldHUD* HUD;
	
};

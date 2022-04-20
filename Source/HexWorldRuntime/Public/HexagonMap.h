
#pragma once

#include "CoreMinimal.h"
#include "Containers/CircularQueue.h"
#include "GameFramework/Actor.h"
#include "hexworld/hex_lib.h"
#include "HexagonMap.generated.h"

UCLASS()
class HEXWORLDRUNTIME_API AHexagonMap : public AActor
{
	GENERATED_BODY()

public:
	AHexagonMap();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Options)
	void RetrieveMap();

	UPROPERTY(EditAnywhere, Category = Options, meta = (DisplayName = "Use this location as center of the map"))
	FIntVector CenterLocation = FIntVector{0,0,0};

	UPROPERTY(EditAnywhere, Category = Options, meta = (DisplayName = "Diameter of retrieved area"))
	int Diameter = 1;

	UPROPERTY(EditAnywhere, Category = Connection, meta = (DisplayName = "Backend server address"))
	FString Address = "localhost:8080";
	
	UPROPERTY(EditAnywhere, Category = Connection, meta = (DisplayName = "Secure connection over SSL"))
	bool bSecure = false;

	TCircularQueue<Hexagon>* HexCoordData = new TCircularQueue<Hexagon>(120);

	FVector HexToLocation(const Hexagon* Hex, const int Size) const;
	
};

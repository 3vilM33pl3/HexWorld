// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/HexData.h"
#include "Containers/CircularQueue.h"
#include "hexworld/hex_lib.h"
#include "HexWorldRunnable.generated.h"

USTRUCT(Blueprintable)
struct FHexagonMetaData {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Hexagon World")
	int32 X;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Hexagon World")
	int32 Y;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Hexagon World")
	int32 Z;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Hexagon World");
	FString Type;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Hexagon World");
	FString Direction; 

	FHexagonMetaData(int x_, int y_, int z_):X(x_), Y(y_), Z(z_) {};
	FHexagonMetaData(): X(0), Y(0), Z(0) {} ;
};

class HEXWORLDRUNTIME_API FHexWorldRunnable : FRunnable
{
private:
	FRunnableThread* Thread;
	TFunction<void()> FunctionPointer;
	FThreadSafeBool Finished;
	static uint64 ThreadNumber;
	uint64 Number;
	
public:
	FHexWorldRunnable(TCircularQueue<UHexData*>* Data ,TFunction< void()> InFunction);
	~FHexWorldRunnable();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	static FHexWorldRunnable* RunLambdaOnBackgroundThread(TCircularQueue<UHexData*>* Data ,TFunction<void()> InFunction);
private:
	TCircularQueue<UHexData*>* HexCoordData;
	
};

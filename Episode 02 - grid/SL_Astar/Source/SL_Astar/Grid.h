// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

USTRUCT(Atomic)
struct FNode {
	GENERATED_USTRUCT_BODY()

public:
	bool bWalkable;
	FVector worldPosition;

	FNode() {
		bWalkable = true;
		worldPosition = FVector(0, 0, 0);
	}

	FNode(bool _walkable, FVector _worldPos) {
		bWalkable = _walkable;
		worldPosition = _worldPos;
	}

	bool operator==(const FNode& other) const {
		return worldPosition == other.worldPosition;
	}
};

USTRUCT()
struct FGridRow {
	GENERATED_USTRUCT_BODY()
	TArray<FNode> Nodes;
};

UCLASS()
class SL_ASTAR_API AGrid : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float nodeRadius = 100.0f;
	UPROPERTY()
	TArray<FGridRow> grid;

	UPROPERTY()
	int gridSizeX = 10;
	UPROPERTY()
	int gridSizeY = 10;
	UPROPERTY()
	float nodeDiameter;
	UPROPERTY()
	float gridWorldSizeX;
	UPROPERTY()
	float gridWorldSizeY;

public:
	// Sets default values for this actor's properties
	AGrid();

	// 박스 콜리전
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollision;

	UFUNCTION(BlueprintCallable)
	void CreateGrid();

	UFUNCTION()
	FNode NodeFromWorldPoint(FVector worldPosition);

	bool IsWalkable(const FVector& WorldPoint);

	UFUNCTION()
	void DrawDebugGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pathfinding.generated.h"

struct FNode;
class AGrid;

UCLASS()
class SL_ASTAR_API APathfinding : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APathfinding();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
	AGrid* Grid;

	UPROPERTY(EditAnywhere)
	AActor* Seeker;

	UPROPERTY(EditAnywhere)
	AActor* Target;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FindPath(const FVector& StartPos, const FVector& TargetPos);

	int GetDistance(FNode* NodeA, FNode* NodeB);

	void RetracePath(FNode* StartNode, FNode* EndNode);
};

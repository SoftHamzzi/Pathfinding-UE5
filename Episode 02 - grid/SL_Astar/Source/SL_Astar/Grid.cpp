// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "Engine/OverlapResult.h"
#include "Grid.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AGrid::AGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxCollision);

}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

	nodeDiameter = nodeRadius * 2;
	FVector origin;
	FVector boxExtent;
	GetActorBounds(false, origin, boxExtent);

	gridWorldSizeX = boxExtent.X * 2;
	gridWorldSizeY = boxExtent.Y * 2;

	gridSizeX = FMath::RoundToInt(gridWorldSizeX / nodeDiameter);
	gridSizeY = FMath::RoundToInt(gridWorldSizeY / nodeDiameter);

	CreateGrid();

}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugGrid();

}

void AGrid::CreateGrid() {
	grid.SetNum(gridSizeX);
	FVector worldBottomLeft = GetActorLocation() - FVector(gridWorldSizeX / 2, gridWorldSizeY / 2, 0);

	for (int x = 0; x < gridSizeX; x++) {
		grid[x].Nodes.SetNum(gridSizeY);

		for (int y = 0; y < gridSizeY; y++) {
			FVector worldPoint = worldBottomLeft + FVector(x * nodeDiameter + nodeRadius, y * nodeDiameter + nodeRadius, 0);
			bool walkable = IsWalkable(worldPoint);
			grid[x].Nodes[y] = FNode(walkable, worldPoint);
		}
	}
}

FNode AGrid::NodeFromWorldPoint(FVector WorldPosition) {
	float percentX = (WorldPosition.X + gridWorldSizeX / 2) / gridWorldSizeX;
	float percentY = (WorldPosition.Y + gridWorldSizeY / 2) / gridWorldSizeY;

	percentX = FMath::Clamp(percentX, 0.0f, 1.0f);
	percentY = FMath::Clamp(percentY, 0.0f, 1.0f);

	int x = FMath::RoundToInt((gridSizeX - 1) * percentX);
	int y = FMath::RoundToInt((gridSizeY - 1) * percentY);

	return grid[x].Nodes[y];
}

bool AGrid::IsWalkable(const FVector& WorldPoint) {
	TArray<FOverlapResult> OverlapResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(nodeRadius);

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		WorldPoint,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		CollisionShape
	);

	return OverlapResults.Num() == 0;
}

void AGrid::DrawDebugGrid() {

	FNode PlayerNode = NodeFromWorldPoint(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());

	for (int x = 0; x < gridSizeX; x++) {
		for (int y = 0; y < gridSizeY; y++) {
			FNode Nodes = grid[x].Nodes[y];
			FColor NodesColor = Nodes.bWalkable ? FColor::Green : FColor::Red;
			if (Nodes == PlayerNode) {
				NodesColor = FColor::Blue;
			}
			DrawDebugBox(GetWorld(), Nodes.worldPosition, FVector(nodeRadius - 1, nodeRadius - 1, nodeRadius - 1), NodesColor);
		}
	}
}
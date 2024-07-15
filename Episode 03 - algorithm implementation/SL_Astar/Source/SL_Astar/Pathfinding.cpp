// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Algo/Reverse.h"
#include "Pathfinding.h"

// Sets default values
APathfinding::APathfinding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APathfinding::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APathfinding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Seeker&Target 검사
	if (!Seeker || !Target) return;
	FindPath(Seeker->GetActorLocation(), Target->GetActorLocation());

}

void APathfinding::FindPath(const FVector& StartPos, const FVector& TargetPos) {
	FNode* StartNode = Grid->NodeFromWorldPoint(StartPos);
	FNode* TargetNode = Grid->NodeFromWorldPoint(TargetPos);

	TArray<FNode*> OpenSet;
	TSet<FNode*> ClosedSet;
	OpenSet.Emplace(StartNode);

	while (OpenSet.Num() > 0) {
		// OpenSet에서 FCost가 가장 작으면서, HCost 또한 작은 노드
		FNode* CurrentNode = OpenSet[0];
		for (int i = 0; i < OpenSet.Num(); i++) {
			if (
				OpenSet[i]->FCost() < CurrentNode->FCost() ||
				(OpenSet[i]->FCost() == CurrentNode->FCost() && OpenSet[i]->HCost < CurrentNode->HCost)
				) {
				CurrentNode = OpenSet[i];
			}
		}
		OpenSet.Remove(CurrentNode);
		ClosedSet.Emplace(CurrentNode);

		if (CurrentNode == TargetNode) {
			RetracePath(StartNode, TargetNode);
			return;
		}

		for (FNode* Neighbour : Grid->GetNeighbours(CurrentNode)) {
			if (!Neighbour->bWalkable || ClosedSet.Contains(Neighbour)) continue;

			int NewMovementCostToNeighbour = CurrentNode->GCost + GetDistance(CurrentNode, Neighbour);
			if (NewMovementCostToNeighbour < Neighbour->GCost || !OpenSet.Contains(Neighbour)) {
				Neighbour->GCost = NewMovementCostToNeighbour;
				Neighbour->HCost = GetDistance(Neighbour, TargetNode);
				Neighbour->Parent = CurrentNode;

				if (!OpenSet.Contains(Neighbour)) OpenSet.Emplace(Neighbour);

			}

		}

	}
}

int APathfinding::GetDistance(FNode* NodeA, FNode* NodeB) {

	int DstX = abs(NodeA->GridX - NodeB->GridX);
	int DstY = abs(NodeA->GridY - NodeB->GridY);

	if (DstX > DstY)
		return 14 * DstY + 10 * (DstX - DstY);
	return 14 * DstX + 10 * (DstY - DstX);
}

void APathfinding::RetracePath(FNode* StartNode, FNode* EndNode) {

	Grid->Path.Empty();
	FNode* CurrentNode = EndNode;

	while (CurrentNode != StartNode) {
		Grid->Path.Emplace(CurrentNode);
		if (!CurrentNode->Parent) {
			UE_LOG(LogTemp, Warning, TEXT("Parent is nullptr"));
			return;
		}
		CurrentNode = CurrentNode->Parent;
	}
	Grid->Path.Emplace(StartNode);

	Algo::Reverse(Grid->Path);

}
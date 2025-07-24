// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarComponent.h"

#include <corecrt_io.h>

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAStarComponent::UAStarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAStarComponent::BeginPlay()
{
	Super::BeginPlay();


	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	
	GetGridArray();

	UE_LOG(LogTemp, Warning, TEXT("Grid Array Got"));

	UE_LOG(LogTemp, Warning, TEXT("A* Starting"));

	AStar();
}


// Called every frame
void UAStarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAStarComponent::GetGridArray()
{
	AActor* MapGenActor = UGameplayStatics::GetActorOfClass(GetWorld(), AAStarMapGeneration::StaticClass());
	MapGeneration = Cast<AAStarMapGeneration>(MapGenActor);
	Grid = MapGeneration->GetGrid();

	NodeGrid.SetNum(MapGeneration->GetGridWidth());

	for (TArray<FAStarNode>& currentRow: NodeGrid)
	{
		currentRow.SetNum(MapGeneration->GetGridHeight());
	}
}

void UAStarComponent::AStar()
{
	UE_LOG(LogTemp, Warning, TEXT("A* Started"));

	// need to get spawn location from whatever class I do that in / ML one

	FAStarNode start = FAStarNode(MapGeneration->GetRandomOpenSpace());
	FAStarNode target = FAStarNode(TargetPos);

	if (Grid[TargetPos.X][TargetPos.Y] == 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "No Target Found");
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Start Node Locations : %s"), *start.GetPosition().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Target Node Locations : %s"), *target.GetPosition().ToString());

	Nodes.Add(StartPos, start);
	
	OpenLocations.push_back(start);

	UE_LOG(LogTemp, Warning, TEXT("Start added to open"));
	
	while (OpenLocations.size() > 0)
	{
		CurrentNode = OpenLocations.front(); 
		
		int index = -1;
		int indexToDelete = 0;
		
		for (FAStarNode item : OpenLocations)
		{
			index++;
			if (item.GetF() < CurrentNode.GetF())
			{
				indexToDelete = index;
				CurrentNode = item;
			}
		}		

		
		OpenLocations.erase(OpenLocations.begin() + indexToDelete);
		ClosedLocations.push_back(CurrentNode.GetPosition());

		if (CurrentNode.GetPosition() == target.GetPosition())
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Target Found");

			OpenLocations.clear();
			
			FAStarNode current = CurrentNode;
			TArray<FAStarNode> path;

			while (current.GetPosition() != start.GetPosition())
			{
				path.Add(current);
				current = NodeGrid[current.GetPosition().X][current.GetPosition().Y];
			}

			path.Add(start);

			for (int i = path.Num(); i > 0; i--)
			{
				if (i != path.Num())
				{
					DrawDebugLine(GetWorld(), FVector(path[i].GetPosition().X * 100, path[i].GetPosition().Y * 100, 100 ),
						FVector(path[i-1].GetPosition().X * 100, path[i-1].GetPosition().Y * 100, 100), FColor::Red, true);
				}
			}

			
			for (int i = 0; i < MapGeneration->GetGridWidth(); i++)
			{
				for (int j = 0; j < MapGeneration->GetGridHeight(); j++)
				{
					if (NodeGrid[i][j].GetG() != 0)
					{
						DrawDebugBox(GetWorld(), FVector((i * 100) - 50, (j * 100) - 50, 50), FVector(100, 100, 1), FColor::Green, true);
					}
				}
			}

			break;
		}
		
		AddChild(FVector2D(CurrentNode.GetPosition().X - 1, CurrentNode.GetPosition().Y));
		AddChild(FVector2D(CurrentNode.GetPosition().X + 1, CurrentNode.GetPosition().Y));		
		AddChild(FVector2D(CurrentNode.GetPosition().X, CurrentNode.GetPosition().Y - 1));		
		AddChild(FVector2D(CurrentNode.GetPosition().X, CurrentNode.GetPosition().Y + 1));

		AddChild(FVector2D(CurrentNode.GetPosition().X - 1, CurrentNode.GetPosition().Y - 1));
		AddChild(FVector2D(CurrentNode.GetPosition().X + 1, CurrentNode.GetPosition().Y -1));		
		AddChild(FVector2D(CurrentNode.GetPosition().X - 1, CurrentNode.GetPosition().Y + 1));		
		AddChild(FVector2D(CurrentNode.GetPosition().X + 1, CurrentNode.GetPosition().Y + 1));

		for (FAStarNode child : ChildrenNodes)
		{
			for (FAStarNode open : OpenLocations)
			{
				if (child.GetPosition() == open.GetPosition())
				{
					continue;
				}
			}

			OpenLocations.push_back(child);
			OpenLocationsPositions.push_back(child.GetPosition());
			NodeGrid[child.GetPosition().X][child.GetPosition().Y] = CurrentNode;
		}

		ChildrenNodes.Empty();
	}
}

void UAStarComponent::AddChild(FVector2D childNodePos)
{
	if (childNodePos.X >= 0 && childNodePos.Y >= 0 && childNodePos.X < MapGeneration->GetGridWidth() && childNodePos.Y < MapGeneration->GetGridHeight())
	{
		if (Grid[childNodePos.X][childNodePos.Y] == 0)
		{			
			if (!(std::find(ClosedLocations.begin(), ClosedLocations.end(), childNodePos) != ClosedLocations.end()))
			{
				if (!(std::find(OpenLocationsPositions.begin(), OpenLocationsPositions.end(), childNodePos) != OpenLocationsPositions.end()))
				{
					FAStarNode newChildNode = FAStarNode(childNodePos);

					newChildNode.SetG(CurrentNode.GetG() + 1);
					newChildNode.SetH(FMath::Pow((newChildNode.GetPosition().X - TargetPos.X), 2) + FMath::Pow((newChildNode.GetPosition().Y - TargetPos.Y), 2));
					newChildNode.SetF(newChildNode.GetG() + newChildNode.GetH());
				
					ChildrenNodes.Add(newChildNode);	
				}							
			}
		}
	}

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarComponent.h"

#include <corecrt_io.h>

#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogAStarComponent, Log, All);

// Sets default values for this component's properties
UAStarComponent::UAStarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UAStarComponent::BeginPlay()
{
	Super::BeginPlay();

	StartTime = FPlatformTime::Seconds();
	
	GetOwner()->SetActorLocation(FVector(100, 100, 0));

	AStar();
}


// Called every frame
void UAStarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner()->ActorHasTag("A*DefaultPawn") && PathIndex >= 0 && MovePawn)
	{
		FVector CurrentLocation = GetOwner()->GetActorLocation();
		FVector NextLocation = FVector(Path[PathIndex].X * 100, Path[PathIndex].Y * 100, 0);

		FVector Direction = (NextLocation - CurrentLocation).GetSafeNormal();
		FVector NewLocation = CurrentLocation + Direction * DeltaTime * 600;

		GetOwner()->SetActorLocation(NewLocation);

		if (FVector::Dist(CurrentLocation, NextLocation) <= 25)
		{
			PathIndex--;

			if (PathIndex < 0)
			{
				EndTime = FPlatformTime::Seconds();
				UE_LOG(LogAStarComponent, Log, TEXT("Arrived at target : %f ms"), EndTime - StartTime);
			}
		}
	}
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
	Nodes.Empty();
	OpenLocations.clear();
	ClosedLocations.clear();
	Path.Empty();
	NodeGrid.Empty();
	OpenLocationsPositions.clear();
	ChildrenNodes.Empty();

	GetGridArray();
	
	UE_LOG(LogTemp, Warning, TEXT("A* Started"));

	FAStarNode start = FAStarNode(StartPos);
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
				Path.Add(current.GetPosition());
				current = NodeGrid[current.GetPosition().X][current.GetPosition().Y];
			}

			path.Add(start);
			Path.Add(StartPos);

			for (int i = path.Num(); i > 0; i--)
			{
				if (i != path.Num())
				{
					DrawDebugLine(GetWorld(), FVector(path[i].GetPosition().X * 100, path[i].GetPosition().Y * 100, 100 ),
						FVector(path[i-1].GetPosition().X * 100, path[i-1].GetPosition().Y * 100, 100), FColor::Red, true);
				}
			}

			int NodeVisitedCount = 0;
			for (int i = 0; i < MapGeneration->GetGridWidth(); i++)
			{
				for (int j = 0; j < MapGeneration->GetGridHeight(); j++)
				{					
					if (NodeGrid[i][j].GetG() != 0)
					{
						NodeVisitedCount++;
						DrawDebugBox(GetWorld(), FVector((i * 100) - 50, (j * 100) - 50, 50), FVector(100, 100, 1), FColor::Green, true);
					}
				}
			}

			UE_LOG(LogAStarComponent, Log, TEXT("Pawn : %s, Nodes Visited : %d"), *GetOwner()->GetName() ,NodeVisitedCount);
			
			EndTime = FPlatformTime::Seconds();
			UE_LOG(LogAStarComponent, Log, TEXT("Path Generation took : %f ms"), EndTime - StartTime);			
			
			PathIndex = Path.Num() - 1; 
			MovePawn = true;

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
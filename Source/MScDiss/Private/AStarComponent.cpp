// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarComponent.h"

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
		
}

void UAStarComponent::AStar()
{
	UE_LOG(LogTemp, Warning, TEXT("A* Started"));

	// need to get spawn location from whatever class I do that in / ML one

	FAStarNode start = FAStarNode(StartPos, StartPos);
	FAStarNode target = FAStarNode(TargetPos, TargetPos);
	
	OpenLocations.push_back(start);

	UE_LOG(LogTemp, Warning, TEXT("Start added to open"));
	
	while (OpenLocations.size() > 0)
	{
		CurrentNode = OpenLocations.front(); 
		CurrentNodePointer = &CurrentNode;
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
		//OpenLocations.remove(index); //TODO: not removing?
		//OpenLocations.pop_front(); //removed the front but current node was second element for some reason
		ClosedLocations.push_back(CurrentNode.GetPosition());

		UE_LOG(LogTemp, Warning, TEXT("Current Added to close"));

		if (CurrentNode.GetPosition() == target.GetPosition())
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Target Found");

			OpenLocations.clear();
			
			FAStarNode* current = &CurrentNode;
			TArray<FVector2D> path;

			/*while (current->GetPosition() != start.GetPosition())
			{
				path.Add(current->GetPosition());
				current = current->GetParentPosition();
			}

			for (int i = path.Num() - 1; i >= 0; i--)
			{
				if (i != path.Num())
				{
					DrawDebugLine(GetWorld(), FVector(path[i].X * 100, path[i].Y * 100, 100 ), FVector(path[i-1].X * 100, path[i-1].Y * 100, 100), FColor::Red, true);
				}
			}*/

			/*while (current != nullptr)
			{
				path.Add(current->GetPosition());
				current = current->GetParent();
			}

			for (int i = path.Num() - 1; i >= 0; i--)
			{			
				if (i != path.Num())
				{
					DrawDebugLine(GetWorld(), FVector(path[i].X * 100, path[i].Y * 100, 100 ), FVector(path[i-1].X * 100, path[i-1].Y * 100, 100), FColor::Red, true);
				}
				Path.Add(path[i]);*/
		}
		// SO : we find the path, following the psuedocode, then we reverse it, then we send a signal to the pawn to move through the reversed path somehow

		AddChild(FVector2D(CurrentNode.GetPosition().X - 1, CurrentNode.GetPosition().Y));
		AddChild(FVector2D(CurrentNode.GetPosition().X + 1, CurrentNode.GetPosition().Y));		
		AddChild(FVector2D(CurrentNode.GetPosition().X, CurrentNode.GetPosition().Y - 1));		
		AddChild(FVector2D(CurrentNode.GetPosition().X, CurrentNode.GetPosition().Y + 1));

		UE_LOG(LogTemp, Warning, TEXT("Children Found"));
		UE_LOG(LogTemp, Warning, TEXT("Children Array Num : %d"), ChildrenNodes.Num());


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
				UE_LOG(LogTemp, Warning, TEXT("Child floor found"));
				FAStarNode newChildNode = FAStarNode(childNodePos, CurrentNode.GetPosition());

				newChildNode.SetG(CurrentNode.GetG() + 1);
				newChildNode.SetH(FMath::Pow((newChildNode.GetPosition().X - TargetPos.X), 2) + FMath::Pow((newChildNode.GetPosition().Y - TargetPos.Y), 2));
				newChildNode.SetF(newChildNode.GetG() + newChildNode.GetH());
				UE_LOG(LogTemp, Warning, TEXT("Set ghf"));
				
				ChildrenNodes.Add(newChildNode);		
			}
		}
	}
}
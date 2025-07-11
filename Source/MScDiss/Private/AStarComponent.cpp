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

	FAStarNode start = FAStarNode(StartPos, nullptr);
	FAStarNode target = FAStarNode(TargetPos, nullptr);
	
	OpenLocations.Add(&start);

	UE_LOG(LogTemp, Warning, TEXT("Start added to open"));
	
	while (OpenLocations.Num() > 0)
	{
		FAStarNode* currentNode = OpenLocations[0];
		int currentIndex = 0;

		for (FAStarNode* item : OpenLocations)
		{
			if (item->GetF() < currentNode->GetF())
			{
				currentNode  = item;				
			}
		}		

		OpenLocations.Remove(currentNode);
		ClosedLocations.Add(currentNode);

		UE_LOG(LogTemp, Warning, TEXT("Current Added to close"));

		if (currentNode->GetPosition() == target.GetPosition())
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Target Found");
			/*FAStarNode* current = CurrentNode;
			TArray<FVector2D> path;

			while (current != nullptr)
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
				Path.Add(path[i]);
			}*/

			

			// SO : we find the path, following the psuedocode, then we reverse it, then we send a signal to the pawn to move through the reversed path somehow
		}

		AddChild(FVector2D(currentNode->GetPosition().X - 1, currentNode->GetPosition().Y));
		AddChild(FVector2D(currentNode->GetPosition().X + 1, currentNode->GetPosition().Y));		
		AddChild(FVector2D(currentNode->GetPosition().X, currentNode->GetPosition().Y - 1));		
		AddChild(FVector2D(currentNode->GetPosition().X, currentNode->GetPosition().Y + 1));

		UE_LOG(LogTemp, Warning, TEXT("Children Found"));


		for (FAStarNode* child : ChildrenNodes)
		{
			for (FAStarNode* closed : ClosedLocations)
			{
				if (child->GetPosition() == closed->GetPosition())
				{
					continue;
				}
			}

			child->SetG(currentNode->GetG() + 1);
			child->SetH(FMath::Pow((child->GetPosition().X - target.GetPosition().X), 2) + FMath::Pow((child->GetPosition().Y - target.GetPosition().Y), 2));
			child->SetF(child->GetG() + child->GetH());
			UE_LOG(LogTemp, Warning, TEXT("Set ghf"));


			for (FAStarNode* open : OpenLocations)
			{
				if (OpenLocations.Contains(child))
				{
					if (child->GetG() > open->GetG())
					{
						continue;
					}
				}
			}

			OpenLocations.Add(child);
		}
		
	}
}

void UAStarComponent::AddChild(FVector2D childNodePos)
{
	if (childNodePos.X >= 0 && childNodePos.Y >= 0 && childNodePos.X < MapGeneration->GetGridWidth() && childNodePos.Y < MapGeneration->GetGridHeight())
	{
		if (Grid[childNodePos.X][childNodePos.Y] == 0)
		{
			FAStarNode newChildNode = FAStarNode(childNodePos, CurrentNode);
			ChildrenNodes.Add(&newChildNode);			
		}
	}
}
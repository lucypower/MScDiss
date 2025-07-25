// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AStarMapGeneration.h"
#include "Components/ActorComponent.h"
#include "AStarComponent.generated.h"

USTRUCT(BlueprintType)
struct FAStarNode
{
	GENERATED_USTRUCT_BODY();

private :

	int G = 0, H = 0, F = 0;

	FVector2D Position;

	//FAStarPathNode ParentNode;


public :

	FAStarNode(){};

	FAStarNode(FVector2D position)
	{
		Position = position;
		//ParentPosition = parentPosition;
	};

	int GetG() { return G; };

	int GetH() { return H; };

	int GetF() { return F; };

	void SetG(int g) {G = g;}

	void SetH(int h) {H = h;}

	void SetF(int f) {F = f;}

	FVector2D GetPosition() { return Position; };

	//FVector2D GetParentPosition() { return ParentPosition; };
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MSCDISS_API UAStarComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Grid")
	AAStarMapGeneration* MapGeneration;

	TArray<TArray<int>> Grid;
	TArray<TArray<FAStarNode>> NodeGrid;
	
	FAStarNode CurrentNode;

	UPROPERTY()
	TMap<FVector2D, FAStarNode> Nodes;
	
	std::vector<FAStarNode> OpenLocations;
	std::list<FVector2D> ClosedLocations;
	TArray<FAStarNode> ChildrenNodes;
	
	std::vector<FVector2D> OpenLocationsPositions;

	TArray<FVector2D> Path;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category = "Grid")
	FVector2D StartPos;
	UPROPERTY(EditAnywhere, Category = "Grid")
	FVector2D TargetPos;
	
	UAStarComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GetGridArray();

	void AStar();

	void AddChild(FVector2D childNodePos);

	TArray<FVector2D> GetPath() const {return Path;} ;
};

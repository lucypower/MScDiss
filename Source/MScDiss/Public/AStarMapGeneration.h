// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AStarMapGeneration.generated.h"

UCLASS()
class MSCDISS_API AAStarMapGeneration : public AActor
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;

public:	

	AAStarMapGeneration();
	
	virtual void Tick(float DeltaTime) override;

	TArray<TArray<int>> Grid, TempGrid;

	TArray<FVector2D> OpenSpaces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generation")
	int GridHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generation")
	int GridWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generation")
	int GridDensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generation")
	int GridIterations;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generation")
	TSubclassOf<AActor> GridWalls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generation")
	int MinRegionSize;

	int CorridorIterations = 0;
	
	void GetRegionsAndEdges();

	TArray<TArray<FVector2D>> GetMapRegions();

	TArray<FVector2D> GetRegion(int x, int y);

	TArray<TArray<FVector2D>> GetMapEdges(TArray<TArray<FVector2D>> regions);

	TArray<FVector2D> GetRegionEdges(TArray<FVector2D> region);

	void FindConnectingRooms(TArray<TArray<FVector2D>> edges);

	void ConnectRooms(FVector2D startCoord, FVector2D endCoord);
	
	void GenerateGrid();

	void IterateGrid();

	void AddBorder();

	void InstantiateGrid();

	int GetNeighbouringWallCount(int x, int y);

	TArray<TArray<int>> GetGrid() {return Grid;};

	int GetGridWidth() {return GridWidth;};

	int GetGridHeight() {return GridHeight;};

	FVector2D GetRandomOpenSpace() {return OpenSpaces[FMath::RandRange(0, OpenSpaces.Num())];}; // TODO: Something about this makes the searching weird, come back to?
};

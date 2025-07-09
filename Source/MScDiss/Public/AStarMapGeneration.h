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

	
	void GenerateGrid();

	void IterateGrid();

	void InstantiateGrid();

	int GetNeighbouringWallCount(int x, int y);

};

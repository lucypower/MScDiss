
#include "AStarMapGeneration.h"

AAStarMapGeneration::AAStarMapGeneration()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAStarMapGeneration::BeginPlay()
{
	Super::BeginPlay();

	GenerateGrid();

	IterateGrid();

	InstantiateGrid();
}

void AAStarMapGeneration::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAStarMapGeneration::GenerateGrid()
{
	Grid.SetNum(GridWidth);

	for (TArray<int>& currentRow: Grid)
	{
		currentRow.SetNum(GridHeight);
	}

	TempGrid.SetNum(GridWidth);
	
	for (TArray<int>& currentRow: TempGrid)
	{
		currentRow.SetNum(GridHeight);
	}

	for (int i = 0; i < GridWidth; i++)
	{
		for (int j = 0; j < GridHeight; j++)
		{
			Grid[i][j] = FMath::RandRange(0, 100) > GridDensity ? 0 : 1;
		}
	}
}

void AAStarMapGeneration::IterateGrid()
{
	for (int k = 0; k < GridIterations; k++)
	{
		for (int i = 0; i < GridWidth; i++)
		{
			for (int j = 0; j < GridHeight; j++)
			{
				int neighbouringWalls = GetNeighbouringWallCount(i, j);

				TempGrid[i][j] = neighbouringWalls > 4 ? 1 : 0;
			}
		}

		for (int i = 0; i < GridWidth; i++)
		{
			for (int j = 0; j < GridHeight; j++)
			{
				Grid[i][j] = TempGrid[i][j];
			}
		}
	}

	for (int i = 0; i < GridWidth; i++)
	{
		for (int j = 0; j < GridHeight; j++)
		{
			if (i == 0 || j == 0 || i == GridWidth - 1 || j == GridHeight - 1)
			{
				Grid[i][j] = 1;
			}
		}
	}
}

int AAStarMapGeneration::GetNeighbouringWallCount(int x, int y)
{
	int neighbouringWalls = 0;

	for (int neighbourX = x -1; neighbourX <= x + 1; neighbourX++)
	{
		for (int neighbourY = y - 1; neighbourY <= y + 1; neighbourY++)
		{
			if (neighbourX >= 0 && neighbourX < GridWidth && neighbourY >= 0 && neighbourY < GridHeight)
			{
				if (neighbourX != 0 || neighbourY != 0)
				{
					neighbouringWalls += Grid[neighbourX][neighbourY];
				}
			}
		}
	}

	return neighbouringWalls;
}

void AAStarMapGeneration::InstantiateGrid()
{
	FActorSpawnParameters spawnParameters;

	for (int i = 0; i < GridWidth; i++)
	{
		for (int j = 0; j < GridHeight; j++)
		{
			if (Grid[i][j] == 1)
			{
				GetWorld()->SpawnActor<AActor>(GridWalls, FVector(i * 100, j * 100, 50), FRotator(0, 0, 0), spawnParameters);
			}
		}
	}
}

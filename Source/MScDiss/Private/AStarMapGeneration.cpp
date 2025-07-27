
#include "AStarMapGeneration.h"
#include "Components/LineBatchComponent.h"

AAStarMapGeneration::AAStarMapGeneration()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAStarMapGeneration::BeginPlay()
{
	Super::BeginPlay();

	GenerateGrid();
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

	IterateGrid();

	AddBorder();

	GetRegionsAndEdges();

	InstantiateGrid();
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
}

void AAStarMapGeneration::AddBorder()
{
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

void AAStarMapGeneration::ResetGrid()
{
	GetWorld()->PersistentLineBatcher->Flush();
	
	for (AActor* wall : Walls)
	{
		wall->Destroy();
	}

	Walls.Empty();
	TempGrid.Empty();
	Grid.Empty();
	OpenSpaces.Empty();
	WallSpaces.Empty();

	GenerateGrid();

	if (Grid[1][1] == 1 || Grid[GridWidth - 2][GridHeight - 2] == 1)
	{
		ResetGrid();
	}
	
	/*if (Grid[GridWidth - 2][GridHeight - 2] == 1)
	{
		ResetGrid();
	}*/
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
				Walls.Add(GetWorld()->SpawnActor<AActor>(GridWalls, FVector(i * 100, j * 100, 50), FRotator(0, 0, 0), spawnParameters));
			}
			else
			{
				OpenSpaces.Add(FVector2D(i, j));
			}
		}
	}
}

void AAStarMapGeneration::GetRegionsAndEdges()
{
	TArray<TArray<FVector2D>> regions = GetMapRegions();
	TArray<TArray<FVector2D>> regionsToRemove;

	for (TArray<FVector2D> region : regions)
	{
		if (region.Num() < MinRegionSize)
		{
			for (int i = 0; i < region.Num(); i++)
			{
				Grid[region[i].X][region[i].Y] = 1;
			}

			region.Empty();
			regionsToRemove.Add(region);
		}
	}

	for (int i = 0; i < regionsToRemove.Num(); i++)
	{
		regions.Remove(regionsToRemove[i]);
	}
	TArray<TArray<FVector2D>> edges = GetMapEdges(regions);

	if (regions.Num() > 1)
	{
		FindConnectingRooms(edges);
		AddBorder();
	}

	CorridorIterations++;

	if (regions.Num() > 1)
	{
		regions.Empty();
		edges.Empty();
		WallSpaces.Empty();
		
		GetRegionsAndEdges();

		/*if (CorridorIterations <= 5)
		{
			GetRegionsAndEdges();
		}*/
	}
}

TArray<TArray<FVector2d>> AAStarMapGeneration::GetMapRegions()
{
	TArray<TArray<FVector2D>> mapRegions;
	TArray<TArray<int>> tilesChecked;	

	tilesChecked.SetNum(GridWidth);
	for (TArray<int>& currentRow : tilesChecked)
	{
		currentRow.SetNum(GridHeight);
	}

	for (int i = 0; i < GridWidth; i++)
	{
		for (int j = 0; j < GridHeight; j++)
		{
			if (tilesChecked[i][j] == 0 && Grid[i][j] == 0)
			{
				TArray<FVector2D> region = GetRegion(i, j);
				mapRegions.Add(region);

				for (FVector2D tile : region)
				{
					tilesChecked[tile.X][tile.Y] = 1;
				}
			}
			else
			{
				tilesChecked[i][j] = 1;
			}
		}
	}
	
	return mapRegions;
}

TArray<FVector2D> AAStarMapGeneration::GetRegion(int x, int y)
{
	TArray<FVector2D> tiles;
	TArray<TArray<int>> tilesChecked;	

	tilesChecked.SetNum(GridWidth);
	for (TArray<int>& currentRow : tilesChecked)
	{
		currentRow.SetNum(GridHeight);
	}
	
	TQueue<FVector2D> tilesToCheck;
	tilesToCheck.Enqueue(FVector2D(x, y));
	tilesChecked[x][y] = 1;

	while (!tilesToCheck.IsEmpty())
	{
		FVector2D tile;
		tilesToCheck.Dequeue(tile);
		
		tiles.Add(tile);

		for (int i = tile.X - 1; i <= tile.X + 1; i++)
		{
			for (int j = tile.Y - 1; j <= tile.Y + 1; j++)
			{
				if (i >= 0 && i < GridWidth && j >= 0 && j < GridHeight && (i == tile.X || j == tile.Y))
				{
					if (tilesChecked[i][j] == 0 && Grid[i][j] == 0)
					{
						tilesChecked[i][j] = 1;
						tilesToCheck.Enqueue(FVector2D(i, j));
					}
					else
					{
						tilesChecked[i][j] = 1;
					}
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Region Count : %d"), tiles.Num());
	
	return tiles;
}

TArray<TArray<FVector2D>> AAStarMapGeneration::GetMapEdges(TArray<TArray<FVector2D>> regions)
{
	TArray<TArray<FVector2D>> edgeRegions;

	for (TArray<FVector2D> region : regions)
	{
		TArray<FVector2D> edges = GetRegionEdges(region);
		edgeRegions.Add(edges);
	}

	return edgeRegions;
}

TArray<FVector2D> AAStarMapGeneration::GetRegionEdges(TArray<FVector2D> region)
{
	TArray<FVector2D> roomTiles = region;
	TArray<FVector2D> edgeTiles;

	for (FVector2D tile : roomTiles)
	{
		for (int i = tile.X - 1; i <= tile.X + 1; i++)
		{
			for (int j = tile.Y - 1; j <= tile.Y + 1; j++)
			{
				if ((i == tile.X || j == tile.Y) && Grid[i][j] == 1)
				{
					edgeTiles.Add(tile);
					WallSpaces.Add(FVector2D(tile.X * 100, tile.Y * 100));
				}
			}
		}
	}

	return edgeTiles;
}

void AAStarMapGeneration::FindConnectingRooms(TArray<TArray<FVector2D>> edges)
{
	bool connectionFound = false;
	int shortestDistance = 0;

	FVector2D bestA, bestB;

	for (TArray<FVector2D> regionA : edges)
	{
		connectionFound = false;

		for (TArray<FVector2D> regionB : edges)
		{
			if (regionA == regionB)
			{
				continue;
			}

			for (int i = 0; i < regionA.Num(); i++)
			{
				for (int j = 0; j < regionB.Num(); j++)
				{
					FVector2D a = regionA[i];
					FVector2D b = regionB[j];

					if (!regionA.Contains(b) && !regionB.Contains(a))
					{
						int distanceBetweenPoints = FMath::Pow(a.X - b.X, 2) + FMath::Pow(a.Y - b.Y, 2);

						if (distanceBetweenPoints < shortestDistance || !connectionFound)
						{
							connectionFound = true;
							shortestDistance = distanceBetweenPoints;

							bestA = a;
							bestB = b;
						}
					}
				}
			}
		}

		if (connectionFound)
		{
			if (bestA.X < bestB.X)
			{
				ConnectRooms(bestA, bestB);
			}
			else if (bestA.X > bestB.X)
			{
				ConnectRooms(bestB, bestA);
			}
			else
			{
				if (bestA.Y <= bestB.Y)
				{
					ConnectRooms(bestA, bestB);
				}
				else
				{
					ConnectRooms(bestB, bestA);
				}
			}
		}
	}
}

void AAStarMapGeneration::ConnectRooms(FVector2D startCoord, FVector2D endCoord)
{
	float dx = endCoord.X - startCoord.X;
	float dy = endCoord.Y - startCoord.Y;

	float gradientOfLine = dy / dx;
	float c = endCoord.Y - (gradientOfLine * endCoord.X);

	if (startCoord.X == endCoord.X)
	{
		for (int i = startCoord.Y; i <= endCoord.Y; i++)
		{
			Grid[startCoord.X][i] = 0;
		}
	}
	else if (startCoord.Y == endCoord.Y)
	{
		for (int i = startCoord.X; i <= endCoord.X; i++)
		{
			Grid[i][startCoord.Y] = 0;
		}
	}
	else
	{
		for (int i = startCoord.X; i <= endCoord.X; i++)
		{
			int x = i;
			float y = (gradientOfLine * x) + c;

			y = FMath::RoundToInt(y);

			if (!isnan(y))
			{
				for (int j = x - 1; j <= x + 1; j++)
				{
					for (int k = y - 1; k <= y + 1; k++)
					{
						Grid[j][k] = 0;
					}
				}				
			}			
		}
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GBGridManager.h"
#include "Grid/GBTileBase.h"

// Sets default values
AGBGridManager::AGBGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	/* Set root component for grid. Needed for moving around in editor. */
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	//Set default as C++ TileBase. Otherwise default value would be none
	TileBase = AGBTileBase::StaticClass();
}

void AGBGridManager::SpawnGrid()
{
	FVector GridTileLocation;

	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	
	/* Remove old attached actors in this Grid Manager */
	for (AActor* AttachedTileActor : AttachedActors)
	{
		if (AttachedTileActor->IsA(AGBTileBase::StaticClass()))
		{
			AttachedTileActor->Destroy();
		}
	}

	/* Spawns the grid and calculates needed positions */
	for (int IndexX = 0; IndexX != GridTileCount.X; ++IndexX)
	{
		for (int IndexY = 0; IndexY != GridTileCount.Y; ++IndexY)
		{
			GridTileLocation = CalculateGridTileLocation(IndexX, IndexY);	
			if (AGBTileBase* SpawnedTile = GetWorld()->SpawnActorDeferred<AGBTileBase>(TileBase, FTransform(GridTileLocation)))
			{
				FIntPoint IndexPos = FIntPoint(IndexX, IndexY);
				SpawnedTile->Index = IndexPos;
				// Need to get actor location before we attach it into the Grid which makes the tile location be relative to the GridManager's position in the world.
				SpawnedTile->WorldLocation = SpawnedTile->GetActorLocation() + GetActorLocation();
				
				UGameplayStatics::FinishSpawningActor(SpawnedTile, FTransform(GridTileLocation));

				// Attach tile actor to the grid for group movement and deleting when spawning new grid.
				SpawnedTile->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);

				TileBaseMap.Add(IndexPos,SpawnedTile);
			}
		}
	}


	// Sets tile neighbors based on intpoint values
	for (auto& tile : TileBaseMap)
	{
		tile.Value->SetNeighbors(this);
	}
}

FVector AGBGridManager::CalculateGridTileLocation(float IndexX, float IndexY)
{
	FVector Location = FVector(IndexX, IndexY, 0);
	Location *= GridSnapValue;

	Location = Location + (GridSnapValue * FVector(0.5, 0.5, 0));

	return Location;
}

FIntPoint AGBGridManager::CalculateGridTileIndex(float IndexX, float IndexY)
{
	FIntPoint TileIndex;
	int Index;

	Index = IndexY * GridTileCount.X;
	Index += IndexX;

	TileIndex.X = Index % GridTileCount.Y;
	TileIndex.Y = Index / GridTileCount.Y;

	return TileIndex;
}

AGBTileBase* AGBGridManager::GetTileAtPosition(FIntPoint pos)
{
	AGBTileBase* tile = TileBaseMap.FindRef(pos);

	return tile;
}

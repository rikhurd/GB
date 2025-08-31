// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GBGlobalGridManager.h"
#include "Grid/GBGridChunk.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGBGlobalGridManager::AGBGlobalGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
}

void AGBGlobalGridManager::OnConstruction(const FTransform& Transform)
{
	if (GlobalGridSize != LastGridCount || ChunkSize != LastChunkSize)
	{
		ClearChunks();
		SpawnGrid();

		LastGridCount = GlobalGridSize;
		LastChunkSize = ChunkSize;
	}
	
}

// Called when the game starts or when spawned
void AGBGlobalGridManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGBGlobalGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGBGlobalGridManager::SpawnGrid() {

	int32 NumChunksX = FMath::DivideAndRoundUp(GlobalGridSize.X, ChunkSize.X);
	int32 NumChunksY = FMath::DivideAndRoundUp(GlobalGridSize.Y, ChunkSize.Y);

	/* Spawns the grid and calculates needed positions */
	for (int IndexX = 0; IndexX != NumChunksX; ++IndexX)
	{
		for (int IndexY = 0; IndexY != NumChunksY; ++IndexY)
		{
			FIntPoint StartTile(NumChunksX * ChunkSize.X, NumChunksY * ChunkSize.Y);

			FIntPoint ActualChunkSize;
			ActualChunkSize.X = FMath::Min(ChunkSize.X, GlobalGridSize.X - StartTile.X);
			ActualChunkSize.Y = FMath::Min(ChunkSize.Y, GlobalGridSize.Y - StartTile.Y);

			FVector SpawnLocation = FVector(StartTile.X * TileSize, StartTile.Y * TileSize, 0);

			AGBGridChunk* GridChunk = GetWorld()->SpawnActorDeferred<AGBGridChunk>(
				GridManagerClass,
				FTransform(SpawnLocation),
				this,           // Owner
				nullptr,        // Instigator
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			if (IsValid(GridChunk)) {
				//GridChunk->ChunkSize = ChunkSize;
			}

			UGameplayStatics::FinishSpawningActor(GridChunk, FTransform(SpawnLocation));
			SpawnedChunks.Add(GridChunk);
			/*
			GridTileLocation = CalculateGridTileLocation(IndexX, IndexY);
			if (AGBTileBase* SpawnedTile = GetWorld()->SpawnActorDeferred<AGBTileBase>(TileBase, FTransform(GridTileLocation)))
			{
				FIntPoint IndexPos = FIntPoint(IndexX, IndexY);
				SpawnedTile->Index = IndexPos;
				// Need to get actor location before we attach it into the Grid which makes the tile location be relative to the GridManager's position in the world.
				SpawnedTile->WorldLocation = SpawnedTile->GetActorLocation() + GetActorLocation();

				UGameplayStatics::FinishSpawningActor(SpawnedTile, FTransform(GridTileLocation));

				// Attach tile actor to the grid for group movement and deleting when spawning new grid.
				SpawnedTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

				TileBaseMap.Add(IndexPos, SpawnedTile);
			}
			*/
		}
	}
}

void AGBGlobalGridManager::ClearChunks()
{
	for (AGBGridChunk* Chunk : SpawnedChunks)
	{
		if (Chunk && !Chunk->IsPendingKillPending())
		{
			Chunk->Destroy();
		}
	}
	SpawnedChunks.Empty();
}

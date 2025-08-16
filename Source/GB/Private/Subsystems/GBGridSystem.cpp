// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GBGridSystem.h"
#include "Grid/GBGridChunk.h"

void UGBGridSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ChunkMap.Empty();

	UE_LOG(LogTemp, Log, TEXT("GridSubsystem initialized with ChunkSize=%d, TileSize=%f"), ChunkSize, TileSize);
}

void UGBGridSystem::Deinitialize()
{
	// Clean up chunks
	ClearGrid();

	Super::Deinitialize();
}

AGBGridChunk* UGBGridSystem::GetOrCreateChunk(const FChunkCoord& ChunkCoord)
{
    if (AGBGridChunk** ChunkPtr = ChunkMap.Find(ChunkCoord))
    {
        return *ChunkPtr;
    }

    // Spawn new chunk
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    // Calculate world position (center of chunk)
    FVector SpawnLocation(ChunkCoord.X * ChunkSize * TileSize, ChunkCoord.Y * ChunkSize * TileSize, 0.0f);
    AGBGridChunk* NewChunk = World->SpawnActor<AGBGridChunk>(AGBGridChunk::StaticClass(), SpawnLocation, FRotator::ZeroRotator);

    if (NewChunk)
    {
        // Initialize chunk with size and tile data (e.g., procedural or from mission)
        NewChunk->InitializeChunk(ChunkCoord, ChunkSize, TileSize);
        ChunkMap.Add(ChunkCoord, NewChunk);
    }

    return NewChunk;
}

void UGBGridSystem::ClearGrid()
{
    for (auto& ChunkPair : ChunkMap)
    {
        if (ChunkPair.Value)
        {
            ChunkPair.Value->Destroy();
        }
    }
    ChunkMap.Empty();
}

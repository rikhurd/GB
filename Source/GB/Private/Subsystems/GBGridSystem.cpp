// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GBGridSystem.h"
#include "Grid/GBGridChunk.h"

void UGBGridSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ChunkMap.Empty();

    // Determine starting chunk coord (usually 0,0)
    FChunkCoord StartChunk(0, 0);

    // Spawn the first chunk actor
    CreateChunk(StartChunk);

	UE_LOG(LogTemp, Log, TEXT("GridSubsystem initialized with ChunkSize=%d, TileSize=%f"), ChunkSize, TileSize);
}

void UGBGridSystem::Deinitialize()
{
	// Clean up chunks
	ClearGrid();

	Super::Deinitialize();
}

AGBGridChunk* UGBGridSystem::GetChunk(const FChunkCoord& ChunkCoord) const
{
    if (AGBGridChunk* const* ChunkPtr = ChunkMap.Find(ChunkCoord))
    {
        return *ChunkPtr;
    }
    return nullptr;
}

AGBGridChunk* UGBGridSystem::CreateChunk(const FChunkCoord& ChunkCoord)
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    FVector SpawnLocation(
        ChunkCoord.X * ChunkSize * TileSize,
        ChunkCoord.Y * ChunkSize * TileSize,
        0.0f);

    AGBGridChunk* NewChunk = World->SpawnActor<AGBGridChunk>(
        AGBGridChunk::StaticClass(),
        SpawnLocation,
        FRotator::ZeroRotator);

    if (NewChunk)
    {
        NewChunk->SetChunkParams(ChunkCoord, ChunkSize, TileSize);
        NewChunk->InitializeChunk();
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

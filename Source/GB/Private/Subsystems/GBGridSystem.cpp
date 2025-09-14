// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GBGridSystem.h"
#include "Grid/GBGridChunk.h"

void UGBGridSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ChunkMap.Empty();
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

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Grid/GBGridStructs.h"
#include "GBGridSystem.generated.h"

class AGBGridChunk;
/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class GB_API UGBGridSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    virtual void Deinitialize() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    TSubclassOf<AGBGridChunk> ChunkClass;

    // Get chunk at given coordinates
    AGBGridChunk* GetChunk(const FChunkCoord& ChunkCoord) const;

    // Spawn a chunk at given coordinates
    AGBGridChunk* CreateChunk(const FChunkCoord& ChunkCoord);

    // Apply a stamp template at a global tile position
    // void ApplyStamp(const FVector2D& GlobalTilePos, UStampTemplateAsset* Template);

    // Remake grid for a mission (with optional premade asset)
    void RemakeGridForMission(const FString& MissionID, UObject* PremadeGridAsset = nullptr);

protected:

    // Size of each chunk (e.g., 32 for 32x32 tiles)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 ChunkSize = 32;

    // Tile size in world units (for positioning chunks)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    float TileSize = 100;

    // Map of loaded chunks
    UPROPERTY()
    TMap<FChunkCoord, AGBGridChunk*> ChunkMap;

private:
    // Helper to load stamp templates from content folder
    void LoadStampTemplates();

    // Helper to clear all chunks
    void ClearGrid();
};

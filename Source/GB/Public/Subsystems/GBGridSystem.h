// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GBGridSystem.generated.h"

class AGBGridChunk;
/**
 * 
 */

 // Struct for chunk coordinates (global grid)
USTRUCT(BlueprintType)
struct FChunkCoord
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "Grid")
    int32 X;
    UPROPERTY(BlueprintReadWrite, Category = "Grid")
    int32 Y;

    FChunkCoord() : X(0), Y(0) {}
    FChunkCoord(int32 InX, int32 InY) : X(InX), Y(InY) {}

    bool operator==(const FChunkCoord& Other) const
    {
        return X == Other.X && Y == Other.Y;
    }

    friend uint32 GetTypeHash(const FChunkCoord& Coord)
    {
        return HashCombine(GetTypeHash(Coord.X), GetTypeHash(Coord.Y));
    }
};

// Struct for tile data (simplified for demo)
USTRUCT(BlueprintType)
struct FTileData
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "Grid")
    FString Type;
    UPROPERTY(BlueprintReadWrite, Category = "Grid")
    float Height;
    UPROPERTY(BlueprintReadWrite, Category = "Grid")
    bool bIsWalkable;

    FTileData() : Type("Grass"), Height(0.0f), bIsWalkable(true) {}
};

UCLASS()
class GB_API UGBGridSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    virtual void Deinitialize() override;

    // Get or spawn a chunk at given coordinates
    AGBGridChunk* GetOrCreateChunk(const FChunkCoord& ChunkCoord);

    // Apply a stamp template at a global tile position
    // void ApplyStamp(const FVector2D& GlobalTilePos, UStampTemplateAsset* Template);

    // Remake grid for a mission (with optional premade asset)
    void RemakeGridForMission(const FString& MissionID, UObject* PremadeGridAsset = nullptr);

protected:
    // Size of each chunk (e.g., 32 for 32x32 tiles)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid", meta = (ClampMin = "8", ClampMax = "128", UIMin = "8", UIMax = "128"))
    int32 ChunkSize = 32;

    // Tile size in world units (for positioning chunks)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid", meta = (ClampMin = "1", UIMin = "1"))
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

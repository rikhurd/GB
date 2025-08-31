// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GBGridStructs.generated.h"

// Struct for chunk coordinates in the infinite grid
USTRUCT(BlueprintType)
struct FChunkCoord : public FIntPoint {

    GENERATED_BODY()

    FChunkCoord() : FIntPoint(0, 0) {}
    FChunkCoord(int32 InX, int32 InY) : FIntPoint(InX, InY) {}
};

// 2D tile index (used for both local and global)
USTRUCT(BlueprintType)
struct FTileIndex : public FIntPoint {

    GENERATED_BODY()

    FTileIndex() : FIntPoint(0, 0) {}
    FTileIndex(int32 InX, int32 InY) : FIntPoint(InX, InY) {}
};

// Struct for tile data within a chunk
USTRUCT(BlueprintType)
struct FTileData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Grid")
    FString Type;

    UPROPERTY(BlueprintReadWrite, Category = "Grid")
    float Height;

    UPROPERTY(BlueprintReadWrite, Category = "Grid")
    bool Walkable;

    UPROPERTY(BlueprintReadWrite, Category = "Grid")
    bool Buildable;

    UPROPERTY(BlueprintReadWrite, Category = "Grid")
    bool Solid;

    FTileData() : Type("Grass"), Height(10.0f), Walkable(true), Buildable(true), Solid(true) {}
};
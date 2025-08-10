// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GBDynamicGridManager.h"

void AGBDynamicGridManager::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    // This runs every time you move or change the actor in the editor
    UE_LOG(LogTemp, Warning, TEXT("OnConstruction called"));
}

double AGBDynamicGridManager::GridHeight() const
{
    return GridChunkSize.X * TileSize;;
}

double AGBDynamicGridManager::GridWidth() const
{
    return GridChunkSize.Y * TileSize;;
}

bool AGBDynamicGridManager::TileValid(FIntPoint TileIndex) const
{
    if (TileIndex.X >= GridChunkSize.X && TileIndex.X < GridChunkSize.X &&
        TileIndex.Y >= GridChunkSize.Y && TileIndex.Y < GridChunkSize.Y) {
        return true;
    }
    return false;
}

void AGBDynamicGridManager::CreateLine(int32 X, int32 Y, UPARAM(ref)TArray<FVector>& Vertices, UPARAM(ref)TArray<FIntVector>& Triangles)
{
    float XStartPosition = X * TileSize;
    float YStartPosition = Y * TileSize;

    /**
    * Define two triangles to form a square tile using the 4 new vertices we are about to add.
    * Since we're adding 4 vertices (Vertex1 to Vertex4), we use the current Vertices.Num() 
    * to calculate the correct indices for these new points.
    * 
    * The numbers (+0, +1, +2, +3) are offsets from the current size of the Vertices array
    * so the triangles reference the new vertices, not any old ones already in the array.
    *
    * Triangle layout (as indices):
    * Triangle 1: Vertex3 (2), Vertex2 (1), Vertex1 (0)
    * Triangle 2: Vertex3 (2), Vertex4 (3), Vertex2 (1)
    */
    FIntVector TileTriangle1 = FIntVector(Vertices.Num() + 2, Vertices.Num() + 1, Vertices.Num() + 0);
    FIntVector TileTriangle2 = FIntVector(Vertices.Num() + 2, Vertices.Num() + 3, Vertices.Num() + 1);

    Triangles.Append({ TileTriangle1, TileTriangle2 });

    /**
     * Define the 4 corners of the tile as 3D points lying flat on the ground (Z = 0).
     *
     * Vertex layout:
     * Vertex1 = Bottom-left
     * Vertex2 = Bottom-right
     * Vertex3 = Top-left
     * Vertex4 = Top-right
     */
    FVector Vertex1 = FVector(XStartPosition, YStartPosition, 0);
    FVector Vertex2 = FVector(XStartPosition + TileSize, YStartPosition, 0);
    FVector Vertex3 = FVector(XStartPosition, YStartPosition + TileSize, 0);
    FVector Vertex4 = FVector(XStartPosition + TileSize, YStartPosition + TileSize, 0);

    Vertices.Append({ Vertex1, Vertex2, Vertex3, Vertex4 });
}



void AGBDynamicGridManager::LocationToTile(FVector Location, bool& Valid, FIntPoint& TileIndex)
{
    // Translate world location relative to the grid origin
    float LocalX = Location.X - GetActorLocation().X;
    float LocalY = Location.Y - GetActorLocation().Y;

    //LocalX /= GridWidth();
   // LocalY /= GridHeight();

   // LocalX *= GridChunkSize.X;
   // LocalY *= GridChunkSize.Y;

    LocalX = FMath::FloorToInt(LocalX / TileSize);
    LocalY = FMath::FloorToInt(LocalY / TileSize);

    LocalX = FMath::Clamp(LocalX, 0, GridChunkSize.X - 1);
    LocalY = FMath::Clamp(LocalY, 0, GridChunkSize.Y - 1);

    TileIndex.X = LocalX;
    TileIndex.Y = LocalY;

    Valid = TileValid(TileIndex);
}

bool AGBDynamicGridManager::TileToGridLocation(FIntPoint& TileIndex, bool Center)
{
    if (!TileValid(TileIndex)) {
        return false;
    }

    float LocalX = TileIndex.X * TileSize;
    float LocalY = TileIndex.Y * TileSize;

    LocalX += GetActorLocation().X;
    LocalY += GetActorLocation().Y;

    if (Center) {
        LocalX += TileSize / 2;
        LocalY += TileSize / 2;
    }

    TileIndex.X = LocalX;
    TileIndex.Y = LocalY;

    return true;
}

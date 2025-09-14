// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GBGridChunk.h"
#include "DynamicMesh/MeshNormals.h"
#include "Components/BoxComponent.h"
#include "Components/DynamicMeshComponent.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "Components/SceneComponent.h"

AGBGridChunk::AGBGridChunk()
{
    ChunkRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = ChunkRoot;

    ChunkMeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DynamicMeshComponent"));

    ChunkBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("ChunkBoundsComponent"));
    ChunkBounds->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ChunkMeshComponent->SetupAttachment(RootComponent);
    ChunkBounds->SetupAttachment(RootComponent);
}

void AGBGridChunk::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (RegenerateMesh) {
        if (!ChunkMeshComponent) {
            return;
        }

        // Calculate plane dimensions
        float HalfWidth = (ChunkSize.X * TileSize) * 0.5f;
        float HalfHeight = (ChunkSize.Y * TileSize) * 0.5f;
        float ZPlane = 5.0f;

        // Build a simple quad (two triangles)
        FDynamicMesh3 Mesh;
        Mesh.EnableAttributes();

        int32 V0 = Mesh.AppendVertex(FVector3d(-HalfWidth, -HalfHeight, ZPlane));
        int32 V1 = Mesh.AppendVertex(FVector3d(HalfWidth, -HalfHeight, ZPlane));
        int32 V2 = Mesh.AppendVertex(FVector3d(HalfWidth, HalfHeight, ZPlane));
        int32 V3 = Mesh.AppendVertex(FVector3d(-HalfWidth, HalfHeight, ZPlane));

        Mesh.AppendTriangle(V0, V2, V1);
        Mesh.AppendTriangle(V0, V3, V2);

        ChunkMeshComponent->SetMesh(MoveTemp(Mesh));

        int32 BoxHeight = TileSize * 0.5f * ChunkHeight;

        FVector BoxExtent = FVector(HalfWidth, HalfHeight, BoxHeight);
        ChunkBounds->SetBoxExtent(BoxExtent);

        ChunkBounds->SetRelativeLocation(FVector(0, 0, BoxHeight));
    }
}

void AGBGridChunk::SetChunkParams(const FChunkCoord& InChunkCoord, int32 InChunkSize, float InTileSize)
{
    ChunkCoord = InChunkCoord;
    ChunkSize = InChunkSize;
    TileSize = InTileSize;
}

bool AGBGridChunk::TileValid(FIntPoint TileIndex) const
{
    if (TileIndex.X >= ChunkSize.X && TileIndex.X < ChunkSize.X &&
        TileIndex.Y >= ChunkSize.Y && TileIndex.Y < ChunkSize.Y) {
        return true;
    }
    return false;
}

/*
void AGBGridChunk::CreateLine(int32 X, int32 Y, TArray<FVector>& InVertices, TArray<int32>& InTriangles)
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
    *
    FIntVector TileTriangle1 = FIntVector(Vertices.Num() + 2, Vertices.Num() + 1, Vertices.Num() + 0);
    FIntVector TileTriangle2 = FIntVector(Vertices.Num() + 2, Vertices.Num() + 3, Vertices.Num() + 1);

    Triangles.Append({ TileTriangle1, TileTriangle2 });

    
    FVector Vertex1 = FVector(XStartPosition, YStartPosition, 0);
    FVector Vertex2 = FVector(XStartPosition + TileSize, YStartPosition, 0);
    FVector Vertex3 = FVector(XStartPosition, YStartPosition + TileSize, 0);
    FVector Vertex4 = FVector(XStartPosition + TileSize, YStartPosition + TileSize, 0);

    Vertices.Append({ Vertex1, Vertex2, Vertex3, Vertex4 });
}
*/


void AGBGridChunk::LocationToTile(FVector Location, bool& Valid, FIntPoint& TileIndex)
{
    // Translate world location relative to the grid origin
    float LocalX = Location.X - GetActorLocation().X;
    float LocalY = Location.Y - GetActorLocation().Y;

    //LocalX /= GridWidth();
   // LocalY /= GridHeight();

   // LocalX *= ChunkSize.X;
   // LocalY *= ChunkSize.Y;

    LocalX = FMath::FloorToInt(LocalX / TileSize);
    LocalY = FMath::FloorToInt(LocalY / TileSize);

    LocalX = FMath::Clamp(LocalX, 0, ChunkSize.X - 1);
    LocalY = FMath::Clamp(LocalY, 0, ChunkSize.Y - 1);

    TileIndex.X = LocalX;
    TileIndex.Y = LocalY;

    Valid = TileValid(TileIndex);
}

bool AGBGridChunk::TileToGridLocation(FIntPoint& TileIndex, bool Center)
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

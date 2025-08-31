// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GBGridChunk.h"
#include "DynamicMesh/MeshNormals.h"

AGBGridChunk::AGBGridChunk()
{
    // Create the DynamicMeshComponent
    ChunkMeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DynamicMeshComponent"));

    SetRootComponent(ChunkMeshComponent);
}

void AGBGridChunk::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

#if WITH_EDITOR
   //InitializeChunk();
#endif
}

void AGBGridChunk::SetChunkParams(const FChunkCoord& InChunkCoord, int32 InChunkSize, float InTileSize)
{
    ChunkCoord = InChunkCoord;
    ChunkSize = InChunkSize;
    TileSize = InTileSize;
}

void AGBGridChunk::InitializeChunk()
{
    // Initialize tile array
    int32 VertexCount = (ChunkSize + 1) * (ChunkSize + 1);
    TileData.Init(FTileData(), VertexCount);

    // Set world position based on chunk coordinates
    FVector Location(ChunkCoord.X * ChunkSize * TileSize, ChunkCoord.Y * ChunkSize * TileSize, 0.0f);
    SetActorLocation(Location);

    FDynamicMesh3 Mesh;
    TArray<int> VertexIndices;
    VertexIndices.SetNum(VertexCount);

    BuildVertices(Mesh, VertexIndices);
    BuildTriangles(Mesh, VertexIndices);

    // Enable mesh attributes (normals, etc.)
    Mesh.EnableAttributes();
    Mesh.EnableVertexNormals(FVector3f::ZeroVector);  // Initialize normals overlay

    // Quick-compute per-vertex normals based on the triangles
    UE::Geometry::FMeshNormals::QuickComputeVertexNormals(Mesh);

    // Optional: If you want UVs for texturing, add these too
    Mesh.EnableVertexUVs(FVector2f::ZeroVector);
    // Then set UVs per-vertex in BuildVertices, e.g., Mesh.SetVertexUV(VertexID, FVector2f(x / ChunkSize, y / ChunkSize));

    ChunkMeshComponent->SetMesh(MoveTemp(Mesh));
    ChunkMeshComponent->NotifyMeshUpdated();

    UE_LOG(LogTemp, Log, TEXT("Chunk init"));

}

void AGBGridChunk::BuildVertices(FDynamicMesh3& Mesh, TArray<int>& VertexIndices)
{
    /**
     * BuildVertices generates the full grid of vertex positions for this chunk.
     * The grid has (ChunkSize+1) x (ChunkSize+1) vertices so that every tile
     * shares edges and corners with its neighbors (no duplicate vertices).
     *
     * Each vertex is placed at (x * TileSize, y * TileSize, Height).
     * Height is sampled from the TileData array when inside valid tile bounds,
     * otherwise it defaults to the actor's base Z.
     *
     * The resulting vertices are stored in VertexIndices so that BuildTriangles()
     * can later stitch them into quads (two triangles per tile).
     */
    for (int32 y = 0; y <= ChunkSize; y++)
    {
        for (int32 x = 0; x <= ChunkSize; x++)
        {
            int32 VertexIndex = y * (ChunkSize + 1) + x;
            float Height = TileData[VertexIndex].Height;

            FVector3d Pos(x * TileSize, y * TileSize, Height);
            VertexIndices[VertexIndex] = Mesh.AppendVertex(Pos);
        }
    }
}

void AGBGridChunk::BuildTriangles(FDynamicMesh3& Mesh, const TArray<int>& VertexIndices)
{
    /**
     * BuildTriangles connects existing vertices into faces by adding their indices
     * to the Triangles array. Each group of 3 indices represents one triangle,
     * defining how the GPU should render the surface.
     *
     * In our grid system, every 4 vertices form a single quad (tile). We add two
     * triangles per quad:
     *   - Triangle 1: BottomLeft, BottomRight, TopLeft
     *   - Triangle 2: TopLeft, BottomRight, TopRight
     *
     * This function does not create new vertices — it only reuses existing ones
     * and ensures they are stitched together into renderable geometry.
     */
    for (int32 y = 0; y < ChunkSize; y++)
    {
        for (int32 x = 0; x < ChunkSize; x++)
        {
            // Grab the 4 corners of this tile (quad) from the vertex array
            int32 BottomLeft = VertexIndices[y * (ChunkSize + 1) + x];
            int32 BottomRight = VertexIndices[y * (ChunkSize + 1) + (x + 1)];
            int32 TopLeft = VertexIndices[(y + 1) * (ChunkSize + 1) + x];
            int32 TopRight = VertexIndices[(y + 1) * (ChunkSize + 1) + (x + 1)];

            // Two triangles per tile: (BL, TL, BR) and (BR, TL, TR)
            Mesh.AppendTriangle(BottomLeft, TopLeft, BottomRight);
            Mesh.AppendTriangle(BottomRight, TopLeft, TopRight);
        }
    }
}

void AGBGridChunk::UpdateTile(int32 TileX, int32 TileY)
{
    FDynamicMesh3* Mesh = ChunkMeshComponent->GetMesh();
    if (!Mesh) return;

    // Vertex indices for the 4 corners of this tile
    int32 BottomLeft = (TileY * (ChunkSize + 1)) + TileX;
    int32 BottomRight = BottomLeft + 1;
    int32 TopLeft = BottomLeft + (ChunkSize + 1);
    int32 TopRight = TopLeft + 1;

    // Heights of the 4 corners (sampled from TileData)
    float HeightBL = TileData[TileY * ChunkSize + TileX].Height;
    float HeightBR = TileData[TileY * ChunkSize + (TileX + 1)].Height;
    float HeightTL = TileData[(TileY + 1) * ChunkSize + TileX].Height;
    float HeightTR = TileData[(TileY + 1) * ChunkSize + (TileX + 1)].Height;

    // Update mesh vertex positions in 3D
    Mesh->SetVertex(BottomLeft, FVector3d(TileX * TileSize, TileY * TileSize, HeightBL));
    Mesh->SetVertex(BottomRight, FVector3d((TileX + 1) * TileSize, TileY * TileSize, HeightBR));
    Mesh->SetVertex(TopLeft, FVector3d(TileX * TileSize, (TileY + 1) * TileSize, HeightTL));
    Mesh->SetVertex(TopRight, FVector3d((TileX + 1) * TileSize, (TileY + 1) * TileSize, HeightTR));

    // Tell the mesh component that the geometry has changed
    ChunkMeshComponent->NotifyMeshUpdated();
}

bool AGBGridChunk::TileValid(FIntPoint TileIndex) const
{
    if (TileIndex.X >= ChunkSize && TileIndex.X < ChunkSize &&
        TileIndex.Y >= ChunkSize && TileIndex.Y < ChunkSize) {
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

    LocalX = FMath::Clamp(LocalX, 0, ChunkSize - 1);
    LocalY = FMath::Clamp(LocalY, 0, ChunkSize - 1);

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

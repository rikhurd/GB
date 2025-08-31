// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Grid/GBGridStructs.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "Components/DynamicMeshComponent.h"

#include "GBGridChunk.generated.h"

/**
 * 
 */
UCLASS()
class GB_API AGBGridChunk : public AActor
{
	GENERATED_BODY()

public:

	AGBGridChunk();

	// OnConstruction is called in the editor whenever the actor is moved or a property is changed.
	// Similar to Constructor in Blueprints
	virtual void OnConstruction(const FTransform& Transform) override;

	void SetChunkParams(const FChunkCoord& InChunkCoord, int32 InChunkSize, float InTileSize);

	UFUNCTION(CallInEditor)
	void InitializeChunk();

	void BuildVertices(FDynamicMesh3& Mesh, TArray<int>& VertexIndices);

	void BuildTriangles(FDynamicMesh3& Mesh, const TArray<int>& VertexIndices);

	void UpdateTile(int32 TileX, int32 TileY);

	/** Returns tile location based on given world location */
	UFUNCTION(BlueprintCallable, Category = "Default")
	void LocationToTile(FVector Location, bool& Valid, FIntPoint& TileIndex);

	/** Gets the TileIndex and returns tile's world location */
	UFUNCTION(BlueprintCallable, Category = "Default")
	bool TileToGridLocation(FIntPoint& TileIndex, bool Center);


protected:

	// Chunk's coordinates in the global grid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
	FChunkCoord ChunkCoord;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDynamicMeshComponent> ChunkMeshComponent;

	// Array of tile data (ChunkSize x ChunkSize)
	UPROPERTY()
	TArray<FTileData> TileData;

	// Size of the chunk (tiles per side, e.g., 32 for 32x32)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 ChunkSize;

	// World size of each tile (in cm)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
	float TileSize;

private:

	/*
	UPROPERTY()
	TArray<FVector> Normals;

	UPROPERTY()
	TArray<FVector2D> UVs;

	UPROPERTY()
	TArray<FProcMeshTangent> Tangents;
	*/

	/** Checks if tile is valid. WIthin ChunkSize bounds */
	UFUNCTION(BlueprintPure, Category = "Private")
	bool TileValid(FIntPoint TileIndex) const;

	/** Please add a function description */
	//UFUNCTION(BlueprintCallable, Category = "Private")
	//void CreateLine(int32 X, int32 Y,TArray<FVector>& InVertices, TArray<int32>& InTriangles);

};
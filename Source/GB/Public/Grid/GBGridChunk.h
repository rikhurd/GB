// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Grid/GBGridStructs.h"

#include "GBGridChunk.generated.h"

class USceneComponent;
class UDynamicMeshComponent;
class UBoxComponent;
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

	UPROPERTY(EditAnywhere, Category="Grid")
	bool RegenerateMesh = true;

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

	// Size of the chunk by tiles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FIntPoint ChunkSize = FIntPoint(12,12);

	// Chunk height by tiles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 ChunkHeight = 5;

	// World size of each tile (in cm)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
	float TileSize = 100;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> ChunkRoot;

	UPROPERTY(VisibleAnywhere, Category = "Grid")
	TObjectPtr<UBoxComponent> ChunkBounds;

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
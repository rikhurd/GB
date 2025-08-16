// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshActor.h"
#include "GBGridChunk.generated.h"

/**
 * 
 */
UCLASS()
class GB_API AGBGridChunk : public ADynamicMeshActor
{
	GENERATED_BODY()

public:

	// OnConstruction is called in the editor whenever the actor is moved or a property is changed.
	// Similar to Constructor in Blueprints
	virtual void OnConstruction(const FTransform& Transform) override;

	void InitializeChunk(const FGridChunkCoord& InChunkCoord, int32 InChunkSize, float InTileSize);

	/** Size of the GridChunk */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere = "Default")
	FIntPoint GridChunkSize = { 3,3 };

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	double TileSize = 100;

	/** Returns tile location based on given world location */
	UFUNCTION(BlueprintCallable, Category = "Default")
	void LocationToTile(FVector Location, bool& Valid, FIntPoint& TileIndex);

	/** Gets the TileIndex and returns tile's world location */
	UFUNCTION(BlueprintCallable, Category = "Default")
	bool TileToGridLocation(FIntPoint& TileIndex, bool Center);

protected:

	// Array of tile data (ChunkSize x ChunkSize)
	UPROPERTY()
	TArray<FGridTileData> TileData;

private:

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Private")
	double GridHeight() const;

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Private")
	double GridWidth() const;

	/** Checks if tile is valid. WIthin GridChunkSize bounds */
	UFUNCTION(BlueprintPure, Category = "Private")
	bool TileValid(FIntPoint TileIndex) const;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Private")
	void CreateLine(int32 X, int32 Y, UPARAM(ref)TArray<FVector>& Vertices, UPARAM(ref)TArray<FIntVector>& Triangles);

};

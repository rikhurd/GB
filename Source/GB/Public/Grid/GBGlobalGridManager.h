// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GBGlobalGridManager.generated.h"

/*

THIS ACTOR IS NO MORE USED REFER TO GBGridSystem

Still in project for reference. Deleted later
*/

//Forward declaration
class AGBGridChunk;

UCLASS()
class GB_API AGBGlobalGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGBGlobalGridManager();

	// OnConstruction is called in the editor whenever the actor is moved or a property is changed.
	// Similar to Constructor in Blueprints
	virtual void OnConstruction(const FTransform& Transform) override;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	FIntPoint GlobalGridSize = { 3,3 };

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	FIntPoint GridChunkSize = { 3,3 };

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double TileSize = 100;

private:

	FIntPoint LastGridCount;
	FIntPoint LastChunkSize;

	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<AGBGridChunk> GridManagerClass;

	UPROPERTY()
	TArray<AGBGridChunk*> SpawnedChunks;

	/** Used to spawn the grid to manager's location */
	UFUNCTION(Category = "Grid functions")
	void SpawnGrid();

	void ClearChunks();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

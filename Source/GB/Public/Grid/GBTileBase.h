// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Math/IntPoint.h"

#include "GBTileBase.generated.h"

//Forward declaration
class AGBGridManager;
class UBoxComponent;
class UGBGridEntityData;
class UPaperSprite;

UCLASS(Blueprintable)
class GB_API AGBTileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGBTileBase();

	/** TileBase Index */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Pathfinding Variables")
		FIntPoint Index;

	// Need to get actor location before we attach it into the Grid which makes the tile location be relative to the GridManager's position in the world.
	/** TileBase World Location */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Pathfinding Variables")
		FVector WorldLocation;

	/** Distance from the tile to the start tile */
	UPROPERTY(BlueprintReadOnly, Category = "Pathfinding Variables")
		double G;

	/** Distance from the tile to the target tile */
	UPROPERTY(BlueprintReadOnly, Category = "Pathfinding Variables")
		double H;

	/** G + H */
	UPROPERTY(BlueprintReadOnly, Category = "Pathfinding Variables")
		double F = G + H;

	/** Is this tile counted on pathfinding */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Pathfinding Variables")
		bool Walkable = false;

	/** Can you place traps/towers on this tile */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Pathfinding Variables")
		bool Placeable = false;

	/** Does the tile spawn a grid entity on spawn and not a Tile */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pathfinding Variables")
		bool SpawnMesh = true;

	/** Array of neighboring grid tiles */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile location variables")
		TArray<AGBTileBase*> Neighbors;

	/** Collision component. Shows grid size and location in the game and editor */
		TObjectPtr<UBoxComponent> BoxCollision;

	/** Static mesh component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile collision")
		TObjectPtr<UStaticMeshComponent> TileMesh;

	/** Please add a variable description */
		AGBTileBase* Connection;

	/** Set distance from the tile to the start tile */
	UFUNCTION(BlueprintCallable, Category = "Set Variable Functions")
		void SetG(double InputG);

	/** Set distance from the tile to the end tile */
	UFUNCTION(BlueprintCallable, Category = "Set Variable Functions")
		void SetH(double InputH);

	/** Set connected tiles */
	UFUNCTION(BlueprintCallable, Category = "Set Variable Functions")
		void SetConnection(AGBTileBase* tileBase);

	/** Set neighbor tiles */
	UFUNCTION(BlueprintCallable, Category = "Set Variable Functions")
		void SetNeighbors(AGBGridManager* GridManager);

	/** Set neighbor tiles */
	UFUNCTION(BlueprintCallable, Category = "Set Variable Functions")
		float GetDistance(AGBTileBase* targetTile);

	/** Process edit action on the tile */
	UFUNCTION(BlueprintCallable, Category = "Set Variable Functions")
		bool ProcessEditAction(UGBGridEntityData* CurrentData);

	/** Make tile be able to place GridEntities */
	UFUNCTION(BlueprintCallable, Category = "Set Variable Functions")
		void MakeGridPlaceable();

	/** Place grid entity on tile, trap, tower etc. */
	UFUNCTION(BlueprintCallable, Category = "Set Variable Functions")
		void PlaceGridEntityOnTile(UClass* GridEntity, UPaperSprite* GridEntitySprite);


protected:
	virtual void BeginPlay() override;

private:

	/** Array of neighboring tile directions
		Here we can for example remove diagonal movement.
	*/
		const TArray<FIntPoint> Dirs = {
		FIntPoint(0, 1), FIntPoint(-1, 0), FIntPoint(0, -1), FIntPoint(1, 0),
		FIntPoint(1, 1), FIntPoint(1, -1), FIntPoint(-1, -1), FIntPoint(-1, 1) };
};

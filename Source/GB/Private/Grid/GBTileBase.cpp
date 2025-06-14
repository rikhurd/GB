// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GBTileBase.h"
#include "Grid/GBGridManager.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Grid/GBGridEditActionType.h"
#include "GridEntities/GBGridEntityData.h"
#include "GridEntities/GBGridEntityBase.h"
#include "PaperSprite.h"

// Sets default values
AGBTileBase::AGBTileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Make box collision which handles the collision for mouse
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TileCollisionComponent"));
	BoxCollision->SetBoxExtent(FVector(100, 100, 16));
	BoxCollision->SetCollisionProfileName("GridTileCollision");
	
	SetRootComponent(BoxCollision);
	BoxCollision->AddLocalOffset(FVector(0, 0, -15));
}

void AGBTileBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGBTileBase::SetG(double InputG)
{
	G = InputG;
	F = G + H;
}

void AGBTileBase::SetH(double InputH)
{
	H = InputH;
	F = G + H;
}

void AGBTileBase::SetConnection(AGBTileBase* tileBase)
{
	Connection = tileBase;
}

//Set tiles neighbors. Get TileBaseArray from the GridManager that calls it
void AGBTileBase::SetNeighbors(AGBGridManager* GridManager)
{
	for (const FIntPoint& Dir : Directions)
	{
		if (AGBTileBase* tileToAdd = GridManager->GetTileAtPosition(Dir + Index))
		{
			Neighbors.Add(tileToAdd);
		}
	}
}

float AGBTileBase::GetDistance(AGBTileBase* targetTile)
{
	FIntPoint dist = FIntPoint(abs(Index.X - targetTile->Index.X), abs(Index.Y - targetTile->Index.Y));

	int lowest = std::min(dist.X, dist.Y);
	int highest = std::max(dist.X, dist.Y);

	int horizontalMovesRequired = highest - lowest;

	return (lowest * 14 + horizontalMovesRequired * 10);
}

bool AGBTileBase::ProcessEditAction(UGBGridEntityData* CurrentData)
{
	switch (CurrentData->GridEditAction) {
		case EGBGridEditActions::Dig:
			SetTilePlaceable();
			return true;
			break;
		case EGBGridEditActions::Tower:
			if (IsValid(CurrentData->GridEntityActor) && Placeable == true) {
				GridEntityActorClass = CurrentData->GridEntityActor;
				PlaceGridEntityOnTile();
				return true;
			}
			else {
				return false;
			}
			break;
			// Trap is not needed?

			/*
		case EGBGridEditActions::Trap:
			PlaceGridEntityOnTile(GridEntity);
			UE_LOG(LogTemp, Warning, TEXT("Trap Enum Hit"));
			return true;
			break;
			*/
		default:
			break;
	}
	return false;
}

void AGBTileBase::UpdateTile()
{

}

void AGBTileBase::SetTilePlaceable() {
	Walkable = true;
	Placeable = true;

	if (IsValid(TileMesh)) {
		TileMesh->SetVisibility(false);
		TileMesh->SetCollisionProfileName("NoCollision");
	}
}

void AGBTileBase::SetTileBlocked() {
		Walkable = false;
		Placeable = false;

		if (IsValid(TileMesh)) {
			TileMesh->SetVisibility(true);
			TileMesh->SetCollisionProfileName("GridTileCollision");
		}
}

/* Modify this so we get Sprite from GridEntityActorClass and the class is changed to be from a base class
* Or on base actor spawn in Construct calculate its placement
*
*/
void AGBTileBase::PlaceGridEntityOnTile() {
	if (!IsValid(GridEntityActorClass)) {
		UE_LOG(LogTemp, Error, TEXT("No occupying GridEntity set on: %s"), *GetName());
		return;
	}
	if (!IsValid(GridEntityActorClass->GetDefaultObject<AGBGridEntityBase>()->GridEntitySprite)) {
		UE_LOG(LogTemp, Error, TEXT("No sprite set on this GridEntity: %s"), *GridEntityActorClass->GetName());
		return;
	}

	/*
	checkf(IsValid(GridEntityActorClass), TEXT("No occupying GridEntity set on: %s"), *GetName());
	checkf(IsValid(GridEntityActorClass->GetDefaultObject<AGBGridEntityBase>()->GridEntitySprite), TEXT("No sprite set on this GridEntity: %s"), *GridEntityActorClass->GetName());
	*/

	if (Placeable)
	{
		FVector GridEntitySpawnLocation = GetActorLocation();
		if (IsValid(GridEntityActorClass)) {
			TObjectPtr<AGBGridEntityBase> ObjectReference = GridEntityActorClass->GetDefaultObject<AGBGridEntityBase>();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Source size: X=%.2f, Z=%.2f"), ObjectReference->GridEntitySprite->GetRenderBounds().BoxExtent.X, ObjectReference->GridEntitySprite->GetRenderBounds().BoxExtent.Z));
			GridEntitySpawnLocation += FVector(0,0, ObjectReference->GridEntitySprite->GetRenderBounds().BoxExtent.Z);
		}

		// Spawn the actor
		GetWorld()->SpawnActor<AGBGridEntityBase>(GridEntityActorClass, GridEntitySpawnLocation, GetActorRotation());

		Walkable = true;
		Placeable = false;
	}
}

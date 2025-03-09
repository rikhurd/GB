// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GBTileBase.h"
#include "Grid/GBGridManager.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Grid/GBGridEditActionType.h"
#include "GridEntities/GBGridEntityData.h"
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

	//if (SpawnMesh == true) {
	//	// TileMesh is set in BP for easier modifications
	//	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMeshComponent"));
	//	TileMesh->SetupAttachment(RootComponent);
	//}
}

void AGBTileBase::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnMesh == false) {
		MakeGridPlaceable();
	}
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
	for (auto& dir : Dirs)
	{
		if (AGBTileBase* tileToAdd = GridManager->GetTileAtPosition(dir + Index))
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
			MakeGridPlaceable();
			return true;
			break;
		case EGBGridEditActions::Tower:
			if (IsValid(CurrentData->GridEntityActor) && Placeable == true) {
				PlaceGridEntityOnTile(CurrentData->GridEntityActor, CurrentData->GridEntitySprite);
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

void AGBTileBase::MakeGridPlaceable() {
	Walkable = true;
	Placeable = true;

	if (IsValid(TileMesh)) {
		TileMesh->DestroyComponent();
	}
}

void AGBTileBase::PlaceGridEntityOnTile(UClass* GridEntityActorClass, UPaperSprite* GridEntitySprite) {
	if (IsValid(GridEntityActorClass) && GetWorld())
	{
		FVector GridEntitySpawnLocation = GetActorLocation();
		if (IsValid(GridEntitySprite)) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Source size: X=%.2f, Y=%.2f"), GridEntitySprite->GetSourceSize().X, GridEntitySprite->GetSourceSize().Y));
			GridEntitySpawnLocation += FVector(0,0,GridEntitySprite->GetSourceSize().Y * 0.5f);
		}

		// Spawn the actor
		GetWorld()->SpawnActor<AActor>(GridEntityActorClass, GridEntitySpawnLocation, GetActorRotation());

		Walkable = true;
		Placeable = false;
	}
}

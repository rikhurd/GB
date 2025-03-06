// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GBPathfinding.generated.h"

//Forward declaration
class AGBTileBase;

UCLASS()
class GB_API UGBPathfinding : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	//UPathfinding();

	UFUNCTION(BlueprintCallable, Category = "Find Path function")
	static TArray<AGBTileBase*> FindPath(AGBTileBase* startNode, AGBTileBase* targetNode);
};

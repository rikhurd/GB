// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PaperSprite.h"
#include "Grid/GBGridEditActionType.h"
#include "GBGridEntityData.generated.h"

/**
 * Use this class to hold data for GridEntitities to display to player.
 */

UCLASS(BlueprintType)
class GB_API UGBGridEntityData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> GridEntityActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText GridEntityName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText GridEntityNameDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGBGridEditActions GridEditAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperSprite* GridEntitySprite;
};
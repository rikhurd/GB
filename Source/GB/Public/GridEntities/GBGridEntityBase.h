// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GBGridEntityBase.generated.h"

class UPaperSprite;

UCLASS()
class GB_API AGBGridEntityBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGBGridEntityBase();

	/** Sprite for the entity */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid Entity Variables")
	TObjectPtr<UPaperSprite> GridEntitySprite;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

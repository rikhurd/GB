// Fill out your copyright notice in the Description page of Project Settings.


#include "GridEntities/GBGridEntityBase.h"
#include "PaperSprite.h"

// Sets default values
AGBGridEntityBase::AGBGridEntityBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGBGridEntityBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGBGridEntityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


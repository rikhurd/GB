// Fill out your copyright notice in the Description page of Project Settings.


#include "GridEntities/GBTrapBase.h"

// Sets default values
AGBTrapBase::AGBTrapBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGBTrapBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGBTrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


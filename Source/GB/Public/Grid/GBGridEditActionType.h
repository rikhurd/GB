// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "GBGridEditActionType.generated.h"

UENUM(BlueprintType)
enum class EGBGridEditActions : uint8
{
    Dig     UMETA(DisplayName = "Dig"),
    Tower   UMETA(DisplayName = "Tower"),
    Trap    UMETA(DisplayName = "Trap"),
    MAX     UMETA(Hidden)
};

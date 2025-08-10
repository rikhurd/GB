// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "GBPlayerController.generated.h"

// Forward declarations
class UInputMappingContext;
class UEnhancedInputComponent;
class UInputAction;
class AGBTileBase;
class UGBGridEntityData;

/**
 * 
 */
UCLASS()
class GB_API AGBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    AGBPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UGBGridEntityData* CurrentData;

    // Enables functionality to choose a tile. Binds input so we can click on tiles.
    UFUNCTION(BlueprintCallable)
    void EnableGridEdit(UGBGridEntityData* GridEntityData);

    // Disables functionality to choose a tile. Breaks input binds. Currently only has a player input for doing this.
    UFUNCTION(BlueprintCallable)
    void DisableGridEdit();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    // Input Mapping Context
    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY()
    UEnhancedInputComponent* EnhancedInputComponent;

    // Handle for tracking the dynamic input binding
    uint32 ChooseBindingHandle;

    // Handle for tracking the dynamic input binding
    uint32 CancelBindingHandle;

    // Input Actions
    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputAction> ChooseAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputAction> CancelAction;

    // Movement Functions
    void Move(const FInputActionValue& Value);
    void Choose(const FInputActionValue& Value);

    UFUNCTION(BlueprintCallable)
    AGBTileBase* GetTileUnderCursor();

    TArray<TEnumAsByte<EObjectTypeQuery>> GridObjectTypes;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/GBPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "Grid/GBTileBase.h"
#include "GridEntities/GBGridEntityData.h"

AGBPlayerController::AGBPlayerController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Initialize default values
}

void AGBPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    // Add Input Mapping Context
    if (IsValid(Subsystem))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}

void AGBPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
    // Set up Enhanced Input Component
    if (IsValid(EnhancedInputComponent))
    {
        // Bind Move action
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGBPlayerController::Move);
    }
}

void AGBPlayerController::Move(const FInputActionValue& Value)
{
    // Get the input vector from the Input Action
    FVector2D MovementVector = Value.Get<FVector2D>();

    APawn* ControlledPawn = GetPawn();
    // Get the controlled pawn
    if (IsValid(ControlledPawn))
    {
        // Calculate movement direction
        FRotator Rotation = GetControlRotation();
        FRotator YawRotation(0, Rotation.Yaw, 0);

        // Forward/Backward direction
        FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        // Left/Right direction
        FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // Apply movement
        ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AGBPlayerController::EnableGridEdit(UGBGridEntityData* GridEntityData)
{
    if (IsValid(GridEntityData)) {
        CurrentData = GridEntityData;
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("No Data found on %s"), *GetName());
        return;
    }
    /*
    * Check if Handles are empty then bind them.
    * Do toggline for the Disable on button press "later"
    */
    if (ChooseBindingHandle == 0 && CancelBindingHandle == 0) {
        ChooseBindingHandle = EnhancedInputComponent->BindAction(ChooseAction, ETriggerEvent::Started, this, &AGBPlayerController::Choose).GetHandle();
        CancelBindingHandle = EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &AGBPlayerController::DisableGridEdit).GetHandle();
    }
}

void AGBPlayerController::DisableGridEdit()
{
    if (ChooseBindingHandle != 0 && CancelBindingHandle != 0) {
        EnhancedInputComponent->RemoveBindingByHandle(ChooseBindingHandle);
        EnhancedInputComponent->RemoveBindingByHandle(CancelBindingHandle);

        ChooseBindingHandle = 0;
        CancelBindingHandle = 0;
    }
}

void AGBPlayerController::Choose(const FInputActionValue& Value)
{
    TObjectPtr<AGBTileBase> HitTile = GetTileUnderCursor();

    if (IsValid(HitTile)) {
        HitTile->ProcessEditAction(CurrentData);
    }
}

AGBTileBase* AGBPlayerController::GetTileUnderCursor()
{
    // ECC_GameTraceChannel1 = Custom collision Object channel "GridTile"
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

    FHitResult HitResult;
    bool Hit = GetHitResultUnderCursorForObjects(ObjectTypes, true, HitResult);

    if (Hit && HitResult.GetActor())
    {
        AGBTileBase* HitTile = Cast<AGBTileBase>(HitResult.GetActor());
        UE_LOG(LogTemp, Warning, TEXT("Tile hit: %s"), *HitTile->GetName());
        return HitTile;
    }
    return nullptr;
}
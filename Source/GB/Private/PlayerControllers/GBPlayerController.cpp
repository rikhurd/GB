// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/GBPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "Grid/GBGridChunk.h"

AGBPlayerController::AGBPlayerController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    //Constructor

    // ECC_GameTraceChannel1 = Custom collision Object channel "Grid"
    GridObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
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

void AGBPlayerController::DisableGridEdit()
{
    if (ChooseBindingHandle != 0 && CancelBindingHandle != 0) {
        EnhancedInputComponent->RemoveBindingByHandle(ChooseBindingHandle);
        EnhancedInputComponent->RemoveBindingByHandle(CancelBindingHandle);

        ChooseBindingHandle = 0;
        CancelBindingHandle = 0;
    }
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/GBPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "Grid/GBTileBase.h"
#include "GridEntities/GBGridEntityData.h"
#include "Grid/GBDynamicGridManager.h"

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
    FHitResult HitResult;
    bool Hit = GetHitResultUnderCursorForObjects(GridObjectTypes, true, HitResult);

    if (Hit && HitResult.GetActor())
    {
        AGBDynamicGridManager* HitGrid = Cast<AGBDynamicGridManager>(HitResult.GetActor());

        bool ValidTile = false;

        FIntPoint TileIndex;

        HitGrid->LocationToTile(HitResult.Location, ValidTile, TileIndex);

        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Tile hit at location: %s"), *HitResult.Location.ToString()));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Actor hit: %s"), *HitResult.GetActor()->GetName()));

        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Tile Row: %d, Tile Column: %d"), TileIndex.X, TileIndex.Y));

        // Make it so we get the GRID actor when clicking the 1 Grid tile mesh ontop of the grid or 2 the floor mesh
        //
       UE_LOG(LogTemp, Warning, TEXT("Actor hit: %s"), *HitResult.GetActor()->GetName());
       // return HitTile;
    }
    return nullptr;
}
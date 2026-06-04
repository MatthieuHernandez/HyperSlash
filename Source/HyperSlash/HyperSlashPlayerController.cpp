#include "HyperSlashPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "HyperSlashCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "HyperSlash.h"

AHyperSlashPlayerController::AHyperSlashPlayerController()
{
    bIsTouch = false;
    bMoveToMouseCursor = false;

    // create the path following comp
    PathFollowingComponent = CreateDefaultSubobject<UPathFollowingComponent>(TEXT("Path Following Component"));

    // configure the controller
    DefaultMouseCursor = EMouseCursor::Default;
    CachedDestination = FVector::ZeroVector;
    FollowTime = 0.f;
}

void AHyperSlashPlayerController::BeginPlay()
{
    Super::BeginPlay();

    FInputModeGameAndUI InputMode;
    InputMode.SetHideCursorDuringCapture(false);
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    SetInputMode(InputMode);

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void AHyperSlashPlayerController::Tick(float DeltaSeconds)
{
    // Update the move destination to wherever the cursor is pointing at
    UpdateCachedDestination();

    // Move towards mouse pointer
    auto* player = Cast<AHyperSlashCharacter>(GetPawn());
    if (!Player || !player->CanAct())
    {
        return;
    }
    FVector Delta = CachedDestination - player->GetActorLocation();
    if (Delta.Size() > 100.0f)
    {
        FVector WorldDirection = Delta.GetSafeNormal();
        player->AddMovementInput(WorldDirection, 1.0, false);
    }
}

void AHyperSlashPlayerController::SetupInputComponent()
{
    // set up gameplay key bindings
    Super::SetupInputComponent();

    // Only set up input on local player controllers
    if (IsLocalPlayerController())
    {
        // Add Input Mapping Context
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
        {
            EnhancedInputComponent->BindAction(SetAttackInputAction, ETriggerEvent::Started, this, &AHyperSlashPlayerController::OnAttack);
            EnhancedInputComponent->BindAction(SetDashAttackInputAction, ETriggerEvent::Started, this, &AHyperSlashPlayerController::OnDashAttack);
        }
    }
}

void AHyperSlashPlayerController::OrientPlayer(AHyperSlashCharacter* Charactere)
{
    UpdateCachedDestination();
    FVector Direction = CachedDestination - Charactere->GetActorLocation();
    Direction.Z = 0.0f;
    Charactere->SetActorRotation(Direction.Rotation());
}

void AHyperSlashPlayerController::OnAttack()
{
    AHyperSlashCharacter* Charactere = Cast<AHyperSlashCharacter>(GetPawn());
    if (Charactere)
    {
        OrientPlayer(Charactere);
        Charactere->PerformAttack();
    }
}

void AHyperSlashPlayerController::OnDashAttack()
{
    AHyperSlashCharacter* Charactere = Cast<AHyperSlashCharacter>(GetPawn());
    if (Charactere)
    {
        OrientPlayer(Charactere);
        Charactere->PerformDashAttack();
    }
}

void AHyperSlashPlayerController::UpdateCachedDestination()
{
    // We look for the location in the world where the player has pressed the input
    FHitResult Hit;
    bool bHitSuccessful = false;
    if (bIsTouch)
    {
        bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
    }
    else
    {
        bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
    }

    // If we hit a surface, cache the location
    if (bHitSuccessful)
    {
        CachedDestination = Hit.Location;
    }
}

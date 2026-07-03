#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HyperSlashPlayerController.generated.h"

class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class UPathFollowingComponent;
class AHyperSlashCharacter;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);


UCLASS(abstract)
class AHyperSlashPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:

    /** Component used for moving along a NavMesh path. */
    UPROPERTY(VisibleDefaultsOnly, Category = AI)
    TObjectPtr<UPathFollowingComponent> PathFollowingComponent;

    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputAction> SetCircularAttackInputAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputAction> SetDashAttackInputAction;

    /** Time Threshold to know if it was a short press */
    UPROPERTY(EditAnywhere, Category="Input")
    float ShortPressThreshold;

    /** FX Class that we will spawn when clicking */
    UPROPERTY(EditAnywhere, Category="Input")
    TObjectPtr<UNiagaraSystem> FXCursor;

    /** MappingContext */
    UPROPERTY(EditAnywhere, Category="Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditAnywhere, Category = "Distance")
    float DistanceMinBeforeMoving;

    UPROPERTY(EditAnywhere, Category = "Distance")
    float DistanceMinBeforeTeleportation;

    /** True if the controlled character should navigate to the mouse cursor. */
    uint32 bMoveToMouseCursor : 1;

    /** Set to true if we're using touch input */
    uint32 bIsTouch : 1;

    /** Saved location of the character movement destination */
    FVector CachedDestination;

    /** Time that the click input has been pressed */
    float FollowTime = 0.0f;

public:

    AHyperSlashPlayerController();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

    void TeleportPlayer();

protected:

    /** Initialize input bindings */
    virtual void SetupInputComponent() override;

    void OrientPlayer(AHyperSlashCharacter* Charactere);
    
    /** Helper function to get the move destination */
    void UpdateCachedDestination();

    void OnCircularAttack();

    void OnDashAttack();

    void OnTeleportation();
};



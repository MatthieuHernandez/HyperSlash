#pragma once

#include "HyperSlashAttack.h"
#include "HyperSlashDashAttack.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Direction.h"
#include "HyperSlashCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScoreChanged, int32, score, int32, scoreMultiplier);

/**
 *  A controllable top-down perspective character
 */
UCLASS(abstract)
class AHyperSlashCharacter : public ACharacter
{
    GENERATED_BODY()

private:
    bool canBeHit = true;
    bool canAct = true;

    int32 numberOfEnemyKilledByPreviousAttack = 0;

    /** The score multiplier increases with each successful attack. */
    int32 scoreMultiplier;
    /** The score is only going up.*/
    int32 score;

    FTimerHandle hitTimer;
    FTimerHandle actTimer;
    FTimerHandle dieTimer;

    void PlayAttackAnimation();
    void PlayDashAttackAnimation();
    void SpawnAttack();
    void SpawnDashAttack();

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Health = 5;

    UPROPERTY(EditAnywhere, Category = "Action")
    TSubclassOf<AHyperSlashAttack> AttackClass;

    UPROPERTY(EditAnywhere, Category = "Action")
    TSubclassOf<AHyperSlashDashAttack> DashAttackClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* AttackAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* DashAttackAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* HitFrontAnnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* HitBackAnnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* HitLeftAnnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* HitRightAnnimation;

public:

    /** Constructor */
    AHyperSlashCharacter();

    /** Initialization */
    virtual void BeginPlay() override;

    /** Update */
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(BlueprintAssignable, Category = "Score")
    FOnScoreChanged OnScoreChanged;

    UFUNCTION()
    void PerformAttack();

    UFUNCTION()
    void PerformDashAttack();

    void BeHit(Direction D);

    void Attack();

    void EnemyKilled();

    UFUNCTION()
    void Die();

    bool CanAct() const;
};


#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Direction.h"
#include "HyperSlashCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class AWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScoreChanged, int32, score, int32, scoreMultiplier);

UCLASS(abstract)
class AHyperSlashCharacter : public ACharacter
{
    GENERATED_BODY()

private:
    bool canBeHit = true;
    bool canAct = true;
    bool isDashing = false;
    bool isAttacking = false;
    bool isTeleporting = false;


    void UpdateScoreStartAttack();
    void UpdateScoreEndAttack();

    UFUNCTION()
    void EndCircularAttack();

    UFUNCTION()
    void EndDashAttack();


    int32 numberOfEnemyKilledByPreviousAttack = 0;

    /** The score multiplier increases with each successful attack. */
    int32 scoreMultiplier;
    /** The score is only going up.*/
    int32 score;

    FVector dashAttackVector;

    void PlayCircularAttackAnimation();
    void PlayDashAttackAnimation();
    void PlayTeleportationAnimation();

    void PerformCircularAttack();
    void PerformDashAttack();
    void PerformTeleportation();

    AWeapon* equippedWeapon;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Health = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float DashSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float DashDistance = 600.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AWeapon> WeaponClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* CircularAttackAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* DashAttackAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* TeleportationAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* HitFrontAnnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* HitBackAnnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* HitLeftAnnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* HitRightAnnimation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
    TObjectPtr<USoundBase> SlashSound;

public:

    /** Constructor */
    AHyperSlashCharacter();

    /** Initialization */
    virtual void BeginPlay() override;

    /** Update */
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(BlueprintAssignable, Category = "Score")
    FOnScoreChanged OnScoreChanged;

    void BeHit(Direction D);

    void EnemyKilled();

    UFUNCTION()
    void Die();

    bool CanAct() const;

    int32 GetScore() const;

    bool WantPerformCircularAttack;
    bool WantPerformDashAttack;
    bool WantPerformTeleportation;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Direction.h"
#include "HyperSlashEnemy.generated.h"

class AActor;
class AHyperSlashCharacter;
class USoundBase;
class UNiagaraSystem;
class AWeapon;

UCLASS(abstract)
class AHyperSlashEnemy : public ACharacter
{
    GENERATED_BODY()
private:
    bool canAct = true;
    bool wasHitRecently = false;
    bool isAttacking = false;

    AWeapon* equippedWeapon;

    void Die();

    void PerformAttack();

    UFUNCTION()
    void EndAttack();


protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Health = 1;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AWeapon> WeaponClass;

    UPROPERTY(EditAnywhere, Category = "Death", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
    float DeferredDestructionTime = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
    TObjectPtr<USoundBase> DeathSound;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
    TObjectPtr<UNiagaraSystem> BloodSpurt;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    TObjectPtr<UNiagaraSystem> DigUpDirt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* AttackAnimation;

    /** Deferred destruction timer */
    FTimerHandle DestructionTimer;

    virtual void BeginPlay() override;

    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

    virtual void Destroyed() override;

    virtual void Tick(float DeltaSeconds) override;

    /** Called from timer to complete the destruction process for this NPC */
    void DeferredDestroy();

    Direction GetHitDirection(AHyperSlashCharacter* Player);

    /** Called When hit an actor*/
    UFUNCTION()
    void OnHit(
        UPrimitiveComponent* HitComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        FVector NormalImpulse,
        const FHitResult& Hit);


public:

    /** If true, this NPC has already been hit by a projectile and is being destroyed. Exposed to BP so it can be read by StateTree */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
    bool bHit = false;

    // Sets default values for this character's properties
    AHyperSlashEnemy();

    void GetHit(const FVector& Knockback);

    bool CanAct() const;
};

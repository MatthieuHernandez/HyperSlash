// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "HyperSlashAttack.h"
#include "HyperSlashDashAttack.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HyperSlashCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

/**
 *  A controllable top-down perspective character
 */
UCLASS(abstract)
class AHyperSlashCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	void PlayAttackAnimation();
	void PlayDashAttackAnimation();
	void SpawnAttack();
	void SpawnDashAttack();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Health = 3;

	UPROPERTY(EditAnywhere, Category = "Action")
	TSubclassOf<AHyperSlashAttack> AttackClass;

	UPROPERTY(EditAnywhere, Category = "Action")
	TSubclassOf<AHyperSlashDashAttack> DashAttackClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimSequence* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimSequence* DashAttackAnimation;

public:

	/** Constructor */
	AHyperSlashCharacter();

	/** Initialization */
	virtual void BeginPlay() override;

	/** Update */
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void PerformAttack();

	UFUNCTION()
	void PerformDashAttack();

	void TakeDamage();

	UFUNCTION()
	void Die();
};


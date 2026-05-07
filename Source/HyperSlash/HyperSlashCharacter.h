// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "HyperSlashAttack.h"
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
	void SpawnAttack();

protected:


	/** Type of AoE attack actor to spawn */
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AHyperSlashAttack> AttackClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimSequence* AttackAnimation;

public:

	/** Constructor */
	AHyperSlashCharacter();

	/** Initialization */
	virtual void BeginPlay() override;

	/** Update */
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void PerformAttack();
};


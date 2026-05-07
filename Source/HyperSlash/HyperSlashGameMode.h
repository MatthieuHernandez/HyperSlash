// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HyperSlashGameMode.generated.h"

/**
 *  Simple Game Mode for a top-down perspective game
 *  Sets the default gameplay framework classes
 *  Check the Blueprint derived class for the set values
 */
UCLASS(abstract)
class AHyperSlashGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	/** Max number of enemies to allow in the level at once */
	UPROPERTY(EditAnywhere, Category = "Gameplay", meta = (ClampMin = 0, ClampMax = 100))
	int32 MaxEnemiesAtOnce = 10;

	/** Current number of enemies in the level */
	int32 EnemyCount = 0;

public:

	/** Constructor */
	AHyperSlashGameMode();

	void StartPlay() override;

	/** Returns true if the number of NPCs is under the cap */
	bool CanSpawnEnemies();

	/** Increases the NPC count */
	void IncreaseEnemyCount();

	/** Decreases the NPC count */
	void DecreaseEnemyCount();
};




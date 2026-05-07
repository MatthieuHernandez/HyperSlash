#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HyperSlashEnemy.h"
#include "HyperSlashEnemySpawner.generated.h"

class ARecastNavMesh;

UCLASS(abstract)
class AHyperSlashEnemySpawner : public AActor
{
	GENERATED_BODY()


protected:

	/** Type of NPC to spawn */
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AHyperSlashEnemy> EnemyClass;

	/** Time delay between enemy group spawns */
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (ClampMin = 0, ClampMax = 20, Units = "s"))
	float SpawnGroupDelay = 2.0f;

	/** Radius around the spawner where it can spawn enemies */
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (ClampMin = 0, ClampMax = 20, Units = "cm"))
	float SpawnRadius = 600.0f;

	/** Number of enemies to spawn per group */
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (ClampMin = 0, ClampMax = 10))
	int32 SpawnGroupSize = 3;


	/** Number of enemies spawned in the current group */
	int32 SpawnCount = 0;

	/** NPC group spawn timer */
	FTimerHandle SpawnEnemyGroupTimer;

	/** NPC spawn timer */
	FTimerHandle SpawnEnemyTimer;

	/** Pointer to the recast nav mesh actor, used to provide enemy spawn locations */
	TObjectPtr<ARecastNavMesh> NavData;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Gameplay cleanup */
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Spawns a new enemy group */
	void SpawnEnemyGroup();

	/** Spawns an individual Enemy */
	void SpawnEnemy();


public:
	// Sets default values for this actor's properties
	AHyperSlashEnemySpawner();

};

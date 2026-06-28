#pragma once

#include "CoreMinimal.h"
#include "HyperSlashEnemySpawner.h"
#include "OnScreenEnemySpawner.generated.h"

UCLASS(abstract)
class HYPERSLASH_API AOnScreenEnemySpawner : public AHyperSlashEnemySpawner
{
    GENERATED_BODY()
private:
    FVector spawnLocation = FVector::ZeroVector;

protected:
    FVector GetRandomSpawnLocation() override;
};

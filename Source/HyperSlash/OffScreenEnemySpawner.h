#pragma once

#include "CoreMinimal.h"
#include "HyperSlashEnemySpawner.h"
#include "OffScreenEnemySpawner.generated.h"

UCLASS(abstract)
class HYPERSLASH_API AOffScreenEnemySpawner : public AHyperSlashEnemySpawner
{
    GENERATED_BODY()

protected:
    FVector GetRandomSpawnLocation() override;
};

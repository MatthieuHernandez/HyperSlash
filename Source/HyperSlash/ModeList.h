#pragma once
#include "CoreMinimal.h"
#include "ModeList.generated.h"

USTRUCT(BlueprintType)
struct HYPERSLASH_API FModeList
{
    GENERATED_BODY()

    UPROPERTY()
    bool EnableTeleportation = false;

    UPROPERTY()
    bool EnableGlassEnemies = false;

    UPROPERTY()
    bool EnablePickupDefense = false;
};

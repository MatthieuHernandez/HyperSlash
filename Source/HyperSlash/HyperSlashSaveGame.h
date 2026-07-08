#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HyperSlashSaveGame.generated.h"

UCLASS()
class HYPERSLASH_API UHyperSlashSaveGame : public USaveGame
{
    GENERATED_BODY()

public:

    UPROPERTY()
    float MusicVolume;

    UPROPERTY()
    float SoundEffectVolume;

    UPROPERTY()
    uint32 ScreenModeIndex;

    UPROPERTY()
    uint32 ResolutionIndex;

    UPROPERTY()
    uint32 VsyncIndex;

    UPROPERTY()
    int32 MaxScoreOnDesert;

    UPROPERTY()
    int32 MaxScoreOnJungle;
};

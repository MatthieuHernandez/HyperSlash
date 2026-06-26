#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MusicEngine.generated.h"

UCLASS()
class HYPERSLASH_API UMusicEngine : public UGameInstanceSubsystem
{
    GENERATED_BODY()
private:
    FString liveFile;
    bool load(FString& fileContent, const FString& file);
    bool save(const FString fileContent, const FString& file);
    void clearLive();
    void copyIntoLive(const FString& file);

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    void SetMasterVolume(float value);
    void PlayMenuMusic();
    void PlayDesertMusic();
};

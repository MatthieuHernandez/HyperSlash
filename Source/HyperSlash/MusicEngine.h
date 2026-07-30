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
    FString variablesFile;

    bool load(FString& fileContent, const FString& file);
    bool save(const FString fileContent, const FString& file);
    void clearLive();
    void copyIntoLive(const FString& file);
    void SetMasterVolume(float value, const FString& file);
    void SetGlobalTempo(int32 value, const FString& file);

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    void SetMasterVolume(float value);
    void SetGlobalTempo(int32 value);
    void PlayMenuMusic();
    void PlayDesertMusic();
};

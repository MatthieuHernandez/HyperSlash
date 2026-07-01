#include "HyperSlashGameInstance.h"
#include "HyperSlashSaveGame.h"
#include "MusicEngine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"

void UHyperSlashGameInstance::Init()
{
    Super::Init();
    if (auto* settings = GEngine->GetGameUserSettings())
    {
        settings->ApplySettings(false);
    }
    Settings = Cast<UHyperSlashSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Settings"), 0));
    if (!Settings)
    {
        Settings = Cast<UHyperSlashSaveGame>(UGameplayStatics::CreateSaveGameObject(UHyperSlashSaveGame::StaticClass()));
        UGameplayStatics::SaveGameToSlot(Settings, TEXT("Settings"), 0);
    }
}

void UHyperSlashGameInstance::OnStart()
{
    Super::OnStart();
    UMusicEngine* musicEngine = GetSubsystem<UMusicEngine>();
    if (musicEngine) {
        UE_LOG(LogTemp, Warning, TEXT("Settings->MusicVolume: %.2f"), Settings->MusicVolume);
        musicEngine->SetMasterVolume(Settings->MusicVolume);
    }
    const auto ExecutableName =
    #if PLATFORM_LINUX
        TEXT("strudel-autoplay-linux");
    #else
        TEXT("strudel-autoplay.exe");
    #endif
    const auto ExePath = FPaths::Combine(
        FPaths::ProjectDir(),
        TEXT("Content/strudel-autoplay"),
        ExecutableName
    );
    FString WorkingDirectory = FPaths::GetPath(ExePath);
    audioProcessHandle = FPlatformProcess::CreateProc(
        *ExePath,
        TEXT(""),
        false,
        true,
        true,
        nullptr,
        0,
        *WorkingDirectory,
        nullptr
    );
}

void UHyperSlashGameInstance::Shutdown()
{
    Super::Shutdown();
    if (audioProcessHandle.IsValid())
    {
        FPlatformProcess::TerminateProc(audioProcessHandle, true);
        FPlatformProcess::CloseProc(audioProcessHandle);
        audioProcessHandle.Reset();
    }
}
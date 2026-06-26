#include "HyperSlashGameInstance.h"

void UHyperSlashGameInstance::OnStart()
{
    Super::OnStart();
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
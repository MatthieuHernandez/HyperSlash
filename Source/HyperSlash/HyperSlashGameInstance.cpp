#include "HyperSlashGameInstance.h"

void UHyperSlashGameInstance::OnStart()
{
    Super::OnStart();
    FString ExePath = FPaths::Combine(
        FPaths::ProjectDir(),
        TEXT("strudel-autoplay/strudel-autoplay.exe")
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
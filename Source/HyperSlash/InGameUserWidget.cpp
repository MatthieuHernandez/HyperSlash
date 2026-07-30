#include "InGameUserWidget.h"
#include "MusicEngine.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "HyperSlashGameMode.h"

void UInGameUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    formatOptions.MinimumIntegralDigits = 7;
    formatOptions.UseGrouping = false;

    endOfTimer = GetWorld()->GetTimeSeconds() + 177.0f;
    UpdateTimerText();
    if (auto* world = GetWorld())
    {
        world->GetTimerManager().SetTimer(countdownTimerHandle, this, &UInGameUserWidget::TickTimer, 1.0f, true);
    }
}

void UInGameUserWidget::NativeDestruct()
{
    if (auto* world = GetWorld())
    {
        world->GetTimerManager().ClearTimer(countdownTimerHandle);
    }
    Super::NativeDestruct();
}

void UInGameUserWidget::TickTimer()
{
    UpdateTimerText();
    if (auto* world = GetWorld())
    {
        if (world->GetTimeSeconds() >= endOfTimer)
        {
            world->GetTimerManager().ClearTimer(countdownTimerHandle);
            if (auto* GM = Cast<AHyperSlashGameMode>(world->GetAuthGameMode()))
            {
                GM->GameOver();
            }
        }
    }
}

void UInGameUserWidget::UpdateScore(int32 score, int32 scoreMultiplier)
{
    if (ScoreText && ScoreMultiplierText)
    {
        ScoreText->SetText(FText::AsNumber(score, &formatOptions));
        ScoreMultiplierText->SetText(FText::Format(FText::FromString("x{0}"), scoreMultiplier));
    }
    if (auto* gameInstance = GetGameInstance())
    {
        if (auto* musicEngine = gameInstance->GetSubsystem<UMusicEngine>())
        {
            musicEngine->SetGlobalTempo(60 + scoreMultiplier);
        }
    }
}

void UInGameUserWidget::UpdateTimerText()
{
    if (TimerText)
    {
        if (auto* world = GetWorld()) {
            const int32 remainingSec = FMath::Max(0, FMath::FloorToInt(endOfTimer - GetWorld()->GetTimeSeconds()));
            const auto min = remainingSec / 60;
            const auto sec = remainingSec % 60;
            TimerText->SetText(FText::FromString(FString::Printf(TEXT("%d:%02d"), min, sec)));
        }
    }
}




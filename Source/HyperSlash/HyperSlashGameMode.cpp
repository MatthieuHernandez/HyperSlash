// Copyright Epic Games, Inc. All Rights Reserved.

#include "HyperSlashGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameOverUserWidget.h"
#include "InGameUserWidget.h"
#include "HyperSlashCharacter.h"

AHyperSlashGameMode::AHyperSlashGameMode()
{
}

void AHyperSlashGameMode::StartPlay()
{
    Super::StartPlay();
    auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController && InGameUserWidgetClass)
    {
        auto* InGameWidget = CreateWidget<UInGameUserWidget>(PlayerController, InGameUserWidgetClass);
        if (InGameWidget)
        {
            InGameWidget->AddToViewport();
            auto* player = Cast<AHyperSlashCharacter>(PlayerController->GetPawn());
            if (player)
            {
                player->OnScoreChanged.AddDynamic(InGameWidget, &UInGameUserWidget::UpdateScore);
            }
        }
    }
}


bool AHyperSlashGameMode::CanSpawnEnemies()
{
    return EnemyCount < MaxEnemiesAtOnce;
}

void AHyperSlashGameMode::IncreaseEnemyCount()
{
    EnemyCount++;
}

void AHyperSlashGameMode::DecreaseEnemyCount()
{
    EnemyCount--;
}

void AHyperSlashGameMode::GameOver()
{
    auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController && GameOverWidgetClass)
    {
        auto* GameOverWidget = CreateWidget<UGameOverUserWidget>(PlayerController, GameOverWidgetClass);
        if (GameOverWidget)
        {
            GameOverWidget->AddToViewport();
            FInputModeUIOnly InputMode;
            PlayerController->SetInputMode(InputMode);
            UGameplayStatics::SetGamePaused(GetWorld(), true);
        }
    }
}
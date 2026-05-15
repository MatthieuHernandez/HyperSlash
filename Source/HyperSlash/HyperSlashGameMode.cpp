// Copyright Epic Games, Inc. All Rights Reserved.

#include "HyperSlashGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameOverUserWidget.h"

AHyperSlashGameMode::AHyperSlashGameMode()
{
}

void AHyperSlashGameMode::StartPlay()
{
	Super::StartPlay();
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
	UE_LOG(LogTemp, Warning, TEXT("AHyperSlashGameMode::GameOver called A"));
	auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController && GameOverWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("AHyperSlashGameMode::GameOver called B"));
		auto* GameOverWidget = CreateWidget<UGameOverUserWidget>(PlayerController, GameOverWidgetClass);
		if (GameOverWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("AHyperSlashGameMode::GameOver called C"));
			GameOverWidget->AddToViewport();
			FInputModeUIOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}
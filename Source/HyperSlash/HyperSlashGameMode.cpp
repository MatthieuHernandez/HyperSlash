// Copyright Epic Games, Inc. All Rights Reserved.

#include "HyperSlashGameMode.h"

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
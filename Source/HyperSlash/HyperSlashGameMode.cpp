// Copyright Epic Games, Inc. All Rights Reserved.

#include "HyperSlashGameMode.h"

AHyperSlashGameMode::AHyperSlashGameMode()
{
	// stub
	UE_LOG(LogTemp, Warning, TEXT("Authority: %s"),
		HasAuthority() ? TEXT("YES") : TEXT("NO"));
}

void AHyperSlashGameMode::StartPlay()
{
	Super::StartPlay();

	UE_LOG(LogTemp, Warning, TEXT("GameMode started"));
}


bool AHyperSlashGameMode::CanSpawnEnemies()
{
	return EnemyCount < MaxEnemiesAtOnce;
}

void AHyperSlashGameMode::IncreaseEnemyCount()
{
	UE_LOG(LogTemp, Warning, TEXT("EnemyCount++"));
	EnemyCount++;
}

void AHyperSlashGameMode::DecreaseEnemyCount()
{
	UE_LOG(LogTemp, Warning, TEXT("EnemyCount--"));
	EnemyCount--;
}
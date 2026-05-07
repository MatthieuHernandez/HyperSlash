// Fill out your copyright notice in the Description page of Project Settings.


#include "HyperSlashEnemySpawner.h"
#include "HyperSlashGameMode.h"
#include "NavigationSystem.h"
#include "NavMesh/RecastNavMesh.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "TwinStickNPC.h"

// Sets default values
AHyperSlashEnemySpawner::AHyperSlashEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHyperSlashEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// find the recast navmesh actor on the level
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARecastNavMesh::StaticClass(), ActorList);

	if (ActorList.Num() > 0)
	{
		NavData = Cast<ARecastNavMesh>(ActorList[0]);
	}
	else {

		UE_LOG(LogTemp, Log, TEXT("Could not find recast navmesh"));

	}

	// set up the spawn timer
	GetWorld()->GetTimerManager().SetTimer(SpawnEnemyGroupTimer, this, &AHyperSlashEnemySpawner::SpawnEnemyGroup, SpawnGroupDelay, true);

	// spawn the first group of enemies
	SpawnEnemyGroup();
	
}

void AHyperSlashEnemySpawner::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(SpawnEnemyGroupTimer);
	GetWorld()->GetTimerManager().ClearTimer(SpawnEnemyTimer);
}

void AHyperSlashEnemySpawner::SpawnEnemyGroup()
{
	// reset the group spawn counter
	SpawnCount = 0;

	// check if we're still under the max NPC cap
	if (AHyperSlashGameMode* GM = Cast<AHyperSlashGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (GM->CanSpawnEnemies())
		{
			SpawnEnemy();
		}
	}
}

void AHyperSlashEnemySpawner::SpawnEnemy()
{
	// find a random point around the spawner
	FVector SpawnLoc;
	if (UNavigationSystemV1::K2_GetRandomReachablePointInRadius(GetWorld(), GetActorLocation(), SpawnLoc, 600, NavData))
	{
		FTransform SpawnTransform(GetActorRotation(), SpawnLoc, FVector::OneVector);

		// spawn the NPC
		AHyperSlashEnemy* enemy = GetWorld()->SpawnActor<AHyperSlashEnemy>(EnemyClass, SpawnTransform);
		if (enemy)
		{
			enemy->SpawnDefaultController();
		}
	}

	// increase the spawn counter
	++SpawnCount;

	// do we still have enemies left to spawn?
	if (SpawnCount < SpawnGroupSize)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnEnemyTimer, this, &AHyperSlashEnemySpawner::SpawnEnemy, FMath::RandRange(0.33f, 0.66f), false);
	}

}
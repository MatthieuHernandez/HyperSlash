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
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AHyperSlashEnemySpawner::SpawnEnemyGroup, SpawnGroupDelay, true);

	// spawn the first group of enemies
	SpawnEnemyGroup();
	
}

void AHyperSlashEnemySpawner::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
}

void AHyperSlashEnemySpawner::SpawnEnemyGroup()
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnEnemyGroup triggered"));
	// reset the group spawn counter
	SpawnCount = 0;

	UE_LOG(LogTemp, Warning, TEXT("A"));
	// check if we're still under the max NPC cap
	if (AHyperSlashGameMode* GM = Cast<AHyperSlashGameMode>(GetWorld()->GetAuthGameMode()))
	{
		UE_LOG(LogTemp, Warning, TEXT("B"));
		if (GM->CanSpawnEnemies())
		{
			UE_LOG(LogTemp, Warning, TEXT("C"));
			SpawnEnemy();
		}
	}
}

void AHyperSlashEnemySpawner::SpawnEnemy()
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnEnemy triggered"));
	FTransform SpawnTransform;

	// find a random point around the spawner
	FVector SpawnLoc;
	if (UNavigationSystemV1::K2_GetRandomReachablePointInRadius(GetWorld(), GetActorLocation(), SpawnLoc, 600, NavData))
	{
		SpawnTransform.SetLocation(SpawnLoc);

		// spawn the NPC
		ATwinStickNPC* NPC = GetWorld()->SpawnActor<ATwinStickNPC>(EnemyClass, SpawnTransform);
	}

	// increase the spawn counter
	++SpawnCount;

	// do we still have enemies left to spawn?
	if (SpawnCount < SpawnGroupSize)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AHyperSlashEnemySpawner::SpawnEnemy, FMath::RandRange(0.33f, 0.66f), false);
	}

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "HyperSlashEnemySpawner.h"
#include "HyperSlashGameMode.h"
#include "NavigationSystem.h"
#include "NavMesh/RecastNavMesh.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

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
    else
    {
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
FVector AHyperSlashEnemySpawner::GetRandomSpawnLocation()
{
    FVector SpawnLocation = FVector::ZeroVector;

    const int32 Side = FMath::RandRange(0, 3);
    const auto Top = 3040.0;
    const auto Bot = -2760.0;
    const auto W = 4200.0;

    switch (Side)
    {
    case 0: // Left
        SpawnLocation.X = FMath::RandRange(Bot, Top);
        SpawnLocation.Y = -W;
        break;

    case 1: // Right
        SpawnLocation.X = FMath::RandRange(Bot, Top);
        SpawnLocation.Y = W;
        break;

    case 2: // Top
        SpawnLocation.X = Top;
        SpawnLocation.Y = FMath::RandRange(-W, W);
        break;

    case 3: // Bottom
        SpawnLocation.X = Bot;
        SpawnLocation.Y = FMath::RandRange(-W, W);
        break;

    default:
        return FVector::ZeroVector;
    }
    return SpawnLocation;
}

void AHyperSlashEnemySpawner::SpawnEnemy()
{
    FTransform SpawnTransform(GetActorRotation(), GetRandomSpawnLocation(), FVector::OneVector);

    // spawn the Enemy
    AHyperSlashEnemy* enemy = GetWorld()->SpawnActor<AHyperSlashEnemy>(EnemyClass, SpawnTransform);
    if (enemy)
    {
        enemy->SpawnDefaultController();
    }

    // increase the spawn counter
    ++SpawnCount;

    // do we still have enemies left to spawn?
    if (SpawnCount < SpawnGroupSize)
    {
        GetWorld()->GetTimerManager().SetTimer(SpawnEnemyTimer, this, &AHyperSlashEnemySpawner::SpawnEnemy, 0.1f, false); //FMath::RandRange(0.33f, 0.66f)
    }
}
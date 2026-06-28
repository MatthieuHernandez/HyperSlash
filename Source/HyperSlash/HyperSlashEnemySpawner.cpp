#include "HyperSlashEnemySpawner.h"
#include "HyperSlashGameMode.h"
#include "NavigationSystem.h"
#include "NavMesh/RecastNavMesh.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

AHyperSlashEnemySpawner::AHyperSlashEnemySpawner()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AHyperSlashEnemySpawner::BeginPlay()
{
    Super::BeginPlay();
    TArray<AActor*> ActorList;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARecastNavMesh::StaticClass(), ActorList);

    if (ActorList.Num() > 0)
    {
        NavData = Cast<ARecastNavMesh>(ActorList[0]);
    }
    GetWorld()->GetTimerManager().SetTimer(SpawnEnemyGroupTimer, this, &AHyperSlashEnemySpawner::SpawnEnemyGroup, SpawnGroupDelay, true);
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
    SpawnCount = 0;
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
    if (SpawnCount < SpawnGroupSize)
    {
        FTransform SpawnTransform(GetActorRotation(), GetRandomSpawnLocation(), FVector::OneVector);
        AHyperSlashEnemy* enemy = GetWorld()->SpawnActor<AHyperSlashEnemy>(EnemyClass, SpawnTransform);
        if (enemy)
        {
            enemy->SpawnDefaultController();
        }
        ++SpawnCount;
        GetWorld()->GetTimerManager().SetTimer(SpawnEnemyTimer, this, &AHyperSlashEnemySpawner::SpawnEnemy, FMath::RandRange(0.3f, 0.6f), false);
    }
}

FVector AHyperSlashEnemySpawner::GetRandomSpawnLocation()
{
    FVector SpawnLocation = FVector::ZeroVector;
    SpawnLocation.Z = 1;
    return SpawnLocation;
}

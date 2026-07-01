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
        AHyperSlashEnemy* enemy = nullptr;
        for (int32 attempt = 0; attempt < 15; ++attempt)
        {
            const FTransform spawnTransform(GetActorRotation(), GetRandomSpawnLocation(), FVector::OneVector);
            FActorSpawnParameters spawnParams;
            spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
            enemy = GetWorld()->SpawnActor<AHyperSlashEnemy>(EnemyClass, spawnTransform, spawnParams);
            if (enemy)
            {
                enemy->SpawnDefaultController();
                ++SpawnCount;
                GetWorld()->GetTimerManager().SetTimer(SpawnEnemyTimer, this, &AHyperSlashEnemySpawner::SpawnEnemy, FMath::RandRange(0.3f, 0.6f), false);
                break;
            }
        }
        if (!enemy)
        {
            UE_LOG(LogTemp, Warning, TEXT("Enemy cannot spawn"));
        }
    }
}

FVector AHyperSlashEnemySpawner::GetRandomSpawnLocation()
{
    FVector SpawnLocation = FVector::ZeroVector;
    SpawnLocation.Z = 1;
    return SpawnLocation;
}

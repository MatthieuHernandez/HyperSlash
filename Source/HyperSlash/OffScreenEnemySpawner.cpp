#include "OffScreenEnemySpawner.h"

FVector AOffScreenEnemySpawner::GetRandomSpawnLocation()
{
    FVector SpawnLocation = FVector::ZeroVector;
    SpawnLocation.Z = 1;

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

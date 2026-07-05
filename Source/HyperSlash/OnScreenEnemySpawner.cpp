#include "OnScreenEnemySpawner.h"

FVector AOnScreenEnemySpawner::GetRandomSpawnLocation()
{
    if (SpawnCount == 0)
    {
        const auto top = 2380.0f;
        const auto bot = -2000.0f;
        const auto w = 3420.0f;
        spawnLocation.X = FMath::RandRange(bot, top);
        spawnLocation.Y = FMath::RandRange(-w, w);
        spawnLocation.Z = 0;
    }
    else {
        const int32 side = FMath::RandRange(0, 3);
        const auto d = 60.0f;
        switch (side)
        {
        case 0: // Left
            spawnLocation.X += FMath::RandRange(-d, d);
            spawnLocation.Y += -d;
            break;

        case 1: // Right
            spawnLocation.X += FMath::RandRange(-d, d);
            spawnLocation.Y += d;
            break;

        case 2: // Top
            spawnLocation.X += d;
            spawnLocation.Y += FMath::RandRange(-d, d);
            break;

        case 3: // Bottom
            spawnLocation.X += -d;
            spawnLocation.Y += FMath::RandRange(-d, d);
            break;

        default:
            return FVector::ZeroVector;
        }
    }
    return spawnLocation;
}

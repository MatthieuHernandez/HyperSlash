#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UStateTreeAIComponent;

UCLASS(abstract)
class HYPERSLASH_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()
public:
    AEnemyAIController();
};

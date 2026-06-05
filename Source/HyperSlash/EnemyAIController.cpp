#include "EnemyAIController.h"
#include "Components/StateTreeAIComponent.h"

AEnemyAIController::AEnemyAIController()
{
    StateTreeAI = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAI"));
    check(StateTreeAI);
    bStartAILogicOnPossess = true;
    bStopAILogicOnUnposses = true;
    bAttachToPawn = true;
}

#include "EnemyAIController.h"
#include "Components/StateTreeAIComponent.h"

AEnemyAIController::AEnemyAIController()
{
    bStartAILogicOnPossess = true;
    bStopAILogicOnUnposses = true;
    bAttachToPawn = true;
}

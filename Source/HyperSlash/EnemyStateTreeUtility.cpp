#include "EnemyStateTreeUtility.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeExecutionTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

EStateTreeRunStatus FStateTreeGetPlayerTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    FStateTreeGetPlayerInstanceData& InstanceData = Context.GetInstanceData(*this);
    InstanceData.TargetPlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(InstanceData.Character, 0));

    return EStateTreeRunStatus::Running;
}
#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "EnemyStateTreeUtility.generated.h"

class ACharacter;

USTRUCT()
struct FStateTreeGetPlayerInstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Context")
    TObjectPtr<ACharacter> Character;

    UPROPERTY(VisibleAnywhere, Category = "Output")
    TObjectPtr<ACharacter> TargetPlayerCharacter;
};

USTRUCT(meta = (DisplayName = "GetPlayer", Category = "Enemy"))
struct FStateTreeGetPlayerTask : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()
    using FInstanceDataType = FStateTreeGetPlayerInstanceData;
    virtual const UStruct* GetInstanceDataType() const override { return FStateTreeGetPlayerInstanceData::StaticStruct(); }

    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
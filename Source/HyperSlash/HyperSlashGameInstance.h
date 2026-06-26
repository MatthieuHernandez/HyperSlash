#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HyperSlashGameInstance.generated.h"


UCLASS()
class HYPERSLASH_API UHyperSlashGameInstance : public UGameInstance
{
    GENERATED_BODY()

private:
    FProcHandle audioProcessHandle;

public:
    virtual void OnStart() override;
    virtual void Shutdown() override;
};

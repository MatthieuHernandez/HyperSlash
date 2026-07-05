#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HyperSlashSaveGame.h"
#include "HyperSlashGameInstance.generated.h"

UCLASS()
class HYPERSLASH_API UHyperSlashGameInstance : public UGameInstance
{
    GENERATED_BODY()

private:
    FProcHandle audioProcessHandle;

public:
    UPROPERTY()
    TObjectPtr<UHyperSlashSaveGame> Settings;

    virtual void Init() override;
    virtual void OnStart() override;
    virtual void Shutdown() override;
};

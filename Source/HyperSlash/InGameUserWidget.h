#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "InGameUserWidget.generated.h"

class UTextBlock;

UCLASS(abstract)
class HYPERSLASH_API UInGameUserWidget : public UUserWidget
{
    GENERATED_BODY()
protected:

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* ScoreText;

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* ScoreMultiplierText;

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* TimerText;

public:

    UFUNCTION()
    void UpdateScore(int32 score, int32 scoreMultiplier);

private:

    void TickTimer();
    void UpdateTimerText();

    int32 gameDuration = 177.0f; // in secondes
    float endOfTimer;
    FTimerHandle countdownTimerHandle;
    FNumberFormattingOptions formatOptions;
};

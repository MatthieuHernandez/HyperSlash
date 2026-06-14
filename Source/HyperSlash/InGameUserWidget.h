#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUserWidget.generated.h"

class UTextBlock;

UCLASS(abstract)
class HYPERSLASH_API UInGameUserWidget : public UUserWidget
{
    GENERATED_BODY()
protected:

    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* ScoreText;

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* ScoreMultiplierText;

public:

    UFUNCTION()
    void UpdateScore(int32 score, int32 scoreMultiplier);

private:

    FNumberFormattingOptions FormatOptions;
};

#include "InGameUserWidget.h"
#include "Components/TextBlock.h"

void UInGameUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    FormatOptions.MinimumIntegralDigits = 7;
    FormatOptions.UseGrouping = false;
}

void UInGameUserWidget::UpdateScore(int32 score, int32 scoreMultiplier)
{
    if (ScoreText && ScoreMultiplierText)
    {
        ScoreText->SetText(FText::AsNumber(score, &FormatOptions));
        ScoreMultiplierText->SetText(FText::Format(FText::FromString("x{0}"), scoreMultiplier));
    }
}

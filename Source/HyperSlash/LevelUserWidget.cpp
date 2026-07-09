#include "LevelUserWidget.h"
#include "MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "HyperSlashGameInstance.h"

void ULevelUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (DesertButton && BackButton)
    {
        DesertButton->OnClicked.AddDynamic(this, &ULevelUserWidget::OnDesertClicked);
        BackButton->OnClicked.AddDynamic(this, &ULevelUserWidget::OnBackClicked);
    }
    if (MaxScoreOnDesertText &&
        MaxScoreOnJungleText)
    {
        if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
        {
            FNumberFormattingOptions formatOptions;
            formatOptions.MinimumIntegralDigits = 7;
            formatOptions.UseGrouping = false;
            MaxScoreOnDesertText->SetText(FText::AsNumber(gameInstance->Settings->MaxScoreOnDesert, &formatOptions));
            MaxScoreOnJungleText->SetText(FText::AsNumber(gameInstance->Settings->MaxScoreOnJungle, &formatOptions));
        }
    }
}

void ULevelUserWidget::OnDesertClicked()
{
    RemoveFromParent();
    UGameplayStatics::OpenLevel(this, FName("Lvl_Desert"));
}


void ULevelUserWidget::OnBackClicked()
{
    if (MainMenuClass)
    {
        RemoveFromParent();
        auto* widget = CreateWidget<UMainMenuUserWidget>(GetWorld(), MainMenuClass);
        if (widget)
        {
            widget->AddToViewport();
        }
    }
}

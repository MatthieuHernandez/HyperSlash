#include "LevelUserWidget.h"
#include "MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void ULevelUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (DesertButton && BackButton)
    {
        DesertButton->OnClicked.AddDynamic(this, &ULevelUserWidget::OnDesertClicked);
        BackButton->OnClicked.AddDynamic(this, &ULevelUserWidget::OnBackClicked);
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

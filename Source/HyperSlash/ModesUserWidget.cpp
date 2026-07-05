#include "ModesUserWidget.h"
#include "MainMenuUserWidget.h"
#include "Components/Button.h"

void UModesUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UModesUserWidget::OnBackClicked);
    }
}

void UModesUserWidget::OnBackClicked()
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

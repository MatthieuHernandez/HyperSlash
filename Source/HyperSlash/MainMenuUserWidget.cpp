#include "MainMenuUserWidget.h"
#include "LevelUserWidget.h"
#include "ModesUserWidget.h"
#include "SettingsUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (PlayButton &&
        ModesButton &&
        SettingsButton &&
        QuitButton)
    {
        PlayButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnPlayClicked);
        ModesButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnModesClicked);
        SettingsButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnSettingsClicked);
        QuitButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnQuitClicked);
    }
}

void UMainMenuUserWidget::OnPlayClicked()
{
    if (LevelMenuClass)
    {
        RemoveFromParent();
        auto* widget = CreateWidget<ULevelUserWidget>(GetWorld(), LevelMenuClass);
        if (widget)
        {
            widget->AddToViewport();
        }
    }
}

void UMainMenuUserWidget::OnModesClicked()
{
    if (ModesMenuClass)
    {
        RemoveFromParent();
        auto* widget = CreateWidget<UModesUserWidget>(GetWorld(), ModesMenuClass);
        if (widget)
        {
            widget->AddToViewport();
        }
    }
}

void UMainMenuUserWidget::OnSettingsClicked()
{
    if (SettingsMenuClass)
    {
        RemoveFromParent();
        auto* widget = CreateWidget<USettingsUserWidget>(GetWorld(), SettingsMenuClass);
        if (widget)
        {
            widget->AddToViewport();
        }
    }
}

void UMainMenuUserWidget::OnQuitClicked()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}

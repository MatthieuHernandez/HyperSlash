#include "MainMenuUserWidget.h"
#include "SettingsUserWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuUserWidget::NativeConstruct()
{
    USettingsUserWidget::InitializeSettings();
    Super::NativeConstruct();
    if (PlayButton &&
        QuitButton &&
        SettingsButton)
    {
        PlayButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnPlayClicked);
        SettingsButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnSettingsClicked);
        QuitButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnQuitClicked);
    }
}

void UMainMenuUserWidget::OnPlayClicked()
{
    RemoveFromParent();
    UGameplayStatics::OpenLevel(this, FName("Lvl_TopDown"));
}

void UMainMenuUserWidget::OnSettingsClicked()
{
    RemoveFromParent();
    if (SettingsMenuClass)
    {
        auto* SettingsWidget = CreateWidget<USettingsUserWidget>(GetWorld(), SettingsMenuClass);
        if (SettingsWidget)
        {
            SettingsWidget->AddToViewport();
        }
    }
}

void UMainMenuUserWidget::OnQuitClicked()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}
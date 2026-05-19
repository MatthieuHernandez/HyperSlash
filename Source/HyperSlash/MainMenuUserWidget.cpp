#include "MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (PlayButton && QuitButton)
    {
        PlayButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnPlayClicked);
        QuitButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnQuitClicked);
    }
}

void UMainMenuUserWidget::OnPlayClicked()
{
    RemoveFromParent();
    UGameplayStatics::OpenLevel(this, FName("Lvl_TopDown"));
}

void UMainMenuUserWidget::OnQuitClicked()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}
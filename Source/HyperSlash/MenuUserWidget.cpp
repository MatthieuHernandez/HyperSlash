#include "MenuUserWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UMenuUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (PlayButton && QuitButton)
    {
        PlayButton->OnClicked.AddDynamic(this, &UMenuUserWidget::OnPlayClicked);
        QuitButton->OnClicked.AddDynamic(this, &UMenuUserWidget::OnQuitClicked);
    }
}

void UMenuUserWidget::OnPlayClicked()
{
    RemoveFromParent();
    UGameplayStatics::OpenLevel(this, FName("Lvl_TopDown"));
}

void UMenuUserWidget::OnQuitClicked()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}
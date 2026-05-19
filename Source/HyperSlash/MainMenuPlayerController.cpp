#include "MainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (MainMenuClass)
    {
        MainMenu = CreateWidget<UUserWidget>(this, MainMenuClass);
        if (MainMenu)
        {
            MainMenu->AddToViewport();
            bShowMouseCursor = true;
            FInputModeUIOnly InputMode;
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            SetInputMode(InputMode);
        }
    }
}
#include "SettingsUserWidget.h"
#include "MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameUserSettings.h"

void USettingsUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (LeftScreenModeButton &&
        RightScreenModeButton &&
        LeftResolutionButton &&
        RightResolutionButton &&
        LeftVsyncButton &&
        RightVsyncButton &&
        BackButton)
    {
        LeftScreenModeButton->OnClicked.AddDynamic(this, &USettingsUserWidget::OnLeftScreenModeClicked);
        RightScreenModeButton->OnClicked.AddDynamic(this, &USettingsUserWidget::OnRightScreenModeClicked);
        LeftResolutionButton->OnClicked.AddDynamic(this, &USettingsUserWidget::OnLeftResolutionClicked);
        RightResolutionButton->OnClicked.AddDynamic(this, &USettingsUserWidget::OnRightResolutionClicked);
        LeftVsyncButton->OnClicked.AddDynamic(this, &USettingsUserWidget::OnLeftVsyncClicked);
        RightVsyncButton->OnClicked.AddDynamic(this, &USettingsUserWidget::OnRightVsyncClicked);
        BackButton->OnClicked.AddDynamic(this, &USettingsUserWidget::OnBackClicked);
    }
    if (ScreenModeText &&
        ResolutionText &&
        VsyncText)
    {
        ScreenModeText->SetText(FText::FromString(ScreenModeOptions[ScreenModeIndex]));
        ResolutionText->SetText(FText::FromString(ResolutionOptions[ResolutionIndex]));
        VsyncText->SetText(FText::FromString(VsyncOptions[VsyncIndex]));
    }
}

void USettingsUserWidget::InitializeSettings()
{
    ChangeScreenMode(ScreenModeIndex);
    ChangeResolution(ResolutionIndex);
    ChangeVsync(VsyncIndex);
}

void USettingsUserWidget::ChangeScreenMode(uint32 index)
{
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        EWindowMode::Type WindowMode = EWindowMode::WindowedFullscreen;
        switch (index)
        {
        case 0:
            WindowMode = EWindowMode::WindowedFullscreen;
            break;
        case 1:
            WindowMode = EWindowMode::Fullscreen;
            break;
        case 2:
            WindowMode = EWindowMode::Windowed;
            break;
        default:
            break;
        }
        Settings->SetFullscreenMode(WindowMode);
        Settings->ApplySettings(false);
        Settings->SaveSettings();
    }
}

void USettingsUserWidget::OnLeftScreenModeClicked()
{
    ScreenModeIndex = ScreenModeIndex == 0 ? ScreenModeOptions.size() - 1 : ScreenModeIndex - 1;
    ChangeScreenMode(ScreenModeIndex);
    if (ScreenModeText)
    {
        ScreenModeText->SetText(FText::FromString(ScreenModeOptions[ScreenModeIndex]));
    }
}

void USettingsUserWidget::OnRightScreenModeClicked()
{
    ScreenModeIndex = (ScreenModeIndex == ScreenModeOptions.size() - 1) ? 0 : ScreenModeIndex + 1;
    ChangeScreenMode(ScreenModeIndex);
    if (ScreenModeText)
    {
        ScreenModeText->SetText(FText::FromString(ScreenModeOptions[ScreenModeIndex]));
    }
}

void USettingsUserWidget::ChangeResolution(uint32 index)
{
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        FIntPoint Resolution;
        switch (index)
        {
        case 0:
            Resolution = FIntPoint(2560, 1440);
            break;
        case 1:
            Resolution = FIntPoint(1366, 768);
            break;
        case 2:
            Resolution = FIntPoint(1600, 900);
            break;
        case 3:
            Resolution = FIntPoint(1920, 1080);
            break;
        default:
            return;
        }
        Settings->SetScreenResolution(Resolution);
        Settings->ApplySettings(false);
        Settings->SaveSettings();
    }
}

void USettingsUserWidget::OnLeftResolutionClicked()
{
    ResolutionIndex = ResolutionIndex == 0 ? ResolutionOptions.size() - 1 : ResolutionIndex - 1;
    ChangeResolution(ResolutionIndex);
    if (ResolutionText)
    {
        ResolutionText->SetText(FText::FromString(ResolutionOptions[ResolutionIndex]));
    }
}

void USettingsUserWidget::OnRightResolutionClicked()
{
    ResolutionIndex = ResolutionIndex == (ResolutionOptions.size() - 1) ? 0 : ResolutionIndex + 1;
    ChangeResolution(ResolutionIndex);
    if (ResolutionText)
    {
        ResolutionText->SetText(FText::FromString(ResolutionOptions[ResolutionIndex]));
    }
}

void USettingsUserWidget::ChangeVsync(uint32 index)
{
    const bool value = (index == 0);
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        Settings->SetVSyncEnabled(value);
        Settings->ApplySettings(false);
        Settings->SaveSettings();
    }
}

void USettingsUserWidget::OnLeftVsyncClicked()
{
    VsyncIndex = VsyncIndex == 0 ? VsyncOptions.size() - 1 : VsyncIndex - 1;
    ChangeVsync(VsyncIndex);
    if (VsyncText)
    {
        VsyncText->SetText(FText::FromString(VsyncOptions[VsyncIndex]));
    }
}

void USettingsUserWidget::OnRightVsyncClicked()
{
    VsyncIndex = (VsyncIndex == VsyncOptions.size() - 1) ? 0 : VsyncIndex + 1;
    ChangeVsync(VsyncIndex);
    if (VsyncText)
    {
        VsyncText->SetText(FText::FromString(VsyncOptions[VsyncIndex]));
    }
}

void USettingsUserWidget::OnBackClicked()
{
    RemoveFromParent();
    if (MainMenuClass)
    {
        auto* MainMenuWidget = CreateWidget<UMainMenuUserWidget>(GetWorld(), MainMenuClass);
        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport();
        }
    }
}

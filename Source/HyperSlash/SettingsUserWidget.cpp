#include "SettingsUserWidget.h"
#include "MainMenuUserWidget.h"
#include "MusicEngine.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "GameFramework/GameUserSettings.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "HyperSlashGameInstance.h"

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
    if (MusicVolumeSlider && SoundEffectsVolumeSlider) {
        MusicVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsUserWidget::OnMusicVolumeSliderChanged);
        MusicVolumeSlider->OnMouseCaptureEnd.AddDynamic(this, &USettingsUserWidget::OnMusicVolumeSliderReleased);
        SoundEffectsVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsUserWidget::OnSoundEffectsVolumeSliderChanged);
        SoundEffectsVolumeSlider->OnMouseCaptureEnd.AddDynamic(this, &USettingsUserWidget::OnSoundEffectsVolumeSliderReleased);
    }
    InitializeSettings();
}

void USettingsUserWidget::InitializeSettings()
{
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        ChangeScreenMode(gameInstance->Settings->ScreenModeIndex);
        ChangeResolution(gameInstance->Settings->ResolutionIndex);
        ChangeVsync(gameInstance->Settings->VsyncIndex);
        ChangeMusicVolume(gameInstance->Settings->MusicVolume);
        ChangeSoundEffectVolume(gameInstance->Settings->SoundEffectVolume);
    }
}

void USettingsUserWidget::ChangeMusicVolume(float value)
{
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        gameInstance->Settings->MusicVolume = value;
        if (auto* MusicEngine = gameInstance->GetSubsystem<UMusicEngine>())
        {
            MusicEngine->SetMasterVolume(value);
        }
    }
    if (MusicVolumeSlider)
    {
        MusicVolumeSlider->SetValue(value);
    }
    if (MusicVolumeText)
    {
        const auto text = FText::Format(FText::FromString(TEXT("{0}%")), FText::AsNumber(FMath::RoundToInt(value * 100.0f)));
        MusicVolumeText->SetText(text);
    }
}

void USettingsUserWidget::ChangeSoundEffectVolume(float value)
{
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        gameInstance->Settings->SoundEffectVolume = value;
    }
    if (MasterSoundClass)
    {
        MasterSoundClass->Properties.Volume = value;
    }
    if (SoundEffectsVolumeSlider)
    {
        SoundEffectsVolumeSlider->SetValue(value);
    }
    if (SoundEffectsVolumeText)
    {
        const auto text = FText::Format(FText::FromString(TEXT("{0}%")), FText::AsNumber(FMath::RoundToInt(value * 100.0f)));
        SoundEffectsVolumeText->SetText(text);
    }
}

void USettingsUserWidget::ChangeScreenMode(uint32 index)
{
    if (auto* settings = GEngine->GetGameUserSettings())
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
        settings->SetFullscreenMode(WindowMode);
        settings->ApplySettings(false);
        settings->SaveSettings();
        if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
        {
            UGameplayStatics::SaveGameToSlot(gameInstance->Settings, TEXT("Settings"), 0);
            if (ScreenModeText)
            {
                ScreenModeText->SetText(FText::FromString(ScreenModeOptions[gameInstance->Settings->ScreenModeIndex]));
            }
        }
    }
}

void USettingsUserWidget::OnLeftScreenModeClicked()
{
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        gameInstance->Settings->ScreenModeIndex = gameInstance->Settings->ScreenModeIndex == 0 ? ScreenModeOptions.size() - 1 : gameInstance->Settings->ScreenModeIndex - 1;
        ChangeScreenMode(gameInstance->Settings->ScreenModeIndex);
    }
}

void USettingsUserWidget::OnRightScreenModeClicked()
{
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        gameInstance->Settings->ScreenModeIndex = (gameInstance->Settings->ScreenModeIndex == ScreenModeOptions.size() - 1) ? 0 : gameInstance->Settings->ScreenModeIndex + 1;
        ChangeScreenMode(gameInstance->Settings->ScreenModeIndex);
    }
}

void USettingsUserWidget::ChangeResolution(uint32 index)
{
    if (auto* settings = GEngine->GetGameUserSettings())
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
        settings->SetScreenResolution(Resolution);
        settings->ApplySettings(false);
        settings->SaveSettings();
        if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
        {
            UGameplayStatics::SaveGameToSlot(gameInstance->Settings, TEXT("Settings"), 0);
            if (ResolutionText)
            {
                ResolutionText->SetText(FText::FromString(ResolutionOptions[gameInstance->Settings->ResolutionIndex]));
            }
        }
    }
}

void USettingsUserWidget::OnLeftResolutionClicked()
{
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        gameInstance->Settings->ResolutionIndex = gameInstance->Settings->ResolutionIndex == 0 ? ResolutionOptions.size() - 1 : gameInstance->Settings->ResolutionIndex - 1;
        ChangeResolution(gameInstance->Settings->ResolutionIndex);
    }
}

void USettingsUserWidget::OnRightResolutionClicked()
{
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        gameInstance->Settings->ResolutionIndex = gameInstance->Settings->ResolutionIndex == (ResolutionOptions.size() - 1) ? 0 : gameInstance->Settings->ResolutionIndex + 1;
        ChangeResolution(gameInstance->Settings->ResolutionIndex);
    }
}

void USettingsUserWidget::ChangeVsync(uint32 index)
{
    const bool value = (index == 0);
    if (auto* settings = GEngine->GetGameUserSettings())
    {
        settings->SetVSyncEnabled(value);
        settings->ApplySettings(false);
        settings->SaveSettings();
        if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
        {
            UGameplayStatics::SaveGameToSlot(gameInstance->Settings, TEXT("Settings"), 0);
            if (VsyncText)
            {
                VsyncText->SetText(FText::FromString(VsyncOptions[gameInstance->Settings->VsyncIndex]));
            }
        }
    }
}

void USettingsUserWidget::OnLeftVsyncClicked()
{
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        gameInstance->Settings->VsyncIndex = gameInstance->Settings->VsyncIndex == 0 ? VsyncOptions.size() - 1 : gameInstance->Settings->VsyncIndex - 1;
        ChangeVsync(gameInstance->Settings->VsyncIndex);
    }
}

void USettingsUserWidget::OnRightVsyncClicked()
{
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        gameInstance->Settings->VsyncIndex = (gameInstance->Settings->VsyncIndex == VsyncOptions.size() - 1) ? 0 : gameInstance->Settings->VsyncIndex + 1;
        ChangeVsync(gameInstance->Settings->VsyncIndex);
    }
}

void USettingsUserWidget::OnMusicVolumeSliderChanged(float value)
{
    ChangeMusicVolume(value);
}

void USettingsUserWidget::OnMusicVolumeSliderReleased()
{
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        UGameplayStatics::SaveGameToSlot(gameInstance->Settings, TEXT("Settings"), 0);
        UE_LOG(LogTemp, Warning, TEXT("Settings->MusicVolume: %.2f"), gameInstance->Settings->MusicVolume);
        UE_LOG(LogTemp, Warning, TEXT("Settings->MusicVolume saved"));
    }
}

void USettingsUserWidget::OnSoundEffectsVolumeSliderChanged(float value)
{
    ChangeSoundEffectVolume(value);
}

void USettingsUserWidget::OnSoundEffectsVolumeSliderReleased()
{
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        UGameplayStatics::SaveGameToSlot(gameInstance->Settings, TEXT("Settings"), 0);
    }
    if (TestSound)
    {
        UGameplayStatics::PlaySound2D(this, TestSound);
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

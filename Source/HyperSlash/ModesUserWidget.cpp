#include "ModesUserWidget.h"
#include "HyperSlashGameInstance.h"
#include "MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Kismet/GameplayStatics.h"

void UModesUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (BackButton &&
        TeleportationCheckBox &&
        GlassEnemiesCheckBox &&
        PickupDefenseCheckBox)
    {
        if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
        {
            TeleportationCheckBox->SetIsChecked(gameInstance->Settings->Modes.EnableTeleportation);
            GlassEnemiesCheckBox->SetIsChecked(gameInstance->Settings->Modes.EnableGlassEnemies);
            PickupDefenseCheckBox->SetIsChecked(gameInstance->Settings->Modes.EnablePickupDefense);
        }

        BackButton->OnClicked.AddDynamic(this, &UModesUserWidget::OnBackClicked);
        TeleportationCheckBox->OnCheckStateChanged.AddDynamic(this, &UModesUserWidget::OnTeleportaionClicked);
        GlassEnemiesCheckBox->OnCheckStateChanged.AddDynamic(this, &UModesUserWidget::OnGlassEnemiesClicked);
        PickupDefenseCheckBox->OnCheckStateChanged.AddDynamic(this, &UModesUserWidget::OnPickupDefenseClicked);
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
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        UGameplayStatics::SaveGameToSlot(gameInstance->Settings, TEXT("Settings"), 0);
    }
}

void UModesUserWidget::OnTeleportaionClicked(bool isChecked)
{
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        gameInstance->Settings->Modes.EnableTeleportation = isChecked;
    }
}

void UModesUserWidget::OnGlassEnemiesClicked(bool isChecked)
{
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        gameInstance->Settings->Modes.EnableGlassEnemies = isChecked;
    }
}

void UModesUserWidget::OnPickupDefenseClicked(bool isChecked)
{
    if (auto* gameInstance = GetGameInstance<UHyperSlashGameInstance>())
    {
        gameInstance->Settings->Modes.EnablePickupDefense = isChecked;
    }
}

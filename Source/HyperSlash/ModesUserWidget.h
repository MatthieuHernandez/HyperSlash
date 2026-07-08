#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ModesUserWidget.generated.h"

class UButton;
class UCheckBox;
class UMainMenuUserWidget;

UCLASS(abstract)
class HYPERSLASH_API UModesUserWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UMainMenuUserWidget> MainMenuClass;

    UPROPERTY(meta = (BindWidgetOptional))
    UButton* BackButton;

    UPROPERTY(meta = (BindWidgetOptional))
    UCheckBox* TeleportationCheckBox;

    UPROPERTY(meta = (BindWidgetOptional))
    UCheckBox* GlassEnemiesCheckBox;

    UPROPERTY(meta = (BindWidgetOptional))
    UCheckBox* PickupDefenseCheckBox;

private:
    UFUNCTION()
    void OnBackClicked();

    UFUNCTION()
    void OnTeleportaionClicked(bool isChecked);

    UFUNCTION()
    void OnGlassEnemiesClicked(bool isChecked);

    UFUNCTION()
    void OnPickupDefenseClicked(bool isChecked);
};

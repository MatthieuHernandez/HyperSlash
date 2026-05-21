#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUserWidget.generated.h"

class UButton;
class USettingsUserWidget;

UCLASS(abstract)
class HYPERSLASH_API UMainMenuUserWidget : public UUserWidget
{
    GENERATED_BODY()
protected:

    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<USettingsUserWidget> SettingsMenuClass;

    UPROPERTY(meta = (BindWidgetOptional))
    UButton* PlayButton;

    UPROPERTY(meta = (BindWidgetOptional))
    UButton* SettingsButton;

    UPROPERTY(meta = (BindWidgetOptional))
    UButton* QuitButton;

private:

    UFUNCTION()
    void OnPlayClicked();

    UFUNCTION()
    void OnSettingsClicked();

    UFUNCTION()
    void OnQuitClicked();
};

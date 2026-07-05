#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ModesUserWidget.generated.h"

class UButton;
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

private:
    UFUNCTION()
    void OnBackClicked();
};

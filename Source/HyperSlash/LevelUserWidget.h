#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelUserWidget.generated.h"

class UButton;
class UMainMenuUserWidget;

UCLASS(abstract)
class HYPERSLASH_API ULevelUserWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UMainMenuUserWidget> MainMenuClass;

    UPROPERTY(meta = (BindWidgetOptional))
    UButton* DesertButton;

    UPROPERTY(meta = (BindWidgetOptional))
    UButton* BackButton;

private:
    UFUNCTION()
    void OnDesertClicked();

    UFUNCTION()
    void OnBackClicked();
 };

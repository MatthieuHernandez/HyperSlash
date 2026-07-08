#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelUserWidget.generated.h"

class UButton;
class UTextBlock;
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

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* MaxScoreOnDesertText;

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* MaxScoreOnJungleText;

private:
    UFUNCTION()
    void OnDesertClicked();

    UFUNCTION()
    void OnBackClicked();
 };

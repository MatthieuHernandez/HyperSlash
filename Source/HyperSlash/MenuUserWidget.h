#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuUserWidget.generated.h"

class UButton;

UCLASS(abstract)
class HYPERSLASH_API UMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidgetOptional))
    UButton* PlayButton;

    UPROPERTY(meta = (BindWidgetOptional))
    UButton* QuitButton;

private:

    UFUNCTION()
    void OnPlayClicked();

    UFUNCTION()
    void OnQuitClicked();
};

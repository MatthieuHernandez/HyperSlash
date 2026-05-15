#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverUserWidget.generated.h"

class UButton;

UCLASS(abstract)
class HYPERSLASH_API UGameOverUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BackButton;

private:

	UFUNCTION()
	void OnBackClicked();
};

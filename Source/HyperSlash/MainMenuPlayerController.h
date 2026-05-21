#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS(abstract)
class AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY()
	UUserWidget* MainMenu;

	void BeginPlay() override;
};

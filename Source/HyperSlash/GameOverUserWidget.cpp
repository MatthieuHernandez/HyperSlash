#include "GameOverUserWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UGameOverUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UGameOverUserWidget::OnBackClicked);
    }
}
void UGameOverUserWidget::OnBackClicked()
{
    RemoveFromParent();
    UGameplayStatics::SetGamePaused(this, false);
    UGameplayStatics::OpenLevel(this, FName("Lvl_Menu"));
}

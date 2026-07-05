#include "TeleportationAnimNotify.h"
#include "HyperSlashCharacter.h"
#include "HyperSlashPlayerController.h"

void UTeleportationAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp) return;
    if (auto* character = Cast<AHyperSlashCharacter>(MeshComp->GetOwner()))
    {
        if (auto* playerController = Cast<AHyperSlashPlayerController>(character->GetController()))
        {
            playerController->TeleportPlayer();
        }
    }
}

#include "MenuGameMode.h"
#include "MusicEngine.h"

void AMenuGameMode::StartPlay()
{
    Super::StartPlay();
    auto* musicEngine = GetGameInstance()->GetSubsystem<UMusicEngine>();
    musicEngine->PlayMenuMusic();
}

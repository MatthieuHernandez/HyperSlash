#include "MenuGameMode.h"
#include "MusicEngine.h"

void AMenuGameMode::StartPlay()
{
    Super::StartPlay();
    UMusicEngine* musicEngine = GetGameInstance()->GetSubsystem<UMusicEngine>();
    musicEngine->PlayMenuMusic();
}

#include "MusicEngine.h"
#include "Misc/FileHelper.h"

void UMusicEngine::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    liveFile = "live.strudel";
    variablesFile = "variables.strudel";
    SetMasterVolume(1.0f);
}

bool UMusicEngine::load(FString& fileContent, const FString& file)
{
    auto filePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Content/strudel-autoplay"), file);
    if (!FFileHelper::LoadFileToString(fileContent, *filePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to load Strudel file: %s"), *filePath);
        return false;
    }
    return true;
}

bool UMusicEngine::save(const FString fileContent, const FString& file)
{
    auto filePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Content/strudel-autoplay"), file);
    if (!FFileHelper::SaveStringToFile(fileContent, *filePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to load Strudel file: %s"), *filePath);
        return false;
    }
    return true;
}

void UMusicEngine::clearLive()
{
    save(TEXT(""), *liveFile);
}

void UMusicEngine::copyIntoLive(const FString& file)
{
    FString fileContent;
    FString liveContent;
    if (load(fileContent, *file))
    {
        load(liveContent, *liveFile);
        liveContent += fileContent + '\n';
        save(liveContent, *liveFile);
    }
}

void UMusicEngine::SetMasterVolume(float value, const FString& file)
{
    FString fileContent;
    if (load(fileContent, *file))
    {
        const FRegexPattern Pattern(TEXT(R"(const\s+MASTER_VOLUME\s*=\s*[\d.]+;)"));
        FRegexMatcher Matcher(Pattern, fileContent);
        if (Matcher.FindNext())
        {
            FString Replacement = FString::Printf(TEXT("const MASTER_VOLUME = %.2f;"), value);
            fileContent = fileContent.Left(Matcher.GetMatchBeginning())
                + Replacement
                + fileContent.Mid(Matcher.GetMatchEnding());
            save(fileContent, *file);
        }
    }
}

void UMusicEngine::SetGlobalTempo(int32 value, const FString& file)
{
    FString fileContent;
    if (load(fileContent, *file))
    {
        const FRegexPattern Pattern(TEXT(R"(const\s+GLOBAL_TEMPO\s*=\s*[\d.]+;)"));
        FRegexMatcher Matcher(Pattern, fileContent);
        if (Matcher.FindNext())
        {
            FString Replacement = FString::Printf(TEXT("const GLOBAL_TEMPO = %i;"), value);
            fileContent = fileContent.Left(Matcher.GetMatchBeginning())
                + Replacement
                + fileContent.Mid(Matcher.GetMatchEnding());
            save(fileContent, *file);
        }
    }
}

void UMusicEngine::SetMasterVolume(float value)
{
    SetMasterVolume(value, variablesFile);
    SetMasterVolume(value, liveFile);
}

void UMusicEngine::SetGlobalTempo(int32 value)
{
    SetGlobalTempo(value, variablesFile);
    SetGlobalTempo(value, liveFile);
}


void UMusicEngine::PlayMenuMusic()
{
    clearLive();
    copyIntoLive(variablesFile);
    copyIntoLive("menu.strudel");
}

void UMusicEngine::PlayDesertMusic()
{
    clearLive();
    copyIntoLive(variablesFile);
    copyIntoLive("desert.strudel");
    SetGlobalTempo(60);
}

using UnrealBuildTool;

public class HyperSlash : ModuleRules
{
    public HyperSlash(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "AIModule",
            "NavigationSystem",
            "StateTreeModule",
            "GameplayStateTreeModule",
            "Niagara",
            "UMG",
            "Slate"
        });

        PublicIncludePaths.AddRange(new string[] {"HyperSlash"});
    }
}

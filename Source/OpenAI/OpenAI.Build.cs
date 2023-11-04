// OpenAI, Copyright LifeEXE. All Rights Reserved.

using UnrealBuildTool;

public class OpenAI : ModuleRules
{
    public OpenAI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        // clang-format off
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Json",
                "JsonUtilities",
                "ImageWrapper",
                "HTTP"
            });
        // clang-format on
    }
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

using UnrealBuildTool;

public class OpenAI : ModuleRules
{
    public OpenAI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        // clang-format off
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Json",
                "JsonUtilities",
                "HTTP",
                "ImageWrapper"
            });
        // clang-format on
    }
}

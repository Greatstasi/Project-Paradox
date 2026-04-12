// Copyright (c), Firelight Technologies Pty, Ltd. 2012-2026.

using UnrealBuildTool;
using System;

public class FMODStudioOpenXR : ModuleRules
{
    public FMODStudioOpenXR(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "OpenXRHMD",
            });

        PublicIncludePathModuleNames.AddRange(new string[] { "OpenXRHMD" });

        PrivateDependencyModuleNames.AddRange(
            new[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Projects",
                "FMODStudio",

                "OpenXR",
                "OpenXRHMD",
            });
    }
}

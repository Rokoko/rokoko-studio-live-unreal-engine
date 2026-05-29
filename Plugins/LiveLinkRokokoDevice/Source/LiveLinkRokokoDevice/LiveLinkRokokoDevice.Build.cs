// Copyright Rokoko. All Rights Reserved.

using UnrealBuildTool;

public class LiveLinkRokokoDevice : ModuleRules
{
    public LiveLinkRokokoDevice(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "HTTP",
                "Json",
                "LiveLinkCapabilities",
                "LiveLinkDevice",
                "LiveLinkHub",
                "NamingTokens",
                "NamingTokensUI",
                "PropertyEditor",
                "Slate",
                "SlateCore"
            }
        );

        CppCompileWarningSettings.UnsafeTypeCastWarningLevel = WarningLevel.Error;
    }
}

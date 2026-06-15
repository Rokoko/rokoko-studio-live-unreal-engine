// Copyright Rokoko. All Rights Reserved.

using UnrealBuildTool;

public class LiveLinkRokokoDevice : ModuleRules
{
    public LiveLinkRokokoDevice(ReadOnlyTargetRules Target) : base(Target)
    {
        bool bWithLiveLinkHub = Target.Version.MajorVersion > 5
            || (Target.Version.MajorVersion == 5 && Target.Version.MinorVersion >= 8);

        PrivateDefinitions.Add($"ROKOKO_WITH_LIVELINKHUB={(bWithLiveLinkHub ? 1 : 0)}");

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "HTTP",
                "Json",
				"LiveLinkDevice",
				"LiveLinkInterface",
                "PropertyEditor",
                "Slate",
                "SlateCore"
            }
        );

        if (bWithLiveLinkHub)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "LiveLinkHub",
                    "NamingTokens",
                    "NamingTokensUI"
                }
            );
        }

        CppCompileWarningSettings.UnsafeTypeCastWarningLevel = WarningLevel.Error;
    }
}

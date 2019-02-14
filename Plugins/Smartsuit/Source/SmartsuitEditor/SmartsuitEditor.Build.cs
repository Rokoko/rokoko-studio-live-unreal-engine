// Copyright 2019 Rokoko Electronics. All Rights Reserved.

using UnrealBuildTool;
/// @cond doc_hidden
public class SmartsuitEditor : ModuleRules
{
    public SmartsuitEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.AddRange(
            new string[] {
                "SmartsuitEditor/Public",
				// ... add public include paths required here ...
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                "SmartsuitEditor/Private",
				// ... add other private include paths required here ...
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                //"Core",
                //"Core", "AnimGraph", "BlueprintGraph", "AnimGraphRuntime", "CoreUObject", "Engine", "InputCore", "Sockets", "Networking",
                "ApplicationCore", "Core", "CoreUObject", "Engine", "InputCore", "Sockets", "Networking", "AnimGraph", "BlueprintGraph","AnimGraphRuntime", "Smartsuit"
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "UnrealEd",
                "GraphEditor",
                //"CoreUObject",
                //"Engine",
                //"Slate",
                //"SlateCore",
                //"InputCore",
                //"AnimGraph",
                //"BlueprintGraph",
                //"Networking",
                //"Sockets",
                //"AnimGraphRuntime",
				// ... add private dependencies that you statically link with here ...	
			}
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
                
				// ... add any modules that your module loads dynamically here ...
			}
            );
    }
}
/// @endcond
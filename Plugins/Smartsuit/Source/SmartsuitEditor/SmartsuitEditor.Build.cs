// Copyright 2019 Rokoko Electronics. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
/// @cond doc_hidden
public class SmartsuitEditor : ModuleRules
{
    public SmartsuitEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        MinFilesUsingPrecompiledHeaderOverride = 1;
        //bFasterWithoutUnity = true;
        bUseUnity = false;

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
        //     PublicIncludePaths.AddRange(
        //         new string[] {
        //             "SmartsuitEditor/Public",
        //	// ... add public include paths required here ...
        //}
        //         );


   //     PrivateIncludePaths.AddRange(
   //         new string[] {
   //             "SmartsuitEditor/Private",
   //             // ... add other private include paths required here ...
			//}
   //         );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "AnimGraph",
                "BlueprintGraph",
                "AnimGraphRuntime",
                "Smartsuit",
                "LiveLinkInterface"
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "UnrealEd",
                "GraphEditor",
                
                "SlateCore",
                "Slate",

                "LiveLinkInterface"
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
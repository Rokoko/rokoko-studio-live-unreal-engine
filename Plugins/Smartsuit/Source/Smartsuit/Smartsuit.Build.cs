// Copyright 2019 Rokoko Electronics. All Rights Reserved.

using System.IO;
using UnrealBuildTool;
/// @cond doc_hidden
public class Smartsuit : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string SmartsuitLibPath
    {
        get
        {
            return Path.GetFullPath(Path.Combine(ModulePath, "../../Lib/"));
        }
    }

    public Smartsuit(ReadOnlyTargetRules Target) : base (Target)
    {
        //MinFilesUsingPrecompiledHeaderOverride = 1;
        //bFasterWithoutUnity = true;

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
        //     PublicIncludePaths.AddRange(
        //         new string[] {
        //             "Smartsuit/Public",
        //             "LiveLink/Public",
        //	// ... add public include paths required here ...
        //}
        //         );


   //     PrivateIncludePaths.AddRange(
   //         new string[] {
   //             "Smartsuit/Private",
   //             "LiveLink/Private",

			//	// ... add other private include paths required here ...
			//}
   //         );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                //"Core",
                //"Core", "AnimGraph", "BlueprintGraph", "AnimGraphRuntime", "CoreUObject", "Engine", "Sockets", "Networking",
                "Core", "CoreUObject", "Engine", "Sockets", "Http", "Networking", "AnimGraphRuntime", "Json", "JsonUtilities", "LiveLink", "RenderCore", "InputCore"/*, "LiveLinkInterface"*/
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                //"LiveLinkEditor", "LiveLinkInterface", "LiveLinkMessageBusFramework",
                //"Core",
                //"CoreUObject",
                //"Engine",
                //"MovieScene",
                //"MovieSceneTracks",
                //"Projects",

                "Messaging",
                "LiveLink",
                "LiveLinkInterface",
                "LiveLinkMessageBusFramework",
                "HeadMountedDisplay",
                "TimeManagement",
                "SlateCore",
                "Slate"
                //"CoreUObject",
                //"Engine",
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

        BuildVersion version;
        if (BuildVersion.TryRead(BuildVersion.GetDefaultFileName(), out version))
#if UE_4_19_OR_LATER
            PublicDefinitions.Add(string.Format("BUILT_MINOR_VERSION={0}", version.MinorVersion));
#else
            Definitions.Add(string.Format("BUILT_MINOR_VERSION={0}", version.MinorVersion));
#endif

        LoadSmartsuitMagic(Target);
    }

    public bool LoadSmartsuitMagic(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            isLibrarySupported = true;

            string PlatformString = "x64";
            string LibrariesPath = Path.Combine(SmartsuitLibPath, "V8", "Libraries");

            /*
            test your path with:
            using System; // Console.WriteLine("");
            Console.WriteLine("... LibrariesPath -> " + LibrariesPath);
            */

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "SmartsuitLib." + PlatformString + ".lib"));
        } else if (Target.Platform == UnrealTargetPlatform.Mac){

            isLibrarySupported = true;

            string PlatformString = "x64";
            string LibrariesPath = Path.Combine(SmartsuitLibPath, "V8", "Libraries");

            /*
            test your path with:
            using System; // Console.WriteLine("");
            Console.WriteLine("... LibrariesPath -> " + LibrariesPath);
            */

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "SmartsuitLib." + PlatformString + ".a"));
        
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(SmartsuitLibPath, "V8", "Includes"));
        }

#if UE_4_19_OR_LATER
        PublicDefinitions.Add(string.Format("WITH_BOBS_MAGIC_BINDING={0}", isLibrarySupported ? 1 : 0));
#else
        Definitions.Add(string.Format("WITH_BOBS_MAGIC_BINDING={0}", isLibrarySupported ? 1 : 0));
#endif
        return isLibrarySupported;
    }
}
/// @endcond

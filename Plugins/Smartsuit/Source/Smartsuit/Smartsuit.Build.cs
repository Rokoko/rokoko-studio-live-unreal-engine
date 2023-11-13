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

        PublicDefinitions.Add("USE_SMARTSUIT_ANIMATION_ROLE");

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                //"Core",
                //"Core", "AnimGraph", "BlueprintGraph", "AnimGraphRuntime", "CoreUObject", "Engine", "Sockets", "Networking",
                "Core", "CoreUObject", "Engine", "Sockets", "HTTP", "Networking", "AnimGraphRuntime", "Json", "JsonUtilities", "LiveLink", "RenderCore", "InputCore", "LiveLinkInterface", "LiveLinkAnimationCore"
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

        //LoadSmartsuitMagic(Target);

        //PublicDefinitions.Add("LZ4F_STATIC_LINKING_ONLY");

	    LoadLZ4(Target);


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
        
        }  else if (Target.Platform == UnrealTargetPlatform.Linux){

            isLibrarySupported = true;

            string PlatformString = "x64";
            string LibrariesPath = Path.Combine(SmartsuitLibPath, "V8", "Libraries");

            /*
            test your path with:
            using System; // Console.WriteLine("");
            Console.WriteLine("... LibrariesPath -> " + LibrariesPath);
            */

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "SmartsuitLib." + PlatformString + ".so"));
        
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

    public bool LoadLZ4(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            isLibrarySupported = true;

            string LibrariesPath = Path.Combine(SmartsuitLibPath, "lz4", "static");

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "liblz4_static.lib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            isLibrarySupported = true;

            string LibrariesPath = Path.Combine(SmartsuitLibPath, "lz4", "static");

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "liblz4.a"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            isLibrarySupported = true;

            string LibrariesPath = Path.Combine(SmartsuitLibPath, "lz4", "static");

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "liblz4.so"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(SmartsuitLibPath, "lz4", "include"));
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

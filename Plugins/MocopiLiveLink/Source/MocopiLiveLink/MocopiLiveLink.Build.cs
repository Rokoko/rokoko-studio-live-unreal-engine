// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class MocopiLiveLink : ModuleRules
{
	public MocopiLiveLink(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp20;

		PublicDefinitions.Add("LOCTEXT_NAMESPACE=MocopiLiveLinkModule");

		// Enable for debugging
		// OptimizeCode = CodeOptimization.Never;


		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);

		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"LiveLinkInterface",
				"LiveLink",
				"LiveLinkAnimationCore",

				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Sockets",
				"Networking",
				"Projects",
				
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		// Adding SMF library to project packaging
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string LibraryPath = Path.Combine(ModuleDirectory, "../SonyMotionFormat/windows/x64/sony_motion_format.dll");
			RuntimeDependencies.Add(LibraryPath);
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			string BundlePath = Path.Combine(ModuleDirectory, "../SonyMotionFormat/mac/sony_motion_format.bundle");

			string LibraryPath = Path.Combine(BundlePath, "Contents/MacOS/sony_motion_format");
			RuntimeDependencies.Add(LibraryPath);

			string CodeSignaturePath = Path.Combine(BundlePath, "Contents/_CodeSignature/CodeResources");
			RuntimeDependencies.Add(CodeSignaturePath);

			string InfoPlistPath = Path.Combine(BundlePath, "Contents/Info.plist");
			RuntimeDependencies.Add(InfoPlistPath);
		}

	}
}

// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UnrealSampleProjectEditorTarget : TargetRules
{
	public UnrealSampleProjectEditorTarget(TargetInfo Target) : base(Target)
	{
        bOverrideBuildEnvironment = true;
        Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "UnrealSampleProject" } );
	}
}

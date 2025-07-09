// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MScDiss : ModuleRules
{
	public MScDiss(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}

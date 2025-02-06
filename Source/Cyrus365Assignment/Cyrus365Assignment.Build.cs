// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Cyrus365Assignment : ModuleRules
{
	public Cyrus365Assignment(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}

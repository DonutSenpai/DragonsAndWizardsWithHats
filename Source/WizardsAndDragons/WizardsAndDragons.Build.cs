// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WizardsAndDragons : ModuleRules
{
	public WizardsAndDragons(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "GameplayTasks", "UMG", "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemNull" });

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}

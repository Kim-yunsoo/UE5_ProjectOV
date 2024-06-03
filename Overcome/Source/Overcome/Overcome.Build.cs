// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Overcome : ModuleRules
{
	public Overcome(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "Overcome" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" ,"Niagara", "NavigationSystem", "AIModule", "GameplayTasks", "Slate", "SlateCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

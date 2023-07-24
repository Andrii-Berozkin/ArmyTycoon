// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ArmyTycoon : ModuleRules
{
	public ArmyTycoon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		//	AbilitySystem
		PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });

		//	AI
		PublicDependencyModuleNames.AddRange(new string[] { "AIModule" });

		//	Navigation
		PublicDependencyModuleNames.AddRange(new string[] { "NavigationSystem" });

		//	Anim
		PublicDependencyModuleNames.AddRange(new string[] { "LevelSequence", "MovieScene" });

		//	Input
		PublicDependencyModuleNames.AddRange(new string[] { "EnhancedInput" });

		//	UI
		PublicDependencyModuleNames.AddRange(new string[] { "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

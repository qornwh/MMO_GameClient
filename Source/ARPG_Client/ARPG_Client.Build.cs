// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ARPG_Client : ModuleRules
{
	public ARPG_Client(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "HeadMountedDisplay", "EnhancedInput", "Sockets", "Networking", "SlateCore", "AnimGraphRuntime", "Niagara" });

		PrivateDependencyModuleNames.AddRange(new string[] { "NetWorkCore" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

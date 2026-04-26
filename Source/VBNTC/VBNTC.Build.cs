// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VBNTC : ModuleRules
{
	public VBNTC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"VBNTC",
			"VBNTC/Variant_Horror",
			"VBNTC/Variant_Horror/UI",
			"VBNTC/Variant_Shooter",
			"VBNTC/Variant_Shooter/AI",
			"VBNTC/Variant_Shooter/UI",
			"VBNTC/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

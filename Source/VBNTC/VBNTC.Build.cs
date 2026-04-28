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

		
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class YorgenMeneses_Task : ModuleRules
{
	public YorgenMeneses_Task(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(
			new string[]
			 {
                "YorgenMeneses_Task",
			});

        PublicDependencyModuleNames.AddRange(new string[]
        {
                "Core",
                "Engine",
                "CoreUObject",
                "InputCore",
                "EnhancedInput",
                "GameplayAbilities",
                "GameplayTags",
                "GameplayTasks",
                "UMG",
                "Slate",
                "SlateCore",
                "Niagara",
                "NetCore",
                "OnlineSubsystem"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

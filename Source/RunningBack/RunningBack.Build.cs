// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RunningBack : ModuleRules
{
	public RunningBack(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "OnlineSubsystem", "OnlineSubsystemNull", "OnlineSubsystemUtils", "PhysXVehicles" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "OnlineSubsystem" });
        PublicIncludePaths.AddRange(new string[] { "PhysXVehicles/Public", "PhysXVehicles/Classes" });
    }
}

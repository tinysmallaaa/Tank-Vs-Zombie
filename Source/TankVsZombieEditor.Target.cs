// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class TankVsZombieEditorTarget : TargetRules
{
	public TankVsZombieEditorTarget(TargetInfo Target) : base(Target)
	{
        bUseUnityBuild = true;
        Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "TankVsZombie" } );
	}
}

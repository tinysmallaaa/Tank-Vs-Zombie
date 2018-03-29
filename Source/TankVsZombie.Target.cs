// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class TankVsZombieTarget : TargetRules
{
	public TankVsZombieTarget(TargetInfo Target) : base(Target)
	{
        bUseUnityBuild = true;

        Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "TankVsZombie" } );
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TankVsZombie : ModuleRules
{
	public TankVsZombie(ReadOnlyTargetRules Target):base(Target)  
	{
		// PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

         PublicDependencyModuleNames.AddRange( new string[] { "Core"            ,
                                                             "CoreUObject"     ,
                                                             "Engine"          ,
                                                             "InputCore"       ,
                                                             "AIModule"        ,
                                                             "PerfCounters"    ,
                                                             "MoviePlayer"     ,    
															 "AudioMixer"      ,  
															 "OnlineSubsystem" ,
															 "UMG" ,
                                                             "PhysxVehicles"   });

         PrivateDependencyModuleNames.AddRange(new string[] {     });

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            DynamicallyLoadedModuleNames.Add("OnlineSubSystemGooglePlay");
            DynamicallyLoadedModuleNames.Add("AndroidAdvertising");
        } 

        // Uncomment if you are using Slate UI
         PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
         PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}

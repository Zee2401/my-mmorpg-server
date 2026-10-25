using UnrealBuildTool;
using System.Collections.Generic;

public class MusicMMORPGServerTarget : TargetRules
{
    public MusicMMORPGServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        
        // Server-specific configurations
        bBuildEditor = false;
        bBuildDevelopmentEditor = false;
        
        // Server only modules
        bUseLogs = true;
        
        // Server build configurations
        LinkType = TargetLinkType.Modular;
        
        // Additional server-only dependencies
        PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "NavigationSystem",
            "AIModule",
            "GameplayTasks",
            "OnlineSubsystem",
            "OnlineSubsystemNull"
        });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "Slate",
            "SlateCore",
            "OnlineSubsystemUtils",
            "ReplicationGraph",
            "AIModule",
            "NavigationSystem",
            "GameplayTags",
            "NetworkReplayStreaming"
        });

        // Server doesn't need rendering
        bCompileAgainstEngine = true;
        bCompileAgainstCoreUObject = true;
        
        // Server doesn't need audio
        bCompileWithEngineAudio = false;
        
        // Server doesn't need rendering
        bCompileWithRenderer = false;
        bCompileWithEditor = false;
        
        // Server doesn't need HMD
        bCompileWithHMD = false;
        
        // Server-specific definitions
        PublicDefinitions.Add("WITH_SERVER=1");
        PublicDefinitions.Add("WITH_EDITOR=0");
        PublicDefinitions.Add("WITH_DEVELOPER_TOOLS=0");
        PublicDefinitions.Add("UE_SERVER=1");
        PublicDefinitions.Add("IS_MONOLITHIC=0");
        PublicDefinitions.Add("WITH_ENGINE=1");
        PublicDefinitions.Add("WITH_UNREAL_DEVELOPER_TOOLS=0");
        
        // Disable client-specific features
        PublicDefinitions.Add("WITH_CLIENT=0");
        PublicDefinitions.Add("WITH_GAME=1");
        
        // Enable server-only optimizations
        PublicDefinitions.Add("WITH_SERVER_OPTIMIZATIONS=1");
        
        // Enable dedicated server
        PublicDefinitions.Add("WITH_DEDICATED_SERVER=1");
    }
}

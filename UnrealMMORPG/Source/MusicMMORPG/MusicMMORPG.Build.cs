using UnrealBuildTool;

public class MusicMMORPG : ModuleRules
{
    public MusicMMORPG(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "NavigationSystem",
            "AIModule",
            "GameplayTasks",
            "GameplayAbilities",
            "GameFeatures",
            "ModularGameplay",
            "ModularGameplayActors"
        });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "Slate",
            "SlateCore",
            "UMG",
            "CommonUI",
            "GameplayTags",
            "GameplayTasks",
            "OnlineSubsystem",
            "OnlineSubsystemNull",
            "OnlineSubsystemUtils",
            "ReplicationGraph",
            "AIModule",
            "NavigationSystem",
            "PhysicsCore",
            "RHI",
            "RenderCore",
            "Renderer",
            "NetworkReplayStreaming",
            "AudioMixer",
            "AudioPlatformConfiguration",
            "Synthesis",
            "Voice"
        });

        DynamicallyLoadedModuleNames.AddRange(new string[] 
        {
            "OnlineSubsystemSteam",
            "OnlineSubsystemIOS",
            "VoiceChat"
        });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        PrivateDependencyModuleNames.Add("OnlineSubsystem");
        
        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            if (Target.bWithEditor == true)
            {
                PrivateDependencyModuleNames.Add("OnlineSubsystemSteam");
            }
            else
            {
                PrivateDependencyModuleNames.Add("OnlineSubsystemSteam");
            }
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PrivateDependencyModuleNames.Add("OnlineSubsystemIOS");
        }

        // To include OnlineSubsystemNull when building for dedicated server
        if (Target.bBuildServerOnly == true)
        {
            PrivateDependencyModuleNames.Add("OnlineSubsystemNull");
        }

        // Enable IWYU (Include What You Use)
        bEnableIWYU = true;
        
        // Enable PCH for faster compilation
        bUsePCHFiles = true;
        bUseSharedPCHs = true;
        
        // Enable Undefined Identifier Warnings
        bWarnAboutMissingPCHFiles = true;
        
        // Enable exceptions
        bEnableExceptions = true;
        
        // Enable RTTI
        bEnableRTTI = true;
        
        // Enable Unity build for faster compilation
        bUseUnity = true;
        
        // Enable Incremental Linking
        bUseIncrementalLinking = true;
        
        // Enable Whole Program Optimization for Release builds
        if (Target.Configuration == UnrealTargetConfiguration.Release)
        {
            bUsePGO = true;
        }
        
        // Platform-specific settings
        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            // iOS specific settings
            bEnableExceptions = true;
            bUsePCHFiles = true;
            bUseSharedPCHs = true;
            
            // Enable Metal API
            PublicDefinitions.Add("USE_METAL=1");
            
            // iOS minimum version
            PublicDefinitions.Add("MIN_IOS_VERSION=120000");
            
            // iOS multiplayer support
            PublicDefinitions.Add("SUPPORTS_MULTIPLAYER=1");
            PublicDefinitions.Add("SUPPORTS_NETWORKING=1");
        }
        else if (Target.Platform == UnrealTargetPlatform.Win64 || 
                 Target.Platform == UnrealTargetPlatform.Mac ||
                 Target.Platform == UnrealTargetPlatform.Linux)
        {
            // Desktop settings
            bEnableExceptions = true;
            bUsePCHFiles = true;
            bUseSharedPCHs = true;
            
            // Desktop multiplayer support
            PublicDefinitions.Add("SUPPORTS_MULTIPLAYER=1");
            PublicDefinitions.Add("SUPPORTS_NETWORKING=1");
        }
        
        // Add custom preprocessor definitions
        PublicDefinitions.Add("MUSIC_MMORPG=1");
        PublicDefinitions.Add("WITH_ONLINE=1");
        PublicDefinitions.Add("WITH_REPLICATION=1");
        PublicDefinitions.Add("WITH_AI=1");
        PublicDefinitions.Add("WITH_GAMEPLAY_ABILITIES=1");
        PublicDefinitions.Add("WITH_GAMEPLAY_TAGS=1");
        PublicDefinitions.Add("WITH_SYNTHESIS=1");
        
        // Enable multiplayer networking
        PublicDefinitions.Add("WITH_NETWORKING=1");
        PublicDefinitions.Add("WITH_REPLICATION_GRAPH=1");
        
        // Enable voice chat
        PublicDefinitions.Add("WITH_VOICE_CHAT=1");
        
        // Enable audio mixer
        PublicDefinitions.Add("WITH_AUDIO_MIXER=1");
        
        // Enable modular gameplay
        PublicDefinitions.Add("WITH_MODULAR_GAMEPLAY=1");
        
        // MMORPG-specific definitions
        PublicDefinitions.Add("MUSIC_MMORPG_GAME=1");
        PublicDefinitions.Add("WITH_MMORPG_NETWORKING=1");
        PublicDefinitions.Add("WITH_MMORPG_SYSTEMS=1");
        
        // Cross-platform definitions
        PublicDefinitions.Add("SUPPORTS_MULTIPLAYER=1");
        PublicDefinitions.Add("SUPPORTS_NETWORKING=1");
        PublicDefinitions.Add("SUPPORTS_ONLINE=1");
        
        // Music game specific
        PublicDefinitions.Add("WITH_AUDIO_SYNTHESIZER=1");
        PublicDefinitions.Add("WITH_RHYTHM_COMBAT=1");
    }
}

using UnrealBuildTool;
using System.Collections.Generic;

public class MusicMMORPGEditorTarget : TargetRules
{
    public MusicMMORPGEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        
        // Editor-specific configurations
        bBuildEditor = true;
        bBuildDevelopmentEditor = true;
        
        // Editor build configurations
        LinkType = TargetLinkType.Modular;
        
        // Additional editor-only dependencies
        PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UnrealEd",
            "LevelEditor",
            "PropertyEditor",
            "ContentBrowser",
            "SceneOutliner",
            "WorldBrowser",
            "DetailCustomizations",
            "AssetTools",
            "Kismet",
            "KismetWidgets",
            "KismetCompiler",
            "BlueprintGraph",
            "GraphEditor",
            "Personna",
            "SettingsEditor",
            "DirectoryWatcher",
            "SourceControl",
            "SourceControlWindows",
            "GitSourceControl",
            "PerforceSourceControl",
            "SVNSourceControl"
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
            "OnlineSubsystemSteam",
            "OnlineSubsystemIOS",
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
            "Voice",
            "VoiceChat",
            "UATHelper",
            "UnrealHeaderTool",
            "DerivedDataCache",
            "SandboxFile",
            "AssetDefinition",
            "AssetRegistry"
        });

        // Editor-specific definitions
        PublicDefinitions.Add("WITH_EDITOR=1");
        PublicDefinitions.Add("WITH_EDITORONLY_DATA=1");
        PublicDefinitions.Add("WITH_DEVELOPER_TOOLS=1");
        PublicDefinitions.Add("UE_EDITOR=1");
        PublicDefinitions.Add("IS_MONOLITHIC=0");
        PublicDefinitions.Add("WITH_ENGINE=1");
        PublicDefinitions.Add("WITH_UNREAL_DEVELOPER_TOOLS=1");
        PublicDefinitions.Add("WITH_HOT_RELOAD=1");
        PublicDefinitions.Add("WITH_HOT_RELOAD_GAME=1");
        
        // Enable editor features
        PublicDefinitions.Add("WITH_EDITOR_ONLY_DATA=1");
        PublicDefinitions.Add("WITH_SERVER=0");
        PublicDefinitions.Add("WITH_CLIENT=1");
        
        // Enable editor optimizations
        PublicDefinitions.Add("WITH_EDITOR_OPTIMIZATIONS=1");
        
        // Enable Blueprints
        PublicDefinitions.Add("WITH_BLUEPRINTS=1");
        PublicDefinitions.Add("WITH_VISUAL_SCRIPTING=1");
        
        // Enable animation editor
        PublicDefinitions.Add("WITH_ANIMATION_EDITOR=1");
        
        // Enable material editor
        PublicDefinitions.Add("WITH_MATERIAL_EDITOR=1");
        
        // Enable particle editor
        PublicDefinitions.Add("WITH_PARTICLE_EDITOR=1");
        
        // Enable UMG editor
        PublicDefinitions.Add("WITH_UMG_EDITOR=1");
    }
}

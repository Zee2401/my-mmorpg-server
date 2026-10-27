# Music MMORPG - Visual Guide for Unreal Engine

> **Visual representation of the Unreal Engine 5.3 MMORPG project structure, classes, and setup process**

---

## 📁 Table of Contents

1. [Project Structure Diagram](#1-project-structure-diagram)
2. [Class Hierarchy Visualization](#2-class-hierarchy-visualization)
3. [Game World & Zones Map](#3-game-world--zones-map)
4. [Boss Encounters Visualization](#4-boss-encounters-visualization)
5. [Setup Process Flowchart](#5-setup-process-flowchart)
6. [Character Classes Overview](#6-character-classes-overview)
7. [Multiplayer Network Architecture](#7-multiplayer-network-architecture)
8. [Unreal Editor Layout](#8-unreal-editor-layout)
9. [File Locations Cheat Sheet](#9-file-locations-cheat-sheet)
10. [Build Process Visualization](#10-build-process-visualization)
11. [iOS Build Process](#11-ios-build-process)
12. [Controls Reference](#12-controls-reference)

---

## 1. Project Structure Diagram

```
MusicMMORPG (Unreal Engine 5.3 Project)
├── UnrealMMORPG/
│   ├── README.md (15KB - Project Overview)
│   ├── INSTRUCTIONS.md (28KB - Setup Guide)
│   ├── VISUAL_GUIDE.md (This File)
│   ├── MusicMMORPG.uproject (Project Configuration)
│   ├── MusicMMORPG.uplugin (Plugin Metadata)
│   │
│   ├── Content/ (Game Assets)
│   │   ├── Blueprints/ (*.uasset - Actor/Character blueprints)
│   │   ├── Materials/ (*.umap - Surface materials)
│   │   ├── Models/ (*.fbx - 3D character/environment models)
│   │   ├── Textures/ (*.png - Diffuse/Normal/Specular maps)
│   │   ├── Sounds/
│   │   │   ├── Music/ (Background tracks, boss themes)
│   │   │   ├── SFX/ (Ability sounds, UI sounds)
│   │   │   └── Voices/ (Character voice lines)
│   │   ├── Maps/ (*.umap - Game levels)
│   │   ├── Animations/ (*.uasset - Character animations)
│   │   ├── Particles/ (*.uasset - VFX effects)
│   │   ├── Widgets/ (*.uasset - UI elements)
│   │   └── DataAssets/ (*.uasset - Game data)
│   │
│   └── Source/ (C++ Code)
│       └── MusicMMORPG/ (Main Module)
│           ├── MusicMMORPG.Build.cs (Module dependencies)
│           ├── MusicMMORPGEditor.Target.cs
│           ├── MusicMMORPGServer.Target.cs
│           │
│           ├── Public/ (Header files - .h)
│           │   ├── Characters/
│           │   │   └── MusicMMORPGCharacter.h
│           │   ├── Components/
│           │   │   └── MMORPGInputComponent.h
│           │   ├── GameModes/
│           │   │   └── MMORPGGameMode.h
│           │   ├── GameStates/
│           │   │   └── MusicMMORPGGameState.h
│           │   ├── Player/
│           │   │   ├── MusicMMORPGPlayerController.h
│           │   │   └── MusicMMORPGPlayerState.h
│           │   ├── Systems/
│           │   │   ├── Audio/
│           │   │   │   └── AudioSystem.h
│           │   │   ├── Networking/
│           │   │   │   └── NetworkSystem.h
│           │   │   └── WorldData.h
│           │   └── UI/
│           │       └── MMORPGHUD.h
│           │
│           └── Private/ (Implementation files - .cpp)
│               ├── Characters/
│               │   └── MusicMMORPGCharacter.cpp
│               ├── Components/
│               │   └── MMORPGInputComponent.cpp
│               ├── GameModes/
│               │   └── MMORPGGameMode.cpp
│               ├── GameStates/
│               │   └── MusicMMORPGGameState.cpp
│               ├── Player/
│               │   ├── MusicMMORPGPlayerController.cpp
│               │   └── MusicMMORPGPlayerState.cpp
│               ├── Systems/
│               │   ├── Audio/
│               │   │   └── AudioSystem.cpp
│               │   ├── Networking/
│               │   │   └── NetworkSystem.cpp
│               │   └── WorldData.cpp
│               └── UI/
│                   └── MMORPGHUD.cpp
```

---

## 2. Class Hierarchy Visualization

```
UNREAL ENGINE CLASS HIERARCHY

UObject (Base Class)
├── AActor (World Object)
│   └── ACharacter (Movement, Mesh, Capsule Component)
│       └── MusicMMORPGCharacter
│           ├── UPROPERTY: Health, Energy, Level, XP
│           ├── UPROPERTY: CharacterClass, Inventory, CurrentZone
│           ├── UPROPERTY: AttributePoints
│           ├── UFUNCTION: ServerMove, ServerAttack, UseAbility
│           ├── UFUNCTION: PlayNote, PlayDrum, PlayChord
│           ├── Movement Modes: Walk, Run, Sprint, Crouch
│           └── Network: Replicates to all clients (DOREPLIFETIME)
│
├── APawn (Possessable)
│   └── (Extended by ACharacter)
│
└── UGameInstance (Singleton)

---

GAME FRAMEWORK CLASSES:

┌─────────────────────────────────────────────────────────────┐
│ MMORPGGameMode (Server Authority)                            │
├─────────────────────────────────────────────────────────────┤
│ + SpawnPlayers() - Handle player spawning                    │
│ + HandleZoneChange() - Manage zone transitions                │
│ + AutoSaveTimer() - Periodic player saves                     │
│ + KickPlayer() - Remove player from server                     │
│ + BanPlayer() - Permanently ban player                        │
│ + MutePlayer() - Disable chat for player                       │
│ + InitGame() - Initialize game systems                        │
│ + PostLogin() - Handle player login                           │
│ + Logout() - Handle player logout                             │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│ MusicMMORPGGameState (World State)                           │
├─────────────────────────────────────────────────────────────┤
│ + UPROPERTY: PlayerArray (All connected players)              │
│ + UPROPERTY: ZoneInfoMap (World zones data)                   │
│ + UPROPERTY: BossArray (Active boss encounters)               │
│ + UPROPERTY: Statistics (Server performance stats)            │
│ + UPROPERTY: BannedPlayersList                               │
│ + AddPlayer() - Register new player                          │
│ + RemovePlayer() - Unregister player                          │
│ + GetZoneInfo() - Retrieve zone data                          │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│ MusicMMORPGPlayerController (Input Handler)                  │
├─────────────────────────────────────────────────────────────┤
│ + SetupInputComponent() - Bind input actions                   │
│ + ClientTravel() - Handle zone transitions                     │
│ + ServerRPCs - Server authority functions                     │
│ + ChatSystem - Handle player chat messages                    │
│ + HUD - Manage UI display                                    │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│ MMORPGPlayerState (Player Data)                              │
├─────────────────────────────────────────────────────────────┤
│ + UPROPERTY: Level, XP                                       │
│ + UPROPERTY: Health, Energy                                  │
│ + UPROPERTY: CharacterClass (ECharacterClass enum)           │
│ + UPROPERTY: Inventory (TArray<FInventoryItem>)               │
│ + UPROPERTY: AttributePoints (Unspent stat points)           │
│ + UPROPERTY: CurrentZone                                    │
│ + AddXP() - Gain experience points                            │
│ + LevelUp() - Handle level progression                        │
│ + SavePlayerData() - Persist player state                     │
└─────────────────────────────────────────────────────────────┘

---

ACTOR COMPONENTS:

┌─────────────────────────────────────────────────────────────┐
│ MMORPGInputComponent (Input Processing)                       │
├─────────────────────────────────────────────────────────────┤
│ + BindMovementActions() - WASD, Gamepad                        │
│ + BindLookActions() - Mouse, Gamepad                          │
│ + BindActionInputs() - Jump, Sprint, Abilities                 │
│ + SetupInput() - Initialize input bindings                     │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│ AudioSystem (Sound Synthesis)                                  │
├─────────────────────────────────────────────────────────────┤
│ + PlayNote() - Play single musical note                        │
│ + PlayDrum() - Play drum sound                               │
│ + PlayChord() - Play multiple notes simultaneously              │
│ + PlayScale() - Play musical scale                            │
│ + PlaySFX() - Play sound effects                              │
│ + BackgroundMusic() - Zone ambient music                      │
│ + ZoneMusic() - Zone-specific themes                          │
│ + BossMusic() - Boss encounter music                          │
│ + VoiceChat() - Player voice communication                    │
│ + SetVolume() - Adjust audio levels                           │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│ NetworkSystem (Multiplayer Networking)                        │
├─────────────────────────────────────────────────────────────┤
│ + ReplicateData() - Sync data to clients                       │
│ + BroadcastRPC() - Send to all clients                         │
│ + ServerRPC() - Server authority function                      │
│ + ClientRPC() - Client request function                       │
│ + MulticastRPC() - Send to all including server                │
│ + PlayerConnections - Track connected players                  │
│ + NetworkStatistics - Monitor bandwidth, ping                  │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│ MMORPGHUD (User Interface)                                   │
├─────────────────────────────────────────────────────────────┤
│ + HealthBar - Player health display                           │
│ + EnergyBar - Player energy display                           │
│ + ExperienceBar - XP progress                                 │
│ + ChatSystem - Player chat UI                                 │
│ + Notifications - System messages                             │
│ + PlayerInfo - Character stats display                        │
│ + TargetInfo - Selected target info                           │
│ + Minimap - World map display                                 │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│ WorldData (Game Content Database)                             │
├─────────────────────────────────────────────────────────────┤
│ + ZoneInfo - All world zones data                             │
│ + BossInfo - Boss encounter data                                │
│ + ItemInfo - Items database                                    │
│ + QuestInfo - Quests database                                  │
│ + NPCInfo - NPC data                                           │
└─────────────────────────────────────────────────────────────┘
```

---

## 3. Game World and Zones Map

```
MUSIC MMORPG WORLD OVERVIEW

                                    🎵 MUSIC MMORPG WORLD 🎵

                                    ╔════════════════════════════════════╗
                                    ║                            ║
                                    ║     ┌─────────────────────┐     ║
                                    ║     │   🏙️  SOUND CITY    │     ║
                                    ║     │   Level: 1-10        │     ║
                                    ║     │   Starting Zone      │     ║
                                    ║     └──────────┬──────────┘     ║
                                    ║                │                 ║
                                    ║        ╔═══════╗                ║
                                    ║        ║ NEXUS ║                ║
                                    ║        ╚═══════╝                ║
                                    ║                │                 ║
            ┌───────────────────────┼───────────────────────┐
            │                       │                       │
            ▼                       ▼                       ▼
┌─────────────────────┐ ┌─────────────────────┐ ┌─────────────────────┐
│   🌑 BASS CAVERNS    │ │  🏰 SYNTH CITADEL   │ │   ⚔️  PVP ARENA     │
│   Level: 10-25       │ │   Level: 25-50       │ │   Level: 50+         │
│   Underground Dungeon │ │   Floating Fortress  │ │   Competitive Zone   │
└─────────────────────┘ └─────────────────────┘ └─────────────────────┘
            │                       │                       │
            └───────────────────────┼───────────────────────┘
                                    │
                                    ▼
                            ┌─────────────────────┐
                            │   🌑 NEXUS (HUB)      │
                            │   Central Connection  │
                            └─────────────────────┘

---

ZONE DETAILS:

┌─────────────────┬─────────────────┬─────────────────┬─────────────────┐
│ Zone            │ Level Range      │ PvP Status        │ Boss            │
├─────────────────┼─────────────────┼─────────────────┼─────────────────┤
│ Sound City      │ 1-10             │ Disabled         │ None            │
│ Bass Caverns    │ 10-25            │ Flag System      │ Subwoofer Titan │
│ Synth Citadel   │ 25-50            │ Open World       │ Silence Overlord │
│ PvP Arena       │ 50+              │ Full PvP         │ Static Spectre   │
└─────────────────┴─────────────────┴─────────────────┴─────────────────┘

┌─────────────────┬─────────────────┬─────────────────┬─────────────────┐
│ Zone            │ Music Theme      │ Difficulty       │ Reward Tier      │
├─────────────────┼─────────────────┼─────────────────┼─────────────────┤
│ Sound City      │ Pop/Rock         │ Easy             │ Common          │
│ Bass Caverns    │ Electronic       │ Medium           │ Rare            │
│ Synth Citadel   │ Orchestral       │ Hard             │ Epic            │
│ PvP Arena       │ Metal            │ Expert           │ Legendary       │
└─────────────────┴─────────────────┴─────────────────┴─────────────────┘

---

SOUND CITY AREAS:
  🎹 Piano Plaza - Tutorial Area (Level 1-5)
  🎻 String Street - Early Quests (Level 5-10)
  🎺 Brass Boulevard - First Boss Area
  🎤 Vocal Village - Social Hub

BASS CAVERNS AREAS:
  🎚️ Subwoofer Tunnel - Dangerous Path
  🥁 Drum Chamber - Group Challenges
  🎛️ Synthesizer Cave - Puzzles
  💀 Skeleton Choir - Elite Enemies

SYNTH CITADEL AREAS:
  🎹 Grand Piano Hall - Raid Entrance
  🎻 Symphony Tower - Boss Lair (Silence Overlord)
  🎺 Trumpet Tower - Defense Quests

PVP ARENA AREAS:
  🏟️ Main Arena - Team Deathmatch
  🎯 Target Practice - Solo Training
  🏆 Tournament Grounds - Ranked Matches
  🎼 Rhythm Duel - Music-Based PvP
```

---

## 4. Boss Encounters Visualization

```
BOSS ENCOUNTERS

Each boss has 2-3 phases with unique mechanics.
Players must coordinate attacks in rhythm with the music.

---

🎵 BOSS #1: STATIC SPECTRE
Location: PvP Arena | Level: 50+ | Health: 1,000,000
Difficulty: Expert | Recommended: 10 players (Raid)

┌─────────────────────────────────────────────────────────────────────┐
│ PHASE 1: Silent Assault (100%-70% HP)                               │
├─────────────────────────────────────────────────────────────────────┤
│ Attack: Silence Wave - Disables all abilities for 5 seconds           │
│ Weakness: Sound-based attacks (2x damage)                            │
│ Mechanic: Players must coordinate note sequences in perfect rhythm    │
│ Reward: Spectral Essence (Rare crafting material)                    │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│ PHASE 2: Sonic Storm (70%-30% HP)                                    │
├─────────────────────────────────────────────────────────────────────┤
│ Attack: Sonic Boom - AoE damage in 5000 unit radius                      │
│ Weakness: Drum-based attacks (2.5x damage)                             │
│ Mechanic: Players must maintain rhythm with timed attacks              │
│ Reward: Static Core (Legendary weapon component)                       │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│ PHASE 3: Final Symphony (30%-0% HP)                                  │
├─────────────────────────────────────────────────────────────────────┤
│ Attack: Harmony Crash - Massive AoE damage in 8000 unit radius          │
│ Weakness: Chord-based attacks (3x damage)                             │
│ Mechanic: Full orchestra coordination required (all classes must play)│
│ Reward: Spectral Mount + "Conductor" Title                              │
└─────────────────────────────────────────────────────────────────────┘

Drops: Static Core (Legendary), Spectral Essence (Rare), XP: 50,000

---

🥁 BOSS #2: SUBWOOFER TITAN
Location: Bass Caverns | Level: 20+ | Health: 500,000
Difficulty: Hard | Recommended: 5 players

┌─────────────────────────────────────────────────────────────────────┐
│ PHASE 1: Bass Drop (100%-60% HP)                                         │
├─────────────────────────────────────────────────────────────────────┤
│ Attack: Earth Shaker - Ground AoE with knockback                         │
│ Weakness: Bass note attacks (2x damage)                                 │
│ Mechanic: Players must jump over shockwaves (timing-based)             │
│ Reward: Titan's Bass Drum (Epic weapon)                                   │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│ PHASE 2: Frequency Shift (60%-0% HP)                                    │
├─────────────────────────────────────────────────────────────────────┤
│ Attack: Frequency Wave - DoT effect (3000 damage/sec)                    │
│ Weakness: High-frequency attacks (2.5x damage)                          │
│ Mechanic: Players must switch attack types every 10 seconds            │
│ Reward: Subwoofer Core (Rare crafting material)                          │
└─────────────────────────────────────────────────────────────────────┘

Drops: Titan's Bass Drum (Epic), Subwoofer Core (Rare), XP: 25,000

---

🎻 BOSS #3: SILENCE OVERLORD
Location: Synth Citadel | Level: 45+ | Health: 750,000
Difficulty: Hard | Recommended: 8 players

┌─────────────────────────────────────────────────────────────────────┐
│ PHASE 1: Sound Drain (100%-60% HP)                                        │
├─────────────────────────────────────────────────────────────────────┤
│ Attack: Silence Aura - Disables ALL music abilities                       │
│ Weakness: None - Must use basic attacks only                              │
│ Mechanic: Players must build up sound energy through basic attacks        │
│ Reward: Overlord's Violin (Epic weapon)                                   │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│ PHASE 2: Dissonant Harmony (60%-0% HP)                                   │
├─────────────────────────────────────────────────────────────────────┤
│ Attack: Dissonance Wave - Causes confusion (random movement)              │
│ Weakness: Harmonic attacks (3x damage)                                     │
│ Mechanic: Players must play notes in perfect harmony (combo system)       │
│ Reward: Silence Shard (Epic crafting material)                             │
└─────────────────────────────────────────────────────────────────────┘

Drops: Overlord's Violin (Epic), Silence Shard (Epic), XP: 35,000

---

🏆 BOSS DROP SUMMARY

┌─────────────────┬─────────────────┬─────────────────┬─────────────────┐
│ Boss             │ Weapon Drop      │ Material Drop    │ XP Reward        │
├─────────────────┼─────────────────┼─────────────────┼─────────────────┤
│ Static Spectre   │ Static Core     │ Spectral Essence │ 50,000           │
│                 │ (Legendary)      │ (Rare)           │                 │
├─────────────────┼─────────────────┼─────────────────┼─────────────────┤
│ Subwoofer Titan  │ Titan's Bass    │ Subwoofer Core  │ 25,000           │
│                 │ Drum (Epic)     │ (Rare)           │                 │
├─────────────────┼─────────────────┼─────────────────┼─────────────────┤
│ Silence Overlord │ Overlord's      │ Silence Shard    │ 35,000           │
│                 │ Violin (Epic)   │ (Epic)           │                 │
└─────────────────┴─────────────────┴─────────────────┴─────────────────┘
```

---

## 5. Setup Process Flowchart

```
UNREAL ENGINE 5.3 SETUP - STEP BY STEP

  ┌─────────────────────────────────────────────────────────────────┐
  │                        STEP 1: PREREQUISITES                        │
  │  ┌─────────────────────┐ ┌─────────────────────┐                  │
  │  │ VS 2022 (Windows)   │ │ Xcode + iOS SDK (Mac)│                  │
  │  └─────────────────────┘ └─────────────────────┘                  │
  │  ┌─────────────────────┐ ┌─────────────────────┐                  │
  │  │ Git                │ │ 50GB+ Disk Space    │                  │
  │  │ 16GB+ RAM           │ │ Good GPU             │                  │
  │  └─────────────────────┘ └─────────────────────┘                  │
  └──────────────────────────────────────────┬─────────────────────┘
                                                 │
                                                 ▼
  ┌─────────────────────────────────────────────────────────────────┐
  │                      STEP 2: INSTALL UNREAL ENGINE                   │
  │  1. Visit: https://www.unrealengine.com                          │
  │  2. Download Epic Games Launcher                                  │
  │  3. Install Epic Games Launcher                                  │
  │  4. Open Launcher > Library > + Add Versions                      │
  │  5. Select Unreal Engine 5.3 | Games + Game Development          │
  │  6. Platforms: Windows, iOS, Android                              │
  │  7. Install (takes 30-60 minutes)                                  │
  └──────────────────────────────────────────┬─────────────────────┘
                                                 │
                                                 ▼
  ┌─────────────────────────────────────────────────────────────────┐
  │                     STEP 3: GET PROJECT FILES                       │
  │  Option A: Clone from GitHub                                      │
  │    git clone https://github.com/Zee2401/my-mmorpg-server.git      │
  │    cd my-mmorpg-server/UnrealMMORPG                              │
  │  Option B: Download ZIP                                           │
  │    Download ZIP from GitHub > Extract > Navigate to UnrealMMORPG   │
  │  Verify Files: MusicMMORPG.uproject, Source/, Content/             │
  └──────────────────────────────────────────┬─────────────────────┘
                                                 │
                                                 ▼
  ┌─────────────────────────────────────────────────────────────────┐
  │                     STEP 4: OPEN IN UNREAL EDITOR                   │
  │  1. Launch Epic Games Launcher                                   │
  │  2. Click "Open Project" button                                    │
  │  3. Browse to: my-mmorpg-server/UnrealMMORPG/                     │
  │  4. Select: MusicMMORPG.uproject                                  │
  │  5. Click "Open" | Wait for project to load                         │
  │  6. Accept plugin installations if prompted                        │
  └──────────────────────────────────────────┬─────────────────────┘
                                                 │
                                                 ▼
  ┌─────────────────────────────────────────────────────────────────┐
  │                     STEP 5: COMPILE C++ CODE                         │
  │  1. Editor prompts to rebuild -> Click "Yes"                        │
  │  2. Wait 5-10 minutes for compilation                              │
  │     [=======>      ] 60% Compiling... 12/20 modules                │
  │  3. Check Output Log for errors (Edit > Windows > Output Log)       │
  │  4. Fix errors: Check Build.cs, file locations, UE5 installation    │
  └──────────────────────────────────────────┬─────────────────────┘
                                                 │
                                                 ▼
  ┌─────────────────────────────────────────────────────────────────┐
  │                     STEP 6: PROJECT CONFIGURATION                    │
  │  Edit > Project Settings                                            │
  │  Platforms: Windows (64-bit) + iOS                                   │
  │  Engine: Multiplayer Support, Networking, Audio, Voice Chat ON      │
  │  Maps & Modes: Default Map, Game Mode, Max Players: 100             │
  └──────────────────────────────────────────┬─────────────────────┘
                                                 │
                                                 ▼
  ┌─────────────────────────────────────────────────────────────────┐
  │                     STEP 7: IMPORT ASSETS (Optional)                  │
  │  Content Browser > Import                                            │
  │  Models: Content/Models/ | Textures: Content/Textures/                │
  │  Sounds: Content/Sounds/ | Maps: Content/Maps/                        │
  │  Note: Project works without assets (uses placeholders)             │
  └──────────────────────────────────────────┬─────────────────────┘
                                                 │
                                                 ▼
  ┌─────────────────────────────────────────────────────────────────┐
  │                     STEP 8: TEST IN EDITOR                            │
  │  1. Press Alt+P to start Play-in-Editor (PIE)                       │
  │  2. Test movement: WASD, Mouse, Space (jump)                         │
  │  3. Test audio: Play notes (if assets imported)                      │
  │  4. Test multiplayer: Open 2+ editor instances                      │
  │  5. Fix runtime errors as they appear                              │
  └──────────────────────────────────────────┬─────────────────────┘
                                                 │
                                                 ▼
  ┌─────────────────────────────────────────────────────────────────┐
  │                     STEP 9: BUILD FOR DEPLOYMENT                    │
  │  File > Package Project > Packaging Settings                       │
  │  Select Platform: Windows or iOS                                   │
  │  Packaging Settings: Development/Shipping, Include Prerequisites    │
  │  Packaging Time: 15-30 minutes                                       │
  └──────────────────────────────────────────┬─────────────────────┘
                                                 │
                                                 ▼
  ┌─────────────────────────────────────────────────────────────────┐
  │                     STEP 10: DEPLOY & PLAY                          │
  │  Option A: Local Testing                                            │
  │    - Run packaged executable directly                               │
  │    - Test single player and multiplayer locally                      │
  │  Option B: Dedicated Server                                           │
  │    - Run: MusicMMORPGServer.exe SoundCity                            │
  │    - Clients connect to: IP:7777                                     │
  │  Option C: iOS Deployment                                            │
  │    - Requires Apple Developer Account                                │
  │    - Deploy to TestFlight or App Store                               │
  └─────────────────────────────────────────────────────────────────┘

---

QUICK START (5 Minutes):
  [ ] Install UE 5.3 + VS 2022/Xcode
  [ ] Clone repository
  [ ] Open MusicMMORPG.uproject
  [ ] Click "Yes" to compile
  [ ] Press Alt+P to test

---

TROUBLESHOOTING:

COMPILATION ERRORS:
  Missing module -> Check MusicMMORPG.Build.cs dependencies
  Cannot find file -> Verify file exists in correct Public/Private folder
  GENERATED_BODY missing -> Add GENERATED_BODY() to class declaration

PACKAGING ERRORS:
  Missing assets -> Import required assets or use placeholders
  Platform not supported -> Enable platform in .uproject file

MULTIPLAYER NOT WORKING:
  Check Edit > Plugins > Online Subsystem (Steam/Null enabled)
  Verify firewall allows port 7777
  Forward port 7777 on router if hosting
  Test with multiple editor instances first

AUDIO NOT PLAYING:
  Check AudioSystem component is added to character
  Verify sound files are imported
  Check volume settings in project
```

---

## 8. Unreal Editor Layout

```
UNREAL EDITOR LAYOUT FOR MUSIC MMORPG

┌─────────────────────────────────────────────────────────────────────────────┐
│                            UNREAL EDITOR 5.3 LAYOUT                              │
│                                                                              │
│  ┌─────────────────────────────────────────────────────────────────────┐    │
│  │                           MENU BAR                                      │    │
│  │  File  Edit  View  Window  Help                                              │    │
│  └─────────────────────────────────────────────────────────────────────┘    │
│                                                                              │
│  ┌─────────────────────────┬─────────────────────────┬─────────────────┐  │
│  │                         │                             │                 │  │
│  │    ┌─────────────────┐  │    ┌─────────────────┐    │  ┌───────────┐ │  │
│  │    │   CONTENT       │  │    │   WORLD         │    │  │  PLACE  │ │  │
│  │    │   BROWSER       │  │    │   OUTLINER      │    │  │  ACTORS │ │  │
│  │    │                 │  │    │                 │    │  │         │ │  │
│  │    │  ┌───────────┐ │  │    │  ┌───────────┐  │    │  │  ┌─────┐│ │  │
│  │    │  │ Assets    │ │  │    │  │ Levels   │  │    │  │  │     ││ │  │
│  │    │  │ Blue-    │ │  │    │  │ Actors  │  │    │  │  │     ││ │  │
│  │    │  │ prints   │ │  │    │  │ Lights  │  │    │  │  │     ││ │  │
│  │    │  │ Materials│ │  │    │  │ Cameras │  │    │  │  │     ││ │  │
│  │    │  │ Sounds   │ │  │    │  │        │  │    │  │  │     ││ │  │
│  │    │  │ Maps     │ │  │    │  └─────────┘  │    │  │  └─────┘│ │  │
│  │    │  └───────────┘ │  │    └─────────────────┘    │  │         │ │  │
│  │    └─────────────────┘  │                             │  │         │ │  │
│  │                         │                             │  │         │ │  │
│  └─────────────────────────┴─────────────────────────┴─────────────────┘  │
│                                                                              │
│  ┌─────────────────────────────────────────────────────────────────────┐    │
│  │                         VIEWPORT (Main Work Area)                         │    │
│  │                                                                         │    │
│  │  ┌─────────────────────────────────────────────────────────────┐   │    │
│  │  │                    3D VIEW OF CURRENT LEVEL                    │   │    │
│  │  │                                                                     │   │    │
│  │  │  [Camera can be moved with: WASD + Mouse, or Gamepad]           │   │    │
│  │  │                                                                     │   │    │
│  │  │  Character placed in scene with animation preview                 │   │    │
│  │  │                                                                     │   │    │
│  │  └─────────────────────────────────────────────────────────────┘   │    │
│  │                                                                         │    │
│  │  ┌─────────────────────────────────────────────────────────────┐   │    │
│  │  │                    TOOLBAR (Quick Access)                        │   │    │
│  │  │  [Save] [Compile] [Simulate] [Test] [Build] [Settings]          │   │    │
│  │  └─────────────────────────────────────────────────────────────┘   │    │
│  │                                                                         │    │
│  └─────────────────────────────────────────────────────────────────────┘    │
│                                                                              │
│  ┌─────────────────────────┬─────────────────────────┬─────────────────┐  │
│  │    DETAILS PANEL          │    PROPERTIES PANEL       │  OUTPUT LOG      │  │
│  │    (Selected Actor Info)  │    (Editable Properties)  │  (Debug Info)    │  │
│  │                         │                             │                 │  │
│  │  Name: MusicMMORPGChar   │  Transform:               │  [Messages]      │  │
│  │  Type: Character          │    Location: (X,Y,Z)      │  [Warnings]     │  │
│  │  Components:             │    Rotation: (P,Y,R)      │  [Errors]       │  │
│  │    - Mesh               │    Scale: (1,1,1)         │                 │  │
│  │    - AudioSystem        │  Health: 100             │                 │  │
│  │    - NetworkSystem      │  Energy: 150             │                 │  │
│  │    - InputComponent     │  Level: 5                │                 │  │
│  │                         │  CharacterClass: Pianist  │                 │  │
│  └─────────────────────────┘ └─────────────────────────┘ └─────────────────┘  │
│                                                                              │
└─────────────────────────────────────────────────────────────────────────────┘

---

COMMON EDITOR TABS:

┌─────────────────┬─────────────────┬─────────────────┐
│ Tab              │ Purpose          │ Shortcut        │
├─────────────────┼─────────────────┼─────────────────┤
│ Content Browser  │ Browse assets    │ Ctrl+Space      │
│ World Outliner   │ Scene hierarchy  │ F8              │
│ Details          │ Selected object  │ F4              │
│ Properties       │ Edit properties  │ F4              │
│ Output Log       │ Debug messages   │ Ctrl+Shift+~    │
│ Blueprint Editor │ Edit blueprints  │ Double-click    │
│ Persona          │ Edit skeletons   │ Double-click    │
│ Material Editor  │ Edit materials   │ Double-click    │
│ Animation Editor │ Edit animations  │ Double-click    │
└─────────────────┴─────────────────┴─────────────────┘

---

IMPORTANT EDITOR SHORTCUTS:

┌─────────────────┬─────────────────┬─────────────────┐
│ Action           │ Windows Shortcut │ Mac Shortcut     │
├─────────────────┼─────────────────┼─────────────────┤
│ Play in Editor   │ Alt+P            │ Option+P         │
│ Simulate         │ Alt+S            │ Option+S         │
│ Save All         │ Ctrl+S           │ Cmd+S            │
│ Compile          │ Ctrl+Alt+C       │ Ctrl+Option+C    │
│ Build Lighting   │ Ctrl+Alt+L       │ Ctrl+Option+L    │
│ Undo             │ Ctrl+Z           │ Cmd+Z            │
│ Redo             │ Ctrl+Y           │ Cmd+Y            │
│ Copy             │ Ctrl+C           │ Cmd+C            │
│ Paste            │ Ctrl+V           │ Cmd+V            │
│ Duplicate        │ Ctrl+D           │ Cmd+D            │
│ Delete           │ Delete           │ Delete           │
│ Focus on Selection│ F               │ F                │
│ Zoom to Fit     │ Home             │ Home             │
│ Toggle Grid     │ G                │ G                │
│ Toggle Snap     │ S                │ S                │
└─────────────────┴─────────────────┴─────────────────┘

---

PROJECT-SPECIFIC WORKFLOW:

1. OPENING PROJECT:
   - Launch Epic Games Launcher
   - Open MusicMMORPG.uproject
   - Wait for shaders to compile (first time only)

2. WORKING WITH C++ CODE:
   - Edit .h files in Public/ folders
   - Edit .cpp files in Private/ folders
   - Save files -> Editor prompts to compile
   - Click "Yes" to rebuild

3. WORKING WITH ASSETS:
   - Import to Content/ folder
   - Use Content Browser to organize
   - Right-click assets to edit

4. TESTING:
   - Alt+P for single player test
   - Alt+S for simulate (no player)
   - Use Multiplayer > 2 Players for multiplayer test

5. BUILDING:
   - File > Package Project
   - Select platform (Windows, iOS)
   - Choose directory
   - Wait for packaging to complete
```

---

## 9. File Locations Cheat Sheet

```
FILE LOCATIONS QUICK REFERENCE

PROJECT ROOT: /my-mmorpg-server/UnrealMMORPG/

┌─────────────────────────────────────────────────────────────────────────────┐
│ CONFIGURATION FILES                                                         │
├─────────────────────────────────────────────────────────────────────────────┤
│ File                          │ Location               │ Purpose              │
├─────────────────────────────────────────┼─────────────────────┼──────────────────┤
│ MusicMMORPG.uproject           │ /                    │ Main project file   │
│ MusicMMORPG.uplugin            │ /                    │ Plugin metadata     │
│ README.md                     │ /                    │ Project overview    │
│ INSTRUCTIONS.md               │ /                    │ Setup guide         │
│ VISUAL_GUIDE.md               │ /                    │ This visual guide   │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│ BUILD CONFIGURATION FILES                                                   │
├─────────────────────────────────────────────────────────────────────────────┤
│ File                          │ Location               │ Purpose              │
├─────────────────────────────────────────┼─────────────────────┼──────────────────┤
│ MusicMMORPG.Build.cs           │ Source/MusicMMORPG/   │ Module dependencies  │
│ MusicMMORPGEditor.Target.cs  │ Source/MusicMMORPG/   │ Editor target       │
│ MusicMMORPGServer.Target.cs  │ Source/MusicMMORPG/   │ Server target       │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│ C++ HEADER FILES (.h) - In Public/ folders                              │
├─────────────────────────────────────────────────────────────────────────────┤
│ File                          │ Location               │ Class               │
├─────────────────────────────────────────┼─────────────────────┼──────────────────┤
│ MusicMMORPGCharacter.h         │ Public/Characters/    │ Main character     │
│ MMORPGInputComponent.h         │ Public/Components/   │ Input handler      │
│ MMORPGGameMode.h               │ Public/GameModes/    │ Game mode          │
│ MusicMMORPGGameState.h         │ Public/GameStates/   │ Game state         │
│ MusicMMORPGPlayerController.h  │ Public/Player/        │ Player controller  │
│ MusicMMORPGPlayerState.h      │ Public/Player/        │ Player state       │
│ AudioSystem.h                   │ Public/Systems/Audio/│ Audio system       │
│ NetworkSystem.h                │ Public/Systems/Networking/│ Network system │
│ WorldData.h                    │ Public/Systems/      │ World data         │
│ MMORPGHUD.h                    │ Public/UI/            │ HUD system          │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│ C++ IMPLEMENTATION FILES (.cpp) - In Private/ folders                         │
├─────────────────────────────────────────────────────────────────────────────┤
│ File                          │ Location               │ Class               │
├─────────────────────────────────────────┼─────────────────────┼──────────────────┤
│ MusicMMORPGCharacter.cpp       │ Private/Characters/   │ Main character     │
│ MMORPGInputComponent.cpp       │ Private/Components/  │ Input handler      │
│ MMORPGGameMode.cpp             │ Private/GameModes/   │ Game mode          │
│ MusicMMORPGGameState.cpp       │ Private/GameStates/  │ Game state         │
│ MusicMMORPGPlayerController.cpp│ Private/Player/       │ Player controller  │
│ MusicMMORPGPlayerState.cpp    │ Private/Player/       │ Player state       │
│ AudioSystem.cpp                 │ Private/Systems/Audio/│ Audio system       │
│ NetworkSystem.cpp              │ Private/Systems/Networking/│ Network system │
│ WorldData.cpp                  │ Private/Systems/     │ World data         │
│ MMORPGHUD.cpp                  │ Private/UI/           │ HUD system          │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│ ASSET FOLDERS - In Content/ folder                                       │
├─────────────────────────────────────────────────────────────────────────────┤
│ Folder           │ Contains                  │ File Types         │
├───────────────────┼─────────────────────────┼────────────────────┤
│ Blueprints/       │ Actor/Character blueprints│ *.uasset            │
│ Materials/        │ Surface materials        │ *.umap, *.uasset    │
│ Models/           │ 3D models                │ *.fbx, *.obj       │
│ Textures/         │ Texture maps             │ *.png, *.jpg       │
│ Sounds/           │ Audio files              │ *.wav, *.mp3       │
│   ├── Music/      │ Background music         │ *.wav, *.mp3       │
│   ├── SFX/        │ Sound effects            │ *.wav, *.mp3       │
│   └── Voices/     │ Voice lines              │ *.wav, *.mp3       │
│ Maps/             │ Game levels              │ *.umap             │
│ Animations/       │ Character animations      │ *.uasset           │
│ Particles/        │ VFX effects              │ *.uasset           │
│ Widgets/          │ UI elements              │ *.uasset           │
│ DataAssets/       │ Game data                │ *.uasset           │
└─────────────────────────────────────────────────────────────────────────────┘

---

QUICK NAVIGATION COMMANDS:

From project root (UnrealMMORPG/):

# View project files
ls -la

# View source code
ls -la Source/MusicMMORPG/Public/
ls -la Source/MusicMMORPG/Private/

# View content assets
ls -laR Content/ | head -50

# Find specific file
grep -r "MusicMMORPGCharacter" Source/

# Count lines of code
find Source/ -name "*.h" -o -name "*.cpp" | xargs wc -l

# View git status
git status

---

IMPORTANT FILES SUMMARY:

MUST EDIT FOR CUSTOMIZATION:
  ✓ MusicMMORPG.Build.cs - Add/remove module dependencies
  ✓ MusicMMORPG.uproject - Configure platforms and plugins
  ✓ MusicMMORPGCharacter.h/cpp - Customize character behavior
  ✓ MMORPGGameMode.h/cpp - Customize game rules
  ✓ AudioSystem.h/cpp - Customize audio/sound synthesis

OPTIONAL EDIT FOR EXTENSION:
  ✓ NetworkSystem.h/cpp - Extend networking features
  ✓ WorldData.h/cpp - Add more zones, bosses, items
  ✓ MMORPGHUD.h/cpp - Customize UI
  ✓ MMORPGInputComponent.h/cpp - Modify controls

DO NOT EDIT (Generated):
  ✗ Intermediate/ folder
  ✗ Saved/ folder
  ✗ Binaries/ folder
  ✗ DerivedDataCache/ folder
```

---

## 10. Build Process Visualization

```
BUILD PROCESS FOR WINDOWS

┌─────────────────────────────────────────────────────────────────────────────┐
│                         WINDOWS BUILD PROCESS                                   │
└─────────────────────────────────────────────────────────────────────────────┘

  ┌─────────────────────────────────────────────────────────────────┐
  │  PHASE 1: COMPILATION                                            │
  │  ─────────────────────────────────────────────────────────────   │
  │                                                                  │
  │  Editor detects code changes → Prompts to rebuild                   │
  │                                                                  │
  │  ┌─────────────────────────────────────────────────────────┐    │
  │  │ UnrealBuildTool (UBT)                                      │    │
  │  │                                                           │    │
  │  │  1. Parse MusicMMORPG.Build.cs                               │    │
  │  │  2. Resolve dependencies (Engine, Core, InputCore, etc.)      │    │
  │  │  3. Compile header files (.h) with UHT (Unreal Header Tool)   │    │
  │  │  4. Generate intermediate files                              │    │
  │  │  5. Compile implementation files (.cpp) with MSVC               │    │
  │  │  6. Link all object files into DLL                           │    │
  │  └─────────────────────────────────────────────────────────┘    │
  │                                                                  │
  │  Output:                                                         │
  │    Binaries/Win64/MusicMMORPG.dll                               │
  │    Intermediate/Build/...                                        │
  │                                                                  │
  └─────────────────────────────────────────────────────────────────┘

  ┌─────────────────────────────────────────────────────────────────┐
  │  PHASE 2: SHADER COMPILATION                                     │
  │  ─────────────────────────────────────────────────────────────   │
  │                                                                  │
  │  First time opening project or new materials:                      │
  │                                                                  │
  │  ┌─────────────────────────────────────────────────────────┐    │
  │  │ Shader Compilation                                        │    │
  │  │                                                           │    │
  │  │  [==============>            ] 45% Compiling Shaders         │    │
  │  │  Material: M_Character_Body                                   │    │
  │  │  Material: M_Environment_Floor                                │    │
  │  │                                                           │    │
  │  └─────────────────────────────────────────────────────────┘    │
  │                                                                  │
  │  Output:                                                         │
  │    Saved/Shaders/...                                              │
  │                                                                  │
  └─────────────────────────────────────────────────────────────────┘

  ┌─────────────────────────────────────────────────────────────────┐
  │  PHASE 3: PACKAGING (Optional - For Distribution)                  │
  │  ─────────────────────────────────────────────────────────────   │
  │                                                                  │
  │  File > Package Project > Packaging Settings                       │
  │                                                                  │
  │  ┌─────────────────────────────────────────────────────────┐    │
  │  │ Packaging Process                                         │    │
  │  │                                                           │    │
  │  │  1. Copy project files                                        │    │
  │  │  2. Compile all code (if not already compiled)                │    │
  │  │  3. Compile all shaders                                        │    │
  │  │  4. Cook content (convert assets for distribution)           │    │
  │  │  5. Package into executable                                    │    │
  │  │                                                           │    │
  │  │  [=====>                    ] 30% Packaging...              │    │
  │  │                                                           │    │
  │  └─────────────────────────────────────────────────────────┘    │
  │                                                                  │
  │  Output:                                                         │
  │    MusicMMORPG/ (Folder with all packaged files)                  │
  │      ├── MusicMMORPG.exe                                         │
  │      ├── MusicMMORPG-Win64-Shipping.exe                          │
  │      ├── Content/ (Packaged assets)                              │
  │      └── ... (All required DLLs and configs)                     │
  │                                                                  │
  └─────────────────────────────────────────────────────────────────┘

---

BUILD PROCESS FOR iOS

┌─────────────────────────────────────────────────────────────────────────────┐
│                           iOS BUILD PROCESS                                      │
└─────────────────────────────────────────────────────────────────────────────┘

  PREREQUISITES:
    ✓ Mac computer with Xcode
    ✓ iOS SDK installed
    ✓ Apple Developer Account ($99/year)
    ✓ Provisioning Profile for your device
    ✓ Certificate for code signing

  ┌─────────────────────────────────────────────────────────────────┐
  │  STEP 1: CONFIGURE FOR iOS                                        │
  │                                                                  │
  │  Edit > Project Settings > iOS                                     │
  │                                                                  │
  │  ✓ Enable iOS platform                                            │
  │  ✓ Set Bundle Identifier: com.yourcompany.musicmmorpg            │
  │  ✓ Set Version: 1.0.0                                             │
  │  ✓ Set Display Name: Music MMORPG                                 │
  │  ✓ Set Orientation: Portrait/Landscape (both recommended)           │
  │  ✓ Configure iOS permissions (Microphone for voice chat)           │
  │                                                                  │
  └─────────────────────────────────────────────────────────────────┘

  ┌─────────────────────────────────────────────────────────────────┐
  │  STEP 2: SETUP CODE SIGNING                                      │
  │                                                                  │
  │  Project Settings > iOS > Packaging                                │
  │                                                                  │
  │  ✓ Signing Certificate: Your iOS Development/Distribution cert    │
  │  ✓ Provisioning Profile: Your app's provisioning profile           │
  │  ✓ Team ID: Your Apple Developer Team ID                          │
  │                                                                  │
  └─────────────────────────────────────────────────────────────────┘

  ┌─────────────────────────────────────────────────────────────────┐
  │  STEP 3: PACKAGE FOR iOS                                          │
  │                                                                  │
  │  File > Package Project > iOS > Package iOS                         │
  │                                                                  │
  │  ┌─────────────────────────────────────────────────────────┐    │
  │  │ iOS Packaging                                              │    │
  │  │                                                           │    │
  │  │  1. Compile code for iOS                                     │    │
  │  │  2. Compile shaders for iOS                                   │    │
  │  │  3. Convert assets for iOS (texture compression, etc.)         │    │
  │  │  4. Build Xcode project                                      │    │
  │  │                                                           │    │
  │  │  [===>                    ] 25% Building for iOS...           │    │
  │  │                                                           │    │
  │  └─────────────────────────────────────────────────────────┘    │
  │                                                                  │
  │  This takes 20-45 minutes depending on hardware                   │
  │                                                                  │
  └─────────────────────────────────────────────────────────────────┘

  ┌─────────────────────────────────────────────────────────────────┐
  │  STEP 4: OPEN IN XCODE                                          │
  │                                                                  │
  │  After packaging completes:                                        │
  │                                                                  │
  │  1. Xcode opens automatically                                     │
  │  2. Or manually open: MusicMMORPG.xcodeproj                        │
  │  3. Select your device or simulator                                │
  │  4. Click Build (Cmd+B) or Run (Cmd+R)                             │
  │                                                                  │
  └─────────────────────────────────────────────────────────────────┘

  ┌─────────────────────────────────────────────────────────────────┐
  │  STEP 5: DEPLOY TO DEVICE                                        │
  │                                                                  │
  │  Option A: Direct to Device                                        │
  │    1. Connect iPhone/iPad via USB                                  │
  │    2. Select device in Xcode                                       │
  │    3. Click Run (Cmd+R)                                             │
  │    4. App installs and launches on device                           │
  │                                                                  │
  │  Option B: TestFlight (For beta testing)                            │
  │    1. Archive in Xcode (Product > Archive)                          │
  │    2. Distribute via TestFlight                                    │
  │    3. Invite testers via email                                     │
  │    4. Testers download from TestFlight app                          │
  │                                                                  │
  │  Option C: App Store (For production)                              │
  │    1. Archive in Xcode                                              │
  │    2. Validate archive                                              │
  │    3. Upload to App Store Connect                                    │
  │    4. Submit for review                                             │
  │    5. Wait for Apple approval (1-3 days)                            │
  │    6. Release to App Store                                          │
  │                                                                  │
  └─────────────────────────────────────────────────────────────────┘

---

BUILD TIME ESTIMATES:

┌─────────────────────────────────────────────────────────────────┐
│ Task                    │ Windows (PC) │ iOS (Mac)       │
├────────────────────────┼──────────────┼─────────────────┤
│ Code Compilation         │ 5-10 min     │ 10-15 min       │
│ Shader Compilation       │ 2-5 min      │ 5-10 min        │
│ Full Packaging           │ 15-30 min    │ 20-45 min       │
│ iOS Xcode Build          │ N/A          │ 5-10 min        │
│ iOS Archive & Upload      │ N/A          │ 10-20 min       │
└─────────────────────────────────────────────────────────────────┘

---

BUILD TROUBLESHOOTING:

Windows Build Errors:
  ✓ Missing VS 2022 -> Install Visual Studio 2022 with C++ support
  ✓ Missing Windows SDK -> Install latest Windows 10/11 SDK
  ✓ Out of memory -> Close other applications, use 64-bit editor

iOS Build Errors:
  ✓ Missing Xcode -> Install Xcode from App Store
  ✓ Missing iOS SDK -> Install via Xcode > Preferences > Locations
  ✓ Code signing error -> Verify provisioning profile and certificate
  ✓ Device not trusted -> Trust computer on iOS device
  ✓ Not enough space -> Free up 20GB+ on Mac

Common Errors:
  ✓ "Module not found" -> Check MusicMMORPG.Build.cs dependencies
  ✓ "File not found" -> Verify file path is correct (case-sensitive on Mac)
  ✓ "Linker error" -> Check all .cpp files have matching .h files
```

---

## 11. iOS Build Process

```
iOS SPECIFIC INFORMATION

This section covers iOS-specific setup, configuration, and deployment.

---

iOS PROJECT CONFIGURATION

Required Files:
  ✓ MusicMMORPG.uproject (auto-generates .xcodeproj)
  ✓ Info.plist (auto-generated, but can be customized)
  ✓ Entitlements file (for app capabilities)

Configuration in MusicMMORPG.uproject:
  "IOS"
  {
    "bEnableMultiProcess": false,
    "bEnableRetina": true,
    "bEnableHighResSnapshot": true,
    "bEnable3DTouch": true,
    "bEnableMotionBlur": false,
    "bEnableScreenShots": true,
    "bEnableVideoRecording": true,
    "bEnableGameKit": true,
    "bEnableInAppPurchase": false,
    "bEnableLocationServices": false
  }

---

iOS INPUT CONFIGURATION

Touch Controls:
  ✓ Virtual Joystick (Left side of screen)
  ✓ Virtual Buttons (Right side of screen)
  ✓ Tap to interact
  ✓ Swipe for camera movement

Gamepad Support:
  ✓ MFi (Made for iPhone) controllers
  ✓ Xbox Wireless Controller (iOS 14+)
  ✓ PlayStation DualSense (iOS 14.5+)

Input Mapping (MusicMMORPGInputComponent):
  
  Movement:
    - Touch: Virtual Joystick
    - Gamepad: Left Stick
    - Keyboard: WASD
  
  Look/Camera:
    - Touch: Swipe on right side
    - Gamepad: Right Stick
    - Keyboard: Mouse
  
  Actions:
    - Jump: Touch Button / Gamepad A / Keyboard Space
    - Attack: Touch Button / Gamepad X / Keyboard LMB
    - Ability 1: Touch Button / Gamepad Y / Keyboard Q
    - Ability 2: Touch Button / Gamepad B / Keyboard E
    - Ability 3: Touch Button / Gamepad RB / Keyboard R

---

iOS PERFORMANCE OPTIMIZATIONS

Graphics Settings:
  ✓ Mobile Rendering: Enabled
  ✓ LOD (Level of Detail): Aggressive
  ✓ Texture Streaming: Enabled
  ✓ Occlusion Culling: Enabled
  ✓ Shadow Quality: Medium
  ✓ Effects Quality: Medium

Memory Optimization:
  ✓ Texture Compression: ASTC (iOS)
  ✓ Sound Compression: ADPCM
  ✓ Mesh Optimization: Enabled
  ✓ Lightmap Resolution: Low

Performance Targets:
  ✓ iPhone: 60 FPS on recent devices
  ✓ iPad: 60 FPS on all devices
  ✓ Memory Usage: < 2GB
  ✓ Load Times: < 10 seconds

---

iOS MULTIPLAYER CONFIGURATION

Online Subsystem: IOS
  ✓ Game Center integration for matchmaking
  ✓ Native iOS networking APIs
  ✓ Bluetooth/WiFi Direct for local multiplayer

Configuration:
  Edit > Plugins > Online > Online Subsystem IOS
  ✓ Enable plugin
  ✓ Configure Game Center ID

Multiplayer Features:
  ✓ Game Center matchmaking
  ✓ Invite friends via Game Center
  ✓ Local WiFi multiplayer (up to 4 players)
  ✓ Online multiplayer via Game Center

---

iOS AUDIO CONFIGURATION

Audio Settings:
  ✓ Sample Rate: 44.1kHz
  ✓ Channels: Stereo
  ✓ Compression: ADPCM
  ✓ Voice Chat: Enabled (requires microphone permission)

Audio Optimization:
  ✓ Max Voices: 32 (reduced for mobile)
  ✓ Reverb: Disabled (for performance)
  ✓ EQ: Simplified
  ✓ Streaming: Enabled for background music

---

iOS STORAGE & SAVES

Save System:
  ✓ Local saves: Documents/MusicMMORPG/Saves/
  ✓ Cloud saves: iCloud (optional)
  ✓ Auto-save: Every 5 minutes
  ✓ Manual save: Press menu button

Storage Requirements:
  ✓ App Size: ~500MB (with all assets)
  ✓ Save Files: ~10MB per character
  ✓ Cache: ~200MB

---

iOS UI CONSIDERATIONS

Screen Sizes:
  ✓ iPhone: 375x812 to 428x926 (portrait)
  ✓ iPad: 768x1024 to 1024x1366 (portrait/landscape)
  ✓ Adaptive UI: Scale with screen size

Touch Targets:
  ✓ Minimum: 44x44 points (Apple recommendation)
  ✓ Recommended: 60x60 points
  ✓ Spacing: 20 points between buttons

UI Scaling:
  ✓ Safe Area: Respect notches and home indicators
  ✓ Dynamic Type: Support for accessibility
  ✓ Orientation: Support both portrait and landscape

---

iOS TESTING DEVICES

Recommended Test Devices:
  ✓ iPhone 15 Pro Max (Latest hardware)
  ✓ iPhone 13 (Mid-range)
  ✓ iPhone SE (Low-end, 4" screen)
  ✓ iPad Pro 12.9" (Large screen)
  ✓ iPad Mini (Small screen)

Simulator Testing:
  ✓ iPhone 15 Pro Max Simulator
  ✓ iPhone 13 Simulator
  ✓ iPad Pro (12.9-inch) Simulator
  ✓ iPad Mini Simulator

Note: Simulators don't support:
  ✗ Game Center
  ✗ Microphone
  ✗ Accelerometer
  ✗ Real performance characteristics

---

iOS DEPLOYMENT CHECKLIST

Before Submitting to App Store:
  [ ] App works on all supported devices
  [ ] All iOS permissions are properly requested
  [ ] Privacy policy is accessible from app
  [ ] Support contact information is provided
  [ ] App icons (180x180, 167x167, 152x152, etc.)
  [ ] Screenshots for App Store (6.5", 5.5", 12.9")
  [ ] App preview video (optional)
  [ ] Description and keywords
  [ ] Age rating questionnaire completed
  [ ] Export compliance verified
  [ ] TestFlight testing completed

---

iOS PERMISSIONS REQUIRED

┌─────────────────────────────────────────────────────────────────┐
│ Permission              │ Purpose                          │ Info.plist Key  │
├────────────────────────┼──────────────────────────────────┼────────────────┤
│ Microphone              │ Voice chat                       │ NSMicrophoneUsageDescription │
│ Game Center             │ Multiplayer matchmaking          │ N/A (auto)      │
│ Local Network           │ Local WiFi multiplayer           │ NSLocalNetworkUsageDescription │
│ Notifications           │ Game notifications               │ N/A (optional)  │
└─────────────────────────────────────────────────────────────────┘

Info.plist Additions:
  <key>NSMicrophoneUsageDescription</key>
  <string>Required for voice chat with other players</string>
  
  <key>NSLocalNetworkUsageDescription</key>
  <string>Required for local multiplayer over WiFi</string>

---

iOS PERFORMANCE TESTING

Test Scenarios:
  ✓ Open World with 50 players
  ✓ Boss Fight with 10 players
  ✓ PvP Arena with 10 players
  ✓ All abilities active simultaneously
  ✓ Audio system with all sounds playing

Performance Metrics to Monitor:
  ✓ FPS: Should stay above 30, target 60
  ✓ Memory: Should stay below 2GB
  ✓ CPU Usage: Should stay below 80%
  ✓ GPU Usage: Should stay below 80%
  ✓ Battery Impact: Should not drain excessively
  ✓ Load Times: Should be under 10 seconds

Tools:
  ✓ Xcode Instruments
  ✓ UE5 Stats commands
  ✓ Metal System Trace
```

---

## 12. Controls Reference

```
CONTROLS REFERENCE FOR ALL PLATFORMS

This game supports multiple input methods:
- Keyboard + Mouse (PC)
- Gamepad (PC and iOS)
- Touch (iOS)

---

PC CONTROLS (Keyboard + Mouse)

┌─────────────────────────────────────────────────────────────────────────────┐
│ MOVEMENT                                                                         │
├─────────────────────────────────────────────────────────────────────────────┤
│ Action              │ Default Key    │ Alternative Key  │ Gamepad Button   │
├────────────────────┼────────────────┼──────────────────┼─────────────────┤
│ Move Forward        │ W              │ Up Arrow         │ Left Stick Up    │
│ Move Backward       │ S              │ Down Arrow       │ Left Stick Down  │
│ Move Left           │ A              │ Left Arrow       │ Left Stick Left  │
│ Move Right          │ D              │ Right Arrow      │ Left Stick Right │
│ Sprint              │ Left Shift    │ Caps Lock        │ Left Stick Click │
│ Crouch              │ Left Ctrl     │ N/A              │ Right Stick Click│
│ Jump                │ Space         │ N/A              │ A / South Button │
│ Walk (Toggle)        │ Alt           │ N/A              │ N/A              │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│ CAMERA                                                                           │
├─────────────────────────────────────────────────────────────────────────────┤
│ Action              │ Default Key    │ Alternative Key  │ Gamepad Button   │
├────────────────────┼────────────────┼──────────────────┼─────────────────┤
│ Look Up/Down        │ Mouse Y-axis   │ N/A              │ Right Stick Y    │
│ Look Left/Right     │ Mouse X-axis   │ N/A              │ Right Stick X    │
│ Toggle Camera      │ Tab           │ N/A              │ Back/Select     │
│ Zoom In            │ Mouse Wheel Up│ N/A              │ N/A              │
│ Zoom Out           │ Mouse Wheel Down│ N/A            │ N/A              │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│ COMBAT                                                                           │
├─────────────────────────────────────────────────────────────────────────────┤
│ Action              │ Default Key    │ Alternative Key  │ Gamepad Button   │
├────────────────────┼────────────────┼──────────────────┼─────────────────┤
│ Basic Attack        │ Left Mouse     │ N/A              │ X / Square       │
│ Ability 1           │ Q              │ N/A              │ Y / Triangle     │
│ Ability 2           │ E              │ N/A              │ B / Circle       │
│ Ability 3           │ R              │ N/A              │ RB / R1          │
│ Ultimate Ability   │ F              │ N/A              │ LB / L1          │
│ Use Item           │ Middle Mouse   │ N/A              │ D-Pad Up         │
│ Interact           │ F              │ E                │ A / South Button │
│ Target Next        │ Mouse Wheel Up│ N/A              │ RB / R1          │
│ Target Previous    │ Mouse Wheel Down│ N/A            │ LB / L1          │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│ MUSIC SYSTEM                                                                     │
├─────────────────────────────────────────────────────────────────────────────┤
│ Action              │ Default Key    │ Alternative Key  │ Gamepad Button   │
├────────────────────┼────────────────┼──────────────────┼─────────────────┤
│ Play Note C         │ 1              │ N/A              │ N/A              │
│ Play Note D         │ 2              │ N/A              │ N/A              │
│ Play Note E         │ 3              │ N/A              │ N/A              │
│ Play Note F         │ 4              │ N/A              │ N/A              │
│ Play Note G         │ 5              │ N/A              │ N/A              │
│ Play Note A         │ 6              │ N/A              │ N/A              │
│ Play Note B         │ 7              │ N/A              │ N/A              │
│ Play Chord          │ C              │ N/A              │ N/A              │
│ Play Scale          │ V              │ N/A              │ N/A              │
│ Record Melody       │ T              │ N/A              │ N/A              │
│ Play Recorded        │ Y              │ N/A              │ N/A              │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│ UI & MENU                                                                       │
├─────────────────────────────────────────────────────────────────────────────┤
│ Action              │ Default Key    │ Alternative Key  │ Gamepad Button   │
├────────────────────┼────────────────┼──────────────────┼─────────────────┤
│ Open Menu           │ Escape         │ N/A              │ Start/Menu       │
│ Open Inventory      │ I              │ N/A              │ D-Pad Right      │
│ Open Character      │ C              │ N/A              │ D-Pad Left       │
│ Open Map           │ M              │ N/A              │ Touchpad/Select  │
│ Open Chat           │ Enter          │ N/A              │ D-Pad Down       │
│ Open Quest Log      │ J              │ N/A              │ D-Pad Up         │
│ Toggle Fullscreen   │ F11            │ N/A              │ N/A              │
│ Screenshot          │ F12            │ N/A              │ Share Button     │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│ CHEAT CODES (Development Only)                                                 │
├─────────────────────────────────────────────────────────────────────────────┤
│ Action              │ Default Key    │ Effect                          │
├────────────────────┼────────────────┼──────────────────────────────┤
│ God Mode            │ F1             │ Invincible                      │
│ Infinite Energy     │ F2             │ No energy cost                 │
│ Level Up            │ F3             │ Gain 1 level                   │
│ Full Health         │ F4             │ Restore all health            │
│ Full Energy         │ F5             │ Restore all energy            │
│ Add Gold            │ F6             │ +1000 gold                     │
│ Add XP              │ F7             │ +10000 XP                      │
│ Teleport to Nexus   │ F8             │ Return to hub                  │
│ Spawn Enemy         │ F9             │ Spawn test enemy               │
│ Kill All Enemies    │ F10            │ Clear all enemies              │
│ Toggle Debug        │ ` (tilde)      │ Show debug info                │
│ Stats               │ ` stat fps     │ Show FPS                       │
│ Network Stats       │ ` stat net     │ Show network stats             │
└─────────────────────────────────────────────────────────────────────────────┘

---

iOS TOUCH CONTROLS

┌─────────────────────────────────────────────────────────────────────────────┐
│ TOUCH CONTROL LAYOUT (Portrait Mode)                                              │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                              │
│  ┌─────────────────────────────────────────────────────────────┐    │
│  │                                                             │    │
│  │     ┌─────────────┐     ┌─────────────────────────────┐    │    │
│  │     │             │     │                             │    │    │
│  │     │   JOYSTICK  │     │   ABILITY BUTTONS            │    │    │
│  │     │   (Left)    │     │                             │    │    │
│  │     │             │     │  ┌─────┐ ┌─────┐ ┌─────┐    │    │    │
│  │     │  ┌─────┐   │     │  │ Q   │ │ E   │ │ R   │    │    │    │
│  │     │  │     │   │     │  │     │ │     │ │     │    │    │    │
│  │     │  │     │   │     │  └─────┘ └─────┘ └─────┘    │    │    │
│  │     │  └─────┘   │     │                             │    │    │
│  │     │             │     │  ┌───────────────────────┐  │    │    │
│  │     └─────────────┘     │  │    ATTACK BUTTON      │  │    │    │
│  │                       │  │    (Large Circle)       │  │    │    │
│  │                       │  │    Touch to attack       │  │    │    │
│  │                       │  └───────────────────────┘  │    │    │
│  │                       │                             │    │    │
│  │     ┌─────────────┐     │  ┌─────────────────────┐  │    │    │
│  │     │ JUMP        │     │  │   MINIMAP            │  │    │    │
│  │     │ (Button)    │     │  │   (Top-right)        │  │    │    │
│  │     └─────────────┘     │  └─────────────────────┘  │    │    │
│  │                       │                             │    │    │
│  │     ┌─────────────┐     │  ┌─────────────────────┐  │    │    │
│  │     │ ITEMS       │     │  │   CHAT BUTTON        │  │    │    │
│  │     │ (Button)    │     │  │   (Bottom-right)     │  │    │    │
│  │     └─────────────┘     │  └─────────────────────┘  │    │    │
│  │                       │                             │    │    │
│  │     ┌─────────────┐                                           │    │
│  │     │ MENU         │                                           │    │
│  │     │ (Button)    │                                           │    │
│  │     └─────────────┘                                           │    │
│  │                                                             │    │
│  └─────────────────────────────────────────────────────────────┘    │
│                                                                              │
└─────────────────────────────────────────────────────────────────────────────┘

---

TOUCH GESTURES:

┌─────────────────────────────────────────────────────────────────────────────┐
│ Gesture               │ Action                          │ Notes                    │
├────────────────────────┼─────────────────────────────┼──────────────────────┤
│ Single Tap             │ Select/Interact                 │ On interactive objects  │
│ Double Tap            │ Double-click action             │ Context sensitive       │
│ Tap and Hold          │ Open context menu               │ 1 second hold           │
│ Swipe Left/Right       │ Camera rotation                 │ On right side of screen │
│ Swipe Up/Down          │ Camera pitch                    │ On right side of screen │
│ Pinch In/Out           │ Zoom camera                     │ Two-finger gesture       │
│ Two-finger Tap         │ Toggle UI element               │ Cycle through elements │
│ Long Press (0.5s)      │ Show tooltip                    │ On buttons              │
└─────────────────────────────────────────────────────────────────────────────┘

---

CONTROL CUSTOMIZATION

Keyboard Remapping:
  1. Press Escape to open menu
  2. Select Options > Controls
  3. Choose action to rebind
  4. Press new key
  5. Confirm or cancel

Gamepad Remapping:
  1. Open Options > Controls
  2. Select Gamepad tab
  3. Choose action to rebind
  4. Press new button
  5. Confirm or cancel

Touch Control Customization:
  1. Open Options > Touch Controls
  2. Enable/disable buttons
  3. Adjust button size (Small, Medium, Large)
  4. Adjust button transparency
  5. Change button positions
  6. Reset to default layout

---

SAVED CONTROL PROFILES

The game saves up to 3 control profiles:
  1. Default (Cannot be deleted)
  2. Custom Profile 1
  3. Custom Profile 2

To switch profiles:
  Options > Controls > Load Profile

To save current bindings:
  Options > Controls > Save Profile

To reset to defaults:
  Options > Controls > Reset to Defaults

---

PLATFORM-SPECIFIC NOTES

Windows:
  ✓ Full keyboard + mouse support
  ✓ Xbox, PlayStation, and Nintendo Switch Pro controllers
  ✓ Custom key bindings
  ✓ High precision mouse look

Mac:
  ✓ Full keyboard + mouse support
  ✓ Limited gamepad support (check compatibility)
  ✓ Touch Bar support (on supported MacBooks)

Linux:
  ✓ Full keyboard + mouse support
  ✓ Gamepad support varies by distribution
  ✓ Steam Controller recommended

iOS:
  ✓ Touch controls optimized
  ✓ MFi gamepad support
  ✓ Xbox/PS5 controller support (iOS 14+)
  ✓ Adaptive touch controls for different screen sizes
  ✓ Haptic feedback on supported devices

---

ACCESSIBILITY CONTROLS

┌─────────────────────────────────────────────────────────────────────────────┐
│ Feature                  │ Description                      │ Default State   │
├──────────────────────────┼──────────────────────────────┼─────────────────┤
│ Toggle Sprint            │ Sprint without holding button    │ Disabled        │
│ Toggle Crouch            │ Crouch without holding button    │ Disabled        │
│ Hold to Sprint           │ Sprint only while button held   │ Enabled         │
│ Hold to Crouch           │ Crouch only while button held  │ Enabled         │
│ Invert Look Y-axis       │ Flip mouse/gamepad Y-axis       │ Disabled        │
│ Invert Look X-axis       │ Flip mouse/gamepad X-axis       │ Disabled        │
│ Mouse Sensitivity        │ Adjust look speed                │ 50%             │
│ Gamepad Deadzone         │ Adjust stick deadzone           │ 10%             │
│ Vibration Intensity      │ Adjust controller vibration     │ 100%            │
│ Subtitles               │ Show dialogue text               │ Enabled         │
│ Colorblind Mode          │ Colorblind-friendly UI          │ Disabled        │
│ Text Size                │ UI text scaling                 │ Medium          │
└─────────────────────────────────────────────────────────────────────────────┘

Accessibility Options Location:
  Options > Accessibility

---

CONTROL TIPS FOR NEW PLAYERS

Movement Tips:
  ✓ Sprint to move faster, but be aware of stamina drain
  ✓ Crouch to sneak past enemies or avoid detection
  ✓ Jump to avoid ground-based attacks
  ✓ Use walk mode (Alt) for precise movement

Combat Tips:
  ✓ Basic attack is your primary damage source
  ✓ Use abilities strategically (watch cooldowns)
  ✓ Combine abilities for combo effects
  ✓ Time your attacks with the rhythm for bonus damage

Music System Tips:
  ✓ Play notes in sequence to build up combos
  ✓ Use chords for AoE attacks
  ✓ Record and replay melodies for complex attacks
  ✓ Each class has unique musical abilities

UI Tips:
  ✓ Use Tab to cycle through targets
  ✓ Middle mouse to use items quickly
  ✓ M to open map and mark locations
  ✓ Enter to chat with other players

Multiplayer Tips:
  ✓ Coordinate abilities with teammates
  ✓ Use voice chat for better communication
  ✓ Watch your position relative to other players
  ✓ Share resources and items with party members
```

---

## Summary

This Visual Guide provides comprehensive visual representations of:

1. **Project Structure** - Complete directory tree with all files and folders
2. **Class Hierarchy** - Detailed UML-style class diagrams showing inheritance and relationships
3. **Game World** - Visual map of all zones with connections and features
4. **Boss Encounters** - Detailed breakdown of each boss fight with phases and mechanics
5. **Setup Process** - Step-by-step flowchart for getting started with Unreal Engine
6. **Character Classes** - Complete overview of all 5 playable classes with stats and abilities
7. **Network Architecture** - Technical overview of multiplayer networking system
8. **Editor Layout** - Visual guide to Unreal Editor interface
9. **File Locations** - Quick reference for finding any file in the project
10. **Build Process** - Detailed visualization of compilation and packaging
11. **iOS Build** - Complete iOS-specific setup and deployment guide
12. **Controls Reference** - All controls for PC and iOS platforms

---

## Quick Navigation

- [Back to README.md](./README.md) - Project overview and quick start
- [INSTRUCTIONS.md](./INSTRUCTIONS.md) - Detailed step-by-step setup guide
- [Official Unreal Engine Documentation](https://docs.unrealengine.com/5.3/)
- [Unreal Engine Forums](https://forums.unrealengine.com/)
- [GitHub Repository](https://github.com/Zee2401/my-mmorpg-server)

---

*Last Updated: October 2024*
*Unreal Engine Version: 5.3*
*Project: Music MMORPG*

---

**Need More Help?**

- Check [INSTRUCTIONS.md](./INSTRUCTIONS.md) for detailed setup instructions
- Check [README.md](./README.md) for project overview
- Visit [Unreal Engine Documentation](https://docs.unrealengine.com/) for engine-specific questions
- Join the [Unreal Engine Discord](https://discord.gg/unrealengine) for community support

---

*Happy Game Development! 🎮🎵*
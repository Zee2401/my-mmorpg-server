# Music MMORPG - Unreal Engine Version

A **cross-platform multiplayer online game** built with **Unreal Engine 5.3** for **PC and iOS mobile**. This is the Unreal Engine version of the Music MMORPG game, featuring rhythmic battles, sound synthesis, musical guilds, epic instruments, and boss conquests.

---

## 🎮 Key Features

### 🎵 **Rhythm Battle Combat System**
- Real-time falling note lanes with hit accuracy timing (Perfect/Great/Miss)
- Combo multipliers and boss damage calculations
- Special Overdrive moves for maximum impact
- Multiple difficulty levels for all skill levels

### 🎸 **Hero Classes & Attributes**
- **Bard**: Support class with healing and buffing abilities
- **Beatmaker**: Rhythm specialist with powerful combo attacks
- **Vocalist**: High-damage single-target specialist
- **Guitarist**: Balanced class with good attack and defense
- **DJ**: Crowd control expert with area-of-effect abilities
- Upgradable attributes: Rhythm Attack, Pitch Accuracy, Special Power, Harmony Defense

### 🌍 **World Map & Boss Dungeons**
- **Sound City**: The bustling capital with training grounds and shops
- **Bass Caverns**: Dark underground caverns with powerful bass frequencies
- **Synth Citadel**: Futuristic fortress with electronic music themes
- **PvP Arena**: Battleground for player-versus-player combat

### 🎹 **Jam Studio & Beatmaker**
- Interactive drum pads for rhythm practice
- Synthesizer piano scale for creating melodies
- Earn bonus Rhythm XP through practice

### 👥 **Guilds & Bands**
- Form or join musical bands
- Global leaderboards for competitive play
- Cooperative Band Concert Raids
- Guild chat and social features

### 🛍️ **Gear Shop & Inventory**
- **Instruments**: Guitars, keytars, microphones, turntables, synthesizers
- **Amplifiers**: Thunder amps, distortion pedals, and more
- **Consumables**: Energy elixirs, health potions
- **Currency**: Harmony Gems for purchases
- Local persistence via SaveSystem

### 🔊 **Sound Synthesizer Audio Engine**
- Built-in Web Audio API synthesizer (via Unreal Engine's Synthesis module)
- Pitch tones (C4–C5) with different instruments
- Drum beats (Kick, Snare, Hi-Hat, Sub-Bass)
- Hit/miss sound effects
- Special ability power-up melodies
- Real-time audio processing

### 🎯 **Cross-Platform Responsive Engine**
- **PC**: Full keyboard support (A, S, D, F or Arrow keys for rhythm hits, Spacebar for Solo Shred)
- **iOS Mobile**: Touch controls optimized for mobile devices
- **Responsive UI**: Adapts to different screen sizes and orientations

---

## 🚀 Quick Start & Development

### Prerequisites
- **Unreal Engine 5.3** or higher
- **Visual Studio 2022** (for Windows development)
- **Xcode** (for iOS development)
- **Git** (for version control)

### 1. Clone Repository
```bash
# Clone the repository
git clone https://github.com/Zee2401/my-mmorpg-server.git
cd my-mmorpg-server/UnrealMMORPG

# Switch to the Unreal Engine branch (if available)
git checkout unreal-engine
```

### 2. Generate Project Files
```bash
# Right-click on MusicMMORPG.uproject and select "Generate Visual Studio project files"
# Or use the command line:
<UnrealEngineRoot>/Engine/Build/BatchFiles/RunUAT.bat BuildCookRun -project="MusicMMORPG.uproject" -noP4 -nocompile -nocompileeditor -nocleanstage -utf8output -platform=Win64 -clientconfig=Development -serverconfig=Development -buildmachine -nocompile
```

### 3. Open Project in Unreal Editor
```bash
# Double-click MusicMMORPG.uproject
# Or use the command line:
<UnrealEngineRoot>/Engine/Build/Binary/Win64/UE5Editor.exe MusicMMORPG.uproject
```

### 4. Compile the Project
- In Unreal Editor, click **Compile** button or press **Ctrl+Alt+F11**
- Wait for compilation to complete

### 5. Run the Game
- Click **Play** button in Unreal Editor
- Or press **Alt+P** to play in editor
- For standalone game, create a **New Level** and set it as the default map

---

## 📁 Project Structure

```
/UnrealMMORPG
├── /Source
│   └── /MusicMMORPG
│       ├── /Public
│       │   ├── /Characters              # Character classes and components
│       │   ├── /Components              # Game components (Input, Inventory, etc.)
│       │   ├── /GameModes               # Game mode classes
│       │   ├── /GameStates              # Game state classes
│       │   ├── /Interfaces              # Game interfaces
│       │   ├── /Player                  # Player controller and state
│       │   ├── /Subsystems              # Game subsystems
│       │   ├── /Systems                # Game systems (Networking, Audio, World, Items)
│       │   │   ├── /Networking          # Network system
│       │   │   ├── /Audio               # Audio system with synthesizer
│       │   │   ├── /World               # World data and zones
│       │   │   └── /Items               # Item system
│       │   ├── /UI                      # UI components (HUD, Widgets)
│       │   └── /Utils                   # Utility classes
│       └── /Private                    # Private implementation files
│
├── /Content
│   ├── /Blueprints              # Blueprint classes
│   ├── /Materials               # Materials for 3D models
│   ├── /Models                  # 3D models and meshes
│   ├── /Textures                # Textures and images
│   ├── /Sounds                  # Sound files and music
│   ├── /Maps                    # Game levels and maps
│   ├── /Animations              # Character animations
│   ├── /Particles               # Particle effects
│   ├── /Widgets                 # UI widgets
│   └── /DataAssets             # Data assets (JSON, CSV, etc.)
│
├── MusicMMORPG.uproject        # Unreal Engine project file
├── MusicMMORPG.uplugin         # Plugin file
├── MusicMMORPG.Build.cs        # Build configuration
├── MusicMMORPGEditor.Target.cs
└── MusicMMORPGServer.Target.cs
```

---

## 🎮 Controls

### PC Controls
| Action | Key |
|--------|-----|
| Move Forward | W / Up Arrow |
| Move Backward | S / Down Arrow |
| Move Left | A / Left Arrow |
| Move Right | D / Right Arrow |
| Look Up | Mouse Up |
| Look Down | Mouse Down |
| Look Left | Mouse Left |
| Look Right | Mouse Right |
| Jump | Space |
| Run | Left Shift |
| Sprint | Left Ctrl |
| Crouch | C |
| Attack | Left Mouse Button |
| Ability 1 | 1 |
| Ability 2 | 2 |
| Ability 3 | 3 |
| Ability 4 | 4 |
| Interact | E |
| Inventory | Tab |
| Map | M |
| Menu | Escape |
| Chat | Enter |
| Social | O |
| Quest | J |
| Shop | K |

### iOS Touch Controls
| Action | Control |
|--------|---------|
| Move | Virtual Joystick (Left) |
| Look | Virtual Joystick (Right) |
| Jump | Jump Button |
| Attack | Attack Button |
| Abilities | Ability Buttons (1-4) |
| Interact | Interact Button |
| Inventory | Inventory Button |
| Map | Map Button |
| Menu | Menu Button |
| Chat | Chat Button |

---

## 🌐 Multiplayer Setup

### Running a Dedicated Server
1. Open the project in Unreal Editor
2. Go to **File > Package Project > Packaging Settings**
3. Set **Packaging Mode** to **Multi-process Standalone Game**
4. Check **Include Prerequisites**
5. Click **Package**
6. Run the server executable with command line arguments:
```bash
MusicMMORPGServer.exe -NullRHI -nosplash -nopause -log -server -port=7777 -maxplayers=100
```

### Running a Listen Server
1. Open the project in Unreal Editor
2. Click **Play** button
3. Select **Multiplayer > Listen Server**
4. Set **Number of Players** to 2 or more
5. Click **Play**

### Connecting as a Client
1. Run the game client
2. Open the console with **~** key
3. Connect to server:
```
open <ServerIP>:<Port>
```
Or use the in-game server browser (if implemented)

---

## 🎵 Audio System

The game features a **Sound Synthesizer Audio Engine** that uses Unreal Engine's **Synthesis** module for real-time audio generation:

### Note Frequencies
| Note | Frequency (Hz) |
|------|---------------|
| C4 | 261.63 |
| C#4 | 277.18 |
| D4 | 293.66 |
| D#4 | 311.13 |
| E4 | 329.63 |
| F4 | 349.23 |
| F#4 | 369.99 |
| G4 | 392.00 |
| G#4 | 415.30 |
| A4 | 440.00 |
| A#4 | 466.16 |
| B4 | 493.88 |
| C5 | 523.25 |

### Drum Types
- Kick
- Snare
- Hi-Hat
- Sub-Bass
- Crash
- Ride
- Tom

### Sound Effects
- Hit
- Miss
- Perfect
- Great
- Ability
- Level Up
- Damage
- Heal
- Death
- Victory
- Defeat

---

## 🎯 Character Classes

### Bard
- **Role**: Support
- **Specialty**: Healing and buffing allies
- **Starting Weapon**: Standard Guitar
- **Unique Ability**: Harmony Shield (Reduces damage to allies)

### Beatmaker
- **Role**: Rhythm Specialist
- **Specialty**: Combo attacks and rhythm-based damage
- **Starting Weapon**: Drum Pads
- **Unique Ability**: Beat Drop (Area damage based on combo)

### Vocalist
- **Role**: Damage Dealer
- **Specialty**: High single-target damage
- **Starting Weapon**: Standard Microphone
- **Unique Ability**: Power Note (High damage single attack)

### Guitarist
- **Role**: Balanced
- **Specialty**: Good attack and defense balance
- **Starting Weapon**: Electric Guitar
- **Unique Ability**: Solo Shred (Rapid attacks)

### DJ
- **Role**: Crowd Control
- **Specialty**: Area-of-effect abilities
- **Starting Weapon**: Turntables
- **Unique Ability**: Bass Drop (Stuns enemies in radius)

---

## 🌍 Zones

### Sound City
- **Level Range**: 1-20
- **Description**: The bustling capital of the Music MMORPG world
- **Boss**: Static Spectre
- **Features**: Training grounds, shops, guild hall

### Bass Caverns
- **Level Range**: 15-30
- **Description**: Dark underground caverns filled with powerful bass frequencies
- **Boss**: Subwoofer Titan
- **Features**: Mining area, hidden treasures

### Synth Citadel
- **Level Range**: 25-50
- **Description**: A futuristic fortress of synthesis
- **Boss**: Silence Overlord
- **Features**: Technology, electronic music themes

### PvP Arena
- **Level Range**: 1-100
- **Description**: Battleground for player-versus-player combat
- **Boss**: None
- **Features**: Dueling, team battles, tournaments

---

## 🏆 Bosses

### Static Spectre
- **Zone**: Sound City
- **Level**: 10
- **Health**: 2000
- **Attack Power**: 30
- **Defense Power**: 15
- **Experience Reward**: 500
- **Harmony Gems Reward**: 50
- **Loot**: Guitar, Microphone

### Subwoofer Titan
- **Zone**: Bass Caverns
- **Level**: 25
- **Health**: 5000
- **Attack Power**: 60
- **Defense Power**: 30
- **Experience Reward**: 1500
- **Harmony Gems Reward**: 150
- **Loot**: Bass Guitar, Subwoofer

### Silence Overlord
- **Zone**: Synth Citadel
- **Level**: 40
- **Health**: 10000
- **Attack Power**: 100
- **Defense Power**: 50
- **Experience Reward**: 3000
- **Harmony Gems Reward**: 300
- **Loot**: Synthesizer, Master Keytar

---

## 🛠️ Building for iOS

### Prerequisites
- **Xcode** (latest version)
- **iOS Device** or **Simulator**
- **Apple Developer Account** (for device testing)

### Build Steps
1. Open the project in Unreal Editor
2. Go to **File > Package Project > Packaging Settings**
3. Set **Target Platform** to **IOS**
4. Set **Packaging Mode** to **Development** or **Shipping**
5. Check **Include Prerequisites**
6. Click **Package**
7. Open the generated Xcode project
8. Select your device or simulator
9. Build and run

### iOS-Specific Settings
- **Minimum iOS Version**: 12.0
- **Metal API**: Enabled
- **Multiplayer**: Supported
- **Voice Chat**: Supported (via OnlineSubsystemIOS)

---

## 📦 Building for PC (Windows/Mac/Linux)

### Windows
1. Open the project in Unreal Editor
2. Go to **File > Package Project > Packaging Settings**
3. Set **Target Platform** to **Win64**
4. Set **Packaging Mode** to **Development** or **Shipping**
5. Check **Include Prerequisites**
6. Click **Package**

### Mac
1. Open the project in Unreal Editor
2. Go to **File > Package Project > Packaging Settings**
3. Set **Target Platform** to **Mac**
4. Set **Packaging Mode** to **Development** or **Shipping**
5. Check **Include Prerequisites**
6. Click **Package**

### Linux
1. Open the project in Unreal Editor
2. Go to **File > Package Project > Packaging Settings**
3. Set **Target Platform** to **Linux**
4. Set **Packaging Mode** to **Development** or **Shipping**
5. Check **Include Prerequisites**
6. Click **Package**

---

## 🔧 Configuration

### Engine Configuration
Edit `Config/DefaultEngine.ini`:
```ini
[/Script/Engine.Engine]
+ActiveGameNameRedirects=(OldGameName="/Script/MusicMMORPG", NewGameName="/Script/MusicMMORPG")
+NetDriverDefinitions=(DefName="GameNetDriver", DriverClassName="OnlineSubsystemSteam.SteamNetDriver", DriverClassNameFallback="OnlineSubsystemNull.NullNetDriver")

[/Script/Engine.GameEngine]
!NetMode=NM_Standalone
MaxPlayers=100
MaxSpectators=10
MaxInternetClientRate=10000
MaxClientRate=10000
```

### Game Configuration
Edit `Config/DefaultGame.ini`:
```ini
[/Script/MusicMMORPG.MusicMMORPGGameMode]
GameName="Music MMORPG"
GameVersion="1.0.0"
MaxPlayers=100
AutoSaveInterval=300.0
bAutoSaveEnabled=true
bFriendlyFire=false
bPvPEnabled=true
bAllowSpectating=false
DefaultZone=SoundCity

[/Script/MusicMMORPG.MusicMMORPGCharacter]
WalkSpeed=200.0
RunSpeed=400.0
SprintSpeed=600.0
JumpVelocity=600.0
RotationRate=1.0
```

---

## 🐛 Troubleshooting

### Common Issues

#### Compilation Errors
1. **Missing Modules**: Ensure all required modules are in `MusicMMORPG.Build.cs`
2. **Header Issues**: Check include paths and forward declarations
3. **UE5 Compatibility**: Ensure code is compatible with Unreal Engine 5.3

#### Networking Issues
1. **Connection Failed**: Check firewall settings and port forwarding
2. **Multiplayer Not Working**: Ensure `bEnableMultiplayer=true` in project settings
3. **Replication Not Working**: Check RPC declarations and replication settings

#### Audio Issues
1. **No Sound**: Check audio device settings in Unreal Editor
2. **Synthesizer Not Working**: Ensure `WITH_SYNTHESIS=1` in build configuration
3. **Voice Chat Not Working**: Check OnlineSubsystem settings

#### iOS Build Issues
1. **Code Signing**: Ensure proper provisioning profile and certificate
2. **Metal Not Supported**: Check device compatibility
3. **Multiplayer Not Working on iOS**: Ensure Game Center is enabled

---

## 📄 License

MIT License. Created for cross-platform mobile and desktop gaming.

---

## 🙏 Credits

- **Engine**: Unreal Engine 5.3 (Epic Games)
- **Development**: Music MMORPG Team
- **Repository**: [Zee2401/my-mmorpg-server](https://github.com/Zee2401/my-mmorpg-server)

---

## 📞 Support

- **GitHub Issues**: [https://github.com/Zee2401/my-mmorpg-server/issues](https://github.com/Zee2401/my-mmorpg-server/issues)
- **Documentation**: [https://github.com/Zee2401/my-mmorpg-server/wiki](https://github.com/Zee2401/my-mmorpg-server/wiki)
- **Discussions**: [https://github.com/Zee2401/my-mmorpg-server/discussions](https://github.com/Zee2401/my-mmorpg-server/discussions)

---

## 🎉 Contributing

We welcome contributions! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

**Enjoy the Music MMORPG adventure! 🎵🎸🎹**

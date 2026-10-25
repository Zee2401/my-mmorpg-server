# Music MMORPG - Unity Multiplayer Online Game

A **cross-platform MMORPG** built with **Unity** and **Mirror Networking** that works on **PC (Windows/Mac/Linux)** and **iOS Mobile**. This project extends the existing `my-mmorpg-server` React Native game with a full Unity implementation featuring real-time multiplayer, rhythm-based combat, and persistent world mechanics.

---

## \ud83c\udf1f **Key Features**

### \ud83d\udc80 **Cross-Platform Support**
- **PC**: Windows, macOS, Linux (Standalone)
- **Mobile**: iOS (iPhone & iPad)
- **Input**: Full keyboard/mouse + touch controls
- **Resolution**: Adaptive UI for all screen sizes

### \ud83d\udd25 **Multiplayer Networking**
- **Mirror Networking**: High-performance multiplayer framework
- **Dedicated Server**: Host your own game server
- **Peer-to-Peer**: Direct connection for small groups
- **Matchmaking**: Built-in server browser
- **100+ Players**: Supports large concurrent player counts

### \ud83c\udfb5 **Music MMORPG Gameplay**
- **5 Character Classes**: Bard, Beatmaker, Vocalist, Guitarist, DJ
- **Rhythm Combat**: Real-time note-hitting battle system
- **World Zones**: Sound City, Bass Caverns, Synth Citadel
- **Boss Battles**: Epic music-themed boss fights
- **Gear System**: Instruments, amps, mics, pedals
- **Inventory**: 50-slot inventory with stacking
- **Currency**: Harmony Gems for purchases

### \ud83d\ude80 **Technical Features**
- **Entity Component System**: Clean, modular architecture
- **Scene Management**: Seamless zone transitions
- **Save System**: Auto-save every 5 minutes
- **Audio Engine**: Web Audio API synthesizer ported to Unity
- **Input System**: New Input System for cross-platform controls
- **UI Framework**: Responsive, mobile-friendly interface

---

## \ud83d\udcf1 **Quick Start**

### **Prerequisites**
- Unity 2022.3 LTS or later
- .NET 4.x scripting backend
- iOS Build Support (for mobile)
- Mirror Networking package
- TextMeshPro package

### **1. Setup Unity Project**

```bash
# Clone or open this project in Unity Hub
cd UnityMMORPG

# Open in Unity Editor
# Unity Hub -> Add Project -> Select UnityMMORPG folder
```

### **2. Install Required Packages**

In Unity Editor:
1. Open **Window > Package Manager**
2. Install these packages:
   - **Mirror** (com.unity.multiplayer-mirror) v2.10.0+
   - **Input System** (com.unity.inputsystem) v1.7.0+
   - **TextMeshPro** (com.unity.textmeshpro) v3.0.6+

### **3. Configure Project Settings**

#### **For PC Build:**
1. **File > Build Settings**
2. Select **PC, Mac & Linux Standalone**
3. Click **Add Open Scenes**
4. Set **Target Platform**: Windows/Mac/Linux
5. Click **Build**

#### **For iOS Build:**
1. **File > Build Settings**
2. Select **iOS**
3. Click **Switch Platform**
4. Set **Target SDK**: iOS 12.0+
5. Configure **Player Settings**:
   - **Bundle Identifier**: com.yourcompany.musicmmorpg
   - **App Name**: Music MMORPG
   - **Minimum iOS Version**: 12.0
   - **Requires Full Screen**: ON
   - **Allowed Orientations**: Portrait & Landscape
6. Click **Build**

---

## \ud83d\udcf2 **Project Structure**

```
UnityMMORPG/
├── Assets/
│   ├── Scripts/
│   │   ├── Networking/
│   │   │   └── NetworkManagerMMO.cs      # Custom network manager
│   │   ├── Player/
│   │   │   └── PlayerController.cs       # Player movement & combat
│   │   ├── World/
│   │   │   ├── ZoneManager.cs             # Zone/world management
│   │   │   └── BossController.cs          # Boss AI & combat
│   │   ├── Managers/
│   │   │   ├── GameManager.cs             # Main game controller
│   │   │   ├── InputManager.cs            # Cross-platform input
│   │   │   ├── AudioManager.cs            # Audio system
│   │   │   └── UIManager.cs               # UI controller
│   │   ├── Items/
│   │   │   └── InventoryManager.cs        # Inventory system
│   │   └── Systems/
│   │       └── DamageTextManager.cs       # Floating damage text
│   ├── Prefabs/
│   │   ├── Player.prefab
│   │   ├── Bosses/
│   │   └── Items/
│   ├── Scenes/
│   │   ├── MainMenu.unity
│   │   ├── WorldScene.unity
│   │   ├── SoundCity.unity
│   │   ├── BassCaverns.unity
│   │   └── SynthCitadel.unity
│   └── Resources/
│       ├── Audio/
│       ├── Materials/
│       └── Models/
├── ProjectSettings/
│   ├── AudioManager.asset
│   ├── InputSystem.asset
│   └── ProjectVersion.txt
└── Packages/
    └── manifest.json
```

---

## \ud83d\udcc4 **Configuration**

### **Network Settings**

Edit `Assets/Scripts/Networking/NetworkManagerMMO.cs`:

```csharp
public class NetworkManagerMMO : NetworkManager
{
    public string defaultWorldScene = "WorldScene";
    public int maxPlayers = 100;
    public float saveInterval = 300f; // 5 minutes
}
```

### **Input Settings**

Edit `Assets/Scripts/Managers/InputManager.cs`:

```csharp
// PC Controls
controls.Player.Move = "WASD";
controls.Player.Jump = "Space";
controls.Player.Attack = "Mouse/LeftClick";
controls.Player.Run = "LeftShift";

// Mobile Touch Controls
joystickRadius = 100f;
deadzone = 0.1f;
```

### **Audio Settings**

Edit `Assets/Scripts/Managers/AudioManager.cs`:

```csharp
public AudioMixer masterMixer;
public AudioMixerGroup musicGroup;
public AudioMixerGroup sfxGroup;
public AudioMixerGroup voiceGroup;
```

---

## \ud83d\ude80 **Running the Game**

### **Development Mode (Editor)**

1. Open **MainMenu** scene
2. Click **Play** in Unity Editor
3. Use the menu to:
   - Start a new game (single player)
   - Host a server
   - Join a server

### **Testing Multiplayer**

#### **Option 1: Local Host**
1. Build and run the game
2. Click **Host Game**
3. Open another instance
4. Click **Join Game** and enter `localhost`

#### **Option 2: Dedicated Server**
1. Build the dedicated server:
   ```bash
   # In Unity: File > Build Settings > Build
   # Select "Dedicated Server" platform
   ```
2. Run the server executable
3. Connect clients to the server IP

### **Testing on iOS**

1. Build for iOS (see **Quick Start** above)
2. Deploy to iOS device via Xcode
3. Test touch controls:
   - Left side: Virtual joystick
   - Right side: Action buttons
   - Tap to interact

---

## \ud83d\udc83 **Input Controls**

### **PC Controls (Keyboard & Mouse)**

| Action | Key |
|--------|-----|
| Move | WASD / Arrow Keys |
| Look | Mouse |
| Jump | Space |
| Attack | Left Mouse Button |
| Run | Left Shift |
| Interact | E |
| Inventory | Tab |
| Map | M |
| Menu | Escape |
| Abilities | 1, 2, 3, 4 |

### **Mobile Controls (Touch)**

| Action | Control |
|--------|---------|
| Move | Virtual Joystick (Left) |
| Jump | Jump Button |
| Attack | Attack Button |
| Run | Run Button |
| Interact | Tap on object |
| Inventory | Inventory Button |
| Menu | Menu Button |

---

## \ud83c\udf96 **Game Systems**

### **Character System**

5 playable classes with unique abilities:

| Class | Strengths | Weaknesses |
|-------|-----------|------------|
| Bard | High Pitch, Harmony | Low Power |
| Beatmaker | High Rhythm, Power | Low Harmony |
| Vocalist | High Power, Harmony | Low Rhythm |
| Guitarist | High Rhythm, Pitch | Low Energy |
| DJ | Balanced Stats | No Specialization |

### **Combat System**

- **Rhythm-Based**: Hit notes in time for maximum damage
- **Combo System**: Chain hits for multipliers
- **Special Abilities**: Unique class abilities
- **Boss Mechanics**: Special patterns and phases

### **Progression System**

- **XP & Levels**: Gain XP from quests and battles
- **Stats Growth**: Stats increase on level up
- **Gear Upgrades**: Equip better instruments
- **Skills**: Unlock new abilities

### **Inventory System**

- **50 Slots**: Store items
- **Stacking**: Stackable items up to max
- **Equipment**: 4 equipment slots
- **Currency**: Harmony Gems

---

## \ud83d\udcbb **Networking**

### **Mirror Networking**

This project uses **Mirror Networking** for multiplayer:

- **Transport**: Built-in Unity transport (UNET replacement)
- **Serialization**: Automatic sync of GameObjects and variables
- **RPC**: Remote Procedure Calls for game events
- **Commands**: Client-to-server requests
- **ClientRpc**: Server-to-client broadcasts

### **Network Architecture**

```
Client 1 <--> Server <--> Client 2
           ^
           |
Client 3 <--+
```

### **Custom Network Messages**

```csharp
// Player creation
public struct CreatePlayerMessage : NetworkMessage
{
    public string playerName;
    public CharacterClass characterClass;
}

// Player movement
public struct PlayerMovementMessage : NetworkMessage
{
    public Vector3 direction;
    public float velocity;
}
```

---

## \ud83d\udca1 **Audio System**

### **Synthesizer**

The game includes a software synthesizer for generating music notes:

```csharp
// Play a note
audioManager.PlaySynthNote("C4", "sine", 0.2f, 0.5f);

// Play a drum sample
audioManager.PlayDrumSample("Kick", 0.5f);

// Play hit sound
audioManager.PlayHitSound(true); // Perfect hit
```

### **Audio Mixer**

4 audio groups for volume control:
- **Music**: Background and battle music
- **SFX**: Sound effects
- **Voice**: Character voices
- **Ambient**: Environmental sounds

---

## \ud83d\udee0 **Build Settings**

### **PC Build Settings**

```
Resolution: 1920x1080
Fullscreen: Windowed/Fullscreen
Target Platform: Windows x86_64
Scripting Backend: IL2CPP
Api Compatibility: .NET 4.x
```

### **iOS Build Settings**

```
Target SDK: iOS 12.0+
Architecture: ARM64
Scripting Backend: IL2CPP
Api Compatibility: .NET 4.x
Requires Full Screen: YES
Allowed Orientations: Portrait, Landscape Left, Landscape Right
Target Minimum iOS Version: 12.0
Bundle Identifier: com.yourcompany.musicmmorpg
```

### **iOS Permissions**

Add to `Info.plist`:

```xml
<!-- Network access -->
<key>NSAppTransportSecurity</key>
<dict>
    <key>NSAllowsArbitraryLoads</key>
    <true/>
</dict>

<!-- Microphone access (if using voice chat) -->
<key>NSMicrophoneUsageDescription</key>
<string>Required for voice chat in multiplayer</string>

<!-- Game Center (optional) -->
<key>GameKitSupported</key>
<true/>
```

---

## \ud83d\udc82 **Performance Optimization**

### **For PC**

- **Graphics**: High quality settings
- **Physics**: Full precision
- **Audio**: Maximum channels
- **Network**: High update rate

### **For Mobile (iOS)**

- **Graphics**: Medium quality
- **Physics**: Medium precision
- **Audio**: Limited channels (16)
- **Network**: Optimized update rate
- **LOD**: Level of Detail for distant objects
- **Occlusion Culling**: Enabled
- **Batching**: Static & dynamic batching

### **Memory Management**

- **Object Pooling**: For frequently created/destroyed objects
- **Texture Compression**: ASTC for iOS, DXT for PC
- **Audio Compression**: VORBIS for all platforms
- **Asset Bundles**: For downloadable content

---

## \ud83d\udc81 **Troubleshooting**

### **Common Issues**

#### **1. Mirror Networking Not Found**
```
Solution: Install Mirror via Package Manager
Window > Package Manager > + > Add package from git URL
https://github.com/vis2k/Mirror.git?path=/Assets/Mirror
```

#### **2. Input System Not Working**
```
Solution: Enable Input System in Project Settings
Edit > Project Settings > Player > Active Input Handling = Both
```

#### **3. iOS Build Fails**
```
Solution: Check iOS requirements
- Xcode installed
- iOS SDK 12.0+ installed
- Proper provisioning profile
- Valid bundle identifier
```

#### **4. Multiplayer Connection Fails**
```
Solution: Check network settings
- Firewall allows Unity traffic
- Port forwarding if hosting
- Correct IP address for joining
- Same Mirror version on all clients
```

#### **5. Audio Not Playing**
```
Solution: Check audio settings
- Audio Listener in scene
- Audio Sources not muted
- Correct Audio Mixer groups
- Volume levels > 0
```

---

## \ud83d\udc65 **Contributing**

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## \ud83d\udcc4 **License**

MIT License - Feel free to use, modify, and distribute this project.

---

## \ud83d\udc93 **Acknowledgments**

- **Unity Technologies**: Game engine
- **Mirror Networking**: Multiplayer framework
- **TextMeshPro**: Rich text rendering
- **Input System**: Cross-platform input
- **All contributors**: Thank you for your support!

---

## \ud83d\ude80 **Deployment**

### **Steam (PC)**

1. Create Steamworks account
2. Configure Steamworks SDK
3. Set up Steam app ID
4. Build with Steamworks integration
5. Upload to Steam partner site

### **App Store (iOS)**

1. Create Apple Developer account
2. Configure App Store Connect
3. Set up bundle identifier
4. Create certificates and provisioning profiles
5. Archive and upload via Xcode
6. Submit for review

### **Self-Hosting**

1. Build dedicated server
2. Configure port forwarding
3. Set up domain/DNS (optional)
4. Run server executable
5. Share IP with players

---

## \ud83d\udc89 **Future Features**

- [ ] **Android Support**
- [ ] **Steam Integration**
- [ ] **Discord Rich Presence**
- [ ] **Achievements System**
- [ ] **Leaderboards**
- [ ] **Guild System**
- [ ] **Crafting**
- [ ] **Housing**
- [ ] **Pets & Mounts**
- [ ] **Mini-Games**
- [ ] **Custom Music Import**
- [ ] **Mod Support**

---

## \ud83d\udcf0 **Support**

- **GitHub Issues**: Report bugs and request features
- **Discord**: Join our community server
- **Email**: support@musicmmorpg.com
- **Documentation**: Full API documentation available

---

**Built with \ud83d\udc99 by Unity & Mirror Networking**

*Music MMORPG - Where Rhythm Meets Adventure*

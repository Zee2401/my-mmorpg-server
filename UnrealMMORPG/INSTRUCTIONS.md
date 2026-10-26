# Music MMORPG - Detailed Unreal Engine Setup Instructions

## 📋 Complete Step-by-Step Guide to Initiate the Project

This guide provides **detailed instructions** for setting up, compiling, and running the Music MMORPG project in Unreal Engine 5.3.

---

## 📌 Table of Contents

1. [Prerequisites](#-prerequisites)
2. [Unreal Engine Installation](#-unreal-engine-installation)
3. [Project Setup](#-project-setup)
4. [Opening in Unreal Editor](#-opening-in-unreal-editor)
5. [Compiling the Project](#-compiling-the-project)
6. [Creating Content](#-creating-content)
7. [Running the Game](#-running-the-game)
8. [Multiplayer Setup](#-multiplayer-setup)
9. [Building for iOS](#-building-for-ios)
10. [Building for PC](#-building-for-pc)
11. [Configuration](#-configuration)
12. [Troubleshooting](#-troubleshooting)

---

## 1️⃣ Prerequisites

### Required Software

| Software | Version | Purpose | Download Link |
|----------|---------|---------|---------------|
| **Unreal Engine 5.3** | 5.3.0 or higher | Game Engine | [https://www.unrealengine.com](https://www.unrealengine.com) |
| **Visual Studio** | 2022 Community or higher | C++ Compiler (Windows) | [https://visualstudio.microsoft.com](https://visualstudio.microsoft.com) |
| **Xcode** | Latest | iOS Development (Mac) | Mac App Store |
| **Git** | Latest | Version Control | [https://git-scm.com](https://git-scm.com) |
| **.NET SDK** | 6.0 or higher | Required for UE5 | [https://dotnet.microsoft.com](https://dotnet.microsoft.com) |

### System Requirements

#### Windows
- **OS**: Windows 10/11 (64-bit)
- **CPU**: Quad-core Intel or AMD, 2.5 GHz or faster
- **RAM**: 16 GB (32 GB recommended)
- **GPU**: DirectX 12 compatible (NVIDIA GTX 1080 or AMD RX 5700 or better)
- **Storage**: 50 GB SSD free space
- **Visual Studio**: 2022 with "Game Development with C++" workload

#### Mac
- **OS**: macOS 12.0 (Monterey) or higher
- **CPU**: Apple Silicon (M1/M2) or Intel Quad-core
- **RAM**: 16 GB (32 GB recommended)
- **GPU**: Metal-compatible graphics card
- **Storage**: 50 GB SSD free space
- **Xcode**: Latest version

#### iOS Development (Mac Required)
- **Xcode**: Latest version
- **Apple Developer Account**: Required for device testing
- **iOS Device**: iPhone or iPad running iOS 12.0 or higher

---

## 2️⃣ Unreal Engine Installation

### Windows Installation

#### Step 1: Download Epic Games Launcher
1. Go to [https://www.unrealengine.com](https://www.unrealengine.com)
2. Click **Download** in the top-right corner
3. Download and install the **Epic Games Launcher**
4. Launch the Epic Games Launcher

#### Step 2: Install Unreal Engine 5.3
1. Open Epic Games Launcher
2. Click on **Unreal Engine** tab on the left
3. Click **Library** at the top
4. Click the **+ Add Versions** button
5. Select **Unreal Engine 5.3** from the list
6. Click **Install**
7. Choose installation location (default: `C:\Program Files\Epic Games\UE_5.3`)
8. Wait for installation to complete (this may take 30-60 minutes)

#### Step 3: Install Required Components
1. In Epic Games Launcher, go to **Settings** (gear icon)
2. Under **Engine Versions**, ensure 5.3 is installed
3. Install these additional components:
   - **Android Support** (optional, for Android builds)
   - **iOS Support** (optional, for iOS builds)
   - **Linux Support** (optional, for Linux builds)

### Mac Installation

#### Step 1: Download Unreal Engine
1. Go to [https://www.unrealengine.com](https://www.unrealengine.com)
2. Download **Unreal Engine 5.3 for Mac**
3. Open the downloaded `.dmg` file
4. Drag **Unreal Engine** to your Applications folder

#### Step 2: Install Xcode
1. Open the **Mac App Store**
2. Search for **Xcode**
3. Download and install Xcode (this may take 30-60 minutes)
4. Open Xcode and accept the license agreement

#### Step 3: Install Command Line Tools
1. Open **Terminal**
2. Run: `xcode-select --install`
3. Accept the license agreement

---

## 3️⃣ Project Setup

### Step 1: Clone the Repository

```bash
# Navigate to your projects directory
cd ~/Projects  # or any directory you prefer

# Clone the repository
git clone https://github.com/Zee2401/my-mmorpg-server.git

# Navigate to the UnrealMMORPG directory
cd my-mmorpg-server/UnrealMMORPG
```

### Step 2: Verify Project Structure

Your project directory should look like this:

```
UnrealMMORPG/
├── Content/
│   ├── Blueprints/
│   ├── Materials/
│   ├── Models/
│   ├── Textures/
│   ├── Sounds/
│   ├── Maps/
│   ├── Animations/
│   ├── Particles/
│   ├── Widgets/
│   └── DataAssets/
├── Source/
│   └── MusicMMORPG/
│       ├── Public/
│       │   ├── Characters/
│       │   ├── Components/
│       │   ├── GameModes/
│       │   ├── GameStates/
│       │   ├── Player/
│       │   ├── Systems/
│       │   ├── UI/
│       │   └── Utils/
│       └── Private/
│           ├── Characters/
│           ├── Components/
│           ├── GameModes/
│           ├── GameStates/
│           ├── Player/
│           ├── Systems/
│           ├── UI/
│           └── Utils/
├── MusicMMORPG.uproject
├── MusicMMORPG.uplugin
└── README.md
```

### Step 3: Generate Project Files

#### Windows (Visual Studio)

**Method 1: Using File Explorer**
1. Navigate to `my-mmorpg-server/UnrealMMORPG`
2. **Right-click** on `MusicMMORPG.uproject`
3. Select **Generate Visual Studio project files**
4. Wait for the process to complete (you'll see a progress bar)

**Method 2: Using Command Line**
```bash
# Navigate to your Unreal Engine installation
cd "C:\Program Files\Epic Games\UE_5.3\Engine\Build\BatchFiles"

# Generate project files
RunUAT.bat BuildCookRun -project="C:\Path\To\my-mmorpg-server\UnrealMMORPG\MusicMMORPG.uproject" -noP4 -nocompile -nocompileeditor -nocleanstage -utf8output -platform=Win64 -clientconfig=Development -serverconfig=Development
```

#### Mac (Xcode)

**Method 1: Using Finder**
1. Navigate to `my-mmorpg-server/UnrealMMORPG`
2. **Right-click** on `MusicMMORPG.uproject`
3. Select **Generate Xcode project files**

**Method 2: Using Command Line**
```bash
# Navigate to your Unreal Engine installation
cd /Users/Shared/EpicGames/UE_5.3/Engine/Build/BatchFiles

# Generate project files
./RunUAT.sh BuildCookRun -project="/Path/To/my-mmorpg-server/UnrealMMORPG/MusicMMORPG.uproject" -noP4 -nocompile -nocompileeditor -nocleanstage -utf8output -platform=Mac -clientconfig=Development -serverconfig=Development
```

### Step 4: Verify Generated Files

After generating project files, you should see:
- **Windows**: `MusicMMORPG.sln` (Visual Studio Solution)
- **Mac**: `MusicMMORPG.xcodeproj` (Xcode Project)
- **Both**: `Binaries/`, `DerivedDataCache/`, `Intermediate/`, `Saved/` directories

---

## 4️⃣ Opening in Unreal Editor

### Step 1: Launch Unreal Editor

**Windows:**
1. Open **Epic Games Launcher**
2. Click on **Unreal Engine** tab
3. Click **Library**
4. Find **UE 5.3** and click **Launch**
5. Wait for Unreal Editor to open

**Mac:**
1. Open **Finder**
2. Navigate to **Applications**
3. Double-click **Unreal Engine**
4. Wait for Unreal Editor to open

### Step 2: Open the Project

**Method 1: From Unreal Editor**
1. In Unreal Editor, click **Open Project**
2. Navigate to `my-mmorpg-server/UnrealMMORPG`
3. Select `MusicMMORPG.uproject`
4. Click **Open**

**Method 2: Direct Double-Click**
1. Navigate to `my-mmorpg-server/UnrealMMORPG` in File Explorer/Finder
2. Double-click `MusicMMORPG.uproject`
3. Wait for the project to load

### Step 3: First-Time Setup

When opening the project for the first time:

1. **Project Conversion Dialog** may appear (if upgrading from older UE version)
   - Click **Convert** if prompted
   - Wait for conversion to complete

2. **Missing Modules Dialog** may appear
   - Click **Yes** to rebuild missing modules

3. **Plugin Loading**
   - The project will load required plugins (OnlineSubsystem, Voice, etc.)
   - Wait for all plugins to load

4. **Content Browser** will open
   - You should see your Content directory structure

### Step 4: Verify Project Settings

1. Go to **Edit > Project Settings**
2. Verify these settings:
   - **Engine > General Settings**:
     - Project ID: `MusicMMORPG`
     - Project Name: `Music MMORPG`
   - **Engine > Input**:
     - Default Input Component Class: `MMORPGInputComponent`
   - **Engine > Networking**:
     - Enable Multiplayer: **True**
     - Enable Networking: **True**
     - Enable Online: **True**
   - **Game > Game Mode**:
     - Default GameMode: `MusicMMORPGGameMode`
     - Default Pawn Class: `MusicMMORPGCharacter`
     - Default Player Controller Class: `MusicMMORPGPlayerController`
     - Default HUD Class: `MMORPGHUD`

---

## 5️⃣ Compiling the Project

### Step 1: Compile in Unreal Editor

1. In Unreal Editor, look at the top toolbar
2. Click the **Compile** button (hammer icon) or press **Ctrl+Alt+F11**
3. Wait for compilation to start
4. **Compilation Progress**:
   - You'll see a progress bar at the bottom
   - Compilation may take 5-30 minutes depending on your hardware
   - Watch for any errors in the **Output Log** window

### Step 2: Check for Errors

If compilation fails:

1. Open **Window > Developer Tools > Output Log**
2. Look for **red error messages**
3. Common errors and fixes:

| Error | Solution |
|-------|----------|
| `Module MusicMMORPG not found` | Generate project files again |
| `Cannot find header file` | Check include paths in .h files |
| `Linker errors` | Rebuild project, check dependencies |
| `Missing DLL` | Reinstall Unreal Engine |

### Step 3: Hot Reload (Optional)

If you make changes to C++ code:

1. Save your changes in Visual Studio/Xcode
2. In Unreal Editor, click **Compile** again
3. Or press **Ctrl+Alt+F11** for hot reload
4. If hot reload fails, do a full recompile

### Step 4: Verify Compilation Success

After successful compilation:
- ✅ No errors in Output Log
- ✅ **Compile Successful** message appears
- ✅ All C++ classes appear in Content Browser under **C++ Classes**
- ✅ You can create Blueprints based on C++ classes

---

## 6️⃣ Creating Content

### Step 1: Create a New Level (Map)

1. Go to **File > New Level**
2. Select **Empty Level** or **Third Person Template**
3. Click **Create**
4. Save the level:
   - **File > Save As**
   - Navigate to `Content/Maps/`
   - Name: `SoundCity` (or any zone name)
   - Click **Save**

### Step 2: Set as Default Map

1. Go to **Edit > Project Settings**
2. Navigate to **Game > Maps & Modes**
3. Under **Default Maps**, set:
   - Editor Startup Map: `/Game/Maps/SoundCity`
   - Game Default Map: `/Game/Maps/SoundCity`
4. Click **Save**

### Step 3: Create Player Start

1. In the **Modes** panel, search for **Player Start**
2. Drag **Player Start** into your level
3. Position it at the spawn location (e.g., 0, 0, 0)
4. In **Details** panel, set:
   - Player Start Tag: `SoundCity` (or your zone name)

### Step 4: Add Lights

1. In **Modes** panel, search for **Directional Light**
2. Drag it into the level
3. Rotate it to shine down on the level
4. In **Details** panel, adjust:
   - Intensity: 1.0
   - Color: White (255, 255, 255)

### Step 5: Add Skybox (Optional)

1. In **Modes** panel, search for **Sky Sphere**
2. Drag it into the level
3. In **Details** panel, create a new **Material** for the sky

---

## 7️⃣ Running the Game

### Step 1: Single Player Mode

1. In Unreal Editor, click the **Play** button (green triangle) or press **Alt+P**
2. **Play Options**:
   - **Selected Viewport**: Plays in the current viewport
   - **New Editor Window**: Opens a new window
   - **Standalone Game**: Runs as a standalone executable
   - **Simulate**: Simulates gameplay in editor

3. **Controls**:
   - **WASD**: Move
   - **Mouse**: Look
   - **Space**: Jump
   - **Left Shift**: Run
   - **Left Ctrl**: Sprint
   - **C**: Crouch
   - **Left Mouse**: Attack
   - **1-4**: Abilities
   - **E**: Interact
   - **Tab**: Inventory
   - **M**: Map
   - **Escape**: Menu

### Step 2: Play in Standalone Mode

1. Click the dropdown arrow next to **Play** button
2. Select **Standalone Game**
3. Click **Play**
4. The game will launch in a separate window

### Step 3: Play in Selected Viewport

1. Make sure a viewport is selected
2. Press **Alt+P**
3. The game will play in the selected viewport
4. Press **Escape** to exit play mode

---

## 8️⃣ Multiplayer Setup

### Step 1: Configure Multiplayer Settings

1. Go to **Edit > Project Settings**
2. Navigate to **Engine > Networking**
3. Set these values:
   - Enable Multiplayer: **True**
   - Enable Networking: **True**
   - Enable Online: **True**
   - Max Client Count: **100**
   - Max Internet Client Count: **100**
   - Max LAN Server Client Count: **32**
   - Use Compression: **True**
   - Use Encryption: **False** (for testing)
   - Verify Peer: **False** (for testing)

### Step 2: Run as Listen Server

**Method 1: From Unreal Editor**
1. Click the dropdown arrow next to **Play** button
2. Select **Multiplayer > Listen Server**
3. Set **Number of Players** to 2 or more
4. Click **Play**
5. A new window will open for the server
6. Open another instance of Unreal Editor
7. Connect as a client (see below)

**Method 2: From Command Line**
```bash
# Windows
"C:\Program Files\Epic Games\UE_5.3\Engine\Binaries\Win64\UE5Editor.exe" "C:\Path\To\my-mmorpg-server\UnrealMMORPG\MusicMMORPG.uproject" -server -log -port=7777 -maxplayers=100

# Mac
/Users/Shared/EpicGames/UE_5.3/Engine/Binaries/Mac/UE5Editor.app/Contents/MacOS/UE5Editor "/Path/To/my-mmorpg-server/UnrealMMORPG/MusicMMORPG.uproject" -server -log -port=7777 -maxplayers=100
```

### Step 3: Connect as Client

**Method 1: From Unreal Editor**
1. Open another instance of Unreal Editor
2. Click the dropdown arrow next to **Play** button
3. Select **Multiplayer > Client**
4. Set **Number of Players** to 1
5. In the **Server URL** field, enter: `127.0.0.1:7777` (for local testing)
6. Click **Play**

**Method 2: From Command Line**
```bash
# Windows
"C:\Program Files\Epic Games\UE_5.3\Engine\Binaries\Win64\UE5Editor.exe" "C:\Path\To\my-mmorpg-server\UnrealMMORPG\MusicMMORPG.uproject" -game -port=7777

# Mac
/Users/Shared/EpicGames/UE_5.3/Engine/Binaries/Mac/UE5Editor.app/Contents/MacOS/UE5Editor "/Path/To/my-mmorpg-server/UnrealMMORPG/MusicMMORPG.uproject" -game -port=7777
```

**Method 3: Using Console Command**
1. In the client instance, press **~** (tilde) to open console
2. Type: `open 127.0.0.1:7777`
3. Press **Enter**

### Step 4: Test Multiplayer

1. **Server**: Run as Listen Server (as above)
2. **Client 1**: Connect from another Unreal Editor instance
3. **Client 2**: Connect from a third Unreal Editor instance
4. **Test**:
   - Move characters independently
   - Check if players see each other
   - Test chat: Press **Enter** and type a message
   - Test combat: Attack other players (if PvP is enabled)

### Step 5: Dedicated Server

For a dedicated server (no client):

```bash
# Windows
"C:\Program Files\Epic Games\UE_5.3\Engine\Binaries\Win64\UE5Server.exe" "C:\Path\To\my-mmorpg-server\UnrealMMORPG\MusicMMORPG.uproject" -NullRHI -nosplash -nopause -log -server -port=7777 -maxplayers=100

# Mac
/Users/Shared/EpicGames/UE_5.3/Engine/Binaries/Mac/UE5Server.app/Contents/MacOS/UE5Server "/Path/To/my-mmorpg-server/UnrealMMORPG/MusicMMORPG.uproject" -NullRHI -nosplash -nopause -log -server -port=7777 -maxplayers=100
```

---

## 9️⃣ Building for iOS

### Step 1: Configure iOS Settings

1. Go to **Edit > Project Settings**
2. Navigate to **Platforms > iOS**
3. Set these values:
   - Minimum iOS Version: **12.0**
   - Enable Metal: **True**
   - Enable Game Center: **True** (for multiplayer)
   - Enable Voice Chat: **True**
   - Bundle Identifier: `com.yourcompany.MusicMMORPG`
   - Bundle Display Name: `Music MMORPG`
   - Bundle Version: `1.0.0`

### Step 2: Configure Online Subsystem

1. Go to **Edit > Plugins**
2. Search for **Online Subsystem IOS**
3. Ensure it's **Enabled**
4. Search for **Voice**
5. Ensure it's **Enabled**

### Step 3: Package for iOS

**Method 1: Using Unreal Editor**
1. Go to **File > Package Project > Packaging Settings**
2. Set these values:
   - Platform: **IOS**
   - Packaging Mode: **Development** or **Shipping**
   - Include Prerequisites: **True**
   - Build Configuration: **Development** or **Shipping**
3. Click **Package**
4. Select a directory to save the build
5. Wait for packaging to complete (may take 30-60 minutes)

**Method 2: Using Command Line**
```bash
# Navigate to your Unreal Engine installation
cd /Users/Shared/EpicGames/UE_5.3/Engine/Build/BatchFiles

# Package for iOS
./RunUAT.sh BuildCookRun -project="/Path/To/my-mmorpg-server/UnrealMMORPG/MusicMMORPG.uproject" -noP4 -nocompile -nocompileeditor -nocleanstage -utf8output -platform=IOS -clientconfig=Development -serverconfig=Development -buildmachine -nocompile -skipcompile -skipeditor -package -archive -archivedirectory="/Path/To/Output"
```

### Step 4: Open in Xcode

1. Navigate to the packaged output directory
2. Find the `.xcodeproj` file
3. Double-click to open in Xcode
4. Select your **Target Device** or **Simulator**
5. Select your **Development Team** (Apple Developer Account)
6. Click **Build** (or **Cmd+B**)

### Step 5: Run on Device

1. Connect your iOS device to your Mac
2. In Xcode, select your device as the target
3. Click **Run** (or **Cmd+R**)
4. The app will install and launch on your device

### Step 6: Test on Simulator

1. In Xcode, select a simulator (e.g., iPhone 15)
2. Click **Run** (or **Cmd+R**)
3. The app will launch in the simulator

---

## 🔟 Building for PC

### Windows

#### Step 1: Configure Windows Settings

1. Go to **Edit > Project Settings**
2. Navigate to **Platforms > Windows**
3. Set these values:
   - Target Architecture: **x64**
   - Enable Steam: **True** (optional)
   - Enable Voice Chat: **True**

#### Step 2: Package for Windows

**Method 1: Using Unreal Editor**
1. Go to **File > Package Project > Packaging Settings**
2. Set these values:
   - Platform: **Win64**
   - Packaging Mode: **Development** or **Shipping**
   - Include Prerequisites: **True**
   - Build Configuration: **Development** or **Shipping**
3. Click **Package**
4. Select a directory to save the build
5. Wait for packaging to complete (may take 20-40 minutes)

**Method 2: Using Command Line**
```bash
# Navigate to your Unreal Engine installation
cd "C:\Program Files\Epic Games\UE_5.3\Engine\Build\BatchFiles"

# Package for Windows
RunUAT.bat BuildCookRun -project="C:\Path\To\my-mmorpg-server\UnrealMMORPG\MusicMMORPG.uproject" -noP4 -nocompile -nocompileeditor -nocleanstage -utf8output -platform=Win64 -clientconfig=Development -serverconfig=Development -buildmachine -nocompile -skipcompile -skipeditor -package -archive -archivedirectory="C:\Path\To\Output"
```

#### Step 3: Run the Packaged Game

1. Navigate to the output directory
2. Find `MusicMMORPG.exe`
3. Double-click to run
4. Or run from command line:
```bash
cd "C:\Path\To\Output\WindowsNoEditor"
MusicMMORPG.exe
```

### Mac

#### Step 1: Configure Mac Settings

1. Go to **Edit > Project Settings**
2. Navigate to **Platforms > Mac**
3. Set these values:
   - Target Architecture: **x86_64** or **arm64**
   - Enable Voice Chat: **True**

#### Step 2: Package for Mac

**Method 1: Using Unreal Editor**
1. Go to **File > Package Project > Packaging Settings**
2. Set these values:
   - Platform: **Mac**
   - Packaging Mode: **Development** or **Shipping**
   - Include Prerequisites: **True**
3. Click **Package**
4. Select a directory to save the build

**Method 2: Using Command Line**
```bash
# Navigate to your Unreal Engine installation
cd /Users/Shared/EpicGames/UE_5.3/Engine/Build/BatchFiles

# Package for Mac
./RunUAT.sh BuildCookRun -project="/Path/To/my-mmorpg-server/UnrealMMORPG/MusicMMORPG.uproject" -noP4 -nocompile -nocompileeditor -nocleanstage -utf8output -platform=Mac -clientconfig=Development -serverconfig=Development -buildmachine -nocompile -skipcompile -skipeditor -package -archive -archivedirectory="/Path/To/Output"
```

#### Step 3: Run the Packaged Game

1. Navigate to the output directory
2. Find `MusicMMORPG.app`
3. Double-click to run

---

## 📝 Configuration

### Game Configuration

#### Edit `Config/DefaultGame.ini`

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

; Available zones
+AvailableZones=SoundCity
+AvailableZones=BassCaverns
+AvailableZones=SynthCitadel
+AvailableZones=PvPArena

; Spawn tags
+SpawnTags=PlayerStart
+SpawnTags=SoundCity
+SpawnTags=BassCaverns
+SpawnTags=SynthCitadel
```

#### Edit `Config/DefaultEngine.ini`

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

[/Script/Engine.NetworkSettings]
bEnableMultiplayer=true
bEnableNetworking=true
bEnableOnline=true
bEnableVoiceChat=true
MaxClientCount=100
MaxInternetClientCount=100
MaxLanServerClientCount=32
bUseCompression=true
bUseEncryption=false
bVerifyPeer=false
bEnableReplicationGraph=true
```

### Input Configuration

#### Edit `Config/DefaultInput.ini`

```ini
[/Script/Engine.InputSettings]
DefaultInputComponentClass=/Script/MusicMMORPG.MMORPGInputComponent

; Action Mappings
+ActionMappings=(ActionName="Jump", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=SpaceBar)
+ActionMappings=(ActionName="Jump", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=Gamepad_FaceButton_Bottom)
+ActionMappings=(ActionName="Attack", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=LeftMouseButton)
+ActionMappings=(ActionName="Attack", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=Gamepad_FaceButton_Right)
+ActionMappings=(ActionName="Ability1", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=One)
+ActionMappings=(ActionName="Ability2", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=Two)
+ActionMappings=(ActionName="Ability3", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=Three)
+ActionMappings=(ActionName="Ability4", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=Four)
+ActionMappings=(ActionName="Run", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=LeftShift)
+ActionMappings=(ActionName="Sprint", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=LeftControl)
+ActionMappings=(ActionName="Crouch", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=C)
+ActionMappings=(ActionName="Interact", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=E)
+ActionMappings=(ActionName="Inventory", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=Tab)
+ActionMappings=(ActionName="Map", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=M)
+ActionMappings=(ActionName="Menu", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=Escape)
+ActionMappings=(ActionName="Chat", bShift=false, bCtrl=false, bAlt=false, bCmd=false, Key=Enter)

; Axis Mappings
+AxisMappings=(AxisName="MoveForward", Scale=1.0, Key=W)
+AxisMappings=(AxisName="MoveForward", Scale=-1.0, Key=S)
+AxisMappings=(AxisName="MoveForward", Scale=1.0, Key=Gamepad_LeftY)
+AxisMappings=(AxisName="MoveRight", Scale=1.0, Key=D)
+AxisMappings=(AxisName="MoveRight", Scale=-1.0, Key=A)
+AxisMappings=(AxisName="MoveRight", Scale=1.0, Key=Gamepad_LeftX)
+AxisMappings=(AxisName="LookUp", Scale=-1.0, Key=MouseY)
+AxisMappings=(AxisName="LookUp", Scale=1.0, Key=Gamepad_RightY)
+AxisMappings=(AxisName="LookRight", Scale=1.0, Key=MouseX)
+AxisMappings=(AxisName="LookRight", Scale=1.0, Key=Gamepad_RightX)
```

---

## 🐛 Troubleshooting

### Common Issues and Solutions

#### Compilation Errors

| Error | Cause | Solution |
|-------|-------|----------|
| `Module MusicMMORPG not found` | Project files not generated | Regenerate project files |
| `Cannot open include file` | Missing header file | Check include paths, ensure .h files are in Public |
| `Unrecognized type` | Syntax error in C++ | Check C++ syntax, ensure UE5 compatibility |
| `Linker errors` | Missing library | Rebuild project, check module dependencies |
| `Missing DLL` | Engine not installed correctly | Reinstall Unreal Engine |

#### Runtime Errors

| Error | Cause | Solution |
|-------|-------|----------|
| `LogInit: Failed to initialize` | Missing plugins | Enable required plugins in Edit > Plugins |
| `LogUObjectGlobals: Warning: Failed to load` | Missing content | Check Content directory, regenerate project files |
| `Assertion failed` | Invalid state | Check Output Log for details |
| `Network error` | Networking not configured | Enable multiplayer in Project Settings |

#### Packaging Errors

| Error | Cause | Solution |
|-------|-------|----------|
| `Missing prerequisites` | Prerequisites not included | Enable "Include Prerequisites" in packaging settings |
| `Code signing failed` (iOS) | Invalid certificate | Check Apple Developer certificate in Xcode |
| `Build failed` | Compilation error | Fix compilation errors first |
| `Missing files` | Files not copied | Check packaging settings, ensure all files are included |

#### Multiplayer Issues

| Issue | Cause | Solution |
|-------|-------|----------|
| Cannot connect to server | Firewall blocking | Disable firewall or add exception for port 7777 |
| Players don't see each other | Replication not set up | Check replication settings in C++ classes |
| Lag / Desync | Network issues | Check network settings, reduce MaxClientRate |
| Voice chat not working | Voice plugin disabled | Enable Voice plugin in Edit > Plugins |

### Debugging Tools

#### Output Log
1. **Window > Developer Tools > Output Log**
2. Filter by **Errors**, **Warnings**, or **All**
3. Copy error messages for troubleshooting

#### Console Commands
Press **~** (tilde) in-game to open console:

| Command | Description |
|---------|-------------|
| `stat fps` | Show FPS |
| `stat unit` | Show unit stats |
| `stat net` | Show network stats |
| `listplayers` | List all players |
| `open 127.0.0.1:7777` | Connect to server |
| `disconnect` | Disconnect from server |
| `quit` | Quit the game |

#### Visual Debugging
1. **Alt+P**: Play in editor
2. **F8**: Toggle wireframe mode
3. **F9**: Toggle collision visibility
4. **F10**: Toggle lighting visibility

---

## 📞 Support

### Where to Get Help

1. **GitHub Issues**: [https://github.com/Zee2401/my-mmorpg-server/issues](https://github.com/Zee2401/my-mmorpg-server/issues)
2. **Unreal Engine Documentation**: [https://docs.unrealengine.com](https://docs.unrealengine.com)
3. **Unreal Engine Forums**: [https://forums.unrealengine.com](https://forums.unrealengine.com)
4. **Discord**: Join the Unreal Engine Discord server

### Reporting Issues

When reporting issues, include:
- Unreal Engine version
- Platform (Windows/Mac/iOS)
- Steps to reproduce
- Error messages (from Output Log)
- Screenshots (if applicable)

---

## 🎉 Next Steps

After successfully setting up the project:

1. **Create Content**: Add 3D models, textures, sounds to the Content directory
2. **Design Levels**: Create maps for each zone (Sound City, Bass Caverns, Synth Citadel)
3. **Test Gameplay**: Playtest single-player and multiplayer
4. **Add Features**: Implement additional game systems
5. **Optimize**: Improve performance and graphics
6. **Package**: Build for your target platform
7. **Distribute**: Share with players

---

## 📚 Additional Resources

- [Unreal Engine 5 Documentation](https://docs.unrealengine.com)
- [Unreal Engine C++ Programming Guide](https://docs.unrealengine.com/5.3/en-US/programming-and-scripting/programming-with-cpp/)
- [Unreal Engine Multiplayer Networking](https://docs.unrealengine.com/5.3/en-US/multiplayer-networking-in-unreal-engine/)
- [Unreal Engine Audio System](https://docs.unrealengine.com/5.3/en-US/audio-in-unreal-engine/)
- [Unreal Engine iOS Development](https://docs.unrealengine.com/5.3/en-US/platforms/mobile/ios/)

---

**Happy Developing! 🎮🎵**

*Last Updated: October 2024*
*Unreal Engine Version: 5.3*

# MusicMORPG App - Cross-Platform Music MMORPG Game 🎵🎮

A feature-packed, cross-platform Musical MMORPG built with React Native and Expo. Experience rhythmic battles, synthesize audio beats, form musical guilds, equip epic instruments, and conquer sound bosses on both **Mobile (Android/iOS)** and **Desktop/PC (Web)**!

---

## 🌟 Key Features

- **Cross-Platform Responsive Engine**: Seamless touch controls on mobile devices + full desktop PC keyboard support (`A`, `S`, `D`, `F` or Arrow keys for rhythm note hits & Spacebar for Solo Shred).
- **Sound Synthesizer Audio Engine**: Built-in Web Audio API synthesizer that plays pitch tones (C4–C5), drum beats (Kick, Snare, Hi-Hat, Sub-Bass), hit/miss sound effects, and special ability power-up melodies in real-time.
- **Rhythm Battle Combat System**: Real-time falling note lanes with hit accuracy timing (Perfect/Great/Miss), combo multipliers, boss damage calculations, and Special Overdrive moves.
- **Hero Classes & Attributes**: Specializations (Bard, Beatmaker, Vocalist, Guitarist, DJ) with upgradable attributes (Rhythm Attack, Pitch Accuracy, Special Power, Harmony Defense).
- **World Map & Boss Dungeons**: Explore Sound City Alleyways, Sub-Zero Bass Caverns, and Synth Citadel. Battle bosses like Static Spectre, Subwoofer Titan, and Silence Overlord.
- **Jam Studio & Beatmaker**: Interactive drum pads and synthesizer piano scale to practice rhythms and earn bonus Rhythm XP.
- **Guilds & Bands**: Form/join bands (e.g. Cyber Beats Guild), view global leaderboards, and launch cooperative Band Concert Raids.
- **Gear Shop & Inventory**: Equip guitars, keytars, condenser mics, thunder amps, distortion pedals, and consume energy elixirs. Progress persists locally via AsyncStorage.

---

## 🚀 Quick Start & Development

### Prerequisites
- Node.js (v18 or higher)
- npm or yarn

### 1. Install Dependencies
```bash
npm install
```

### 2. Run on Desktop / PC Web Browser
```bash
npm run web
# or
npx expo start --web
```

### 3. Run on Mobile with Expo Go
```bash
npx expo start
```
Scan the displayed QR code with the **Expo Go** app on your Android or iOS device.

---

## 📱 Termux & EAS Android Build Instructions

### Termux Environment Setup
To build or run directly on Android using Termux:
```bash
# Update packages
apt update && apt upgrade -y
apt install -y nodejs npm git

# Verify installations
node --version
npm --version

# Clone repository & install dependencies
git clone <repository-url> MusicMORPGApp
cd MusicMORPGApp
npm install

# Start local server
npx expo start
```

### EAS Android APK Build
Build a standalone `.apk` for distribution or local device installation:

```bash
# Install EAS CLI globally if not installed
npm install -g eas-cli

# Configure EAS (if needed)
npx eas-cli@latest build:configure

# Build standalone Android APK (local / cloud)
npx eas-cli@latest build --platform android --profile preview
```

---

## 🛠 Project Structure

```
/app
├── App.js                         # Application Entry Point
├── app.json                       # Expo Configuration
├── eas.json                       # EAS Build Profiles
├── package.json                   # Project Dependencies
├── src/
│   ├── components/
│   │   └── ResponsiveContainer.tsx# Adaptive Widescreen/Mobile Shell
│   ├── context/
│   │   └── GameContext.tsx        # Game State & Storage Persistence
│   ├── navigation/
│   │   └── AppNavigator.tsx       # Bottom Tab Navigator
│   ├── screens/
│   │   ├── DashboardScreen.tsx    # Character Sheet & Attributes
│   │   ├── WorldMapScreen.tsx     # Regions, Bosses & Quests
│   │   ├── RhythmBattleScreen.tsx # Rhythm Combat Engine
│   │   ├── StudioScreen.tsx       # Synth Beatmaker & Jam Pad
│   │   ├── BandScreen.tsx         # Guilds, Leaderboards & Concerts
│   │   └── ShopScreen.tsx         # Instrument Shop & Gear Inventory
│   ├── types/
│   │   └── game.ts                # TypeScript Interfaces & Models
│   └── utils/
│       └── audioSynth.ts          # Web Audio Synthesizer Engine
```

---

## 🧪 Verification & Type Checks

```bash
# Type check TypeScript files
npx tsc --noEmit

# Export web bundle
npx expo export --platform web
```

---

## 📄 License
MIT License. Created for cross-platform mobile and desktop gaming.

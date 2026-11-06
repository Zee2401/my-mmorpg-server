# Contributing to MusicMORPG App

Thank you for your interest in contributing to MusicMORPG App! We welcome contributions from everyone.

---

## Getting Started

### Prerequisites

- Node.js v18 or higher
- npm, yarn, or pnpm
- Git

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/Zee2401/my-mmorpg-server.git
   cd my-mmorpg-server
   ```

2. Install dependencies:
   ```bash
   npm install
   ```

3. Start the development server:
   ```bash
   npm run start
   # or
   npx expo start
   ```

4. Run on your preferred platform:
   - **Web**: `npm run web` or open http://localhost:19006
   - **Android**: Scan QR code with Expo Go app
   - **iOS**: Scan QR code with Expo Go app

---

## Development Guidelines

### Code Style

- **TypeScript**: Always use TypeScript with strict mode enabled
- **Formatting**: Use consistent indentation (2 spaces)
- **Naming**: Use descriptive names for variables, functions, and components
- **Comments**: Avoid unnecessary comments; code should be self-documenting

### Commit Messages

Follow [Conventional Commits](https://www.conventionalcommits.org/) format:

```
type(scope): description

body

footer
```

Types: `feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `chore`

Examples:
- `feat(game): add new boss character`
- `fix(audio): correct note frequency calculations`
- `docs(readme): update setup instructions`

### Pull Requests

1. Fork the repository
2. Create a feature branch (`git checkout -b feat/your-feature`)
3. Make your changes
4. Run tests and linting:
   ```bash
   npx tsc --noEmit
   npm run lint  # if available
   ```
5. Commit your changes with descriptive messages
6. Push to your fork (`git push origin feat/your-feature`)
7. Open a Pull Request to the main branch

### Review Process

- All PRs require at least one approval
- CI checks must pass
- TypeScript compilation must succeed
- Code must follow project conventions

---

## Project Structure

```
/
├── App.js                    # Root component
├── index.js                  # Entry point
├── app.json                  # Expo configuration
├── eas.json                  # EAS build configuration
├── src/
│   ├── components/           # Reusable UI components
│   │   └── ResponsiveContainer.tsx
│   ├── context/              # React context providers
│   │   └── GameContext.tsx   # Game state management
│   ├── navigation/           # Navigation configuration
│   │   └── AppNavigator.tsx  # Tab navigator
│   ├── screens/              # Application screens
│   │   ├── DashboardScreen.tsx
│   │   ├── WorldMapScreen.tsx
│   │   ├── RhythmBattleScreen.tsx
│   │   ├── StudioScreen.tsx
│   │   ├── BandScreen.tsx
│   │   └── ShopScreen.tsx
│   ├── types/                # TypeScript type definitions
│   │   └── game.ts           # Game data types
│   └── utils/                # Utility functions
│       └── audioSynth.ts     # Audio synthesizer
├── assets/                   # Static assets
└── dist/                     # Build output (generated)
```

---

## Adding New Features

### New Screens

1. Create a new file in `src/screens/`
2. Add it to the navigation in `AppNavigator.tsx`
3. Import and use the `useGame` hook for state management

### New Game Content

Add new content to the appropriate data files:
- **Items**: Add to `INITIAL_ITEMS` in `GameContext.tsx`
- **Regions/Bosses**: Add to `INITIAL_REGIONS` in `GameContext.tsx`
- **Quests**: Add to region definitions in `INITIAL_REGIONS`

### Audio Features

Use the `audioSynth` utility in `src/utils/audioSynth.ts`:
```typescript
import { audioSynth } from '../utils/audioSynth';

// Play a note
audioSynth.playNote('C4', 'sine', 0.3, 0.2);

// Play a drum
audioSynth.playDrum('kick');

// Play hit sound
audioSynth.playHitSound(true); // true = perfect hit
```

---

## Testing

### Running Type Checks

```bash
npx tsc --noEmit
```

### Manual Testing

1. Test on multiple platforms (Web, Android, iOS)
2. Verify all screens are accessible
3. Test game mechanics (battles, progression, inventory)
4. Check audio works correctly
5. Verify state persistence

---

## Reporting Issues

When reporting issues, please include:

1. **Platform**: Web, Android, or iOS
2. **Device/OS**: Specific device and OS version
3. **Steps to Reproduce**: Clear steps to trigger the issue
4. **Expected Behavior**: What should happen
5. **Actual Behavior**: What actually happens
6. **Screenshots/Video**: Visual proof if applicable
7. **Error Logs**: Any console errors or stack traces

---

## Security

If you discover a security vulnerability, please see [SECURITY.md](SECURITY.md) for disclosure instructions.

---

## License

By contributing to this project, you agree that your contributions will be licensed under the [MIT License](LICENSE).

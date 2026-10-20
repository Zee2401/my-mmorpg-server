import React from 'react';
import { SafeAreaProvider } from 'react-native-safe-area-context';
import { GameProvider } from './src/context/GameContext';
import { ResponsiveContainer } from './src/components/ResponsiveContainer';
import { AppNavigator } from './src/navigation/AppNavigator';

export default function App() {
  return (
    <SafeAreaProvider>
      <GameProvider>
        <ResponsiveContainer>
          <AppNavigator />
        </ResponsiveContainer>
      </GameProvider>
    </SafeAreaProvider>
  );
}

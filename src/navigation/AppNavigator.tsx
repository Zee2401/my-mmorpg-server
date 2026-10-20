import React from 'react';
import { NavigationContainer } from '@react-navigation/native';
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';
import { Ionicons } from '@expo/vector-icons';

import { DashboardScreen } from '../screens/DashboardScreen';
import { WorldMapScreen } from '../screens/WorldMapScreen';
import { RhythmBattleScreen } from '../screens/RhythmBattleScreen';
import { StudioScreen } from '../screens/StudioScreen';
import { BandScreen } from '../screens/BandScreen';
import { ShopScreen } from '../screens/ShopScreen';

export type RootTabParamList = {
  Dashboard: undefined;
  Explore: undefined;
  Battle: undefined;
  Studio: undefined;
  Band: undefined;
  Shop: undefined;
};

const Tab = createBottomTabNavigator<RootTabParamList>();

export const AppNavigator = () => {
  return (
    <NavigationContainer>
      <Tab.Navigator
        screenOptions={({ route }) => ({
          headerShown: false,
          tabBarActiveTintColor: '#00F0FF',
          tabBarInactiveTintColor: '#666680',
          tabBarStyle: {
            backgroundColor: '#0A0A12',
            borderTopColor: '#00F0FF22',
            height: 60,
            paddingBottom: 8,
            paddingTop: 6,
          },
          tabBarLabelStyle: {
            fontSize: 11,
            fontWeight: '600',
          },
          tabBarIcon: ({ color, size }) => {
            let iconName: keyof typeof Ionicons.glyphMap = 'musical-notes';

            if (route.name === 'Dashboard') {
              iconName = 'person-circle-outline';
            } else if (route.name === 'Explore') {
              iconName = 'map-outline';
            } else if (route.name === 'Battle') {
              iconName = 'flame-outline';
            } else if (route.name === 'Studio') {
              iconName = 'disc-outline';
            } else if (route.name === 'Band') {
              iconName = 'people-outline';
            } else if (route.name === 'Shop') {
              iconName = 'cart-outline';
            }

            return <Ionicons name={iconName} size={size} color={color} />;
          },
        })}
      >
        <Tab.Screen name="Dashboard" component={DashboardScreen} options={{ tabBarLabel: 'Hero' }} />
        <Tab.Screen name="Explore" component={WorldMapScreen} options={{ tabBarLabel: 'Map' }} />
        <Tab.Screen name="Battle" component={RhythmBattleScreen} options={{ tabBarLabel: 'Battle' }} />
        <Tab.Screen name="Studio" component={StudioScreen} options={{ tabBarLabel: 'Studio' }} />
        <Tab.Screen name="Band" component={BandScreen} options={{ tabBarLabel: 'Band' }} />
        <Tab.Screen name="Shop" component={ShopScreen} options={{ tabBarLabel: 'Shop' }} />
      </Tab.Navigator>
    </NavigationContainer>
  );
};

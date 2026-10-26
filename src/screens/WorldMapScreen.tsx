import React, { useState } from 'react';
import {
  StyleSheet,
  View,
  Text,
  ScrollView,
  TouchableOpacity,
  Alert,
} from 'react-native';
import { SafeAreaView } from 'react-native-safe-area-context';
import { Ionicons } from '@expo/vector-icons';
import { useNavigation } from '@react-navigation/native';
import { BottomTabNavigationProp } from '@react-navigation/bottom-tabs';
import { useGame } from '../context/GameContext';
import { Region, Quest, Boss } from '../types/game';
import { RootTabParamList } from '../navigation/AppNavigator';

type NavigationProp = BottomTabNavigationProp<RootTabParamList, 'Explore'>;

export const WorldMapScreen = () => {
  const navigation = useNavigation<NavigationProp>();
  const { player, regions, setActiveBoss, completeQuest } = useGame();
  const [selectedRegion, setSelectedRegion] = useState<Region>(regions[0]);

  const handleStartBossDuel = (boss: Boss) => {
    if (player.level < selectedRegion.minLevel) {
      Alert.alert(
        'Region Locked',
        `You need Level ${selectedRegion.minLevel} to challenge ${boss.name}. You are currently Level ${player.level}.`
      );
      return;
    }
    setActiveBoss(boss);
    navigation.navigate('Battle');
  };

  const handlePerformQuest = (quest: Quest) => {
    if (player.level < quest.requiredLevel) {
      Alert.alert(
        'Quest Locked',
        `You need Level ${quest.requiredLevel} to perform "${quest.title}". You are currently Level ${player.level}.`
      );
      return;
    }
    completeQuest(quest.id);
  };

  return (
    <SafeAreaView style={styles.container}>
      <ScrollView contentContainerStyle={styles.scrollContent}>
        {/* Header */}
        <View style={styles.header}>
          <Text style={styles.title}>WORLD MAP & REGIONS</Text>
          <Text style={styles.subtitle}>Conquer rhythmic realms & defeat sound bosses</Text>
        </View>

        {/* Region Selector Cards */}
        <ScrollView horizontal showsHorizontalScrollIndicator={false} style={styles.regionList}>
          {regions.map((reg) => {
            const isSelected = selectedRegion.id === reg.id;
            const isLocked = player.level < reg.minLevel;

            return (
              <TouchableOpacity
                key={reg.id}
                style={[
                  styles.regionCard,
                  isSelected && styles.selectedRegionCard,
                  isLocked && styles.lockedRegionCard,
                ]}
                onPress={() => setSelectedRegion(reg)}
              >
                <Ionicons
                  name={reg.icon as keyof typeof Ionicons.glyphMap}
                  size={28}
                  color={isSelected ? '#00F0FF' : isLocked ? '#555566' : '#FFFFFF'}
                />
                <Text style={[styles.regionName, isSelected && { color: '#00F0FF' }]}>
                  {reg.name}
                </Text>
                <Text style={styles.regionReq}>
                  {isLocked ? `Req. Lvl ${reg.minLevel}` : 'Unlocked'}
                </Text>
              </TouchableOpacity>
            );
          })}
        </ScrollView>

        {/* Active Region Overview */}
        <View style={styles.regionDetailCard}>
          <Text style={styles.regionDetailTitle}>{selectedRegion.name}</Text>
          <Text style={styles.regionDetailDesc}>{selectedRegion.description}</Text>

          {/* Boss Arena Banner */}
          <Text style={styles.sectionHeader}>Region Realm Boss</Text>
          <View style={styles.bossCard}>
            <View style={styles.bossRow}>
              <View style={styles.bossIconCircle}>
                <Ionicons
                  name={selectedRegion.boss.avatar as keyof typeof Ionicons.glyphMap}
                  size={32}
                  color="#FF0055"
                />
              </View>
              <View style={{ marginLeft: 12, flex: 1 }}>
                <Text style={styles.bossName}>{selectedRegion.boss.name}</Text>
                <Text style={styles.bossTitle}>{selectedRegion.boss.title} • {selectedRegion.boss.tempoBpm} BPM</Text>
                <Text style={styles.bossHpText}>
                  HP: {selectedRegion.boss.hp} / {selectedRegion.boss.maxHp}
                </Text>
              </View>
            </View>

            <TouchableOpacity
              style={styles.duelButton}
              onPress={() => handleStartBossDuel(selectedRegion.boss)}
            >
              <Ionicons name="flame" size={18} color="#FFFFFF" />
              <Text style={styles.duelButtonText}>DUEL BOSS IN RHYTHM ARENA</Text>
            </TouchableOpacity>
          </View>

          {/* Quests List */}
          <Text style={styles.sectionHeader}>Region Quests</Text>
          {selectedRegion.quests.map((quest) => {
            const isDone = player.completedQuestIds.includes(quest.id);

            return (
              <View key={quest.id} style={styles.questCard}>
                <View style={{ flex: 1 }}>
                  <Text style={styles.questTitle}>{quest.title}</Text>
                  <Text style={styles.questDesc}>{quest.description}</Text>
                  <View style={styles.rewardRow}>
                    <Text style={styles.rewardText}>+{quest.rewardXp} XP</Text>
                    <Text style={styles.rewardGems}>+{quest.rewardGems} Gems</Text>
                  </View>
                </View>

                {isDone ? (
                  <View style={styles.completedBadge}>
                    <Ionicons name="checkmark-circle" size={20} color="#39FF14" />
                    <Text style={styles.completedText}>Done</Text>
                  </View>
                ) : (
                  <TouchableOpacity
                    style={[styles.claimButton, player.level < quest.requiredLevel && styles.disabledClaimButton]}
                    onPress={() => handlePerformQuest(quest)}
                  >
                    <Text style={[styles.claimButtonText, player.level < quest.requiredLevel && styles.disabledClaimText]}>Perform</Text>
                  </TouchableOpacity>
                )}
              </View>
            );
          })}
        </View>
      </ScrollView>
    </SafeAreaView>
  );
};

const styles = StyleSheet.create({
  container: { flex: 1, backgroundColor: '#12121A' },
  scrollContent: { padding: 16 },
  header: { marginBottom: 16 },
  title: { fontSize: 22, fontWeight: '900', color: '#00F0FF', letterSpacing: 1 },
  subtitle: { fontSize: 12, color: '#8888AA' },
  regionList: { marginBottom: 16, flexDirection: 'row' },
  regionCard: {
    backgroundColor: '#1A1A28',
    borderRadius: 14,
    padding: 14,
    width: 140,
    marginRight: 12,
    borderWidth: 1,
    borderColor: '#2A2A3C',
    alignItems: 'center',
  },
  selectedRegionCard: { borderColor: '#00F0FF', backgroundColor: '#00F0FF10' },
  lockedRegionCard: { opacity: 0.6 },
  regionName: { color: '#FFFFFF', fontWeight: 'bold', fontSize: 13, marginTop: 8, textAlign: 'center' },
  regionReq: { color: '#8888AA', fontSize: 10, marginTop: 4 },
  regionDetailCard: { backgroundColor: '#1A1A28', borderRadius: 16, padding: 16, borderWidth: 1, borderColor: '#2A2A3C' },
  regionDetailTitle: { fontSize: 18, fontWeight: 'bold', color: '#FFFFFF' },
  regionDetailDesc: { fontSize: 12, color: '#8888AA', marginVertical: 6 },
  sectionHeader: { fontSize: 14, fontWeight: 'bold', color: '#00F0FF', marginTop: 16, marginBottom: 10 },
  bossCard: { backgroundColor: '#0A0A12', borderRadius: 12, padding: 12, borderWidth: 1, borderColor: '#FF005544' },
  bossRow: { flexDirection: 'row', alignItems: 'center' },
  bossIconCircle: {
    width: 50,
    height: 50,
    borderRadius: 25,
    backgroundColor: '#FF005515',
    justifyContent: 'center',
    alignItems: 'center',
    borderWidth: 1,
    borderColor: '#FF0055',
  },
  bossName: { color: '#FFFFFF', fontWeight: 'bold', fontSize: 16 },
  bossTitle: { color: '#FF0055', fontSize: 11, fontWeight: '600', marginTop: 2 },
  bossHpText: { color: '#8888AA', fontSize: 11, marginTop: 4 },
  duelButton: {
    backgroundColor: '#FF0055',
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
    paddingVertical: 10,
    borderRadius: 8,
    marginTop: 12,
  },
  duelButtonText: { color: '#FFFFFF', fontWeight: 'bold', fontSize: 12, marginLeft: 6 },
  questCard: {
    backgroundColor: '#0A0A12',
    borderRadius: 12,
    padding: 12,
    marginBottom: 10,
    flexDirection: 'row',
    alignItems: 'center',
    borderWidth: 1,
    borderColor: '#2A2A3C',
  },
  questTitle: { color: '#FFFFFF', fontWeight: 'bold', fontSize: 14 },
  questDesc: { color: '#8888AA', fontSize: 11, marginVertical: 2 },
  rewardRow: { flexDirection: 'row', marginTop: 4 },
  rewardText: { color: '#39FF14', fontSize: 11, fontWeight: '600', marginRight: 10 },
  rewardGems: { color: '#FFD700', fontSize: 11, fontWeight: '600' },
  completedBadge: { flexDirection: 'row', alignItems: 'center', marginLeft: 8 },
  completedText: { color: '#39FF14', fontSize: 11, fontWeight: '600', marginLeft: 4 },
  claimButton: { backgroundColor: '#00F0FF', paddingHorizontal: 12, paddingVertical: 6, borderRadius: 6, marginLeft: 8 },
  disabledClaimButton: { backgroundColor: '#333344' },
  claimButtonText: { color: '#12121A', fontWeight: 'bold', fontSize: 11 },
  disabledClaimText: { color: '#8888AA' },
});

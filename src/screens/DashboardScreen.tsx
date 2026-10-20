import React, { useState } from 'react';
import {
  StyleSheet,
  View,
  Text,
  ScrollView,
  TouchableOpacity,
  Modal,
  Image,
} from 'react-native';
import { SafeAreaView } from 'react-native-safe-area-context';
import { Ionicons } from '@expo/vector-icons';
import { useGame } from '../context/GameContext';
import { CharacterClass } from '../types/game';

const CLASSES: { name: CharacterClass; icon: keyof typeof Ionicons.glyphMap; desc: string }[] = [
  { name: 'Bard', icon: 'musical-notes', desc: '+Harmony & Balanced Rhythms' },
  { name: 'Beatmaker', icon: 'disc', desc: '+High Rhythm Power & Fast Combos' },
  { name: 'Vocalist', icon: 'mic', desc: '+High Pitch Accuracy & Power' },
  { name: 'Guitarist', icon: 'flash', desc: '+Critical Solo Hits & Energy' },
  { name: 'DJ', icon: 'headset', desc: '+Sub-Bass Waves & Defense' },
];

export const DashboardScreen = () => {
  const { player, restoreEnergyAndHp, changeClass } = useGame();
  const [classModalVisible, setClassModalVisible] = useState(false);

  const xpPercent = Math.min(100, Math.floor((player.xp / player.nextLevelXp) * 100));
  const hpPercent = Math.min(100, Math.floor((player.stats.hp / player.stats.maxHp) * 100));
  const energyPercent = Math.min(100, Math.floor((player.stats.energy / player.stats.maxEnergy) * 100));

  return (
    <SafeAreaView style={styles.container}>
      <ScrollView contentContainerStyle={styles.scrollContent}>
        {/* Header Bar */}
        <View style={styles.header}>
          <View>
            <Text style={styles.gameTitle}>MUSIC MORPG</Text>
            <Text style={styles.subTitle}>Cross-Platform MMORPG Realm</Text>
          </View>
          <View style={styles.gemBadge}>
            <Ionicons name="diamond" size={18} color="#FFD700" />
            <Text style={styles.gemText}>{player.harmonyGems}</Text>
          </View>
        </View>

        {/* Hero Character Card */}
        <View style={styles.heroCard}>
          <View style={styles.avatarRow}>
            <View style={styles.avatarCircle}>
              <Ionicons name="headset" size={40} color="#00F0FF" />
            </View>
            <View style={styles.heroInfo}>
              <Text style={styles.heroName}>{player.name}</Text>
              <TouchableOpacity
                style={styles.classBadge}
                onPress={() => setClassModalVisible(true)}
              >
                <Text style={styles.classText}>Level {player.level} • {player.characterClass}</Text>
                <Ionicons name="chevron-down" size={14} color="#00F0FF" />
              </TouchableOpacity>
            </View>
          </View>

          {/* XP Progress */}
          <View style={styles.progressContainer}>
            <View style={styles.progressLabelRow}>
              <Text style={styles.progressLabel}>XP Progress</Text>
              <Text style={styles.progressValue}>{player.xp} / {player.nextLevelXp} ({xpPercent}%)</Text>
            </View>
            <View style={styles.progressBarBackground}>
              <View style={[styles.progressBarFill, { width: `${xpPercent}%`, backgroundColor: '#00F0FF' }]} />
            </View>
          </View>

          {/* HP and Energy */}
          <View style={styles.statBarRow}>
            <View style={[styles.progressContainer, { flex: 1, marginRight: 8 }]}>
              <View style={styles.progressLabelRow}>
                <Text style={styles.progressLabel}>HP</Text>
                <Text style={styles.progressValue}>{player.stats.hp}/{player.stats.maxHp}</Text>
              </View>
              <View style={styles.progressBarBackground}>
                <View style={[styles.progressBarFill, { width: `${hpPercent}%`, backgroundColor: '#FF0055' }]} />
              </View>
            </View>

            <View style={[styles.progressContainer, { flex: 1, marginLeft: 8 }]}>
              <View style={styles.progressLabelRow}>
                <Text style={styles.progressLabel}>Energy</Text>
                <Text style={styles.progressValue}>{player.stats.energy}/{player.stats.maxEnergy}</Text>
              </View>
              <View style={styles.progressBarBackground}>
                <View style={[styles.progressBarFill, { width: `${energyPercent}%`, backgroundColor: '#39FF14' }]} />
              </View>
            </View>
          </View>

          {/* Energy Restore Action */}
          <TouchableOpacity style={styles.restButton} onPress={restoreEnergyAndHp}>
            <Ionicons name="flash-outline" size={18} color="#12121A" />
            <Text style={styles.restButtonText}>Rest & Recharge Energy</Text>
          </TouchableOpacity>
        </View>

        {/* Combat Stats Grid */}
        <Text style={styles.sectionTitle}>Musical Attributes</Text>
        <View style={styles.statsGrid}>
          <View style={styles.statBox}>
            <Ionicons name="flame" size={22} color="#FF007F" />
            <Text style={styles.statVal}>{player.stats.rhythm}</Text>
            <Text style={styles.statName}>Rhythm Power</Text>
          </View>
          <View style={styles.statBox}>
            <Ionicons name="pulse" size={22} color="#00F0FF" />
            <Text style={styles.statVal}>{player.stats.pitch}</Text>
            <Text style={styles.statName}>Pitch Accuracy</Text>
          </View>
          <View style={styles.statBox}>
            <Ionicons name="flash" size={22} color="#FFD700" />
            <Text style={styles.statVal}>{player.stats.power}</Text>
            <Text style={styles.statName}>Special Power</Text>
          </View>
          <View style={styles.statBox}>
            <Ionicons name="heart" size={22} color="#39FF14" />
            <Text style={styles.statVal}>{player.stats.harmony}</Text>
            <Text style={styles.statName}>Harmony Def</Text>
          </View>
        </View>

        {/* Equipped Gear Summary */}
        <Text style={styles.sectionTitle}>Equipped Concert Gear</Text>
        <View style={styles.gearContainer}>
          <View style={styles.gearSlot}>
            <Ionicons name="musical-notes" size={20} color="#00F0FF" />
            <View style={styles.gearDetails}>
              <Text style={styles.gearSlotName}>Instrument</Text>
              <Text style={styles.gearTitle}>{player.equipped.instrument?.name || 'Unequipped'}</Text>
            </View>
          </View>

          <View style={styles.gearSlot}>
            <Ionicons name="volume-high" size={20} color="#FF007F" />
            <View style={styles.gearDetails}>
              <Text style={styles.gearSlotName}>Amplifier</Text>
              <Text style={styles.gearTitle}>{player.equipped.amplifier?.name || 'None'}</Text>
            </View>
          </View>

          <View style={styles.gearSlot}>
            <Ionicons name="mic" size={20} color="#FFD700" />
            <View style={styles.gearDetails}>
              <Text style={styles.gearSlotName}>Microphone</Text>
              <Text style={styles.gearTitle}>{player.equipped.microphone?.name || 'None'}</Text>
            </View>
          </View>
        </View>
      </ScrollView>

      {/* Class Selector Modal */}
      <Modal visible={classModalVisible} transparent animationType="slide">
        <View style={styles.modalOverlay}>
          <View style={styles.modalContainer}>
            <Text style={styles.modalTitle}>Choose Class Specialization</Text>
            {CLASSES.map((c) => (
              <TouchableOpacity
                key={c.name}
                style={[
                  styles.classOption,
                  player.characterClass === c.name && styles.activeClassOption,
                ]}
                onPress={() => {
                  changeClass(c.name);
                  setClassModalVisible(false);
                }}
              >
                <Ionicons name={c.icon} size={24} color="#00F0FF" />
                <View style={{ marginLeft: 12, flex: 1 }}>
                  <Text style={styles.classNameText}>{c.name}</Text>
                  <Text style={styles.classDescText}>{c.desc}</Text>
                </View>
              </TouchableOpacity>
            ))}
            <TouchableOpacity style={styles.closeButton} onPress={() => setClassModalVisible(false)}>
              <Text style={styles.closeButtonText}>Close</Text>
            </TouchableOpacity>
          </View>
        </View>
      </Modal>
    </SafeAreaView>
  );
};

const styles = StyleSheet.create({
  container: { flex: 1, backgroundColor: '#12121A' },
  scrollContent: { padding: 16 },
  header: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    marginBottom: 16,
  },
  gameTitle: { fontSize: 22, fontWeight: '900', color: '#00F0FF', letterSpacing: 1 },
  subTitle: { fontSize: 12, color: '#8888AA' },
  gemBadge: {
    flexDirection: 'row',
    alignItems: 'center',
    backgroundColor: '#1E1E2C',
    paddingHorizontal: 12,
    paddingVertical: 6,
    borderRadius: 16,
    borderWidth: 1,
    borderColor: '#FFD70044',
  },
  gemText: { color: '#FFD700', fontWeight: 'bold', marginLeft: 6 },
  heroCard: {
    backgroundColor: '#1A1A28',
    borderRadius: 16,
    padding: 16,
    borderWidth: 1,
    borderColor: '#00F0FF33',
    marginBottom: 20,
  },
  avatarRow: { flexDirection: 'row', alignItems: 'center', marginBottom: 16 },
  avatarCircle: {
    width: 64,
    height: 64,
    borderRadius: 32,
    backgroundColor: '#0A0A12',
    justifyContent: 'center',
    alignItems: 'center',
    borderWidth: 2,
    borderColor: '#00F0FF',
  },
  heroInfo: { marginLeft: 16, flex: 1 },
  heroName: { fontSize: 20, fontWeight: 'bold', color: '#FFFFFF' },
  classBadge: {
    flexDirection: 'row',
    alignItems: 'center',
    backgroundColor: '#00F0FF15',
    paddingHorizontal: 8,
    paddingVertical: 4,
    borderRadius: 8,
    marginTop: 4,
    alignSelf: 'flex-start',
  },
  classText: { color: '#00F0FF', fontWeight: '600', fontSize: 12, marginRight: 4 },
  progressContainer: { marginBottom: 10 },
  progressLabelRow: { flexDirection: 'row', justifyContent: 'space-between', marginBottom: 4 },
  progressLabel: { color: '#AAAACC', fontSize: 11, fontWeight: '600' },
  progressValue: { color: '#FFFFFF', fontSize: 11, fontWeight: '600' },
  progressBarBackground: {
    height: 8,
    backgroundColor: '#0A0A12',
    borderRadius: 4,
    overflow: 'hidden',
  },
  progressBarFill: { height: '100%', borderRadius: 4 },
  statBarRow: { flexDirection: 'row', marginTop: 4 },
  restButton: {
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#39FF14',
    paddingVertical: 10,
    borderRadius: 10,
    marginTop: 8,
  },
  restButtonText: { color: '#12121A', fontWeight: 'bold', marginLeft: 6 },
  sectionTitle: { fontSize: 16, fontWeight: 'bold', color: '#FFFFFF', marginBottom: 12, marginTop: 8 },
  statsGrid: { flexDirection: 'row', flexWrap: 'wrap', justifyContent: 'space-between', marginBottom: 16 },
  statBox: {
    width: '48%',
    backgroundColor: '#1A1A28',
    borderRadius: 12,
    padding: 12,
    alignItems: 'center',
    marginBottom: 12,
    borderWidth: 1,
    borderColor: '#2A2A3C',
  },
  statVal: { fontSize: 18, fontWeight: 'bold', color: '#FFFFFF', marginVertical: 4 },
  statName: { fontSize: 12, color: '#8888AA' },
  gearContainer: { backgroundColor: '#1A1A28', borderRadius: 12, padding: 12 },
  gearSlot: { flexDirection: 'row', alignItems: 'center', paddingVertical: 8, borderBottomWidth: 1, borderBottomColor: '#2A2A3C' },
  gearDetails: { marginLeft: 12 },
  gearSlotName: { fontSize: 10, color: '#8888AA', textTransform: 'uppercase' },
  gearTitle: { fontSize: 14, color: '#FFFFFF', fontWeight: '600' },
  modalOverlay: { flex: 1, backgroundColor: '#000000AA', justifyContent: 'center', padding: 20 },
  modalContainer: { backgroundColor: '#1A1A28', borderRadius: 16, padding: 20 },
  modalTitle: { fontSize: 18, fontWeight: 'bold', color: '#FFFFFF', marginBottom: 16, textAlign: 'center' },
  classOption: {
    flexDirection: 'row',
    alignItems: 'center',
    padding: 12,
    borderRadius: 10,
    backgroundColor: '#0A0A12',
    marginBottom: 10,
  },
  activeClassOption: { borderWidth: 1.5, borderColor: '#00F0FF' },
  classNameText: { color: '#FFFFFF', fontSize: 16, fontWeight: 'bold' },
  classDescText: { color: '#8888AA', fontSize: 12 },
  closeButton: { padding: 12, alignItems: 'center', marginTop: 10 },
  closeButtonText: { color: '#FF0055', fontWeight: 'bold' },
});

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
import { useGame } from '../context/GameContext';
import { audioSynth } from '../utils/audioSynth';

const LEADERBOARD_BANDS = [
  { rank: 1, name: 'Cyber Beats Guild', power: 12500, concerts: 42 },
  { rank: 2, name: 'Neon Synthwave Syndicate', power: 10800, concerts: 38 },
  { rank: 3, name: 'Heavy Bass Drop Army', power: 9400, concerts: 31 },
  { rank: 4, name: 'Acoustic Legends', power: 7200, concerts: 24 },
];

export const BandScreen = () => {
  const { player, gainXpAndGems } = useGame();
  const band = player.band;
  const [concertActive, setConcertActive] = useState(false);

  const handleConcertRaid = () => {
    setConcertActive(true);
    audioSynth.playAbilitySound();

    setTimeout(() => {
      setConcertActive(false);
      gainXpAndGems(200, 100);
      Alert.alert(
        'Concert Raid Successful!',
        'Your band performed an epic world concert! Earned +200 XP and +100 Harmony Gems!'
      );
    }, 2000);
  };

  return (
    <SafeAreaView style={styles.container}>
      <ScrollView contentContainerStyle={styles.scrollContent}>
        {/* Header */}
        <View style={styles.header}>
          <Text style={styles.title}>GUILDS & BANDS</Text>
          <Text style={styles.subtitle}>Form musical bands, perform concert raids & climb rankings</Text>
        </View>

        {/* Current Band Overview */}
        {band ? (
          <View style={styles.bandCard}>
            <View style={styles.bandHeaderRow}>
              <View style={styles.bandIconCircle}>
                <Ionicons name="people" size={32} color="#BF55EC" />
              </View>
              <View style={{ marginLeft: 12, flex: 1 }}>
                <Text style={styles.bandName}>{band.name}</Text>
                <Text style={styles.bandMotto}>"{band.motto}"</Text>
                <Text style={styles.bandPowerText}>
                  Band Level {band.level} • Concert Power: {band.concertPower}
                </Text>
              </View>
            </View>

            {/* Concert Raid Button */}
            <TouchableOpacity
              style={[styles.concertButton, concertActive && { backgroundColor: '#FF0055' }]}
              onPress={handleConcertRaid}
              disabled={concertActive}
            >
              <Ionicons name="sparkles" size={18} color="#FFFFFF" />
              <Text style={styles.concertButtonText}>
                {concertActive ? 'PERFORMING CONCERT...' : 'LAUNCH BAND CONCERT RAID'}
              </Text>
            </TouchableOpacity>

            {/* Band Roster */}
            <Text style={styles.sectionHeader}>Band Member Roster</Text>
            {band.members.map((m) => (
              <View key={m.id} style={styles.memberRow}>
                <Ionicons name="person-circle-outline" size={24} color="#00F0FF" />
                <View style={{ marginLeft: 10, flex: 1 }}>
                  <Text style={styles.memberName}>{m.name}</Text>
                  <Text style={styles.memberRole}>{m.role} • Lvl {m.level}</Text>
                </View>
                <Text style={styles.contribText}>{m.contributionPoints} pts</Text>
              </View>
            ))}
          </View>
        ) : (
          <View style={styles.noBandCard}>
            <Text style={styles.noBandTitle}>No Band Joined</Text>
            <Text style={styles.noBandSub}>Join or create a musical guild to compete in team concerts.</Text>
          </View>
        )}

        {/* Global Leaderboard */}
        <Text style={styles.sectionTitle}>Global Band Leaderboards</Text>
        <View style={styles.leaderboardContainer}>
          {LEADERBOARD_BANDS.map((lb) => (
            <View key={lb.rank} style={styles.lbRow}>
              <Text style={styles.rankText}>#{lb.rank}</Text>
              <View style={{ marginLeft: 12, flex: 1 }}>
                <Text style={styles.lbName}>{lb.name}</Text>
                <Text style={styles.lbSub}>{lb.concerts} World Concerts Won</Text>
              </View>
              <Text style={styles.powerText}>{lb.power} PWR</Text>
            </View>
          ))}
        </View>
      </ScrollView>
    </SafeAreaView>
  );
};

const styles = StyleSheet.create({
  container: { flex: 1, backgroundColor: '#12121A' },
  scrollContent: { padding: 16 },
  header: { marginBottom: 16 },
  title: { fontSize: 22, fontWeight: '900', color: '#BF55EC', letterSpacing: 1 },
  subtitle: { fontSize: 12, color: '#8888AA' },
  bandCard: {
    backgroundColor: '#1A1A28',
    borderRadius: 16,
    padding: 16,
    borderWidth: 1,
    borderColor: '#BF55EC44',
    marginBottom: 20,
  },
  bandHeaderRow: { flexDirection: 'row', alignItems: 'center' },
  bandIconCircle: {
    width: 56,
    height: 56,
    borderRadius: 28,
    backgroundColor: '#BF55EC15',
    justifyContent: 'center',
    alignItems: 'center',
    borderWidth: 1.5,
    borderColor: '#BF55EC',
  },
  bandName: { color: '#FFFFFF', fontWeight: 'bold', fontSize: 18 },
  bandMotto: { color: '#BF55EC', fontSize: 12, fontStyle: 'italic', marginTop: 2 },
  bandPowerText: { color: '#8888AA', fontSize: 11, marginTop: 4 },
  concertButton: {
    backgroundColor: '#BF55EC',
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
    paddingVertical: 12,
    borderRadius: 10,
    marginTop: 16,
    marginBottom: 12,
  },
  concertButtonText: { color: '#FFFFFF', fontWeight: 'bold', fontSize: 13, marginLeft: 8 },
  sectionHeader: { fontSize: 14, fontWeight: 'bold', color: '#00F0FF', marginTop: 8, marginBottom: 8 },
  memberRow: {
    flexDirection: 'row',
    alignItems: 'center',
    backgroundColor: '#0A0A12',
    padding: 10,
    borderRadius: 10,
    marginBottom: 8,
  },
  memberName: { color: '#FFFFFF', fontWeight: 'bold', fontSize: 13 },
  memberRole: { color: '#8888AA', fontSize: 11 },
  contribText: { color: '#FFD700', fontWeight: 'bold', fontSize: 12 },
  noBandCard: { backgroundColor: '#1A1A28', borderRadius: 16, padding: 20, alignItems: 'center', marginBottom: 20 },
  noBandTitle: { color: '#FFFFFF', fontWeight: 'bold', fontSize: 16 },
  noBandSub: { color: '#8888AA', fontSize: 12, textAlign: 'center', marginTop: 4 },
  sectionTitle: { fontSize: 16, fontWeight: 'bold', color: '#FFFFFF', marginBottom: 12 },
  leaderboardContainer: { backgroundColor: '#1A1A28', borderRadius: 16, padding: 12 },
  lbRow: {
    flexDirection: 'row',
    alignItems: 'center',
    paddingVertical: 10,
    borderBottomWidth: 1,
    borderBottomColor: '#2A2A3C',
  },
  rankText: { color: '#FFD700', fontWeight: '900', fontSize: 16, width: 32 },
  lbName: { color: '#FFFFFF', fontWeight: 'bold', fontSize: 14 },
  lbSub: { color: '#8888AA', fontSize: 11 },
  powerText: { color: '#00F0FF', fontWeight: 'bold', fontSize: 13 },
});

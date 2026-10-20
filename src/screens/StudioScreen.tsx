import React, { useState } from 'react';
import {
  StyleSheet,
  View,
  Text,
  TouchableOpacity,
  ScrollView,
} from 'react-native';
import { SafeAreaView } from 'react-native-safe-area-context';
import { Ionicons } from '@expo/vector-icons';
import { audioSynth } from '../utils/audioSynth';
import { useGame } from '../context/GameContext';

const DRUM_PADS = [
  { id: 'kick', label: 'Bass Kick', color: '#FF0055', icon: 'disc' },
  { id: 'snare', label: 'Snare Hit', color: '#00F0FF', icon: 'hardware-chip' },
  { id: 'hihat', label: 'Hi-Hat Cymbal', color: '#39FF14', icon: 'radio' },
  { id: 'synth', label: 'Sub-Bass Wave', color: '#FFD700', icon: 'pulse' },
] as const;

const NOTE_SCALE = [
  { note: 'C4', label: 'C4', color: '#00F0FF' },
  { note: 'D4', label: 'D4', color: '#39FF14' },
  { note: 'E4', label: 'E4', color: '#FFD700' },
  { note: 'F4', label: 'F4', color: '#FF007F' },
  { note: 'G4', label: 'G4', color: '#BF55EC' },
  { note: 'A4', label: 'A4', color: '#00F0FF' },
  { note: 'B4', label: 'B4', color: '#39FF14' },
  { note: 'C5', label: 'C5', color: '#FFD700' },
];

export const StudioScreen = () => {
  const { gainXpAndGems } = useGame();
  const [beatsPlayed, setBeatsPlayed] = useState(0);
  const [activePad, setActivePad] = useState<string | null>(null);

  const handleDrumPress = (type: 'kick' | 'snare' | 'hihat' | 'synth') => {
    audioSynth.playDrum(type);
    setActivePad(type);
    setTimeout(() => setActivePad(null), 150);

    const newCount = beatsPlayed + 1;
    setBeatsPlayed(newCount);

    if (newCount % 20 === 0) {
      gainXpAndGems(25, 10);
    }
  };

  const handleNotePress = (note: string) => {
    audioSynth.playNote(note, 'sawtooth', 0.25, 0.2);
    setActivePad(note);
    setTimeout(() => setActivePad(null), 150);

    const newCount = beatsPlayed + 1;
    setBeatsPlayed(newCount);

    if (newCount % 20 === 0) {
      gainXpAndGems(25, 10);
    }
  };

  return (
    <SafeAreaView style={styles.container}>
      <ScrollView contentContainerStyle={styles.scrollContent}>
        {/* Header */}
        <View style={styles.header}>
          <Text style={styles.title}>JAM STUDIO & BEATMAKER</Text>
          <Text style={styles.subtitle}>Practice notes, synthesize rhythms & earn bonus XP</Text>
        </View>

        {/* Practice Stats Header */}
        <View style={styles.practiceCard}>
          <Ionicons name="disc" size={32} color="#FFD700" />
          <View style={{ marginLeft: 12, flex: 1 }}>
            <Text style={styles.practiceTitle}>Practice Jam Session</Text>
            <Text style={styles.practiceSub}>
              Beats Triggered: <Text style={{ color: '#00F0FF', fontWeight: 'bold' }}>{beatsPlayed}</Text>
            </Text>
          </View>
          <View style={styles.xpBonusBadge}>
            <Text style={styles.xpBonusText}>+25 XP / 20 Beats</Text>
          </View>
        </View>

        {/* Drum Pads Grid */}
        <Text style={styles.sectionTitle}>Rhythm Drum Pads</Text>
        <View style={styles.drumGrid}>
          {DRUM_PADS.map((pad) => {
            const isActive = activePad === pad.id;
            return (
              <TouchableOpacity
                key={pad.id}
                activeOpacity={0.7}
                style={[
                  styles.drumPad,
                  { borderColor: pad.color },
                  isActive && { backgroundColor: pad.color + '44' },
                ]}
                onPress={() => handleDrumPress(pad.id)}
              >
                <Ionicons name={pad.icon as keyof typeof Ionicons.glyphMap} size={28} color={pad.color} />
                <Text style={styles.drumPadText}>{pad.label}</Text>
              </TouchableOpacity>
            );
          })}
        </View>

        {/* Synth Melody Keys Scale */}
        <Text style={styles.sectionTitle}>Synthesizer Piano Scale</Text>
        <View style={styles.synthGrid}>
          {NOTE_SCALE.map((n) => {
            const isActive = activePad === n.note;
            return (
              <TouchableOpacity
                key={n.note}
                activeOpacity={0.7}
                style={[
                  styles.synthKey,
                  { borderColor: n.color },
                  isActive && { backgroundColor: n.color + '55' },
                ]}
                onPress={() => handleNotePress(n.note)}
              >
                <Text style={[styles.synthKeyText, { color: n.color }]}>{n.label}</Text>
              </TouchableOpacity>
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
  title: { fontSize: 22, fontWeight: '900', color: '#FFD700', letterSpacing: 1 },
  subtitle: { fontSize: 12, color: '#8888AA' },
  practiceCard: {
    backgroundColor: '#1A1A28',
    borderRadius: 14,
    padding: 14,
    flexDirection: 'row',
    alignItems: 'center',
    borderWidth: 1,
    borderColor: '#FFD70044',
    marginBottom: 20,
  },
  practiceTitle: { color: '#FFFFFF', fontWeight: 'bold', fontSize: 15 },
  practiceSub: { color: '#8888AA', fontSize: 12, marginTop: 2 },
  xpBonusBadge: { backgroundColor: '#FFD70015', paddingHorizontal: 10, paddingVertical: 6, borderRadius: 8 },
  xpBonusText: { color: '#FFD700', fontWeight: 'bold', fontSize: 11 },
  sectionTitle: { fontSize: 16, fontWeight: 'bold', color: '#FFFFFF', marginBottom: 12, marginTop: 8 },
  drumGrid: { flexDirection: 'row', flexWrap: 'wrap', justifyContent: 'space-between', marginBottom: 16 },
  drumPad: {
    width: '48%',
    height: 100,
    backgroundColor: '#1A1A28',
    borderRadius: 12,
    borderWidth: 2,
    justifyContent: 'center',
    alignItems: 'center',
    marginBottom: 12,
  },
  drumPadText: { color: '#FFFFFF', fontWeight: 'bold', fontSize: 13, marginTop: 8 },
  synthGrid: { flexDirection: 'row', flexWrap: 'wrap', justifyContent: 'space-between' },
  synthKey: {
    width: '23%',
    height: 70,
    backgroundColor: '#1A1A28',
    borderRadius: 10,
    borderWidth: 1.5,
    justifyContent: 'center',
    alignItems: 'center',
    marginBottom: 10,
  },
  synthKeyText: { fontWeight: '900', fontSize: 16 },
});

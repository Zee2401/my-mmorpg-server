import React, { useState, useEffect, useRef } from 'react';
import {
  StyleSheet,
  View,
  Text,
  TouchableOpacity,
  Modal,
  Platform,
} from 'react-native';
import { SafeAreaView } from 'react-native-safe-area-context';
import { Ionicons } from '@expo/vector-icons';
import { useGame } from '../context/GameContext';
import { audioSynth } from '../utils/audioSynth';
import { Boss } from '../types/game';

interface FallingNote {
  id: number;
  lane: number; // 0, 1, 2, 3
  y: number;    // 0 to 100 percentage
  hit: boolean;
}

const LANES = [
  { id: 0, key: 'A', name: 'Bass C4', color: '#00F0FF', note: 'C4' },
  { id: 1, key: 'S', name: 'Lead E4', color: '#FF007F', note: 'E4' },
  { id: 2, key: 'D', name: 'Synth G4', color: '#39FF14', note: 'G4' },
  { id: 3, key: 'F', name: 'Vocal C5', color: '#FFD700', note: 'C5' },
];

export const RhythmBattleScreen = () => {
  const { player, activeBoss, gainXpAndGems, restoreEnergyAndHp } = useGame();

  const currentBoss: Boss = activeBoss || {
    id: 'boss_practice',
    name: 'Practice Soundbot',
    title: 'Training Synthesizer',
    level: 1,
    hp: 300,
    maxHp: 300,
    avatar: 'disc',
    tempoBpm: 110,
    description: 'A friendly soundbot for practicing rhythm combos.',
    rewardXp: 80,
    rewardGems: 40,
  };

  const [isPlaying, setIsPlaying] = useState(false);
  const [bossHp, setBossHp] = useState(currentBoss.hp);
  const [playerHp, setPlayerHp] = useState(player.stats.hp);
  const [score, setScore] = useState(0);
  const [combo, setCombo] = useState(0);
  const [maxCombo, setMaxCombo] = useState(0);
  const [specialGauge, setSpecialGauge] = useState(0);
  const [feedbackText, setFeedbackText] = useState<string | null>(null);
  const [notes, setNotes] = useState<FallingNote[]>([]);
  const [victoryModalVisible, setVictoryModalVisible] = useState(false);
  const [defeatModalVisible, setDefeatModalVisible] = useState(false);

  const noteIdCounter = useRef(1);
  const gameLoopRef = useRef<ReturnType<typeof setInterval> | null>(null);

  // Keyboard controls listener for Desktop PC
  useEffect(() => {
    if (Platform.OS !== 'web') return;

    const handleKeyDown = (e: KeyboardEvent) => {
      if (!isPlaying) return;
      const key = e.key.toLowerCase();
      if (key === 'a') handleLaneHit(0);
      if (key === 's') handleLaneHit(1);
      if (key === 'd') handleLaneHit(2);
      if (key === 'f') handleLaneHit(3);
      if (e.code === 'Space' && specialGauge >= 100) triggerSpecialAbility();
    };

    window.addEventListener('keydown', handleKeyDown);
    return () => window.removeEventListener('keydown', handleKeyDown);
  }, [isPlaying, notes, specialGauge]);

  useEffect(() => {
    setBossHp(currentBoss.hp);
    setPlayerHp(player.stats.hp);
  }, [currentBoss]);

  // Start / Stop Battle loop
  const startBattle = () => {
    setIsPlaying(true);
    setBossHp(currentBoss.hp);
    setPlayerHp(player.stats.hp);
    setScore(0);
    setCombo(0);
    setMaxCombo(0);
    setSpecialGauge(0);
    setNotes([]);
    setFeedbackText('READY... GO!');
  };

  useEffect(() => {
    if (!isPlaying) {
      if (gameLoopRef.current) clearInterval(gameLoopRef.current);
      return;
    }

    gameLoopRef.current = setInterval(() => {
      // Spawn new note periodically based on BPM
      if (Math.random() < 0.35) {
        const lane = Math.floor(Math.random() * 4);
        const newNote: FallingNote = {
          id: noteIdCounter.current++,
          lane,
          y: 0,
          hit: false,
        };
        setNotes((prev) => [...prev, newNote]);
      }

      // Move notes downward
      setNotes((prev) => {
        const updated: FallingNote[] = [];
        let playerDamaged = false;

        prev.forEach((note) => {
          const newY = note.y + 4.5;
          if (newY >= 95 && !note.hit) {
            // Missed note reached bottom!
            playerDamaged = true;
          } else if (newY < 100) {
            updated.push({ ...note, y: newY });
          }
        });

        if (playerDamaged) {
          audioSynth.playMissSound();
          setCombo(0);
          setFeedbackText('MISS!');
          setPlayerHp((prevHp) => {
            const nextHp = Math.max(0, prevHp - 10);
            if (nextHp <= 0) {
              setIsPlaying(false);
              setDefeatModalVisible(true);
            }
            return nextHp;
          });
        }

        return updated;
      });
    }, 100);

    return () => {
      if (gameLoopRef.current) clearInterval(gameLoopRef.current);
    };
  }, [isPlaying]);

  // Handle lane hit (Touch or Keyboard)
  const handleLaneHit = (laneIndex: number) => {
    if (!isPlaying) return;

    audioSynth.playNote(LANES[laneIndex].note, 'sine', 0.2, 0.2);

    let hitFound = false;
    setNotes((prevNotes) => {
      return prevNotes.map((note) => {
        if (!hitFound && note.lane === laneIndex && note.y >= 65 && note.y <= 92 && !note.hit) {
          hitFound = true;
          const isPerfect = note.y >= 75 && note.y <= 88;
          const points = isPerfect ? 200 : 100;
          const heroDmg = player.stats.rhythm + (isPerfect ? 30 : 15);

          audioSynth.playHitSound(isPerfect);
          setScore((s) => s + points);
          setCombo((c) => {
            const nextC = c + 1;
            setMaxCombo((m) => Math.max(m, nextC));
            return nextC;
          });
          setSpecialGauge((g) => Math.min(100, g + 12));
          setFeedbackText(isPerfect ? 'PERFECT!' : 'GREAT!');

          // Damage boss
          setBossHp((bHp) => {
            const nextBHp = Math.max(0, bHp - heroDmg);
            if (nextBHp <= 0) {
              setIsPlaying(false);
              gainXpAndGems(currentBoss.rewardXp, currentBoss.rewardGems);
              setVictoryModalVisible(true);
            }
            return nextBHp;
          });

          return { ...note, hit: true };
        }
        return note;
      });
    });

    if (!hitFound) {
      setFeedbackText('EARLY / LATE');
    }
  };

  const triggerSpecialAbility = () => {
    if (specialGauge < 100 || !isPlaying) return;

    audioSynth.playAbilitySound();
    setSpecialGauge(0);
    setFeedbackText('BASS DROP OVERDRIVE!');
    const specialDmg = player.stats.rhythm * 3 + player.stats.power * 2;

    setBossHp((bHp) => {
      const nextBHp = Math.max(0, bHp - specialDmg);
      if (nextBHp <= 0) {
        setIsPlaying(false);
        gainXpAndGems(currentBoss.rewardXp, currentBoss.rewardGems);
        setVictoryModalVisible(true);
      }
      return nextBHp;
    });
  };

  const bossHpPercent = Math.min(100, Math.floor((bossHp / currentBoss.maxHp) * 100));
  const playerHpPercent = Math.min(100, Math.floor((playerHp / player.stats.maxHp) * 100));

  return (
    <SafeAreaView style={styles.container}>
      <View style={styles.header}>
        <View style={styles.bossHeaderRow}>
          <Ionicons name="skull" size={24} color="#FF0055" />
          <View style={{ marginLeft: 8, flex: 1 }}>
            <Text style={styles.bossTitle}>{currentBoss.name}</Text>
            <View style={styles.hpBarBg}>
              <View style={[styles.hpBarFill, { width: `${bossHpPercent}%`, backgroundColor: '#FF0055' }]} />
            </View>
          </View>
          <Text style={styles.hpVal}>{bossHp}/{currentBoss.maxHp}</Text>
        </View>

        <View style={styles.playerHeaderRow}>
          <Ionicons name="headset" size={20} color="#00F0FF" />
          <View style={{ marginLeft: 8, flex: 1 }}>
            <Text style={styles.playerTitle}>{player.name}</Text>
            <View style={styles.hpBarBg}>
              <View style={[styles.hpBarFill, { width: `${playerHpPercent}%`, backgroundColor: '#39FF14' }]} />
            </View>
          </View>
          <Text style={styles.hpVal}>{playerHp}/{player.stats.maxHp}</Text>
        </View>
      </View>

      {/* Score and Special Gauge */}
      <View style={styles.scoreRow}>
        <View>
          <Text style={styles.scoreLabel}>SCORE: {score}</Text>
          <Text style={styles.comboLabel}>COMBO: {combo} (Max {maxCombo})</Text>
        </View>

        <TouchableOpacity
          style={[styles.specialButton, specialGauge >= 100 ? styles.specialButtonReady : styles.specialButtonDisabled]}
          onPress={triggerSpecialAbility}
          disabled={specialGauge < 100}
          accessibilityRole="button"
          accessibilityLabel="Solo Shred special ability"
          accessibilityHint={
            specialGauge >= 100
              ? 'Triggers heavy overdrive damage to sound boss'
              : `Special gauge is at ${specialGauge} percent`
          }
          accessibilityState={{ disabled: specialGauge < 100 }}
        >
          <Ionicons name="flash" size={16} color="#12121A" />
          <Text style={styles.specialText}>
            {specialGauge >= 100 ? 'SOLO SHRED' : `${specialGauge}%`}
          </Text>
        </TouchableOpacity>
      </View>

      {/* Feedback Banner */}
      {feedbackText && <Text style={styles.feedbackBanner}>{feedbackText}</Text>}

      {/* Rhythm Track Lane Area */}
      <View style={styles.trackArea}>
        {/* Hit Target Line */}
        <View style={styles.targetLine} />

        {/* 4 Lanes */}
        <View style={styles.lanesContainer}>
          {LANES.map((lane) => (
            <View key={lane.id} style={styles.laneTrack}>
              <Text style={styles.laneHeader}>{lane.key}</Text>
              {notes
                .filter((n) => n.lane === lane.id && !n.hit)
                .map((n) => (
                  <View
                    key={n.id}
                    style={[
                      styles.noteBlock,
                      { top: `${n.y}%`, backgroundColor: lane.color, shadowColor: lane.color },
                    ]}
                  />
                ))}
            </View>
          ))}
        </View>
      </View>

      {/* Touch Control Pads for Mobile / Click */}
      <View style={styles.controlsRow}>
        {LANES.map((lane) => (
          <TouchableOpacity
            key={lane.id}
            activeOpacity={0.7}
            accessibilityRole="button"
            accessibilityLabel={`Lane ${lane.key}, ${lane.name}`}
            accessibilityHint={`Hits falling notes in ${lane.name} lane`}
            style={[styles.padButton, { borderColor: lane.color }]}
            onPress={() => handleLaneHit(lane.id)}
          >
            <Text style={[styles.padText, { color: lane.color }]}>{lane.key}</Text>
            <Text style={styles.padSubText}>{lane.name}</Text>
          </TouchableOpacity>
        ))}
      </View>

      {/* Play / Start Button if not playing */}
      {!isPlaying && (
        <TouchableOpacity
          style={styles.startButton}
          onPress={startBattle}
          accessibilityRole="button"
          accessibilityLabel="Start Rhythm Duel"
          accessibilityHint="Begins the rhythm battle with falling notes"
        >
          <Ionicons name="play" size={24} color="#12121A" />
          <Text style={styles.startButtonText}>START RHYTHM DUEL</Text>
        </TouchableOpacity>
      )}

      {/* Victory Modal */}
      <Modal visible={victoryModalVisible} transparent animationType="slide">
        <View style={styles.modalOverlay}>
          <View style={styles.modalContainer}>
            <Ionicons name="trophy" size={48} color="#FFD700" style={{ alignSelf: 'center' }} />
            <Text style={styles.modalTitle}>VICTORY HARMONY!</Text>
            <Text style={styles.modalSub}>Defeated {currentBoss.name}</Text>
            <Text style={styles.rewardText}>+{currentBoss.rewardXp} XP</Text>
            <Text style={styles.rewardGems}>+{currentBoss.rewardGems} Harmony Gems</Text>
            <TouchableOpacity
              style={styles.modalButton}
              onPress={() => {
                setVictoryModalVisible(false);
                restoreEnergyAndHp();
              }}
            >
              <Text style={styles.modalButtonText}>CONTINUE</Text>
            </TouchableOpacity>
          </View>
        </View>
      </Modal>

      {/* Defeat Modal */}
      <Modal visible={defeatModalVisible} transparent animationType="slide">
        <View style={styles.modalOverlay}>
          <View style={styles.modalContainer}>
            <Ionicons name="skull" size={48} color="#FF0055" style={{ alignSelf: 'center' }} />
            <Text style={[styles.modalTitle, { color: '#FF0055' }]}>OUT OF RHYTHM</Text>
            <Text style={styles.modalSub}>The sound boss overpowered your tempo.</Text>
            <TouchableOpacity
              style={[styles.modalButton, { backgroundColor: '#FF0055' }]}
              onPress={() => {
                setDefeatModalVisible(false);
                restoreEnergyAndHp();
              }}
            >
              <Text style={styles.modalButtonText}>RETRY DUEL</Text>
            </TouchableOpacity>
          </View>
        </View>
      </Modal>
    </SafeAreaView>
  );
};

const styles = StyleSheet.create({
  container: { flex: 1, backgroundColor: '#0A0A12', padding: 12 },
  header: { backgroundColor: '#1A1A28', borderRadius: 12, padding: 12, marginBottom: 8 },
  bossHeaderRow: { flexDirection: 'row', alignItems: 'center', marginBottom: 8 },
  playerHeaderRow: { flexDirection: 'row', alignItems: 'center' },
  bossTitle: { color: '#FF0055', fontWeight: 'bold', fontSize: 13 },
  playerTitle: { color: '#00F0FF', fontWeight: 'bold', fontSize: 13 },
  hpBarBg: { height: 8, backgroundColor: '#0A0A12', borderRadius: 4, overflow: 'hidden', marginTop: 4 },
  hpBarFill: { height: '100%' },
  hpVal: { color: '#FFFFFF', fontSize: 11, fontWeight: 'bold', marginLeft: 8 },
  scoreRow: { flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center', marginVertical: 4 },
  scoreLabel: { color: '#00F0FF', fontWeight: 'bold', fontSize: 14 },
  comboLabel: { color: '#FFD700', fontWeight: 'bold', fontSize: 12 },
  specialButton: { paddingHorizontal: 12, paddingVertical: 6, borderRadius: 8, flexDirection: 'row', alignItems: 'center' },
  specialButtonReady: { backgroundColor: '#FFD700' },
  specialButtonDisabled: { backgroundColor: '#333344' },
  specialText: { fontWeight: 'bold', fontSize: 11, color: '#12121A', marginLeft: 4 },
  feedbackBanner: { textAlign: 'center', color: '#39FF14', fontWeight: '900', fontSize: 16, marginVertical: 2 },
  trackArea: { flex: 1, backgroundColor: '#12121E', borderRadius: 12, borderWidth: 1, borderColor: '#2A2A3C', overflow: 'hidden', position: 'relative', marginVertical: 4 },
  targetLine: { position: 'absolute', top: '78%', left: 0, right: 0, height: 4, backgroundColor: '#FF007F', zIndex: 10 },
  lanesContainer: { flex: 1, flexDirection: 'row' },
  laneTrack: { flex: 1, borderRightWidth: 1, borderRightColor: '#1A1A28', position: 'relative', alignItems: 'center' },
  laneHeader: { color: '#666688', fontWeight: 'bold', fontSize: 12, marginTop: 4 },
  noteBlock: { position: 'absolute', width: '80%', height: 24, borderRadius: 6, shadowOffset: { width: 0, height: 0 }, shadowOpacity: 0.8, shadowRadius: 6 },
  controlsRow: { flexDirection: 'row', justifyContent: 'space-between', marginTop: 6 },
  padButton: { flex: 1, backgroundColor: '#1A1A28', paddingVertical: 12, borderRadius: 10, borderWidth: 1.5, alignItems: 'center', marginHorizontal: 3 },
  padText: { fontWeight: '900', fontSize: 18 },
  padSubText: { color: '#8888AA', fontSize: 9, marginTop: 2 },
  startButton: { backgroundColor: '#00F0FF', flexDirection: 'row', justifyContent: 'center', alignItems: 'center', paddingVertical: 12, borderRadius: 12, marginTop: 8 },
  startButtonText: { color: '#12121A', fontWeight: 'bold', fontSize: 14, marginLeft: 8 },
  modalOverlay: { flex: 1, backgroundColor: '#000000CC', justifyContent: 'center', padding: 20 },
  modalContainer: { backgroundColor: '#1A1A28', borderRadius: 16, padding: 24, alignItems: 'center' },
  modalTitle: { fontSize: 22, fontWeight: '900', color: '#39FF14', marginTop: 12 },
  modalSub: { color: '#8888AA', fontSize: 14, marginVertical: 6, textAlign: 'center' },
  rewardText: { color: '#00F0FF', fontWeight: 'bold', fontSize: 16, marginTop: 8 },
  rewardGems: { color: '#FFD700', fontWeight: 'bold', fontSize: 16, marginBottom: 16 },
  modalButton: { backgroundColor: '#00F0FF', paddingHorizontal: 24, paddingVertical: 12, borderRadius: 10 },
  modalButtonText: { color: '#12121A', fontWeight: 'bold', fontSize: 14 },
});

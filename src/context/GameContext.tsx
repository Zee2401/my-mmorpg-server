import React, { createContext, useContext, useState, useEffect } from 'react';
import AsyncStorage from '@react-native-async-storage/async-storage';
import {
  PlayerProfile,
  GameSettings,
  InventoryItem,
  Quest,
  Region,
  Band,
  Boss,
  CharacterClass
} from '../types/game';

const STORAGE_KEY_PLAYER = '@music_morpg_player_profile';
const STORAGE_KEY_SETTINGS = '@music_morpg_settings';

export const INITIAL_ITEMS: InventoryItem[] = [
  {
    id: 'item_starter_guitar',
    name: 'Acoustic Starter Guitar',
    type: 'instrument',
    description: 'A trusty wooden guitar with warm, earthy resonance.',
    price: 0,
    icon: 'musical-notes',
    statBonus: { rhythm: 10, pitch: 5 },
    rarity: 'Common',
    isEquipped: true
  },
  {
    id: 'item_synth_keytar',
    name: 'Neon Cyber Keytar',
    type: 'instrument',
    description: 'Emits electric synthesizer tones with retro flair.',
    price: 150,
    icon: 'headset',
    statBonus: { rhythm: 25, pitch: 15, power: 10 },
    rarity: 'Rare'
  },
  {
    id: 'item_golden_mic',
    name: 'Gold Plated Condenser Mic',
    type: 'microphone',
    description: 'Captures crisp vocal harmonics that stun opponents.',
    price: 300,
    icon: 'mic',
    statBonus: { power: 30, pitch: 20 },
    rarity: 'Epic'
  },
  {
    id: 'item_bass_amp',
    name: 'Thunder 5000 Bass Amp',
    type: 'amplifier',
    description: 'Shakes the ground with rumbling low-end frequency.',
    price: 250,
    icon: 'volume-high',
    statBonus: { hp: 50, harmony: 20, rhythm: 15 },
    rarity: 'Rare'
  },
  {
    id: 'item_overdrive_pedal',
    name: 'Sonic Distortion Pedal',
    type: 'pedal',
    description: 'Overdrives pitch signals into devastating rhythm damage.',
    price: 200,
    icon: 'hardware-chip',
    statBonus: { rhythm: 20, power: 20 },
    rarity: 'Rare'
  },
  {
    id: 'item_energy_drink',
    name: 'Beat Energy Elixir',
    type: 'consumable',
    description: 'Restores 50 Energy and 100 HP instantly.',
    price: 30,
    icon: 'flash',
    statBonus: { energy: 50, hp: 100 },
    rarity: 'Common'
  }
];

export const INITIAL_REGIONS: Region[] = [
  {
    id: 'region_sound_city',
    name: 'Sound City Alleyways',
    description: 'Urban asphalt where underground street musicians duel.',
    icon: 'business',
    minLevel: 1,
    boss: {
      id: 'boss_static_spectre',
      name: 'Static Spectre',
      title: 'Distortion Fiend',
      level: 1,
      hp: 200,
      maxHp: 200,
      avatar: 'bug',
      tempoBpm: 100,
      description: 'Emits harsh buzzing frequency interference.',
      rewardXp: 100,
      rewardGems: 50
    },
    quests: [
      {
        id: 'q1',
        title: 'Street Corner Warmup',
        description: 'Perform your first acoustic set to clear the street static.',
        regionId: 'region_sound_city',
        requiredLevel: 1,
        rewardXp: 50,
        rewardGems: 25,
        isCompleted: false
      },
      {
        id: 'q2',
        title: 'Banish the Static Spectre',
        description: 'Defeat Static Spectre in a rhythm duel to unlock Bass Caverns.',
        regionId: 'region_sound_city',
        requiredLevel: 1,
        rewardXp: 100,
        rewardGems: 50,
        targetBossId: 'boss_static_spectre',
        isCompleted: false
      }
    ]
  },
  {
    id: 'region_bass_caverns',
    name: 'Sub-Zero Bass Caverns',
    description: 'Deep subterranean caves vibrating with heavy 808 bass.',
    icon: 'earth',
    minLevel: 3,
    boss: {
      id: 'boss_subwoofer_titan',
      name: 'Subwoofer Titan',
      title: 'Lord of Low Frequency',
      level: 3,
      hp: 450,
      maxHp: 450,
      avatar: 'disc',
      tempoBpm: 120,
      description: 'Rumbles caverns with earth-shattering bass drops.',
      rewardXp: 250,
      rewardGems: 120
    },
    quests: [
      {
        id: 'q3',
        title: 'Echoes in the Dark',
        description: 'Master low tempo rhythms inside the resonant cave.',
        regionId: 'region_bass_caverns',
        requiredLevel: 3,
        rewardXp: 150,
        rewardGems: 75,
        isCompleted: false
      },
      {
        id: 'q4',
        title: 'Overpower the Subwoofer Titan',
        description: 'Match the Titan bass drops with precision rhythm hits.',
        regionId: 'region_bass_caverns',
        requiredLevel: 3,
        rewardXp: 250,
        rewardGems: 120,
        targetBossId: 'boss_subwoofer_titan',
        isCompleted: false
      }
    ]
  },
  {
    id: 'region_synth_citadel',
    name: 'Synth Citadel',
    description: 'Neon sky-towers lit by glowing synthwaves and laser lights.',
    icon: 'flash',
    minLevel: 5,
    boss: {
      id: 'boss_silence_syndicate',
      name: 'Silence Overlord',
      title: 'Master of Mute',
      level: 5,
      hp: 800,
      maxHp: 800,
      avatar: 'skull',
      tempoBpm: 140,
      description: 'Seeks to mute all music across the MMORPG universe.',
      rewardXp: 500,
      rewardGems: 300
    },
    quests: [
      {
        id: 'q5',
        title: 'Laser Synths & Neon Beat',
        description: 'Play fast 140 BPM synth arpeggios on top of the citadel.',
        regionId: 'region_synth_citadel',
        requiredLevel: 5,
        rewardXp: 300,
        rewardGems: 150,
        isCompleted: false
      },
      {
        id: 'q6',
        title: 'Endless World Harmony',
        description: 'Defeat Silence Overlord and restore harmony to the universe.',
        regionId: 'region_synth_citadel',
        requiredLevel: 5,
        rewardXp: 500,
        rewardGems: 300,
        targetBossId: 'boss_silence_syndicate',
        isCompleted: false
      }
    ]
  }
];

export const INITIAL_PLAYER: PlayerProfile = {
  name: 'MelodyHero',
  characterClass: 'Bard',
  avatar: 'headset',
  level: 1,
  xp: 0,
  nextLevelXp: 100,
  harmonyGems: 100,
  stats: {
    hp: 100,
    maxHp: 100,
    energy: 100,
    maxEnergy: 100,
    rhythm: 20,
    pitch: 10,
    power: 15,
    harmony: 10
  },
  equipped: {
    instrument: INITIAL_ITEMS[0]
  },
  inventory: [INITIAL_ITEMS[0], INITIAL_ITEMS[5]],
  completedQuestIds: [],
  band: {
    id: 'band_cyber_titans',
    name: 'Cyber Beats Guild',
    motto: 'Loud, Proud & Unstoppable',
    level: 1,
    concertPower: 1200,
    totalConcertsWon: 3,
    members: [
      { id: 'p1', name: 'MelodyHero', role: 'Bard', level: 1, contributionPoints: 150 },
      { id: 'p2', name: 'DJ_Aura', role: 'DJ', level: 4, contributionPoints: 450 },
      { id: 'p3', name: 'Vox_Rider', role: 'Vocalist', level: 3, contributionPoints: 320 }
    ]
  }
};

export const INITIAL_SETTINGS: GameSettings = {
  soundEnabled: true,
  volume: 0.8,
  keyboardControls: {
    lane1: 'a',
    lane2: 's',
    lane3: 'd',
    lane4: 'f'
  }
};

interface GameContextType {
  player: PlayerProfile;
  settings: GameSettings;
  regions: Region[];
  activeBoss: Boss | null;
  setActiveBoss: (boss: Boss | null) => void;
  gainXpAndGems: (xpAmount: number, gemsAmount: number) => void;
  buyItem: (item: InventoryItem) => boolean;
  equipItem: (item: InventoryItem) => void;
  useConsumable: (item: InventoryItem) => void;
  completeQuest: (questId: string) => void;
  changeClass: (newClass: CharacterClass) => void;
  updateSettings: (newSettings: Partial<GameSettings>) => void;
  resetProgress: () => void;
  restoreEnergyAndHp: () => void;
}

const GameContext = createContext<GameContextType | undefined>(undefined);

const sanitizeNum = (val: unknown, fallback: number): number => {
  return typeof val === 'number' && Number.isFinite(val) && val >= 0 ? val : fallback;
};

// Security Helper: Validate and sanitize untrusted deserialized state from storage
const sanitizePlayerData = (raw: unknown): PlayerProfile => {
  if (!raw || typeof raw !== 'object' || Array.isArray(raw)) {
    return INITIAL_PLAYER;
  }

  const p = raw as Partial<PlayerProfile>;
  const stats = p.stats && typeof p.stats === 'object' ? (p.stats as Partial<PlayerProfile['stats']>) : {};
  const equipped = p.equipped && typeof p.equipped === 'object' ? p.equipped : {};

  return {
    ...INITIAL_PLAYER,
    ...p,
    level: sanitizeNum(p.level, INITIAL_PLAYER.level),
    xp: sanitizeNum(p.xp, INITIAL_PLAYER.xp),
    nextLevelXp: sanitizeNum(p.nextLevelXp, INITIAL_PLAYER.nextLevelXp),
    harmonyGems: sanitizeNum(p.harmonyGems, INITIAL_PLAYER.harmonyGems),
    stats: {
      ...INITIAL_PLAYER.stats,
      ...stats,
      hp: sanitizeNum(stats.hp, INITIAL_PLAYER.stats.hp),
      maxHp: sanitizeNum(stats.maxHp, INITIAL_PLAYER.stats.maxHp),
      energy: sanitizeNum(stats.energy, INITIAL_PLAYER.stats.energy),
      maxEnergy: sanitizeNum(stats.maxEnergy, INITIAL_PLAYER.stats.maxEnergy),
      rhythm: sanitizeNum(stats.rhythm, INITIAL_PLAYER.stats.rhythm),
      pitch: sanitizeNum(stats.pitch, INITIAL_PLAYER.stats.pitch),
      power: sanitizeNum(stats.power, INITIAL_PLAYER.stats.power),
      harmony: sanitizeNum(stats.harmony, INITIAL_PLAYER.stats.harmony),
    },
    equipped: {
      ...INITIAL_PLAYER.equipped,
      ...equipped,
    },
    inventory: Array.isArray(p.inventory) ? p.inventory : INITIAL_PLAYER.inventory,
    completedQuestIds: Array.isArray(p.completedQuestIds) ? p.completedQuestIds : INITIAL_PLAYER.completedQuestIds,
  };
};

export const GameProvider: React.FC<{ children: React.ReactNode }> = ({ children }) => {
  const [player, setPlayer] = useState<PlayerProfile>(INITIAL_PLAYER);
  const [settings, setSettings] = useState<GameSettings>(INITIAL_SETTINGS);
  const [regions, setRegions] = useState<Region[]>(INITIAL_REGIONS);
  const [activeBoss, setActiveBoss] = useState<Boss | null>(null);

  useEffect(() => {
    loadSavedData();
  }, []);

  useEffect(() => {
    saveData();
  }, [player, settings]);

  const loadSavedData = async () => {
    try {
      const savedPlayer = await AsyncStorage.getItem(STORAGE_KEY_PLAYER);
      const savedSettings = await AsyncStorage.getItem(STORAGE_KEY_SETTINGS);

      if (savedPlayer) {
        setPlayer(sanitizePlayerData(JSON.parse(savedPlayer)));
      }
      if (savedSettings) {
        setSettings(JSON.parse(savedSettings));
      }
    } catch (e) {
      console.error('Failed to load saved game state', e);
    }
  };

  const saveData = async () => {
    try {
      await AsyncStorage.setItem(STORAGE_KEY_PLAYER, JSON.stringify(player));
      await AsyncStorage.setItem(STORAGE_KEY_SETTINGS, JSON.stringify(settings));
    } catch (e) {
      console.error('Failed to save game state', e);
    }
  };

  const gainXpAndGems = (xpAmount: number, gemsAmount: number) => {
    setPlayer((prev) => {
      let newXp = prev.xp + xpAmount;
      let newLevel = prev.level;
      let newNextXp = prev.nextLevelXp;
      const newStats = { ...prev.stats };

      // Check level up
      while (newXp >= newNextXp) {
        newXp -= newNextXp;
        newLevel += 1;
        newNextXp = Math.floor(newNextXp * 1.5);
        newStats.maxHp += 20;
        newStats.hp = newStats.maxHp;
        newStats.maxEnergy += 10;
        newStats.energy = newStats.maxEnergy;
        newStats.rhythm += 5;
        newStats.pitch += 3;
        newStats.power += 4;
        newStats.harmony += 3;
      }

      return {
        ...prev,
        level: newLevel,
        xp: newXp,
        nextLevelXp: newNextXp,
        harmonyGems: prev.harmonyGems + gemsAmount,
        stats: newStats
      };
    });
  };

  const buyItem = (item: InventoryItem): boolean => {
    // Security: Validate price is a valid, finite non-negative number to prevent gem manipulation
    if (typeof item.price !== 'number' || !Number.isFinite(item.price) || item.price < 0) return false;
    if (player.harmonyGems < item.price) return false;

    setPlayer((prev) => {
      if (prev.harmonyGems < item.price) return prev;
      const updatedInventory = [...prev.inventory, item];
      return {
        ...prev,
        harmonyGems: prev.harmonyGems - item.price,
        inventory: updatedInventory
      };
    });
    return true;
  };

  const equipItem = (item: InventoryItem) => {
    if (item.type === 'consumable') return;

    setPlayer((prev) => {
      const slot = item.type as keyof typeof prev.equipped;
      if (!slot) return prev;

      const currentEquipped = prev.equipped[slot];
      const updatedInventory = prev.inventory.map((inv) => {
        if (inv.id === item.id) return { ...inv, isEquipped: true };
        if (currentEquipped && inv.id === currentEquipped.id) return { ...inv, isEquipped: false };
        return inv;
      });

      const newEquipped = { ...prev.equipped, [slot]: item };
      return {
        ...prev,
        equipped: newEquipped,
        inventory: updatedInventory
      };
    });
  };

  const useConsumable = (item: InventoryItem) => {
    setPlayer((prev) => {
      const invIndex = prev.inventory.findIndex((i) => i.id === item.id);
      if (invIndex === -1) return prev;

      const newInv = [...prev.inventory];
      newInv.splice(invIndex, 1);

      const restoreHp = item.statBonus.hp || 0;
      const restoreEnergy = item.statBonus.energy || 0;

      return {
        ...prev,
        inventory: newInv,
        stats: {
          ...prev.stats,
          hp: Math.min(prev.stats.maxHp, prev.stats.hp + restoreHp),
          energy: Math.min(prev.stats.maxEnergy, prev.stats.energy + restoreEnergy)
        }
      };
    });
  };

  const completeQuest = (questId: string) => {
    if (player.completedQuestIds.includes(questId)) return;

    let rewardXp = 0;
    let rewardGems = 0;

    regions.forEach((reg) => {
      reg.quests.forEach((q) => {
        if (q.id === questId) {
          rewardXp = q.rewardXp;
          rewardGems = q.rewardGems;
        }
      });
    });

    setPlayer((prev) => ({
      ...prev,
      completedQuestIds: [...prev.completedQuestIds, questId]
    }));

    gainXpAndGems(rewardXp, rewardGems);
  };

  const changeClass = (newClass: CharacterClass) => {
    setPlayer((prev) => ({
      ...prev,
      characterClass: newClass
    }));
  };

  const updateSettings = (newSettings: Partial<GameSettings>) => {
    setSettings((prev) => ({ ...prev, ...newSettings }));
  };

  const resetProgress = () => {
    setPlayer(INITIAL_PLAYER);
    setSettings(INITIAL_SETTINGS);
    setRegions(INITIAL_REGIONS);
  };

  const restoreEnergyAndHp = () => {
    setPlayer((prev) => ({
      ...prev,
      stats: {
        ...prev.stats,
        hp: prev.stats.maxHp,
        energy: prev.stats.maxEnergy
      }
    }));
  };

  return (
    <GameContext.Provider
      value={{
        player,
        settings,
        regions,
        activeBoss,
        setActiveBoss,
        gainXpAndGems,
        buyItem,
        equipItem,
        useConsumable,
        completeQuest,
        changeClass,
        updateSettings,
        resetProgress,
        restoreEnergyAndHp
      }}
    >
      {children}
    </GameContext.Provider>
  );
};

export const useGame = () => {
  const context = useContext(GameContext);
  if (!context) {
    throw new Error('useGame must be used within a GameProvider');
  }
  return context;
};

export type CharacterClass = 'Bard' | 'Beatmaker' | 'Vocalist' | 'Guitarist' | 'DJ';

export interface PlayerStats {
  hp: number;
  maxHp: number;
  energy: number;
  maxEnergy: number;
  rhythm: number; // Attack power in rhythm combat
  pitch: number;  // Critical chance / accuracy boost
  power: number;  // Special ability power
  harmony: number;// Defense / healing multiplier
}

export type ItemType = 'instrument' | 'amplifier' | 'microphone' | 'pedal' | 'consumable';

export interface InventoryItem {
  id: string;
  name: string;
  type: ItemType;
  description: string;
  price: number;
  icon: string;
  statBonus: Partial<PlayerStats>;
  rarity: 'Common' | 'Rare' | 'Epic' | 'Legendary';
  isEquipped?: boolean;
}

export interface Quest {
  id: string;
  title: string;
  description: string;
  regionId: string;
  requiredLevel: number;
  rewardXp: number;
  rewardGems: number;
  rewardItem?: InventoryItem;
  isCompleted: boolean;
  targetBossId?: string;
}

export interface Boss {
  id: string;
  name: string;
  title: string;
  level: number;
  hp: number;
  maxHp: number;
  avatar: string;
  tempoBpm: number;
  description: string;
  rewardXp: number;
  rewardGems: number;
}

export interface Region {
  id: string;
  name: string;
  description: string;
  icon: string;
  minLevel: number;
  boss: Boss;
  quests: Quest[];
}

export interface BandMember {
  id: string;
  name: string;
  role: CharacterClass;
  level: number;
  contributionPoints: number;
}

export interface Band {
  id: string;
  name: string;
  motto: string;
  level: number;
  members: BandMember[];
  concertPower: number;
  totalConcertsWon: number;
}

export interface PlayerProfile {
  name: string;
  characterClass: CharacterClass;
  avatar: string;
  level: number;
  xp: number;
  nextLevelXp: number;
  harmonyGems: number;
  stats: PlayerStats;
  equipped: {
    instrument?: InventoryItem;
    amplifier?: InventoryItem;
    microphone?: InventoryItem;
    pedal?: InventoryItem;
  };
  inventory: InventoryItem[];
  completedQuestIds: string[];
  band?: Band;
}

export interface GameSettings {
  soundEnabled: boolean;
  volume: number;
  keyboardControls: {
    lane1: string;
    lane2: string;
    lane3: string;
    lane4: string;
  };
}

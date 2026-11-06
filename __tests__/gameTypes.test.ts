/**
 * Unit tests for game type definitions
 */

import {
  CharacterClass,
  PlayerStats,
  ItemType,
  InventoryItem,
  Quest,
  Boss,
  Region,
  BandMember,
  Band,
  PlayerProfile,
  GameSettings,
} from '../src/types/game';

describe('Game Types', () => {
  describe('CharacterClass', () => {
    it('should have all expected character classes', () => {
      const expectedClasses: CharacterClass[] = [
        'Bard',
        'Beatmaker',
        'Vocalist',
        'Guitarist',
        'DJ',
      ];
      expectedClasses.forEach((cls) => {
        expect(cls).toBeDefined();
      });
    });
  });

  describe('PlayerStats', () => {
    it('should have all required stat properties', () => {
      const stats: PlayerStats = {
        hp: 100,
        maxHp: 100,
        energy: 100,
        maxEnergy: 100,
        rhythm: 20,
        pitch: 10,
        power: 15,
        harmony: 10,
      };
      expect(stats.hp).toBeDefined();
      expect(stats.maxHp).toBeDefined();
      expect(stats.energy).toBeDefined();
      expect(stats.maxEnergy).toBeDefined();
      expect(stats.rhythm).toBeDefined();
      expect(stats.pitch).toBeDefined();
      expect(stats.power).toBeDefined();
      expect(stats.harmony).toBeDefined();
    });
  });

  describe('ItemType', () => {
    it('should have all expected item types', () => {
      const expectedTypes: ItemType[] = [
        'instrument',
        'amplifier',
        'microphone',
        'pedal',
        'consumable',
      ];
      expectedTypes.forEach((type) => {
        expect(type).toBeDefined();
      });
    });
  });

  describe('InventoryItem', () => {
    it('should create a valid instrument item', () => {
      const item: InventoryItem = {
        id: 'test_guitar',
        name: 'Test Guitar',
        type: 'instrument',
        description: 'A test instrument',
        price: 100,
        icon: 'musical-notes',
        statBonus: { rhythm: 10, pitch: 5 },
        rarity: 'Rare',
        isEquipped: false,
      };
      expect(item.id).toBe('test_guitar');
      expect(item.type).toBe('instrument');
      expect(item.price).toBe(100);
      expect(item.rarity).toBe('Rare');
    });

    it('should create a valid consumable item', () => {
      const item: InventoryItem = {
        id: 'test_potion',
        name: 'Health Potion',
        type: 'consumable',
        description: 'Restores health',
        price: 50,
        icon: 'flash',
        statBonus: { hp: 50 },
        rarity: 'Common',
      };
      expect(item.type).toBe('consumable');
      expect(item.statBonus.hp).toBe(50);
    });
  });

  describe('Boss', () => {
    it('should create a valid boss with all properties', () => {
      const boss: Boss = {
        id: 'boss_test',
        name: 'Test Boss',
        title: 'The Tester',
        level: 1,
        hp: 100,
        maxHp: 100,
        avatar: 'skull',
        tempoBpm: 120,
        description: 'A test boss',
        rewardXp: 50,
        rewardGems: 25,
      };
      expect(boss.id).toBe('boss_test');
      expect(boss.name).toBe('Test Boss');
      expect(boss.level).toBe(1);
      expect(boss.hp).toBe(100);
      expect(boss.maxHp).toBe(100);
      expect(boss.tempoBpm).toBe(120);
      expect(boss.rewardXp).toBe(50);
      expect(boss.rewardGems).toBe(25);
    });
  });

  describe('Region', () => {
    it('should create a valid region with boss and quests', () => {
      const boss: Boss = {
        id: 'boss_test',
        name: 'Test Boss',
        title: 'The Tester',
        level: 1,
        hp: 100,
        maxHp: 100,
        avatar: 'skull',
        tempoBpm: 120,
        description: 'A test boss',
        rewardXp: 50,
        rewardGems: 25,
      };

      const quest: Quest = {
        id: 'quest_test',
        title: 'Test Quest',
        description: 'A test quest',
        regionId: 'region_test',
        requiredLevel: 1,
        rewardXp: 25,
        rewardGems: 10,
        isCompleted: false,
      };

      const region: Region = {
        id: 'region_test',
        name: 'Test Region',
        description: 'A test region',
        icon: 'map',
        minLevel: 1,
        boss: boss,
        quests: [quest],
      };

      expect(region.id).toBe('region_test');
      expect(region.name).toBe('Test Region');
      expect(region.boss).toBeDefined();
      expect(region.quests).toHaveLength(1);
      expect(region.minLevel).toBe(1);
    });
  });

  describe('PlayerProfile', () => {
    it('should create a valid player profile', () => {
      const player: PlayerProfile = {
        name: 'TestPlayer',
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
          harmony: 10,
        },
        equipped: {},
        inventory: [],
        completedQuestIds: [],
        band: undefined,
      };

      expect(player.name).toBe('TestPlayer');
      expect(player.characterClass).toBe('Bard');
      expect(player.level).toBe(1);
      expect(player.xp).toBe(0);
      expect(player.harmonyGems).toBe(100);
      expect(player.stats).toBeDefined();
      expect(player.inventory).toEqual([]);
      expect(player.completedQuestIds).toEqual([]);
    });

    it('should create a player with band', () => {
      const band: Band = {
        id: 'band_test',
        name: 'Test Band',
        motto: 'Test motto',
        level: 1,
        members: [],
        concertPower: 1000,
        totalConcertsWon: 0,
      };

      const player: PlayerProfile = {
        name: 'TestPlayer',
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
          harmony: 10,
        },
        equipped: {},
        inventory: [],
        completedQuestIds: [],
        band: band,
      };

      expect(player.band).toBeDefined();
      expect(player.band?.name).toBe('Test Band');
    });
  });

  describe('GameSettings', () => {
    it('should create valid game settings', () => {
      const settings: GameSettings = {
        soundEnabled: true,
        volume: 0.8,
        keyboardControls: {
          lane1: 'a',
          lane2: 's',
          lane3: 'd',
          lane4: 'f',
        },
      };

      expect(settings.soundEnabled).toBe(true);
      expect(settings.volume).toBe(0.8);
      expect(settings.keyboardControls.lane1).toBe('a');
      expect(settings.keyboardControls.lane2).toBe('s');
      expect(settings.keyboardControls.lane3).toBe('d');
      expect(settings.keyboardControls.lane4).toBe('f');
    });
  });
});

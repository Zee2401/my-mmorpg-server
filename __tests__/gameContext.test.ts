/**
 * Unit tests for GameContext utility functions and initial data
 */

import {
  INITIAL_PLAYER,
  INITIAL_SETTINGS,
  INITIAL_ITEMS,
  INITIAL_REGIONS,
} from '../src/context/GameContext';
import { PlayerProfile, InventoryItem, Region } from '../src/types/game';

describe('GameContext Initial Data', () => {
  describe('INITIAL_PLAYER', () => {
    it('should have correct initial player properties', () => {
      expect(INITIAL_PLAYER.name).toBe('MelodyHero');
      expect(INITIAL_PLAYER.characterClass).toBe('Bard');
      expect(INITIAL_PLAYER.avatar).toBe('headset');
      expect(INITIAL_PLAYER.level).toBe(1);
      expect(INITIAL_PLAYER.xp).toBe(0);
      expect(INITIAL_PLAYER.nextLevelXp).toBe(100);
      expect(INITIAL_PLAYER.harmonyGems).toBe(100);
    });

    it('should have correct initial stats', () => {
      expect(INITIAL_PLAYER.stats.hp).toBe(100);
      expect(INITIAL_PLAYER.stats.maxHp).toBe(100);
      expect(INITIAL_PLAYER.stats.energy).toBe(100);
      expect(INITIAL_PLAYER.stats.maxEnergy).toBe(100);
      expect(INITIAL_PLAYER.stats.rhythm).toBe(20);
      expect(INITIAL_PLAYER.stats.pitch).toBe(10);
      expect(INITIAL_PLAYER.stats.power).toBe(15);
      expect(INITIAL_PLAYER.stats.harmony).toBe(10);
    });

    it('should have starter guitar equipped', () => {
      expect(INITIAL_PLAYER.equipped.instrument).toBeDefined();
      expect(INITIAL_PLAYER.equipped.instrument?.id).toBe('item_starter_guitar');
    });

    it('should have starter items in inventory', () => {
      expect(INITIAL_PLAYER.inventory.length).toBeGreaterThan(0);
      expect(INITIAL_PLAYER.inventory[0].id).toBe('item_starter_guitar');
    });

    it('should have a band', () => {
      expect(INITIAL_PLAYER.band).toBeDefined();
      expect(INITIAL_PLAYER.band?.name).toBe('Cyber Beats Guild');
      expect(INITIAL_PLAYER.band?.level).toBe(1);
    });
  });

  describe('INITIAL_SETTINGS', () => {
    it('should have correct default settings', () => {
      expect(INITIAL_SETTINGS.soundEnabled).toBe(true);
      expect(INITIAL_SETTINGS.volume).toBe(0.8);
      expect(INITIAL_SETTINGS.keyboardControls.lane1).toBe('a');
      expect(INITIAL_SETTINGS.keyboardControls.lane2).toBe('s');
      expect(INITIAL_SETTINGS.keyboardControls.lane3).toBe('d');
      expect(INITIAL_SETTINGS.keyboardControls.lane4).toBe('f');
    });
  });

  describe('INITIAL_ITEMS', () => {
    it('should have multiple items', () => {
      expect(INITIAL_ITEMS.length).toBeGreaterThan(0);
    });

    it('should have starter guitar', () => {
      const starterGuitar = INITIAL_ITEMS.find((item) => item.id === 'item_starter_guitar');
      expect(starterGuitar).toBeDefined();
      expect(starterGuitar?.name).toBe('Acoustic Starter Guitar');
      expect(starterGuitar?.type).toBe('instrument');
      expect(starterGuitar?.price).toBe(0);
      expect(starterGuitar?.rarity).toBe('Common');
    });

    it('should have energy drink consumable', () => {
      const energyDrink = INITIAL_ITEMS.find((item) => item.id === 'item_energy_drink');
      expect(energyDrink).toBeDefined();
      expect(energyDrink?.type).toBe('consumable');
      expect(energyDrink?.name).toBe('Beat Energy Elixir');
      expect(energyDrink?.statBonus.energy).toBe(50);
      expect(energyDrink?.statBonus.hp).toBe(100);
    });

    it('should have items with different rarities', () => {
      const rarities = INITIAL_ITEMS.map((item) => item.rarity);
      expect(rarities).toContain('Common');
      expect(rarities).toContain('Rare');
      expect(rarities).toContain('Epic');
    });

    it('should have items with stat bonuses', () => {
      INITIAL_ITEMS.forEach((item) => {
        expect(item.statBonus).toBeDefined();
        expect(typeof item.statBonus).toBe('object');
      });
    });
  });

  describe('INITIAL_REGIONS', () => {
    it('should have multiple regions', () => {
      expect(INITIAL_REGIONS.length).toBeGreaterThan(0);
    });

    it('should have Sound City Alleyways', () => {
      const soundCity = INITIAL_REGIONS.find((r) => r.id === 'region_sound_city');
      expect(soundCity).toBeDefined();
      expect(soundCity?.name).toBe('Sound City Alleyways');
      expect(soundCity?.minLevel).toBe(1);
    });

    it('should have Sub-Zero Bass Caverns', () => {
      const bassCaverns = INITIAL_REGIONS.find((r) => r.id === 'region_bass_caverns');
      expect(bassCaverns).toBeDefined();
      expect(bassCaverns?.name).toBe('Sub-Zero Bass Caverns');
      expect(bassCaverns?.minLevel).toBe(3);
    });

    it('should have Synth Citadel', () => {
      const synthCitadel = INITIAL_REGIONS.find((r) => r.id === 'region_synth_citadel');
      expect(synthCitadel).toBeDefined();
      expect(synthCitadel?.name).toBe('Synth Citadel');
      expect(synthCitadel?.minLevel).toBe(5);
    });

    it('each region should have a boss', () => {
      INITIAL_REGIONS.forEach((region) => {
        expect(region.boss).toBeDefined();
        expect(region.boss.id).toBeDefined();
        expect(region.boss.name).toBeDefined();
        expect(region.boss.hp).toBeGreaterThan(0);
      });
    });

    it('each region should have quests', () => {
      INITIAL_REGIONS.forEach((region) => {
        expect(region.quests).toBeDefined();
        expect(Array.isArray(region.quests)).toBe(true);
        expect(region.quests.length).toBeGreaterThan(0);
      });
    });

    it('should have Static Spectre boss', () => {
      const soundCity = INITIAL_REGIONS.find((r) => r.id === 'region_sound_city');
      expect(soundCity?.boss.id).toBe('boss_static_spectre');
      expect(soundCity?.boss.name).toBe('Static Spectre');
      expect(soundCity?.boss.level).toBe(1);
    });

    it('should have Subwoofer Titan boss', () => {
      const bassCaverns = INITIAL_REGIONS.find((r) => r.id === 'region_bass_caverns');
      expect(bassCaverns?.boss.id).toBe('boss_subwoofer_titan');
      expect(bassCaverns?.boss.name).toBe('Subwoofer Titan');
      expect(bassCaverns?.boss.level).toBe(3);
    });

    it('should have Silence Overlord boss', () => {
      const synthCitadel = INITIAL_REGIONS.find((r) => r.id === 'region_synth_citadel');
      expect(synthCitadel?.boss.id).toBe('boss_silence_syndicate');
      expect(synthCitadel?.boss.name).toBe('Silence Overlord');
      expect(synthCitadel?.boss.level).toBe(5);
    });
  });
});

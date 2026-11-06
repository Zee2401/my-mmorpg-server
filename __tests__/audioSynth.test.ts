/**
 * Unit tests for AudioSynthesizer
 */

import { AudioSynthesizer } from '../src/utils/audioSynth';

describe('AudioSynthesizer', () => {
  let audioSynth: AudioSynthesizer;

  beforeEach(() => {
    audioSynth = new AudioSynthesizer();
  });

  describe('NOTE_FREQS', () => {
    it('should have correct note frequencies', () => {
      expect(audioSynth.NOTE_FREQS.C4).toBe(261.63);
      expect(audioSynth.NOTE_FREQS.D4).toBe(293.66);
      expect(audioSynth.NOTE_FREQS.E4).toBe(329.63);
      expect(audioSynth.NOTE_FREQS.F4).toBe(349.23);
      expect(audioSynth.NOTE_FREQS.G4).toBe(392.0);
      expect(audioSynth.NOTE_FREQS.A4).toBe(440.0);
      expect(audioSynth.NOTE_FREQS.B4).toBe(493.88);
      expect(audioSynth.NOTE_FREQS.C5).toBe(523.25);
      expect(audioSynth.NOTE_FREQS.D5).toBe(587.33);
      expect(audioSynth.NOTE_FREQS.E5).toBe(659.25);
    });
  });

  describe('playNote', () => {
    it('should not throw when called with valid note', () => {
      // These tests just verify the methods don't throw errors
      // In a real test environment with AudioContext, they would play sounds
      expect(() => audioSynth.playNote('C4')).not.toThrow();
    });

    it('should not throw when called with numeric frequency', () => {
      expect(() => audioSynth.playNote(440)).not.toThrow();
    });

    it('should handle unknown note gracefully', () => {
      expect(() => audioSynth.playNote('UNKNOWN')).not.toThrow();
    });
  });

  describe('playDrum', () => {
    it('should not throw when called with valid drum type', () => {
      expect(() => audioSynth.playDrum('kick')).not.toThrow();
      expect(() => audioSynth.playDrum('snare')).not.toThrow();
      expect(() => audioSynth.playDrum('hihat')).not.toThrow();
      expect(() => audioSynth.playDrum('synth')).not.toThrow();
    });
  });

  describe('playHitSound', () => {
    it('should not throw when called', () => {
      expect(() => audioSynth.playHitSound()).not.toThrow();
      expect(() => audioSynth.playHitSound(true)).not.toThrow();
      expect(() => audioSynth.playHitSound(false)).not.toThrow();
    });
  });

  describe('playMissSound', () => {
    it('should not throw when called', () => {
      expect(() => audioSynth.playMissSound()).not.toThrow();
    });
  });

  describe('playAbilitySound', () => {
    it('should not throw when called', () => {
      expect(() => audioSynth.playAbilitySound()).not.toThrow();
    });
  });
});

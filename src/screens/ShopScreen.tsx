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
import { useGame, INITIAL_ITEMS } from '../context/GameContext';
import { InventoryItem } from '../types/game';
import { audioSynth } from '../utils/audioSynth';

export const ShopScreen = () => {
  const { player, buyItem, equipItem, useConsumable } = useGame();
  const [activeTab, setActiveTab] = useState<'shop' | 'inventory'>('shop');

  const handleBuy = (item: InventoryItem) => {
    const success = buyItem(item);
    if (success) {
      audioSynth.playHitSound(true);
      Alert.alert('Purchase Successful!', `Purchased ${item.name} for ${item.price} Harmony Gems.`);
    } else {
      Alert.alert('Insufficient Gems', `You need ${item.price} Harmony Gems to buy ${item.name}.`);
    }
  };

  const handleEquip = (item: InventoryItem) => {
    equipItem(item);
    audioSynth.playNote('C5', 'triangle', 0.2, 0.2);
  };

  const handleUse = (item: InventoryItem) => {
    useConsumable(item);
    audioSynth.playNote('E5', 'sine', 0.3, 0.2);
    Alert.alert('Item Used', `Restored HP & Energy with ${item.name}.`);
  };

  return (
    <SafeAreaView style={styles.container}>
      <ScrollView contentContainerStyle={styles.scrollContent}>
        {/* Header */}
        <View style={styles.header}>
          <View>
            <Text style={styles.title}>CONCERT GEAR & SHOP</Text>
            <Text style={styles.subtitle}>Equip epic instruments & boost combat attributes</Text>
          </View>
          <View style={styles.gemBadge}>
            <Ionicons name="diamond" size={18} color="#FFD700" />
            <Text style={styles.gemText}>{player.harmonyGems}</Text>
          </View>
        </View>

        {/* Tab Switcher */}
        <View style={styles.tabContainer}>
          <TouchableOpacity
            style={[styles.tabButton, activeTab === 'shop' && styles.activeTabButton]}
            onPress={() => setActiveTab('shop')}
            accessibilityRole="tab"
            accessibilityState={{ selected: activeTab === 'shop' }}
            accessibilityLabel="Catalog Store"
            accessibilityHint="Switch to Catalog Store tab"
          >
            <Ionicons name="cart" size={18} color={activeTab === 'shop' ? '#00F0FF' : '#8888AA'} />
            <Text style={[styles.tabText, activeTab === 'shop' && styles.activeTabText]}>
              Catalog Store
            </Text>
          </TouchableOpacity>

          <TouchableOpacity
            style={[styles.tabButton, activeTab === 'inventory' && styles.activeTabButton]}
            onPress={() => setActiveTab('inventory')}
            accessibilityRole="tab"
            accessibilityState={{ selected: activeTab === 'inventory' }}
            accessibilityLabel={`My Inventory, ${player.inventory.length} items`}
            accessibilityHint="Switch to My Inventory tab"
          >
            <Ionicons name="briefcase" size={18} color={activeTab === 'inventory' ? '#00F0FF' : '#8888AA'} />
            <Text style={[styles.tabText, activeTab === 'inventory' && styles.activeTabText]}>
              My Inventory ({player.inventory.length})
            </Text>
          </TouchableOpacity>
        </View>

        {/* Catalog Store Tab */}
        {activeTab === 'shop' ? (
          <View style={styles.itemsList}>
            {INITIAL_ITEMS.map((item) => {
              const isOwned = player.inventory.some((i) => i.id === item.id);

              return (
                <View key={item.id} style={styles.itemCard}>
                  <View style={styles.itemIconContainer}>
                    <Ionicons name={item.icon as keyof typeof Ionicons.glyphMap} size={30} color="#00F0FF" />
                  </View>

                  <View style={{ marginLeft: 12, flex: 1 }}>
                    <View style={{ flexDirection: 'row', alignItems: 'center' }}>
                      <Text style={styles.itemName}>{item.name}</Text>
                      <View style={[styles.rarityBadge, { borderColor: getRarityColor(item.rarity) }]}>
                        <Text style={[styles.rarityText, { color: getRarityColor(item.rarity) }]}>
                          {item.rarity}
                        </Text>
                      </View>
                    </View>
                    <Text style={styles.itemDesc}>{item.description}</Text>

                    {/* Stat Bonuses */}
                    <Text style={styles.statBonusText}>
                      {Object.entries(item.statBonus)
                        .map(([k, v]) => `+${v} ${k.toUpperCase()}`)
                        .join(' • ')}
                    </Text>
                  </View>

                  {/* Buy Button or Owned */}
                  {isOwned ? (
                    <View style={styles.ownedBadge}>
                      <Text style={styles.ownedText}>Owned</Text>
                    </View>
                  ) : (
                    <TouchableOpacity
                      style={styles.buyButton}
                      onPress={() => handleBuy(item)}
                      accessibilityRole="button"
                      accessibilityLabel={`Buy ${item.name} for ${item.price} Harmony Gems`}
                      accessibilityHint="Purchases item and adds it to your inventory"
                    >
                      <Ionicons name="diamond" size={14} color="#12121A" />
                      <Text style={styles.buyButtonText}>{item.price}</Text>
                    </TouchableOpacity>
                  )}
                </View>
              );
            })}
          </View>
        ) : (
          /* Inventory Tab */
          <View style={styles.itemsList}>
            {player.inventory.map((item, idx) => {
              const isEquipped = item.isEquipped;

              return (
                <View key={`${item.id}_${idx}`} style={styles.itemCard}>
                  <View style={styles.itemIconContainer}>
                    <Ionicons name={item.icon as keyof typeof Ionicons.glyphMap} size={30} color="#00F0FF" />
                  </View>

                  <View style={{ marginLeft: 12, flex: 1 }}>
                    <View style={{ flexDirection: 'row', alignItems: 'center' }}>
                      <Text style={styles.itemName}>{item.name}</Text>
                      <View style={[styles.rarityBadge, { borderColor: getRarityColor(item.rarity) }]}>
                        <Text style={[styles.rarityText, { color: getRarityColor(item.rarity) }]}>
                          {item.rarity}
                        </Text>
                      </View>
                    </View>
                    <Text style={styles.itemDesc}>{item.description}</Text>
                  </View>

                  {/* Action Button */}
                  {item.type === 'consumable' ? (
                    <TouchableOpacity
                      style={styles.useButton}
                      onPress={() => handleUse(item)}
                      accessibilityRole="button"
                      accessibilityLabel={`Use ${item.name}`}
                      accessibilityHint="Consumes item to restore HP and Energy"
                    >
                      <Text style={styles.useButtonText}>Use</Text>
                    </TouchableOpacity>
                  ) : isEquipped ? (
                    <View style={styles.equippedBadge}>
                      <Ionicons name="checkmark-circle" size={16} color="#39FF14" />
                      <Text style={styles.equippedText}>Equipped</Text>
                    </View>
                  ) : (
                    <TouchableOpacity
                      style={styles.equipButton}
                      onPress={() => handleEquip(item)}
                      accessibilityRole="button"
                      accessibilityLabel={`Equip ${item.name}`}
                      accessibilityHint="Equips item and applies stat bonuses"
                    >
                      <Text style={styles.equipButtonText}>Equip</Text>
                    </TouchableOpacity>
                  )}
                </View>
              );
            })}
          </View>
        )}
      </ScrollView>
    </SafeAreaView>
  );
};

const getRarityColor = (rarity: string) => {
  switch (rarity) {
    case 'Common': return '#AAAAAA';
    case 'Rare': return '#00F0FF';
    case 'Epic': return '#BF55EC';
    case 'Legendary': return '#FFD700';
    default: return '#FFFFFF';
  }
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
  title: { fontSize: 22, fontWeight: '900', color: '#00F0FF', letterSpacing: 1 },
  subtitle: { fontSize: 12, color: '#8888AA' },
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
  tabContainer: {
    flexDirection: 'row',
    backgroundColor: '#1A1A28',
    borderRadius: 12,
    padding: 4,
    marginBottom: 16,
  },
  tabButton: {
    flex: 1,
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
    paddingVertical: 10,
    borderRadius: 10,
  },
  activeTabButton: { backgroundColor: '#00F0FF15' },
  tabText: { color: '#8888AA', fontWeight: '600', fontSize: 13, marginLeft: 6 },
  activeTabText: { color: '#00F0FF', fontWeight: 'bold' },
  itemsList: { marginBottom: 20 },
  itemCard: {
    backgroundColor: '#1A1A28',
    borderRadius: 14,
    padding: 12,
    marginBottom: 12,
    flexDirection: 'row',
    alignItems: 'center',
    borderWidth: 1,
    borderColor: '#2A2A3C',
  },
  itemIconContainer: {
    width: 48,
    height: 48,
    borderRadius: 24,
    backgroundColor: '#0A0A12',
    justifyContent: 'center',
    alignItems: 'center',
    borderWidth: 1,
    borderColor: '#00F0FF33',
  },
  itemName: { color: '#FFFFFF', fontWeight: 'bold', fontSize: 14 },
  itemDesc: { color: '#8888AA', fontSize: 11, marginVertical: 3 },
  statBonusText: { color: '#39FF14', fontSize: 10, fontWeight: 'bold' },
  rarityBadge: { marginLeft: 6, paddingHorizontal: 6, paddingVertical: 2, borderRadius: 6, borderWidth: 1 },
  rarityText: { fontSize: 9, fontWeight: 'bold' },
  ownedBadge: { backgroundColor: '#333344', paddingHorizontal: 10, paddingVertical: 6, borderRadius: 8 },
  ownedText: { color: '#8888AA', fontSize: 11, fontWeight: '600' },
  buyButton: {
    backgroundColor: '#FFD700',
    flexDirection: 'row',
    alignItems: 'center',
    paddingHorizontal: 12,
    paddingVertical: 8,
    borderRadius: 8,
  },
  buyButtonText: { color: '#12121A', fontWeight: 'bold', fontSize: 12, marginLeft: 4 },
  equipButton: { backgroundColor: '#00F0FF', paddingHorizontal: 12, paddingVertical: 8, borderRadius: 8 },
  equipButtonText: { color: '#12121A', fontWeight: 'bold', fontSize: 12 },
  equippedBadge: { flexDirection: 'row', alignItems: 'center' },
  equippedText: { color: '#39FF14', fontWeight: 'bold', fontSize: 11, marginLeft: 4 },
  useButton: { backgroundColor: '#39FF14', paddingHorizontal: 12, paddingVertical: 8, borderRadius: 8 },
  useButtonText: { color: '#12121A', fontWeight: 'bold', fontSize: 12 },
});

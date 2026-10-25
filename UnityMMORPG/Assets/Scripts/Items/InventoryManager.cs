using Mirror;
using System;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Manages player inventory and items
/// </summary>
public class InventoryManager : NetworkBehaviour
{
    public static InventoryManager Instance { get; private set; }
    
    [Header("Inventory Settings")]
    public int maxInventorySlots = 50;
    public int maxEquipmentSlots = 4;
    
    [Header("References")]
    public List<ItemData> allItems = new List<ItemData>();
    public Transform itemDropPrefab;
    
    [SyncVar(hook = nameof(OnInventoryChanged))]
    public List<InventorySlot> inventorySlots = new List<InventorySlot>();
    
    [SyncVar(hook = nameof(OnEquipmentChanged))]
    public List<EquipmentSlot> equipmentSlots = new List<EquipmentSlot>();
    
    [SyncVar]
    public int harmonyGems = 100;
    
    private Dictionary<string, ItemData> itemDatabase = new Dictionary<string, ItemData>();
    private PlayerController localPlayer;
    
    public override void Awake()
    {
        base.Awake();
        Instance = this;
        
        // Initialize item database
        foreach (var item in allItems)
        {
            itemDatabase[item.id] = item;
        }
    }
    
    private void Start()
    {
        // Find local player
        localPlayer = FindObjectOfType<PlayerController>();
        if (localPlayer != null && localPlayer.isLocalPlayer)
        {
            InitializeInventory(localPlayer);
        }
    }
    
    private void InitializeInventory(PlayerController player)
    {
        // Initialize inventory slots
        inventorySlots.Clear();
        for (int i = 0; i < maxInventorySlots; i++)
        {
            inventorySlots.Add(new InventorySlot
            {
                slotIndex = i,
                itemId = string.Empty,
                quantity = 0
            });
        }
        
        // Initialize equipment slots
        equipmentSlots.Clear();
        for (int i = 0; i < maxEquipmentSlots; i++)
        {
            equipmentSlots.Add(new EquipmentSlot
            {
                slotIndex = i,
                slotType = (EquipmentSlotType)i,
                itemId = string.Empty
            });
        }
        
        // Give starter items
        GiveStarterItems(player);
    }
    
    private void GiveStarterItems(PlayerController player)
    {
        // Give starter guitar
        AddItem("item_starter_guitar", 1);
        
        // Give energy drink
        AddItem("item_energy_drink", 5);
        
        // Equip starter guitar
        EquipItem("item_starter_guitar");
    }
    
    public ItemData GetItemData(string itemId)
    {
        itemDatabase.TryGetValue(itemId, out ItemData item);
        return item;
    }
    
    public bool HasItem(string itemId, int quantity = 1)
    {
        foreach (var slot in inventorySlots)
        {
            if (slot.itemId == itemId && slot.quantity >= quantity)
            {
                return true;
            }
        }
        return false;
    }
    
    public int GetItemQuantity(string itemId)
    {
        foreach (var slot in inventorySlots)
        {
            if (slot.itemId == itemId)
            {
                return slot.quantity;
            }
        }
        return 0;
    }
    
    public bool AddItem(string itemId, int quantity = 1)
    {
        if (isServer)
        {
            return InternalAddItem(itemId, quantity);
        }
        else
        {
            CmdAddItem(itemId, quantity);
            return true;
        }
    }
    
    [Command]
    private void CmdAddItem(string itemId, int quantity)
    {
        InternalAddItem(itemId, quantity);
    }
    
    private bool InternalAddItem(string itemId, int quantity)
    {
        var item = GetItemData(itemId);
        if (item == null)
        {
            Debug.LogWarning($"Item not found: {itemId}");
            return false;
        }
        
        // Check if item is stackable
        if (item.maxStack > 1)
        {
            // Find existing slot with same item
            foreach (var slot in inventorySlots)
            {
                if (slot.itemId == itemId && slot.quantity < item.maxStack)
                {
                    int space = item.maxStack - slot.quantity;
                    int addAmount = Mathf.Min(quantity, space);
                    
                    slot.quantity += addAmount;
                    quantity -= addAmount;
                    
                    if (quantity <= 0)
                    {
                        return true;
                    }
                }
            }
        }
        
        // Find empty slot
        foreach (var slot in inventorySlots)
        {
            if (string.IsNullOrEmpty(slot.itemId))
            {
                slot.itemId = itemId;
                slot.quantity = Mathf.Min(quantity, item.maxStack);
                quantity -= slot.quantity;
                
                if (quantity <= 0)
                {
                    return true;
                }
            }
        }
        
        // Not enough space
        Debug.LogWarning($"Not enough inventory space for {itemId}");
        return false;
    }
    
    public bool RemoveItem(string itemId, int quantity = 1)
    {
        if (isServer)
        {
            return InternalRemoveItem(itemId, quantity);
        }
        else
        {
            CmdRemoveItem(itemId, quantity);
            return true;
        }
    }
    
    [Command]
    private void CmdRemoveItem(string itemId, int quantity)
    {
        InternalRemoveItem(itemId, quantity);
    }
    
    private bool InternalRemoveItem(string itemId, int quantity)
    {
        // Remove from equipment first
        foreach (var slot in equipmentSlots)
        {
            if (slot.itemId == itemId)
            {
                UnequipItem(slot.slotIndex);
                quantity--;
                
                if (quantity <= 0)
                {
                    return true;
                }
            }
        }
        
        // Remove from inventory
        foreach (var slot in inventorySlots)
        {
            if (slot.itemId == itemId)
            {
                int removeAmount = Mathf.Min(slot.quantity, quantity);
                slot.quantity -= removeAmount;
                quantity -= removeAmount;
                
                if (slot.quantity <= 0)
                {
                    slot.itemId = string.Empty;
                    slot.quantity = 0;
                }
                
                if (quantity <= 0)
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    public bool UseItem(string itemId)
    {
        var item = GetItemData(itemId);
        if (item == null)
        {
            Debug.LogWarning($"Item not found: {itemId}");
            return false;
        }
        
        if (item.type != ItemType.Consumable)
        {
            Debug.LogWarning($"Item {itemId} is not consumable");
            return false;
        }
        
        if (!HasItem(itemId))
        {
            Debug.LogWarning($"No {itemId} in inventory");
            return false;
        }
        
        if (isServer)
        {
            return InternalUseItem(itemId);
        }
        else
        {
            CmdUseItem(itemId);
            return true;
        }
    }
    
    [Command]
    private void CmdUseItem(string itemId)
    {
        InternalUseItem(itemId);
    }
    
    private bool InternalUseItem(string itemId)
    {
        var item = GetItemData(itemId);
        if (item == null) return false;
        
        // Remove item
        RemoveItem(itemId, 1);
        
        // Apply effects
        ApplyItemEffects(item);
        
        return true;
    }
    
    private void ApplyItemEffects(ItemData item)
    {
        if (localPlayer != null)
        {
            // Apply stat bonuses
            if (item.statBonus.hp > 0)
            {
                localPlayer.syncStats.hp = Mathf.Min(
                    localPlayer.syncStats.maxHp,
                    localPlayer.syncStats.hp + item.statBonus.hp
                );
            }
            
            if (item.statBonus.energy > 0)
            {
                localPlayer.syncStats.energy = Mathf.Min(
                    localPlayer.syncStats.maxEnergy,
                    localPlayer.syncStats.energy + item.statBonus.energy
                );
            }
            
            // Play use sound
            if (AudioManager.Instance != null)
            {
                AudioManager.Instance.PlayItemUseSound();
            }
        }
    }
    
    public bool EquipItem(string itemId)
    {
        var item = GetItemData(itemId);
        if (item == null)
        {
            Debug.LogWarning($"Item not found: {itemId}");
            return false;
        }
        
        if (item.type == ItemType.Consumable)
        {
            Debug.LogWarning($"Cannot equip consumable item: {itemId}");
            return false;
        }
        
        if (!HasItem(itemId))
        {
            Debug.LogWarning($"No {itemId} in inventory");
            return false;
        }
        
        if (isServer)
        {
            return InternalEquipItem(itemId);
        }
        else
        {
            CmdEquipItem(itemId);
            return true;
        }
    }
    
    [Command]
    private void CmdEquipItem(string itemId)
    {
        InternalEquipItem(itemId);
    }
    
    private bool InternalEquipItem(string itemId)
    {
        var item = GetItemData(itemId);
        if (item == null) return false;
        
        // Find equipment slot for this item type
        EquipmentSlotType slotType = GetSlotTypeForItem(item.type);
        if (slotType == EquipmentSlotType.None)
        {
            Debug.LogWarning($"No equipment slot for item type: {item.type}");
            return false;
        }
        
        // Find the slot
        EquipmentSlot slot = equipmentSlots.Find(s => s.slotType == slotType);
        if (slot == null)
        {
            Debug.LogWarning($"Equipment slot not found: {slotType}");
            return false;
        }
        
        // Unequip current item if any
        if (!string.IsNullOrEmpty(slot.itemId))
        {
            UnequipItem(slot.slotIndex);
        }
        
        // Equip new item
        slot.itemId = itemId;
        
        // Remove from inventory
        RemoveItem(itemId, 1);
        
        // Apply stat bonuses
        ApplyEquipmentBonuses();
        
        return true;
    }
    
    public bool UnequipItem(int slotIndex)
    {
        if (slotIndex < 0 || slotIndex >= equipmentSlots.Count)
        {
            Debug.LogWarning($"Invalid equipment slot index: {slotIndex}");
            return false;
        }
        
        EquipmentSlot slot = equipmentSlots[slotIndex];
        if (string.IsNullOrEmpty(slot.itemId))
        {
            Debug.LogWarning($"No item in equipment slot: {slotIndex}");
            return false;
        }
        
        if (isServer)
        {
            return InternalUnequipItem(slotIndex);
        }
        else
        {
            CmdUnequipItem(slotIndex);
            return true;
        }
    }
    
    [Command]
    private void CmdUnequipItem(int slotIndex)
    {
        InternalUnequipItem(slotIndex);
    }
    
    private bool InternalUnequipItem(int slotIndex)
    {
        EquipmentSlot slot = equipmentSlots[slotIndex];
        if (string.IsNullOrEmpty(slot.itemId)) return false;
        
        // Add back to inventory
        AddItem(slot.itemId, 1);
        
        // Clear slot
        slot.itemId = string.Empty;
        
        // Apply stat bonuses
        ApplyEquipmentBonuses();
        
        return true;
    }
    
    private EquipmentSlotType GetSlotTypeForItem(ItemType itemType)
    {
        switch (itemType)
        {
            case ItemType.Instrument:
                return EquipmentSlotType.Instrument;
            case ItemType.Amplifier:
                return EquipmentSlotType.Amplifier;
            case ItemType.Microphone:
                return EquipmentSlotType.Microphone;
            case ItemType.Pedal:
                return EquipmentSlotType.Pedal;
            default:
                return EquipmentSlotType.None;
        }
    }
    
    private void ApplyEquipmentBonuses()
    {
        if (localPlayer == null) return;
        
        // Reset stats to base
        PlayerStats baseStats = new PlayerStats
        {
            hp = 100,
            maxHp = 100,
            energy = 100,
            maxEnergy = 100,
            rhythm = 20,
            pitch = 10,
            power = 15,
            harmony = 10
        };
        
        // Apply class bonuses
        switch (localPlayer.characterClass)
        {
            case CharacterClass.Bard:
                baseStats.pitch += 5;
                baseStats.harmony += 5;
                break;
            case CharacterClass.Beatmaker:
                baseStats.rhythm += 5;
                baseStats.power += 5;
                break;
            case CharacterClass.Vocalist:
                baseStats.power += 5;
                baseStats.harmony += 5;
                break;
            case CharacterClass.Guitarist:
                baseStats.rhythm += 5;
                baseStats.pitch += 5;
                break;
            case CharacterClass.DJ:
                baseStats.rhythm += 3;
                baseStats.power += 3;
                baseStats.harmony += 3;
                baseStats.pitch += 3;
                break;
        }
        
        // Apply equipment bonuses
        foreach (var slot in equipmentSlots)
        {
            if (!string.IsNullOrEmpty(slot.itemId))
            {
                var item = GetItemData(slot.itemId);
                if (item != null)
                {
                    baseStats.hp += item.statBonus.hp;
                    baseStats.maxHp += item.statBonus.maxHp;
                    baseStats.energy += item.statBonus.energy;
                    baseStats.maxEnergy += item.statBonus.maxEnergy;
                    baseStats.rhythm += item.statBonus.rhythm;
                    baseStats.pitch += item.statBonus.pitch;
                    baseStats.power += item.statBonus.power;
                    baseStats.harmony += item.statBonus.harmony;
                }
            }
        }
        
        // Update player stats
        localPlayer.syncStats = baseStats;
    }
    
    public void DropItem(string itemId, int quantity = 1, Vector3 position = default)
    {
        if (!HasItem(itemId, quantity)) return;
        
        // Remove from inventory
        RemoveItem(itemId, quantity);
        
        // Spawn item in world
        if (itemDropPrefab != null)
        {
            GameObject dropObj = Instantiate(itemDropPrefab.gameObject, position, Quaternion.identity);
            ItemDrop itemDrop = dropObj.GetComponent<ItemDrop>();
            
            if (itemDrop != null)
            {
                itemDrop.Initialize(itemId, quantity);
            }
        }
    }
    
    public bool CanAfford(int cost)
    {
        return harmonyGems >= cost;
    }
    
    public bool SpendHarmonyGems(int amount)
    {
        if (harmonyGems >= amount)
        {
            harmonyGems -= amount;
            return true;
        }
        return false;
    }
    
    public void AddHarmonyGems(int amount)
    {
        harmonyGems += amount;
    }
    
    public List<InventorySlot> GetInventorySlots()
    {
        return new List<InventorySlot>(inventorySlots);
    }
    
    public List<EquipmentSlot> GetEquipmentSlots()
    {
        return new List<EquipmentSlot>(equipmentSlots);
    }
    
    // SyncVar hooks
    private void OnInventoryChanged(List<InventorySlot> oldSlots, List<InventorySlot> newSlots)
    {
        inventorySlots = newSlots;
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdateInventoryUI();
        }
    }
    
    private void OnEquipmentChanged(List<EquipmentSlot> oldSlots, List<EquipmentSlot> newSlots)
    {
        equipmentSlots = newSlots;
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdateEquipmentUI();
        }
        
        // Update stats
        ApplyEquipmentBonuses();
    }
}

[Serializable]
public class ItemData
{
    public string id;
    public string name;
    public ItemType type;
    public string description;
    public int price;
    public string icon;
    public PlayerStats statBonus;
    public string rarity;
    public int maxStack = 1;
    public bool isEquippable;
    public bool isConsumable;
    public string[] tags;
    public string prefabPath;
}

[Serializable]
public class InventorySlot
{
    public int slotIndex;
    public string itemId;
    public int quantity;
}

[Serializable]
public enum EquipmentSlotType
{
    None,
    Instrument,
    Amplifier,
    Microphone,
    Pedal
}

[Serializable]
public class EquipmentSlot
{
    public int slotIndex;
    public EquipmentSlotType slotType;
    public string itemId;
}

[Serializable]
public class ItemDrop : MonoBehaviour
{
    public string itemId;
    public int quantity = 1;
    public GameObject pickupEffect;
    
    private ItemData itemData;
    
    public void Initialize(string itemId, int quantity)
    {
        this.itemId = itemId;
        this.quantity = quantity;
        
        // Load item data
        itemData = InventoryManager.Instance.GetItemData(itemId);
        
        // Update visuals
        UpdateVisuals();
    }
    
    private void UpdateVisuals()
    {
        if (itemData != null)
        {
            // Set name
            gameObject.name = $"ItemDrop_{itemData.name}";
            
            // Load model from resources
            if (!string.IsNullOrEmpty(itemData.prefabPath))
            {
                GameObject model = Resources.Load<GameObject>(itemData.prefabPath);
                if (model != null)
                {
                    // Instantiate model as child
                    GameObject modelObj = Instantiate(model, transform);
                    modelObj.transform.localPosition = Vector3.zero;
                    modelObj.transform.localRotation = Quaternion.identity;
                }
            }
        }
    }
    
    private void OnTriggerEnter(Collider other)
    {
        PlayerController player = other.GetComponent<PlayerController>();
        if (player != null && player.isLocalPlayer)
        {
            // Pickup item
            if (InventoryManager.Instance.AddItem(itemId, quantity))
            {
                // Play pickup effect
                if (pickupEffect != null)
                {
                    Instantiate(pickupEffect, transform.position, Quaternion.identity);
                }
                
                // Play sound
                if (AudioManager.Instance != null)
                {
                    AudioManager.Instance.PlayItemPickupSound();
                }
                
                // Destroy self
                Destroy(gameObject);
            }
        }
    }
}

public enum ItemType
{
    Instrument,
    Amplifier,
    Microphone,
    Pedal,
    Consumable,
    Quest,
    Misc
}

public enum ItemRarity
{
    Common,
    Uncommon,
    Rare,
    Epic,
    Legendary
}

public enum ItemCategory
{
    Weapon,
    Armor,
    Accessory,
    Consumable,
    Material,
    Crafting,
    Quest
}

public enum ItemSubtype
{
    None,
    Guitar,
    Bass,
    Keyboard,
    Drums,
    Microphone,
    Amplifier,
    Pedal,
    Potion,
    Food,
    Scroll,
    Material,
    Component
}

public enum ItemUseType
{
    None,
    Consumable,
    Equippable,
    Usable,
    Quest
}

public enum ItemTargetType
{
    None,
    Self,
    Ally,
    Enemy,
    Any
}

public enum ItemEffectType
{
    None,
    Heal,
    Damage,
    Buff,
    Debuff,
    Cure,
    Teleport,
    Summon
}

public enum ItemTriggerType
{
    None,
    OnUse,
    OnEquip,
    OnHit,
    OnKill,
    OnLevelUp,
    OnDeath
}

public enum ItemCooldownType
{
    None,
    PerUse,
    PerTarget,
    Global
}

public enum ItemStackType
{
    None,
    Stackable,
    Unique
}

public enum ItemBindType
{
    None,
    OnPickup,
    OnEquip,
    OnUse
}

public enum ItemDurabilityType
{
    None,
    Uses,
    Duration,
    Permanent
}

public enum ItemQuality
{
    Poor,
    Common,
    Uncommon,
    Rare,
    Epic,
    Legendary,
    Artifact,
    Heirloom
}

public enum ItemSource
{
    None,
    Drop,
    Craft,
    Quest,
    Shop,
    Loot,
    Achievement,
    Event
}

public enum ItemFlag
{
    None,
    Soulbound,
    AccountBound,
    NonTradeable,
    NonSellable,
    NonDroppable,
    NonDestroyable,
    UniqueEquipped
}

public enum ItemRequirementType
{
    None,
    Level,
    Class,
    Reputation,
    Quest,
    Achievement,
    Skill,
    Item
}

public enum ItemStatType
{
    None,
    Hp,
    MaxHp,
    Energy,
    MaxEnergy,
    Rhythm,
    Pitch,
    Power,
    Harmony,
    AllStats
}

public enum ItemDamageType
{
    None,
    Physical,
    Magical,
    True,
    Fire,
    Ice,
    Lightning,
    Poison,
    Holy,
    Shadow
}

public enum ItemHealType
{
    None,
    Hp,
    Energy,
    Both,
    Percentage
}

public enum ItemBuffType
{
    None,
    Flat,
    Percentage,
    Multiplicative
}

public enum ItemDebuffType
{
    None,
    Flat,
    Percentage,
    Multiplicative
}

public enum ItemResistanceType
{
    None,
    Physical,
    Magical,
    All,
    Fire,
    Ice,
    Lightning,
    Poison,
    Holy,
    Shadow
}

public enum ItemPenetrationType
{
    None,
    Physical,
    Magical,
    All,
    Fire,
    Ice,
    Lightning,
    Poison,
    Holy,
    Shadow
}

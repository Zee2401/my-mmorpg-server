using Mirror;
using System;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Manages the in-game shop system
/// </summary>
public class ShopSystem : NetworkBehaviour
{
    public static ShopSystem Instance { get; private set; }
    
    [Header("Shop Settings")]
    public List<ShopData> shops = new List<ShopData>();
    public string defaultShopId = "shop_main";
    
    [SyncVar]
    public string currentShopId;
    
    private Dictionary<string, ShopData> shopDatabase = new Dictionary<string, ShopData>();
    private PlayerController localPlayer;
    
    public override void Awake()
    {
        base.Awake();
        Instance = this;
        
        // Initialize shop database
        foreach (var shop in shops)
        {
            shopDatabase[shop.id] = shop;
        }
    }
    
    private void Start()
    {
        // Find local player
        localPlayer = FindObjectOfType<PlayerController>();
    }
    
    public ShopData GetShop(string shopId)
    {
        shopDatabase.TryGetValue(shopId, out ShopData shop);
        return shop;
    }
    
    public ShopData GetCurrentShop()
    {
        return GetShop(currentShopId);
    }
    
    public List<ShopItem> GetShopItems(string shopId)
    {
        var shop = GetShop(shopId);
        if (shop == null) return new List<ShopItem>();
        
        return shop.items;
    }
    
    public ShopItem GetShopItem(string shopId, string itemId)
    {
        var shop = GetShop(shopId);
        if (shop == null) return null;
        
        foreach (var item in shop.items)
        {
            if (item.itemId == itemId)
            {
                return item;
            }
        }
        
        return null;
    }
    
    public bool CanBuyItem(string shopId, string itemId)
    {
        var shop = GetShop(shopId);
        if (shop == null) return false;
        
        var shopItem = GetShopItem(shopId, itemId);
        if (shopItem == null) return false;
        
        var item = InventoryManager.Instance.GetItemData(itemId);
        if (item == null) return false;
        
        // Check if player can afford
        if (localPlayer == null) return false;
        
        // Check currency
        switch (shopItem.currencyType)
        {
            case ShopCurrencyType.HarmonyGems:
                return localPlayer.harmonyGems >= shopItem.price;
            case ShopCurrencyType.Gold:
                // Not implemented yet
                return true;
            default:
                return true;
        }
    }
    
    public bool BuyItem(string shopId, string itemId, int quantity = 1)
    {
        var shop = GetShop(shopId);
        if (shop == null) return false;
        
        var shopItem = GetShopItem(shopId, itemId);
        if (shopItem == null) return false;
        
        var item = InventoryManager.Instance.GetItemData(itemId);
        if (item == null) return false;
        
        // Check if can buy
        if (!CanBuyItem(shopId, itemId)) return false;
        
        // Check inventory space
        if (!InventoryManager.Instance.HasItem(itemId, quantity))
        {
            // Check if item is stackable
            if (item.maxStack <= 1 && InventoryManager.Instance.GetInventorySlots().Count >= 50)
            {
                // Inventory full
                return false;
            }
        }
        
        if (isServer)
        {
            return InternalBuyItem(shopId, itemId, quantity);
        }
        else
        {
            CmdBuyItem(shopId, itemId, quantity);
            return true;
        }
    }
    
    [Command]
    private void CmdBuyItem(string shopId, string itemId, int quantity)
    {
        InternalBuyItem(shopId, itemId, quantity);
    }
    
    private bool InternalBuyItem(string shopId, string itemId, int quantity)
    {
        var shop = GetShop(shopId);
        if (shop == null) return false;
        
        var shopItem = GetShopItem(shopId, itemId);
        if (shopItem == null) return false;
        
        var item = InventoryManager.Instance.GetItemData(itemId);
        if (item == null) return false;
        
        if (localPlayer == null) return false;
        
        // Check currency
        switch (shopItem.currencyType)
        {
            case ShopCurrencyType.HarmonyGems:
                if (localPlayer.harmonyGems < shopItem.price * quantity)
                {
                    return false;
                }
                localPlayer.SpendHarmonyGems(shopItem.price * quantity);
                break;
            case ShopCurrencyType.Gold:
                // Not implemented yet
                break;
        }
        
        // Add item to inventory
        for (int i = 0; i < quantity; i++)
        {
            if (!InventoryManager.Instance.AddItem(itemId, 1))
            {
                // Refund if failed
                localPlayer.AddHarmonyGems(shopItem.price * (quantity - i));
                return false;
            }
        }
        
        // Play sound
        if (AudioManager.Instance != null)
        {
            AudioManager.Instance.PlayItemPickupSound();
        }
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Purchase Complete!", $"Bought {quantity}x {item.name}", NotificationType.Success);
        }
        
        Debug.Log($"Bought {quantity}x {item.name} for {shopItem.price * quantity} {shopItem.currencyType}");
        
        return true;
    }
    
    public bool SellItem(string shopId, string itemId, int quantity = 1)
    {
        var shop = GetShop(shopId);
        if (shop == null) return false;
        
        var shopItem = GetShopItem(shopId, itemId);
        if (shopItem == null) return false;
        
        var item = InventoryManager.Instance.GetItemData(itemId);
        if (item == null) return false;
        
        // Check if player has item
        if (!InventoryManager.Instance.HasItem(itemId, quantity)) return false;
        
        if (isServer)
        {
            return InternalSellItem(shopId, itemId, quantity);
        }
        else
        {
            CmdSellItem(shopId, itemId, quantity);
            return true;
        }
    }
    
    [Command]
    private void CmdSellItem(string shopId, string itemId, int quantity)
    {
        InternalSellItem(shopId, itemId, quantity);
    }
    
    private bool InternalSellItem(string shopId, string itemId, int quantity)
    {
        var shop = GetShop(shopId);
        if (shop == null) return false;
        
        var shopItem = GetShopItem(shopId, itemId);
        if (shopItem == null) return false;
        
        var item = InventoryManager.Instance.GetItemData(itemId);
        if (item == null) return false;
        
        if (localPlayer == null) return false;
        
        // Remove item from inventory
        if (!InventoryManager.Instance.RemoveItem(itemId, quantity))
        {
            return false;
        }
        
        // Calculate sell price
        int sellPrice = Mathf.FloorToInt(shopItem.price * shop.sellPriceMultiplier);
        
        // Add currency
        switch (shopItem.currencyType)
        {
            case ShopCurrencyType.HarmonyGems:
                localPlayer.AddHarmonyGems(sellPrice * quantity);
                break;
            case ShopCurrencyType.Gold:
                // Not implemented yet
                break;
        }
        
        // Play sound
        if (AudioManager.Instance != null)
        {
            AudioManager.Instance.PlayItemUseSound();
        }
        
        // Show notification
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Item Sold!", $"Sold {quantity}x {item.name} for {sellPrice * quantity} {shopItem.currencyType}", NotificationType.Success);
        }
        
        Debug.Log($"Sold {quantity}x {item.name} for {sellPrice * quantity} {shopItem.currencyType}");
        
        return true;
    }
    
    public void OpenShop(string shopId)
    {
        var shop = GetShop(shopId);
        if (shop == null) return;
        
        currentShopId = shopId;
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowShop();
            UIManager.Instance.UpdateShopUI(shop);
        }
        
        Debug.Log($"Opened shop: {shop.name}");
    }
    
    public void CloseShop()
    {
        currentShopId = string.Empty;
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.HideShop();
        }
        
        Debug.Log("Closed shop");
    }
    
    public List<ShopCategory> GetShopCategories(string shopId)
    {
        var shop = GetShop(shopId);
        if (shop == null) return new List<ShopCategory>();
        
        List<ShopCategory> categories = new List<ShopCategory>();
        
        foreach (var item in shop.items)
        {
            var itemData = InventoryManager.Instance.GetItemData(item.itemId);
            if (itemData != null)
            {
                ShopCategory category = categories.Find(c => c.category == item.category);
                if (category == null)
                {
                    category = new ShopCategory
                    {
                        category = item.category,
                        displayName = item.category.ToString()
                    };
                    categories.Add(category);
                }
            }
        }
        
        return categories;
    }
    
    public List<ShopItem> GetItemsByCategory(string shopId, ShopCategoryType category)
    {
        var shop = GetShop(shopId);
        if (shop == null) return new List<ShopItem>();
        
        List<ShopItem> items = new List<ShopItem>();
        
        foreach (var item in shop.items)
        {
            if (item.category == category)
            {
                items.Add(item);
            }
        }
        
        return items;
    }
    
    public List<ShopItem> GetAffordableItems(string shopId)
    {
        var shop = GetShop(shopId);
        if (shop == null) return new List<ShopItem>();
        
        List<ShopItem> affordable = new List<ShopItem>();
        
        foreach (var item in shop.items)
        {
            if (CanBuyItem(shopId, item.itemId))
            {
                affordable.Add(item);
            }
        }
        
        return affordable;
    }
}

[Serializable]
public class ShopData
{
    public string id;
    public string name;
    public string description;
    public string npcId;
    public string zoneId;
    public Vector3 position;
    public List<ShopItem> items;
    public float sellPriceMultiplier = 0.5f;
    public ShopType type;
    public string icon;
    public string[] tags;
}

[Serializable]
public class ShopItem
{
    public string itemId;
    public int price;
    public ShopCurrencyType currencyType;
    public ShopCategoryType category;
    public int stock;
    public int maxStock;
    public bool isFeatured;
    public bool isNew;
    public bool isOnSale;
    public float saleDiscount;
    public int requiredLevel;
    public string requiredClass;
    public List<string> requiredQuests;
    public List<string> requiredAchievements;
    public string[] tags;
}

[Serializable]
public class ShopCategory
{
    public ShopCategoryType category;
    public string displayName;
}

public enum ShopType
{
    General,
    Instrument,
    Armor,
    Consumable,
    Crafting,
    Guild,
    Auction,
    BlackMarket,
    Special
}

public enum ShopCategoryType
{
    All,
    Weapons,
    Armor,
    Instruments,
    Amplifiers,
    Microphones,
    Pedals,
    Consumables,
    Materials,
    Crafting,
    Quest,
    Special,
    Rare,
    Epic,
    Legendary
}

public enum ShopCurrencyType
{
    HarmonyGems,
    Gold,
    Silver,
    Copper,
    Tokens,
    Points
}

public enum ShopSortType
{
    Default,
    PriceLowToHigh,
    PriceHighToLow,
    NameAToZ,
    NameZToA,
    Newest,
    Popularity,
    Rating,
    Stock
}

public enum ShopFilterType
{
    All,
    Affordable,
    ByCategory,
    ByRarity,
    ByLevel,
    ByClass,
    OnSale,
    Featured,
    New
}

public enum ShopTabType
{
    Buy,
    Sell,
    Trade,
    Craft,
    Auction
}

public enum ShopInteractionType
{
    Menu,
    Dialogue,
    QuickBuy,
    Barter,
    Bidding
}

public enum ShopRestockType
{
    Never,
    Daily,
    Weekly,
    Monthly,
    OnPurchase,
    OnVisit
}

public enum ShopDiscountType
{
    None,
    Percentage,
    Fixed,
    Dynamic,
    Seasonal,
    Event
}

public enum ShopTaxType
{
    None,
    Fixed,
    Percentage,
    Dynamic
}

public enum ShopReputationType
{
    None,
    Faction,
    Guild,
    Player,
    Global
}

public enum ShopAccessType
{
    Public,
    Private,
    GuildOnly,
    FactionOnly,
    VIP,
    InviteOnly
}

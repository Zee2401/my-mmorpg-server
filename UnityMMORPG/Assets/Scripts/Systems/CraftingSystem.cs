using Mirror;
using System;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Manages crafting and item creation
/// </summary>
public class CraftingSystem : NetworkBehaviour
{
    public static CraftingSystem Instance { get; private set; }
    
    [Header("Crafting Settings")]
    public List<RecipeData> recipes = new List<RecipeData>();
    public int maxCraftingQueue = 10;
    public float craftingSpeedMultiplier = 1f;
    
    [Header("References")]
    public Transform craftingStationPrefab;
    
    [SyncVar(hook = nameof(OnRecipesChanged))]
    public List<CraftingQueueItem> craftingQueue = new List<CraftingQueueItem>();
    
    private Dictionary<string, RecipeData> recipeDatabase = new Dictionary<string, RecipeData>();
    private List<CraftingStation> craftingStations = new List<CraftingStation>();
    private PlayerController localPlayer;
    
    public override void Awake()
    {
        base.Awake();
        Instance = this;
        
        // Initialize recipe database
        foreach (var recipe in recipes)
        {
            recipeDatabase[recipe.id] = recipe;
        }
    }
    
    private void Start()
    {
        // Find local player
        localPlayer = FindObjectOfType<PlayerController>();
    }
    
    private void Update()
    {
        // Process crafting queue
        if (isServer)
        {
            ProcessCraftingQueue();
        }
    }
    
    private void ProcessCraftingQueue()
    {
        if (craftingQueue.Count == 0) return;
        
        for (int i = craftingQueue.Count - 1; i >= 0; i--)
        {
            var queueItem = craftingQueue[i];
            
            // Check if crafting is complete
            if (Time.time >= queueItem.completionTime)
            {
                CompleteCrafting(queueItem);
                craftingQueue.RemoveAt(i);
            }
        }
    }
    
    private void CompleteCrafting(CraftingQueueItem queueItem)
    {
        var recipe = GetRecipe(queueItem.recipeId);
        if (recipe == null) return;
        
        // Find the player who started crafting
        PlayerController player = FindPlayerByName(queueItem.playerName);
        if (player == null) return;
        
        // Check if player has space in inventory
        if (InventoryManager.Instance != null)
        {
            // Add crafted item to inventory
            bool success = InventoryManager.Instance.AddItem(recipe.resultItemId, recipe.resultQuantity);
            
            if (success)
            {
                // Notify player
                if (player.isLocalPlayer && UIManager.Instance != null)
                {
                    UIManager.Instance.ShowNotification("Crafting Complete!", 
                        $"{recipe.resultQuantity}x {GetItemName(recipe.resultItemId)}", 
                        NotificationType.Success);
                }
                
                // Gain XP
                player.GainExperience(recipe.xpReward);
                
                Debug.Log($"Crafting completed: {recipe.resultQuantity}x {recipe.resultItemId}");
            }
            else
            {
                // Inventory full - return materials
                ReturnMaterials(recipe, player);
                
                if (player.isLocalPlayer && UIManager.Instance != null)
                {
                    UIManager.Instance.ShowNotification("Crafting Failed", 
                        "Inventory full!", 
                        NotificationType.Error);
                }
            }
        }
    }
    
    private PlayerController FindPlayerByName(string playerName)
    {
        foreach (var player in FindObjectsOfType<PlayerController>())
        {
            if (player.playerName == playerName)
            {
                return player;
            }
        }
        return null;
    }
    
    private void ReturnMaterials(RecipeData recipe, PlayerController player)
    {
        foreach (var ingredient in recipe.ingredients)
        {
            InventoryManager.Instance.AddItem(ingredient.itemId, ingredient.quantity);
        }
    }
    
    private string GetItemName(string itemId)
    {
        var item = InventoryManager.Instance.GetItemData(itemId);
        return item != null ? item.name : itemId;
    }
    
    public RecipeData GetRecipe(string recipeId)
    {
        recipeDatabase.TryGetValue(recipeId, out RecipeData recipe);
        return recipe;
    }
    
    public List<RecipeData> GetAllRecipes()
    {
        return new List<RecipeData>(recipes);
    }
    
    public List<RecipeData> GetAvailableRecipes()
    {
        List<RecipeData> available = new List<RecipeData>();
        
        foreach (var recipe in recipes)
        {
            if (CanCraft(recipe.id))
            {
                available.Add(recipe);
            }
        }
        
        return available;
    }
    
    public List<RecipeData> GetRecipesByCategory(CraftingCategory category)
    {
        List<RecipeData> categoryRecipes = new List<RecipeData>();
        
        foreach (var recipe in recipes)
        {
            if (recipe.category == category)
            {
                categoryRecipes.Add(recipe);
            }
        }
        
        return categoryRecipes;
    }
    
    public bool CanCraft(string recipeId)
    {
        var recipe = GetRecipe(recipeId);
        if (recipe == null) return false;
        
        // Check level requirement
        if (localPlayer != null && localPlayer.level < recipe.requiredLevel) return false;
        
        // Check if player has required materials
        foreach (var ingredient in recipe.ingredients)
        {
            if (!InventoryManager.Instance.HasItem(ingredient.itemId, ingredient.quantity))
            {
                return false;
            }
        }
        
        // Check if player has required tools
        if (!string.IsNullOrEmpty(recipe.requiredTool))
        {
            // Check if tool is equipped or in inventory
            if (!InventoryManager.Instance.HasItem(recipe.requiredTool))
            {
                return false;
            }
        }
        
        // Check if player has space in inventory for result
        var resultItem = InventoryManager.Instance.GetItemData(recipe.resultItemId);
        if (resultItem != null)
        {
            int freeSlots = 50 - InventoryManager.Instance.GetInventorySlots().Count;
            if (resultItem.maxStack <= 1 && freeSlots < recipe.resultQuantity)
            {
                return false;
            }
        }
        
        return true;
    }
    
    public void StartCrafting(string recipeId)
    {
        var recipe = GetRecipe(recipeId);
        if (recipe == null) return;
        
        if (!CanCraft(recipeId)) return;
        
        if (craftingQueue.Count >= maxCraftingQueue)
        {
            if (UIManager.Instance != null)
            {
                UIManager.Instance.ShowNotification("Crafting Queue Full", 
                    "Please wait for current crafts to complete", 
                    NotificationType.Warning);
            }
            return;
        }
        
        if (isServer)
        {
            InternalStartCrafting(recipeId);
        }
        else
        {
            CmdStartCrafting(recipeId);
        }
    }
    
    [Command]
    private void CmdStartCrafting(string recipeId)
    {
        InternalStartCrafting(recipeId);
    }
    
    private void InternalStartCrafting(string recipeId)
    {
        var recipe = GetRecipe(recipeId);
        if (recipe == null) return;
        
        // Remove materials from inventory
        foreach (var ingredient in recipe.ingredients)
        {
            InventoryManager.Instance.RemoveItem(ingredient.itemId, ingredient.quantity);
        }
        
        // Calculate crafting time
        float craftingTime = recipe.craftingTime * (1f / craftingSpeedMultiplier);
        
        // Add to queue
        craftingQueue.Add(new CraftingQueueItem
        {
            recipeId = recipeId,
            playerName = localPlayer.playerName,
            startTime = Time.time,
            completionTime = Time.time + craftingTime
        });
        
        // Notify player
        if (localPlayer.isLocalPlayer && UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Crafting Started", 
                GetItemName(recipe.resultItemId), 
                NotificationType.Info);
        }
        
        Debug.Log($"Crafting started: {recipe.resultItemId} (completes in {craftingTime}s)");
    }
    
    public void CancelCrafting(int queueIndex)
    {
        if (queueIndex < 0 || queueIndex >= craftingQueue.Count) return;
        
        var queueItem = craftingQueue[queueIndex];
        var recipe = GetRecipe(queueItem.recipeId);
        if (recipe == null) return;
        
        if (isServer)
        {
            InternalCancelCrafting(queueIndex);
        }
        else
        {
            CmdCancelCrafting(queueIndex);
        }
    }
    
    [Command]
    private void CmdCancelCrafting(int queueIndex)
    {
        InternalCancelCrafting(queueIndex);
    }
    
    private void InternalCancelCrafting(int queueIndex)
    {
        var queueItem = craftingQueue[queueIndex];
        var recipe = GetRecipe(queueItem.recipeId);
        if (recipe == null) return;
        
        // Return materials
        PlayerController player = FindPlayerByName(queueItem.playerName);
        if (player != null)
        {
            foreach (var ingredient in recipe.ingredients)
            {
                InventoryManager.Instance.AddItem(ingredient.itemId, ingredient.quantity);
            }
        }
        
        // Remove from queue
        craftingQueue.RemoveAt(queueIndex);
        
        // Notify player
        if (localPlayer.isLocalPlayer && UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Crafting Cancelled", 
                GetItemName(recipe.resultItemId), 
                NotificationType.Warning);
        }
        
        Debug.Log($"Crafting cancelled: {recipe.resultItemId}");
    }
    
    public void DiscoverRecipe(string recipeId)
    {
        var recipe = GetRecipe(recipeId);
        if (recipe == null) return;
        
        // Mark recipe as discovered
        // This would be saved in player data
        
        if (UIManager.Instance != null)
        {
            UIManager.Instance.ShowNotification("Recipe Discovered!", 
                recipe.name, 
                NotificationType.Success);
        }
    }
    
    public List<RecipeData> GetDiscoveredRecipes()
    {
        // Return all recipes that player has discovered
        // In a real implementation, this would check player data
        return new List<RecipeData>(recipes);
    }
    
    public List<RecipeData> GetUndiscoveredRecipes()
    {
        List<RecipeData> undiscovered = new List<RecipeData>();
        
        // In a real implementation, this would check which recipes player hasn't discovered
        // For now, return empty list
        
        return undiscovered;
    }
    
    public void RegisterCraftingStation(CraftingStation station)
    {
        if (!craftingStations.Contains(station))
        {
            craftingStations.Add(station);
        }
    }
    
    public void UnregisterCraftingStation(CraftingStation station)
    {
        if (craftingStations.Contains(station))
        {
            craftingStations.Remove(station);
        }
    }
    
    public List<CraftingStation> GetCraftingStations()
    {
        return new List<CraftingStation>(craftingStations);
    }
    
    public CraftingStation GetNearestCraftingStation(Vector3 position)
    {
        CraftingStation nearest = null;
        float nearestDistance = float.MaxValue;
        
        foreach (var station in craftingStations)
        {
            float distance = Vector3.Distance(position, station.transform.position);
            if (distance < nearestDistance)
            {
                nearest = station;
                nearestDistance = distance;
            }
        }
        
        return nearest;
    }
    
    // SyncVar hook
    private void OnRecipesChanged(List<CraftingQueueItem> oldQueue, List<CraftingQueueItem> newQueue)
    {
        craftingQueue = newQueue;
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdateCraftingUI();
        }
    }
}

[Serializable]
public class CraftingQueueItem
{
    public string recipeId;
    public string playerName;
    public float startTime;
    public float completionTime;
}

[Serializable]
public class RecipeData
{
    public string id;
    public string name;
    public string description;
    public CraftingCategory category;
    public int requiredLevel;
    public string requiredTool;
    public List<RecipeIngredient> ingredients;
    public string resultItemId;
    public int resultQuantity;
    public float craftingTime;
    public int xpReward;
    public int gemCost;
    public string icon;
    public string[] tags;
    public bool isDiscovered;
}

[Serializable]
public class RecipeIngredient
{
    public string itemId;
    public int quantity;
}

[Serializable]
public class CraftingStation : MonoBehaviour
{
    public string stationId;
    public string stationName;
    public CraftingStationType stationType;
    public List<CraftingCategory> allowedCategories;
    public float craftingSpeedMultiplier = 1f;
    
    private void Start()
    {
        // Register with crafting system
        CraftingSystem.Instance.RegisterCraftingStation(this);
    }
    
    private void OnDestroy()
    {
        // Unregister from crafting system
        CraftingSystem.Instance.UnregisterCraftingStation(this);
    }
}

public enum CraftingCategory
{
    None,
    Instruments,
    Amplifiers,
    Microphones,
    Pedals,
    Consumables,
    Materials,
    CraftingTools,
    Special
}

public enum CraftingStationType
{
    Basic,
    Advanced,
    Master,
    Special,
    Alchemy,
    Blacksmith,
    Tailor,
    Enchanting
}

public enum CraftingDifficulty
{
    Trivial,
    Easy,
    Normal,
    Hard,
    Expert,
    Master
}

public enum CraftingQuality
{
    Poor,
    Common,
    Uncommon,
    Rare,
    Epic,
    Legendary
}

public enum CraftingMethod
{
    None,
    Combine,
    Forge,
    Brew,
    Enchant,
    Tailor,
    Assemble,
    Cook
}

public enum CraftingSkill
{
    None,
    InstrumentMaking,
    Electronics,
    Alchemy,
    Blacksmithing,
    Tailoring,
    Enchanting,
    Cooking,
    Engineering
}

public enum CraftingProfession
{
    None,
    Artisan,
    Engineer,
    Alchemist,
    Blacksmith,
    Tailor,
    Enchanter,
    Chef,
    MasterCraftsman
}

public enum CraftingSpecialization
{
    None,
    Guitars,
    Keyboards,
    Drums,
    Microphones,
    Amplifiers,
    Pedals,
    Potions,
    Food,
    Armor,
    Weapons
}

public enum CraftingRarity
{
    Common,
    Uncommon,
    Rare,
    Epic,
    Legendary,
    Unique
}

public enum CraftingRequirement
{
    None,
    Level,
    Skill,
    Tool,
    Material,
    Profession,
    Specialization,
    Reputation
}

public enum CraftingEffect
{
    None,
    StatBoost,
    DurationBoost,
    CostReduction,
    QualityImprovement,
    SpeedIncrease,
    DoubleResult,
    NoCost
}

public enum CraftingStatus
{
    None,
    Queued,
    Crafting,
    Completed,
    Failed,
    Cancelled
}

using Mirror;
using System;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Manages quests and objectives
/// </summary>
public class QuestSystem : NetworkBehaviour
{
    public static QuestSystem Instance { get; private set; }
    
    [Header("Quest Settings")]
    public List<QuestData> allQuests = new List<QuestData>();
    public int maxActiveQuests = 10;
    
    [SyncVar(hook = nameof(OnQuestsChanged))]
    public List<QuestSlot> activeQuests = new List<QuestSlot>();
    
    [SyncVar(hook = nameof(OnCompletedQuestsChanged))]
    public List<string> completedQuestIds = new List<string>();
    
    private Dictionary<string, QuestData> questDatabase = new Dictionary<string, QuestData>();
    private PlayerController localPlayer;
    
    public override void Awake()
    {
        base.Awake();
        Instance = this;
        
        // Initialize quest database
        foreach (var quest in allQuests)
        {
            questDatabase[quest.id] = quest;
        }
    }
    
    private void Start()
    {
        // Find local player
        localPlayer = FindObjectOfType<PlayerController>();
        if (localPlayer != null && localPlayer.isLocalPlayer)
        {
            InitializeQuests(localPlayer);
        }
    }
    
    private void InitializeQuests(PlayerController player)
    {
        // Clear existing quests
        activeQuests.Clear();
        completedQuestIds.Clear();
        
        // Add starting quests based on player level
        foreach (var quest in allQuests)
        {
            if (quest.requiredLevel <= player.level && !completedQuestIds.Contains(quest.id))
            {
                AddQuest(quest.id);
            }
        }
    }
    
    public QuestData GetQuest(string questId)
    {
        questDatabase.TryGetValue(questId, out QuestData quest);
        return quest;
    }
    
    public bool HasQuest(string questId)
    {
        return activeQuests.Exists(q => q.questId == questId);
    }
    
    public bool IsQuestCompleted(string questId)
    {
        return completedQuestIds.Contains(questId);
    }
    
    public bool CanAcceptQuest(string questId)
    {
        var quest = GetQuest(questId);
        if (quest == null) return false;
        
        // Check if already have or completed
        if (HasQuest(questId) || IsQuestCompleted(questId)) return false;
        
        // Check level requirement
        if (localPlayer != null && localPlayer.level < quest.requiredLevel) return false;
        
        // Check if at max active quests
        if (activeQuests.Count >= maxActiveQuests) return false;
        
        // Check quest prerequisites
        if (!HasPrerequisites(quest)) return false;
        
        return true;
    }
    
    private bool HasPrerequisites(QuestData quest)
    {
        if (quest.prerequisiteQuests == null || quest.prerequisiteQuests.Count == 0) return true;
        
        foreach (var prerequisiteId in quest.prerequisiteQuests)
        {
            if (!IsQuestCompleted(prerequisiteId)) return false;
        }
        
        return true;
    }
    
    public void AcceptQuest(string questId)
    {
        if (!CanAcceptQuest(questId)) return;
        
        if (isServer)
        {
            InternalAcceptQuest(questId);
        }
        else
        {
            CmdAcceptQuest(questId);
        }
    }
    
    [Command]
    private void CmdAcceptQuest(string questId)
    {
        InternalAcceptQuest(questId);
    }
    
    private void InternalAcceptQuest(string questId)
    {
        var quest = GetQuest(questId);
        if (quest == null) return;
        
        // Add to active quests
        activeQuests.Add(new QuestSlot
        {
            questId = questId,
            progress = 0,
            isTracked = false
        });
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdateQuestUI();
        }
        
        Debug.Log($"Quest accepted: {quest.name}");
    }
    
    public void AbandonQuest(string questId)
    {
        if (!HasQuest(questId)) return;
        
        if (isServer)
        {
            InternalAbandonQuest(questId);
        }
        else
        {
            CmdAbandonQuest(questId);
        }
    }
    
    [Command]
    private void CmdAbandonQuest(string questId)
    {
        InternalAbandonQuest(questId);
    }
    
    private void InternalAbandonQuest(string questId)
    {
        var slot = activeQuests.Find(q => q.questId == questId);
        if (slot != null)
        {
            activeQuests.Remove(slot);
        }
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdateQuestUI();
        }
        
        Debug.Log($"Quest abandoned: {questId}");
    }
    
    public void CompleteQuest(string questId)
    {
        var quest = GetQuest(questId);
        if (quest == null) return;
        
        var slot = activeQuests.Find(q => q.questId == questId);
        if (slot == null) return;
        
        // Check if already completed
        if (IsQuestCompleted(questId)) return;
        
        // Check if quest is complete
        if (!IsQuestComplete(quest, slot)) return;
        
        if (isServer)
        {
            InternalCompleteQuest(questId);
        }
        else
        {
            CmdCompleteQuest(questId);
        }
    }
    
    [Command]
    private void CmdCompleteQuest(string questId)
    {
        InternalCompleteQuest(questId);
    }
    
    private void InternalCompleteQuest(string questId)
    {
        var quest = GetQuest(questId);
        if (quest == null) return;
        
        var slot = activeQuests.Find(q => q.questId == questId);
        if (slot == null) return;
        
        // Remove from active quests
        activeQuests.Remove(slot);
        
        // Add to completed quests
        if (!completedQuestIds.Contains(questId))
        {
            completedQuestIds.Add(questId);
        }
        
        // Award rewards
        if (localPlayer != null)
        {
            localPlayer.GainExperience(quest.rewardXp);
            localPlayer.AddHarmonyGems(quest.rewardGems);
            
            // Award items if any
            if (quest.rewardItem != null && !string.IsNullOrEmpty(quest.rewardItem.id))
            {
                InventoryManager.Instance.AddItem(quest.rewardItem.id, quest.rewardItem.quantity);
            }
        }
        
        // Check for quest chain
        foreach (var q in allQuests)
        {
            if (q.prerequisiteQuests != null && q.prerequisiteQuests.Contains(questId) && CanAcceptQuest(q.id))
            {
                AcceptQuest(q.id);
            }
        }
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdateQuestUI();
            UIManager.Instance.ShowNotification("Quest Complete!", quest.name, NotificationType.Success);
        }
        
        Debug.Log($"Quest completed: {quest.name}");
    }
    
    private bool IsQuestComplete(QuestData quest, QuestSlot slot)
    {
        if (quest.objectives == null || quest.objectives.Count == 0) return true;
        
        foreach (var objective in quest.objectives)
        {
            if (!IsObjectiveComplete(objective, slot)) return false;
        }
        
        return true;
    }
    
    private bool IsObjectiveComplete(QuestObjective objective, QuestSlot slot)
    {
        switch (objective.type)
        {
            case QuestObjectiveType.Kill:
                // Check kill count
                // This would be tracked separately
                return true;
                
            case QuestObjectiveType.Collect:
                // Check item collection
                return InventoryManager.Instance.GetItemQuantity(objective.targetId) >= objective.requiredAmount;
                
            case QuestObjectiveType.ReachLevel:
                // Check player level
                return localPlayer != null && localPlayer.level >= objective.requiredAmount;
                
            case QuestObjectiveType.TalkTo:
                // Check if talked to NPC
                // This would be tracked separately
                return true;
                
            case QuestObjectiveType.EnterZone:
                // Check if in zone
                return ZoneManager.Instance != null && ZoneManager.Instance.currentZoneId == objective.targetId;
                
            case QuestObjectiveType.DefeatBoss:
                // Check if boss defeated
                return ZoneManager.Instance != null && ZoneManager.Instance.GetBossInZone(objective.targetId) == null;
                
            default:
                return true;
        }
    }
    
    public void UpdateQuestProgress(string questId, string objectiveId, int amount = 1)
    {
        var quest = GetQuest(questId);
        if (quest == null) return;
        
        var slot = activeQuests.Find(q => q.questId == questId);
        if (slot == null) return;
        
        var objective = quest.objectives.Find(o => o.id == objectiveId);
        if (objective == null) return;
        
        // Update progress
        slot.progress += amount;
        
        // Check if complete
        if (IsQuestComplete(quest, slot))
        {
            CompleteQuest(questId);
        }
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdateQuestUI();
        }
    }
    
    public void TrackQuest(string questId)
    {
        var slot = activeQuests.Find(q => q.questId == questId);
        if (slot != null)
        {
            // Untrack all others first
            foreach (var q in activeQuests)
            {
                q.isTracked = false;
            }
            
            slot.isTracked = true;
            
            // Update UI
            if (UIManager.Instance != null)
            {
                UIManager.Instance.UpdateQuestUI();
            }
        }
    }
    
    public List<QuestData> GetAvailableQuests()
    {
        List<QuestData> available = new List<QuestData>();
        
        foreach (var quest in allQuests)
        {
            if (CanAcceptQuest(quest.id))
            {
                available.Add(quest);
            }
        }
        
        return available;
    }
    
    public List<QuestData> GetActiveQuests()
    {
        List<QuestData> active = new List<QuestData>();
        
        foreach (var slot in activeQuests)
        {
            var quest = GetQuest(slot.questId);
            if (quest != null)
            {
                active.Add(quest);
            }
        }
        
        return active;
    }
    
    public List<QuestData> GetCompletedQuests()
    {
        List<QuestData> completed = new List<QuestData>();
        
        foreach (var questId in completedQuestIds)
        {
            var quest = GetQuest(questId);
            if (quest != null)
            {
                completed.Add(quest);
            }
        }
        
        return completed;
    }
    
    public void LoadQuestsFromZone(string zoneId)
    {
        var zone = ZoneManager.Instance.GetZone(zoneId);
        if (zone == null) return;
        
        foreach (var quest in zone.quests)
        {
            if (CanAcceptQuest(quest.id))
            {
                AcceptQuest(quest.id);
            }
        }
    }
    
    // SyncVar hooks
    private void OnQuestsChanged(List<QuestSlot> oldQuests, List<QuestSlot> newQuests)
    {
        activeQuests = newQuests;
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdateQuestUI();
        }
    }
    
    private void OnCompletedQuestsChanged(List<string> oldIds, List<string> newIds)
    {
        completedQuestIds = newIds;
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdateQuestUI();
        }
    }
}

[Serializable]
public class QuestSlot
{
    public string questId;
    public int progress;
    public bool isTracked;
}

[Serializable]
public class QuestData
{
    public string id;
    public string name;
    public string description;
    public string regionId;
    public int requiredLevel;
    public int rewardXp;
    public int rewardGems;
    public RewardItem rewardItem;
    public bool isCompleted;
    public string targetBossId;
    public string giverNpcId;
    public List<string> prerequisiteQuests;
    public List<QuestObjective> objectives;
    public QuestType type;
    public QuestCategory category;
    public string icon;
    public string[] tags;
}

[Serializable]
public class QuestObjective
{
    public string id;
    public string description;
    public QuestObjectiveType type;
    public string targetId;
    public int requiredAmount;
    public int currentAmount;
}

[Serializable]
public class RewardItem
{
    public string id;
    public int quantity;
}

public enum QuestType
{
    Main,
    Side,
    Daily,
    Weekly,
    Event,
    Repeatable
}

public enum QuestCategory
{
    Kill,
    Collect,
    Delivery,
    Escort,
    Exploration,
    Crafting,
    Social,
    PvP,
    Raid
}

public enum QuestObjectiveType
{
    None,
    Kill,
    Collect,
    Use,
    TalkTo,
    EnterZone,
    ReachLevel,
    DefeatBoss,
    Craft,
    Trade,
    LearnSkill,
    SpendCurrency,
    EquipItem,
    JoinGuild,
    CompleteAchievement
}

public enum QuestStatus
{
    NotStarted,
    InProgress,
    Completed,
    Failed,
    Abandoned
}

public enum QuestDifficulty
{
    Trivial,
    Easy,
    Normal,
    Hard,
    Epic,
    Legendary
}

public enum QuestFrequency
{
    OneTime,
    Daily,
    Weekly,
    Monthly,
    Repeatable,
    Infinite
}

public enum QuestResetType
{
    None,
    Daily,
    Weekly,
    Monthly,
    OnCompletion,
    OnAbandon
}

public enum QuestVisibility
{
    Visible,
    Hidden,
    Revealed,
    Discovered
}

public enum QuestPriority
{
    Low,
    Normal,
    High,
    Critical,
    Urgent
}

public enum QuestChainType
{
    None,
    Sequential,
    Parallel,
    Optional,
    Branching
}

public enum QuestFailureCondition
{
    None,
    TimeLimit,
    Death,
    LeaveZone,
    Logout,
    Custom
}

public enum QuestRewardType
{
    Fixed,
    Random,
    Scaling,
    Choice
}

public enum QuestScalingType
{
    None,
    PlayerLevel,
    PartySize,
    Difficulty,
    Time
}

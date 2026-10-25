using Mirror;
using System;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

/// <summary>
/// Manages game saving and loading
/// </summary>
public class SaveSystem : MonoBehaviour
{
    public static SaveSystem Instance { get; private set; }
    
    [Header("Save Settings")]
    public string saveDirectory = "Saves";
    public string saveExtension = ".sav";
    public int maxSaveSlots = 10;
    public bool autoSaveEnabled = true;
    public float autoSaveInterval = 300f; // 5 minutes
    public int maxAutoSaves = 5;
    
    private float lastAutoSaveTime;
    private int currentSaveSlot = 0;
    private List<string> saveFiles = new List<string>();
    
    public override void Awake()
    {
        base.Awake();
        Instance = this;
        
        // Create save directory if it doesn't exist
        CreateSaveDirectory();
        
        // Load save files list
        LoadSaveFiles();
    }
    
    private void Update()
    {
        // Handle auto-save
        if (autoSaveEnabled && Time.time - lastAutoSaveTime > autoSaveInterval)
        {
            AutoSave();
            lastAutoSaveTime = Time.time;
        }
    }
    
    private void CreateSaveDirectory()
    {
        string savePath = GetSaveDirectoryPath();
        if (!Directory.Exists(savePath))
        {
            Directory.CreateDirectory(savePath);
            Debug.Log($"Created save directory: {savePath}");
        }
    }
    
    private string GetSaveDirectoryPath()
    {
        return Path.Combine(Application.persistentDataPath, saveDirectory);
    }
    
    private string GetSaveFilePath(int slot)
    {
        return Path.Combine(GetSaveDirectoryPath(), $"save_{slot}{saveExtension}");
    }
    
    private string GetAutoSaveFilePath(int index)
    {
        return Path.Combine(GetSaveDirectoryPath(), $"autosave_{index}{saveExtension}");
    }
    
    private void LoadSaveFiles()
    {
        saveFiles.Clear();
        
        string savePath = GetSaveDirectoryPath();
        if (Directory.Exists(savePath))
        {
            DirectoryInfo dirInfo = new DirectoryInfo(savePath);
            FileInfo[] files = dirInfo.GetFiles($"*{saveExtension}");
            
            foreach (var file in files)
            {
                saveFiles.Add(file.Name);
            }
        }
    }
    
    public List<SaveSlotData> GetSaveSlots()
    {
        List<SaveSlotData> slots = new List<SaveSlotData>();
        
        for (int i = 0; i < maxSaveSlots; i++)
        {
            string filePath = GetSaveFilePath(i);
            SaveSlotData slot = new SaveSlotData
            {
                slotIndex = i,
                exists = File.Exists(filePath),
                fileName = Path.GetFileName(filePath),
                lastSaved = File.Exists(filePath) ? File.GetLastWriteTime(filePath) : DateTime.MinValue
            };
            
            // Try to load metadata
            if (File.Exists(filePath))
            {
                try
                {
                    string json = File.ReadAllText(filePath);
                    SaveData saveData = JsonUtility.FromJson<SaveData>(json);
                    
                    if (saveData != null)
                    {
                        slot.playerName = saveData.playerName;
                        slot.level = saveData.level;
                        slot.zone = saveData.zone;
                        slot.playTime = saveData.playTime;
                    }
                }
                catch (Exception e)
                {
                    Debug.LogError($"Error loading save slot {i}: {e.Message}");
                }
            }
            
            slots.Add(slot);
        }
        
        return slots;
    }
    
    public void SaveGame(int slot = -1)
    {
        if (slot < 0 || slot >= maxSaveSlots)
        {
            // Use current slot
            slot = currentSaveSlot;
        }
        
        string filePath = GetSaveFilePath(slot);
        
        try
        {
            // Create save data
            SaveData saveData = CreateSaveData();
            
            // Convert to JSON
            string json = JsonUtility.ToJson(saveData, true);
            
            // Write to file
            File.WriteAllText(filePath, json);
            
            Debug.Log($"Game saved to slot {slot}: {filePath}");
            
            // Update save files list
            LoadSaveFiles();
        }
        catch (Exception e)
        {
            Debug.LogError($"Error saving game: {e.Message}");
        }
    }
    
    public void AutoSave()
    {
        // Rotate auto-saves
        for (int i = maxAutoSaves - 1; i > 0; i--)
        {
            string oldPath = GetAutoSaveFilePath(i - 1);
            string newPath = GetAutoSaveFilePath(i);
            
            if (File.Exists(oldPath))
            {
                File.Move(oldPath, newPath);
            }
        }
        
        // Create new auto-save
        string filePath = GetAutoSaveFilePath(0);
        
        try
        {
            // Create save data
            SaveData saveData = CreateSaveData();
            
            // Convert to JSON
            string json = JsonUtility.ToJson(saveData, true);
            
            // Write to file
            File.WriteAllText(filePath, json);
            
            Debug.Log($"Auto-save created: {filePath}");
        }
        catch (Exception e)
        {
            Debug.LogError($"Error auto-saving game: {e.Message}");
        }
    }
    
    private SaveData CreateSaveData()
    {
        SaveData saveData = new SaveData
        {
            saveVersion = 1,
            gameVersion = Application.version,
            savedAt = DateTime.Now,
            playTime = Time.realtimeSinceStartup
        };
        
        // Save player data
        PlayerController localPlayer = FindObjectOfType<PlayerController>();
        if (localPlayer != null && localPlayer.isLocalPlayer)
        {
            saveData.playerName = localPlayer.playerName;
            saveData.characterClass = localPlayer.characterClass;
            saveData.level = localPlayer.level;
            saveData.experience = localPlayer.experience;
            saveData.harmonyGems = localPlayer.harmonyGems;
            saveData.currentZone = localPlayer.currentZone;
            saveData.position = localPlayer.transform.position;
            saveData.rotation = localPlayer.transform.rotation;
            saveData.stats = localPlayer.syncStats;
        }
        
        // Save inventory
        if (InventoryManager.Instance != null)
        {
            saveData.inventorySlots = InventoryManager.Instance.GetInventorySlots();
            saveData.equipmentSlots = InventoryManager.Instance.GetEquipmentSlots();
        }
        
        // Save quests
        if (QuestSystem.Instance != null)
        {
            saveData.activeQuestIds = new List<string>();
            foreach (var slot in QuestSystem.Instance.activeQuests)
            {
                saveData.activeQuestIds.Add(slot.questId);
            }
            saveData.completedQuestIds = QuestSystem.Instance.completedQuestIds;
        }
        
        // Save abilities
        if (AbilitySystem.Instance != null)
        {
            saveData.abilitySlots = AbilitySystem.Instance.abilitySlots;
        }
        
        // Save social data
        if (SocialSystem.Instance != null)
        {
            saveData.friends = SocialSystem.Instance.friends;
            saveData.blockedPlayers = SocialSystem.Instance.blockedPlayers;
        }
        
        // Save settings
        saveData.settings = new GameSettingsData
        {
            soundEnabled = true,
            volume = 0.8f,
            qualityLevel = QualitySettings.GetQualityLevel()
        };
        
        return saveData;
    }
    
    public bool LoadGame(int slot = -1)
    {
        if (slot < 0 || slot >= maxSaveSlots)
        {
            // Use current slot
            slot = currentSaveSlot;
        }
        
        string filePath = GetSaveFilePath(slot);
        
        if (!File.Exists(filePath))
        {
            Debug.LogWarning($"No save file found at slot {slot}");
            return false;
        }
        
        try
        {
            // Read JSON
            string json = File.ReadAllText(filePath);
            SaveData saveData = JsonUtility.FromJson<SaveData>(json);
            
            if (saveData == null)
            {
                Debug.LogError("Failed to parse save data");
                return false;
            }
            
            // Load game data
            LoadSaveData(saveData);
            
            Debug.Log($"Game loaded from slot {slot}");
            return true;
        }
        catch (Exception e)
        {
            Debug.LogError($"Error loading game: {e.Message}");
            return false;
        }
    }
    
    private void LoadSaveData(SaveData saveData)
    {
        // Load player data
        PlayerController localPlayer = FindObjectOfType<PlayerController>();
        if (localPlayer != null && localPlayer.isLocalPlayer)
        {
            localPlayer.playerName = saveData.playerName;
            localPlayer.characterClass = saveData.characterClass;
            localPlayer.level = saveData.level;
            localPlayer.experience = saveData.experience;
            localPlayer.harmonyGems = saveData.harmonyGems;
            localPlayer.currentZone = saveData.currentZone;
            localPlayer.transform.position = saveData.position;
            localPlayer.transform.rotation = saveData.rotation;
            localPlayer.syncStats = saveData.stats;
        }
        
        // Load inventory
        if (InventoryManager.Instance != null && saveData.inventorySlots != null)
        {
            InventoryManager.Instance.inventorySlots = saveData.inventorySlots;
            InventoryManager.Instance.equipmentSlots = saveData.equipmentSlots;
            InventoryManager.Instance.harmonyGems = saveData.harmonyGems;
        }
        
        // Load quests
        if (QuestSystem.Instance != null && saveData.activeQuestIds != null)
        {
            QuestSystem.Instance.activeQuests.Clear();
            foreach (var questId in saveData.activeQuestIds)
            {
                QuestSystem.Instance.activeQuests.Add(new QuestSlot
                {
                    questId = questId,
                    progress = 0,
                    isTracked = false
                });
            }
            QuestSystem.Instance.completedQuestIds = saveData.completedQuestIds;
        }
        
        // Load abilities
        if (AbilitySystem.Instance != null && saveData.abilitySlots != null)
        {
            AbilitySystem.Instance.abilitySlots = saveData.abilitySlots;
        }
        
        // Load social data
        if (SocialSystem.Instance != null)
        {
            SocialSystem.Instance.friends = saveData.friends;
            SocialSystem.Instance.blockedPlayers = saveData.blockedPlayers;
        }
        
        // Load settings
        if (saveData.settings != null)
        {
            QualitySettings.SetQualityLevel(saveData.settings.qualityLevel);
        }
    }
    
    public void DeleteSave(int slot)
    {
        string filePath = GetSaveFilePath(slot);
        
        if (File.Exists(filePath))
        {
            File.Delete(filePath);
            Debug.Log($"Save deleted from slot {slot}");
            
            // Update save files list
            LoadSaveFiles();
        }
    }
    
    public void SetCurrentSaveSlot(int slot)
    {
        if (slot >= 0 && slot < maxSaveSlots)
        {
            currentSaveSlot = slot;
        }
    }
    
    public int GetCurrentSaveSlot()
    {
        return currentSaveSlot;
    }
    
    public List<string> GetSaveFileNames()
    {
        return new List<string>(saveFiles);
    }
    
    public bool HasSave(int slot)
    {
        string filePath = GetSaveFilePath(slot);
        return File.Exists(filePath);
    }
    
    public DateTime GetSaveTime(int slot)
    {
        string filePath = GetSaveFilePath(slot);
        if (File.Exists(filePath))
        {
            return File.GetLastWriteTime(filePath);
        }
        return DateTime.MinValue;
    }
    
    public string GetSaveInfo(int slot)
    {
        string filePath = GetSaveFilePath(slot);
        if (!File.Exists(filePath)) return "Empty";
        
        try
        {
            string json = File.ReadAllText(filePath);
            SaveData saveData = JsonUtility.FromJson<SaveData>(json);
            
            if (saveData != null)
            {
                return $"{saveData.playerName} - Level {saveData.level} - {saveData.currentZone}";
            }
        }
        catch (Exception e)
        {
            Debug.LogError($"Error reading save info: {e.Message}");
        }
        
        return "Corrupted";
    }
    
    public void ImportSave(string filePath)
    {
        if (!File.Exists(filePath)) return;
        
        try
        {
            string json = File.ReadAllText(filePath);
            SaveData saveData = JsonUtility.FromJson<SaveData>(json);
            
            if (saveData != null)
            {
                LoadSaveData(saveData);
                Debug.Log("Save imported successfully");
            }
        }
        catch (Exception e)
        {
            Debug.LogError($"Error importing save: {e.Message}");
        }
    }
    
    public void ExportSave(string filePath, int slot = -1)
    {
        if (slot < 0 || slot >= maxSaveSlots)
        {
            slot = currentSaveSlot;
        }
        
        string sourcePath = GetSaveFilePath(slot);
        if (!File.Exists(sourcePath)) return;
        
        try
        {
            File.Copy(sourcePath, filePath, true);
            Debug.Log("Save exported successfully");
        }
        catch (Exception e)
        {
            Debug.LogError($"Error exporting save: {e.Message}");
        }
    }
    
    public void BackupSaves()
    {
        string backupPath = Path.Combine(GetSaveDirectoryPath(), "Backup");
        if (!Directory.Exists(backupPath))
        {
            Directory.CreateDirectory(backupPath);
        }
        
        string timestamp = DateTime.Now.ToString("yyyyMMdd_HHmmss");
        string backupFilePath = Path.Combine(backupPath, $"backup_{timestamp}.zip");
        
        // In a real implementation, this would create a zip file
        // For now, just copy all save files
        for (int i = 0; i < maxSaveSlots; i++)
        {
            string sourcePath = GetSaveFilePath(i);
            if (File.Exists(sourcePath))
            {
                string destPath = Path.Combine(backupPath, Path.GetFileName(sourcePath));
                File.Copy(sourcePath, destPath, true);
            }
        }
        
        Debug.Log($"Saves backed up to: {backupPath}");
    }
    
    public void RestoreBackup(string backupPath)
    {
        if (!File.Exists(backupPath)) return;
        
        // In a real implementation, this would extract from zip
        // For now, just copy the file
        string fileName = Path.GetFileName(backupPath);
        string destPath = Path.Combine(GetSaveDirectoryPath(), fileName);
        
        File.Copy(backupPath, destPath, true);
        
        Debug.Log("Backup restored successfully");
    }
}

[Serializable]
public class SaveSlotData
{
    public int slotIndex;
    public bool exists;
    public string fileName;
    public string playerName;
    public int level;
    public string zone;
    public DateTime lastSaved;
    public float playTime;
}

[Serializable]
public class SaveData
{
    public int saveVersion;
    public string gameVersion;
    public DateTime savedAt;
    public float playTime;
    
    // Player data
    public string playerName;
    public CharacterClass characterClass;
    public int level;
    public long experience;
    public int harmonyGems;
    public string currentZone;
    public Vector3 position;
    public Quaternion rotation;
    public PlayerStats stats;
    
    // Inventory
    public List<InventorySlot> inventorySlots;
    public List<EquipmentSlot> equipmentSlots;
    
    // Quests
    public List<string> activeQuestIds;
    public List<string> completedQuestIds;
    
    // Abilities
    public List<AbilitySlot> abilitySlots;
    
    // Social
    public List<string> friends;
    public List<string> blockedPlayers;
    
    // Settings
    public GameSettingsData settings;
    
    // World state
    public List<ZoneState> zoneStates;
    public List<BossState> bossStates;
    
    // Achievements
    public List<string> unlockedAchievements;
    
    // Customization
    public PlayerCustomization customization;
}

[Serializable]
public class ZoneState
{
    public string zoneId;
    public List<string> discoveredAreas;
    public List<string> defeatedEnemies;
    public List<string> collectedItems;
}

[Serializable]
public class BossState
{
    public string bossId;
    public bool isDefeated;
    public DateTime lastDefeated;
    public int defeatCount;
}

[Serializable]
public class PlayerCustomization
{
    public string hairStyle;
    public string hairColor;
    public string faceType;
    public string bodyType;
    public string outfit;
    public string instrument;
    public List<string> unlockedCosmetics;
}

[Serializable]
public class SaveMetadata
{
    public string saveName;
    public string playerName;
    public int level;
    public string zone;
    public DateTime savedAt;
    public float playTime;
    public string gameVersion;
}

public enum SaveType
{
    Manual,
    Auto,
    Quick,
    Backup
}

public enum SaveCompression
{
    None,
    Gzip,
    Lzma,
    Bzip2
}

public enum SaveEncryption
{
    None,
    Aes,
    Rsa,
    Xor
}

public enum SaveFormat
{
    Json,
    Binary,
    Xml,
    Sqlite
}

public enum SaveStorage
{
    Local,
    Cloud,
    Server,
    Hybrid
}

public enum SaveConflict
{
    Overwrite,
    Rename,
    Skip,
    Merge
}

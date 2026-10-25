using Mirror;
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

/// <summary>
/// Manages game zones/worlds and transitions between them
/// </summary>
public class ZoneManager : NetworkBehaviour
{
    public static ZoneManager Instance { get; private set; }
    
    [Header("Zone Settings")]
    public List<ZoneData> zones = new List<ZoneData>();
    public string startingZone = "SoundCity";
    
    [Header("References")]
    public Transform[] spawnPoints;
    
    [SyncVar(hook = nameof(OnCurrentZoneChanged))]
    public string currentZoneId;
    
    private Dictionary<string, ZoneData> zoneDictionary = new Dictionary<string, ZoneData>();
    private Dictionary<string, List<PlayerController>> zonePlayers = new Dictionary<string, List<PlayerController>>();
    private Dictionary<string, BossController> activeBosses = new Dictionary<string, BossController>();
    
    public override void Awake()
    {
        base.Awake();
        Instance = this;
        
        // Initialize zone dictionary
        foreach (var zone in zones)
        {
            zoneDictionary[zone.id] = zone;
            zonePlayers[zone.id] = new List<PlayerController>();
        }
    }
    
    public override void OnStartServer()
    {
        base.OnStartServer();
        
        // Set starting zone
        currentZoneId = startingZone;
        
        // Spawn bosses for each zone
        SpawnZoneBosses();
        
        Debug.Log("Zone Manager started on server");
    }
    
    public override void OnStopServer()
    {
        base.OnStopServer();
        
        // Clean up bosses
        foreach (var boss in activeBosses.Values)
        {
            if (boss != null && boss.gameObject != null)
            {
                Destroy(boss.gameObject);
            }
        }
        activeBosses.Clear();
        
        Debug.Log("Zone Manager stopped on server");
    }
    
    private void SpawnZoneBosses()
    {
        foreach (var zone in zones)
        {
            if (zone.boss != null)
            {
                SpawnBoss(zone.id, zone.boss);
            }
        }
    }
    
    private void SpawnBoss(string zoneId, BossData bossData)
    {
        // Find boss prefab by name
        GameObject bossPrefab = Resources.Load<GameObject>($"Bosses/{bossData.id}");
        if (bossPrefab == null)
        {
            Debug.LogWarning($"Boss prefab not found: {bossData.id}");
            return;
        }
        
        // Find spawn point for boss
        Transform bossSpawn = GameObject.FindGameObjectWithTag("BossSpawn")?.transform;
        if (bossSpawn == null)
        {
            bossSpawn = spawnPoints.Length > 0 ? spawnPoints[0] : null;
        }
        
        Vector3 spawnPosition = bossSpawn != null ? bossSpawn.position : Vector3.zero;
        
        // Instantiate boss
        GameObject bossObj = Instantiate(bossPrefab, spawnPosition, Quaternion.identity);
        BossController bossController = bossObj.GetComponent<BossController>();
        
        if (bossController != null)
        {
            bossController.Initialize(bossData);
            activeBosses[zoneId] = bossController;
            
            // Register boss with network
            NetworkServer.Spawn(bossObj);
            
            Debug.Log($"Boss spawned: {bossData.name} in {zoneId}");
        }
        else
        {
            Debug.LogError($"Boss prefab missing BossController: {bossData.id}");
            Destroy(bossObj);
        }
    }
    
    public ZoneData GetZone(string zoneId)
    {
        zoneDictionary.TryGetValue(zoneId, out ZoneData zone);
        return zone;
    }
    
    public ZoneData GetCurrentZone()
    {
        return GetZone(currentZoneId);
    }
    
    public List<ZoneData> GetAvailableZones(PlayerController player)
    {
        List<ZoneData> availableZones = new List<ZoneData>();
        
        foreach (var zone in zones)
        {
            if (player.level >= zone.minLevel)
            {
                availableZones.Add(zone);
            }
        }
        
        return availableZones;
    }
    
    public void ChangeZone(PlayerController player, string zoneId)
    {
        if (isServer)
        {
            var zone = GetZone(zoneId);
            if (zone == null)
            {
                Debug.LogWarning($"Zone not found: {zoneId}");
                return;
            }
            
            // Check if player can enter this zone
            if (player.level < zone.minLevel)
            {
                Debug.LogWarning($"Player level {player.level} too low for zone {zoneId} (requires {zone.minLevel})");
                return;
            }
            
            // Remove player from current zone
            if (zonePlayers.TryGetValue(player.currentZone, out List<PlayerController> currentPlayers))
            {
                currentPlayers.Remove(player);
            }
            
            // Add player to new zone
            if (!zonePlayers.ContainsKey(zoneId))
            {
                zonePlayers[zoneId] = new List<PlayerController>();
            }
            zonePlayers[zoneId].Add(player);
            
            // Update player zone
            player.currentZone = zoneId;
            
            // Find spawn point in new zone
            Transform spawnPoint = GetZoneSpawnPoint(zoneId);
            Vector3 spawnPosition = spawnPoint != null ? spawnPoint.position : Vector3.zero;
            
            // Teleport player
            player.transform.position = spawnPosition;
            
            // Update current zone if this is the local player
            if (player.isLocalPlayer)
            {
                currentZoneId = zoneId;
            }
            
            Debug.Log($"{player.playerName} changed to zone: {zoneId}");
        }
        else
        {
            // Client requests zone change
            CmdChangeZone(zoneId);
        }
    }
    
    [Command]
    private void CmdChangeZone(string zoneId)
    {
        PlayerController player = NetworkServer.connections[connectionToClient.identity.netId].identity.GetComponent<PlayerController>();
        if (player != null)
        {
            ChangeZone(player, zoneId);
        }
    }
    
    private Transform GetZoneSpawnPoint(string zoneId)
    {
        // Try to find zone-specific spawn point
        Transform zoneSpawn = GameObject.FindGameObjectWithTag($"Spawn_{zoneId}")?.transform;
        if (zoneSpawn != null)
        {
            return zoneSpawn;
        }
        
        // Fall back to random spawn point
        if (spawnPoints.Length > 0)
        {
            return spawnPoints[UnityEngine.Random.Range(0, spawnPoints.Length)];
        }
        
        return null;
    }
    
    public void RegisterPlayerInZone(PlayerController player)
    {
        if (!zonePlayers.ContainsKey(player.currentZone))
        {
            zonePlayers[player.currentZone] = new List<PlayerController>();
        }
        
        if (!zonePlayers[player.currentZone].Contains(player))
        {
            zonePlayers[player.currentZone].Add(player);
        }
    }
    
    public void UnregisterPlayerFromZone(PlayerController player)
    {
        if (zonePlayers.TryGetValue(player.currentZone, out List<PlayerController> players))
        {
            players.Remove(player);
        }
    }
    
    public List<PlayerController> GetPlayersInZone(string zoneId)
    {
        zonePlayers.TryGetValue(zoneId, out List<PlayerController> players);
        return players ?? new List<PlayerController>();
    }
    
    public List<PlayerController> GetPlayersInCurrentZone()
    {
        return GetPlayersInZone(currentZoneId);
    }
    
    public BossController GetBossInZone(string zoneId)
    {
        activeBosses.TryGetValue(zoneId, out BossController boss);
        return boss;
    }
    
    public BossController GetCurrentZoneBoss()
    {
        return GetBossInZone(currentZoneId);
    }
    
    public void DefeatBoss(string zoneId)
    {
        if (isServer)
        {
            var boss = GetBossInZone(zoneId);
            if (boss != null)
            {
                // Remove boss
                activeBosses.Remove(zoneId);
                NetworkServer.Destroy(boss.gameObject);
                
                // Respawn boss after delay
                Invoke(nameof(RespawnBoss), 300f); // 5 minutes
                
                Debug.Log($"Boss defeated in zone: {zoneId}");
            }
        }
    }
    
    private void RespawnBoss()
    {
        if (isServer)
        {
            var zone = GetCurrentZone();
            if (zone != null && zone.boss != null)
            {
                SpawnBoss(currentZoneId, zone.boss);
            }
        }
    }
    
    public void StartBossBattle(PlayerController player, string zoneId)
    {
        var boss = GetBossInZone(zoneId);
        if (boss != null)
        {
            // Notify boss
            boss.StartBattle(player);
            
            // Notify player
            player.SetActiveBoss(boss.GetBossData());
            
            Debug.Log($"Boss battle started: {player.playerName} vs {boss.bossData.name}");
        }
    }
    
    public void EndBossBattle(PlayerController player, string zoneId, bool victory)
    {
        var boss = GetBossInZone(zoneId);
        if (boss != null)
        {
            boss.EndBattle(victory);
            
            if (victory)
            {
                // Award rewards
                var zone = GetZone(zoneId);
                if (zone != null && zone.boss != null)
                {
                    player.GainExperience(zone.boss.rewardXp);
                    player.AddHarmonyGems(zone.boss.rewardGems);
                    
                    // Complete quest if applicable
                    CompleteZoneQuests(player, zoneId);
                }
                
                // Defeat boss
                DefeatBoss(zoneId);
            }
            
            player.SetActiveBoss(null);
            
            Debug.Log($"Boss battle ended: {player.playerName} - {(victory ? "Victory" : "Defeat")}");
        }
    }
    
    private void CompleteZoneQuests(PlayerController player, string zoneId)
    {
        var zone = GetZone(zoneId);
        if (zone == null) return;
        
        foreach (var quest in zone.quests)
        {
            if (quest.targetBossId == zone.boss?.id && !quest.isCompleted)
            {
                // Mark quest as completed
                quest.isCompleted = true;
                
                // Award quest rewards
                player.GainExperience(quest.rewardXp);
                player.AddHarmonyGems(quest.rewardGems);
                
                Debug.Log($"Quest completed: {quest.title}");
            }
        }
    }
    
    // SyncVar hook
    private void OnCurrentZoneChanged(string oldZoneId, string newZoneId)
    {
        Debug.Log($"Current zone changed from {oldZoneId} to {newZoneId}");
        
        // Update UI
        if (UIManager.Instance != null)
        {
            var zone = GetZone(newZoneId);
            if (zone != null)
            {
                UIManager.Instance.UpdateZoneInfo(zone);
            }
        }
    }
}

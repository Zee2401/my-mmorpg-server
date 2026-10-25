using Mirror;
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

/// <summary>
/// Main game manager that coordinates all game systems
/// </summary>
public class GameManager : NetworkBehaviour
{
    public static GameManager Instance { get; private set; }
    
    [Header("Game Settings")]
    public string gameName = "Music MMORPG";
    public string gameVersion = "1.0.0";
    public bool autoSave = true;
    public float autoSaveInterval = 300f; // 5 minutes
    
    [Header("References")]
    public NetworkManagerMMO networkManager;
    public ZoneManager zoneManager;
    public UIManager uiManager;
    public InputManager inputManager;
    public AudioManager audioManager;
    
    [SyncVar]
    public GameState currentGameState = GameState.MainMenu;
    
    [SyncVar]
    public string serverName = "Music MMORPG Server";
    
    [SyncVar]
    public int maxPlayers = 100;
    
    [SyncVar]
    public int currentPlayerCount = 0;
    
    private float lastAutoSaveTime;
    private List<PlayerController> connectedPlayers = new List<PlayerController>();
    
    public enum GameState
    {
        MainMenu,
        Loading,
        Playing,
        Paused,
        GameOver
    }
    
    public override void Awake()
    {
        base.Awake();
        Instance = this;
    }
    
    private void Start()
    {
        // Initialize game
        InitializeGame();
    }
    
    private void Update()
    {
        // Handle auto-save
        if (autoSave && isServer && Time.time - lastAutoSaveTime > autoSaveInterval)
        {
            SaveGame();
            lastAutoSaveTime = Time.time;
        }
    }
    
    private void InitializeGame()
    {
        Debug.Log("Initializing Game Manager");
        
        // Set game state
        SetGameState(GameState.MainMenu);
        
        // Initialize managers
        if (networkManager == null)
            networkManager = FindObjectOfType<NetworkManagerMMO>();
        
        if (zoneManager == null)
            zoneManager = FindObjectOfType<ZoneManager>();
        
        if (uiManager == null)
            uiManager = FindObjectOfType<UIManager>();
        
        if (inputManager == null)
            inputManager = FindObjectOfType<InputManager>();
        
        if (audioManager == null)
            audioManager = FindObjectOfType<AudioManager>();
        
        // Setup network callbacks
        if (networkManager != null)
        {
            networkManager.onServerReady += OnServerReady;
            networkManager.onClientConnect += OnClientConnect;
            networkManager.onClientDisconnect += OnClientDisconnect;
        }
        
        Debug.Log("Game Manager initialized");
    }
    
    private void OnServerReady()
    {
        Debug.Log("Server is ready");
        SetGameState(GameState.Playing);
    }
    
    private void OnClientConnect()
    {
        Debug.Log("Client connected");
        currentPlayerCount++;
    }
    
    private void OnClientDisconnect()
    {
        Debug.Log("Client disconnected");
        currentPlayerCount--;
    }
    
    public void SetGameState(GameState newState)
    {
        currentGameState = newState;
        
        switch (newState)
        {
            case GameState.MainMenu:
                OnMainMenu();
                break;
            case GameState.Loading:
                OnLoading();
                break;
            case GameState.Playing:
                OnPlaying();
                break;
            case GameState.Paused:
                OnPaused();
                break;
            case GameState.GameOver:
                OnGameOver();
                break;
        }
        
        Debug.Log($"Game state changed to: {newState}");
    }
    
    private void OnMainMenu()
    {
        // Show main menu
        if (uiManager != null)
        {
            uiManager.ShowHUD(false);
            uiManager.ShowMenu();
        }
        
        // Stop all audio
        if (audioManager != null)
        {
            audioManager.StopAll();
        }
    }
    
    private void OnLoading()
    {
        // Show loading screen
        if (uiManager != null)
        {
            uiManager.ShowLoadingScreen(true, "Loading...");
        }
    }
    
    private void OnPlaying()
    {
        // Hide loading screen
        if (uiManager != null)
        {
            uiManager.ShowLoadingScreen(false);
            uiManager.ShowHUD(true);
        }
        
        // Resume audio
        if (audioManager != null)
        {
            audioManager.ResumeAll();
        }
    }
    
    private void OnPaused()
    {
        // Pause game
        Time.timeScale = 0f;
        
        // Show pause menu
        if (uiManager != null)
        {
            uiManager.ShowMenu();
        }
        
        // Pause audio
        if (audioManager != null)
        {
            audioManager.PauseAll();
        }
    }
    
    private void OnGameOver()
    {
        // Game over logic
        Time.timeScale = 0f;
        
        if (uiManager != null)
        {
            uiManager.ShowHUD(false);
        }
    }
    
    public void StartGame()
    {
        if (isClient)
        {
            SetGameState(GameState.Loading);
            
            // Connect to server or start as host
            if (NetworkClient.active)
            {
                // Already connected
                SetGameState(GameState.Playing);
            }
            else
            {
                // Connect to server
                NetworkClient.Connect("localhost");
            }
        }
        else
        {
            // Start as host
            NetworkManager.singleton.StartHost();
        }
    }
    
    public void HostGame()
    {
        SetGameState(GameState.Loading);
        NetworkManager.singleton.StartHost();
    }
    
    public void JoinGame(string ipAddress)
    {
        SetGameState(GameState.Loading);
        NetworkClient.Connect(ipAddress);
    }
    
    public void Disconnect()
    {
        if (NetworkClient.active)
        {
            NetworkClient.Disconnect();
        }
        
        if (NetworkServer.active)
        {
            NetworkServer.Shutdown();
        }
        
        SetGameState(GameState.MainMenu);
    }
    
    public void PauseGame()
    {
        if (currentGameState == GameState.Playing)
        {
            SetGameState(GameState.Paused);
        }
    }
    
    public void ResumeGame()
    {
        if (currentGameState == GameState.Paused)
        {
            SetGameState(GameState.Playing);
            Time.timeScale = 1f;
        }
    }
    
    public void QuitGame()
    {
        Debug.Log("Quitting game");
        
        // Save game before quitting
        SaveGame();
        
        // Disconnect
        Disconnect();
        
        // Quit application
        Application.Quit();
    }
    
    public void RegisterPlayer(PlayerController player)
    {
        if (!connectedPlayers.Contains(player))
        {
            connectedPlayers.Add(player);
            currentPlayerCount = connectedPlayers.Count;
            
            Debug.Log($"Player registered: {player.playerName}");
        }
    }
    
    public void UnregisterPlayer(PlayerController player)
    {
        if (connectedPlayers.Contains(player))
        {
            connectedPlayers.Remove(player);
            currentPlayerCount = connectedPlayers.Count;
            
            Debug.Log($"Player unregistered: {player.playerName}");
        }
    }
    
    public List<PlayerController> GetAllPlayers()
    {
        return new List<PlayerController>(connectedPlayers);
    }
    
    public PlayerController GetLocalPlayer()
    {
        foreach (var player in connectedPlayers)
        {
            if (player.isLocalPlayer)
            {
                return player;
            }
        }
        return null;
    }
    
    public void SaveGame()
    {
        if (!isServer) return;
        
        Debug.Log("Saving game...");
        
        // Save all players
        foreach (var player in connectedPlayers)
        {
            player.SavePlayerData();
        }
        
        // Save game state
        SaveData saveData = new SaveData
        {
            gameState = CreateGameStateData(),
            players = new Dictionary<string, PlayerData>(),
            inventoryItems = new Dictionary<string, InventoryItemData>()
        };
        
        foreach (var player in connectedPlayers)
        {
            saveData.players[player.netId.ToString()] = player.GetPlayerData();
        }
        
        // Save to file
        string jsonData = JsonUtility.ToJson(saveData);
        System.IO.File.WriteAllText(Application.persistentDataPath + "/savegame.json", jsonData);
        
        Debug.Log("Game saved");
    }
    
    public void LoadGame()
    {
        if (!isServer) return;
        
        Debug.Log("Loading game...");
        
        string savePath = Application.persistentDataPath + "/savegame.json";
        if (System.IO.File.Exists(savePath))
        {
            string jsonData = System.IO.File.ReadAllText(savePath);
            SaveData saveData = JsonUtility.FromJson<SaveData>(jsonData);
            
            if (saveData != null)
            {
                // Load game state
                LoadGameState(saveData.gameState);
                
                // Load players
                foreach (var playerData in saveData.players)
                {
                    // Find player by netId
                    foreach (var player in connectedPlayers)
                    {
                        if (player.netId.ToString() == playerData.Key)
                        {
                            player.LoadPlayerData(playerData.Value);
                            break;
                        }
                    }
                }
                
                Debug.Log("Game loaded");
            }
        }
        else
        {
            Debug.LogWarning("No save file found");
        }
    }
    
    private GameState CreateGameStateData()
    {
        return new GameState
        {
            worldData = zoneManager != null ? zoneManager.CreateWorldData() : new WorldData(),
            activeBosses = new Dictionary<string, BossData>(),
            bands = new List<BandData>()
        };
    }
    
    private void LoadGameState(GameState gameState)
    {
        if (zoneManager != null && gameState.worldData != null)
        {
            zoneManager.LoadWorldData(gameState.worldData);
        }
    }
    
    public void CreateNewCharacter(string name, CharacterClass characterClass)
    {
        if (isClient)
        {
            // Store player preferences
            PlayerPrefs.SetString("PlayerName", name);
            PlayerPrefs.SetInt("CharacterClass", (int)characterClass);
            PlayerPrefs.Save();
            
            // Create player on server
            if (NetworkClient.active)
            {
                NetworkClient.Send(new CreatePlayerMessage
                {
                    playerName = name,
                    characterClass = characterClass
                });
            }
        }
    }
    
    public void DeleteCharacter(string characterId)
    {
        // Remove character data
        PlayerPrefs.DeleteKey("PlayerName");
        PlayerPrefs.DeleteKey("CharacterClass");
        PlayerPrefs.Save();
        
        Debug.Log($"Character deleted: {characterId}");
    }
    
    public List<CharacterData> GetAvailableCharacters()
    {
        List<CharacterData> characters = new List<CharacterData>();
        
        if (PlayerPrefs.HasKey("PlayerName"))
        {
            characters.Add(new CharacterData
            {
                name = PlayerPrefs.GetString("PlayerName"),
                characterClass = (CharacterClass)PlayerPrefs.GetInt("CharacterClass", 0),
                level = 1,
                lastPlayed = DateTime.Now
            });
        }
        
        return characters;
    }
}

[Serializable]
public class CharacterData
{
    public string name;
    public CharacterClass characterClass;
    public int level;
    public DateTime lastPlayed;
}

[Serializable]
public class GameSettingsData
{
    public bool soundEnabled;
    public float volume;
    public string language;
    public bool subtitlesEnabled;
    public bool fullscreen;
    public int resolutionWidth;
    public int resolutionHeight;
    public int qualityLevel;
}

[Serializable]
public class GraphicsSettings
{
    public int resolutionWidth;
    public int resolutionHeight;
    public bool fullscreen;
    public int qualityLevel;
    public float brightness;
    public bool vsyncEnabled;
    public int targetFrameRate;
}

[Serializable]
public class AudioSettings
{
    public bool soundEnabled;
    public float masterVolume;
    public float musicVolume;
    public float sfxVolume;
    public float voiceVolume;
}

[Serializable]
public class ControlSettings
{
    public Dictionary<string, KeyCode> keyBindings;
    public float mouseSensitivity;
    public bool invertYAxis;
    public bool controllerEnabled;
}

[Serializable]
public class NetworkSettings
{
    public string serverIp;
    public int serverPort;
    public string playerName;
    public bool autoConnect;
}

[Serializable]
public class SocialSettings
{
    public bool showPlayerNames;
    public bool showChat;
    public bool showPartyInvites;
    public bool showFriendRequests;
}

[Serializable]
public class AccessibilitySettings
{
    public bool colorblindMode;
    public bool dyslexiaFriendly;
    public float textSize;
    public bool subtitlesEnabled;
    public bool highContrast;
}

[Serializable]
public class PrivacySettings
{
    public bool shareData;
    public bool showOnlineStatus;
    public bool allowFriendRequests;
    public bool allowPartyInvites;
}

[Serializable]
public class AccountData
{
    public string accountId;
    public string username;
    public string email;
    public DateTime createdDate;
    public DateTime lastLogin;
    public List<string> characters;
    public List<string> friends;
    public List<string> blockedPlayers;
}

[Serializable]
public class ServerSettings
{
    public string serverName;
    public string serverDescription;
    public int maxPlayers;
    public string gameMode;
    public bool passwordProtected;
    public string password;
    public bool pvpEnabled;
    public bool friendlyFire;
    public int difficulty;
}

[Serializable]
public class WorldSettings
{
    public string worldName;
    public int difficulty;
    public float xpRate;
    public float dropRate;
    public float goldRate;
    public bool permadeath;
    public bool hardMode;
}

[Serializable]
public class CombatSettings
{
    public float attackSpeed;
    public float castSpeed;
    public float cooldownReduction;
    public float criticalChance;
    public float criticalDamage;
    public bool autoTarget;
    public bool lockOn;
}

[Serializable]
public class CameraSettings
{
    public float fieldOfView;
    public float nearClipPlane;
    public float farClipPlane;
    public Vector3 offset;
    public float followSpeed;
    public float rotationSpeed;
    public bool invertX;
    public bool invertY;
}

[Serializable]
public class PerformanceSettings
{
    public int maxFps;
    public int vSyncCount;
    public float lodDistance;
    public int shadowQuality;
    public int textureQuality;
    public int anisotropicFiltering;
}

[Serializable]
public class DebugSettings
{
    public bool showFps;
    public bool showPing;
    public bool showCoordinates;
    public bool showHitboxes;
    public bool showPathfinding;
    public bool showNetworkStats;
}

[Serializable]
public class ExperimentalSettings
{
    public bool enableNewFeatures;
    public bool useNewInputSystem;
    public bool enableMods;
    public bool enableCheats;
    public bool developerMode;
}

[Serializable]
public class ModSettings
{
    public List<string> enabledMods;
    public List<string> disabledMods;
    public string modDirectory;
}

[Serializable]
public class CheatSettings
{
    public bool godMode;
    public bool infiniteHealth;
    public bool infiniteMana;
    public bool oneHitKill;
    public bool infiniteGold;
    public bool infiniteXp;
    public bool flyMode;
    public bool noClip;
    public bool revealMap;
}

[Serializable]
public class DeveloperSettings
{
    public bool showDebugConsole;
    public bool enableLogging;
    public string logLevel;
    public bool showPerformanceStats;
    public bool enableProfiling;
}

[Serializable]
public class TestingSettings
{
    public bool enableTestCommands;
    public bool spawnTestEnemies;
    public bool spawnTestItems;
    public bool enableTestMode;
}

[Serializable]
public class BuildSettings
{
    public string buildVersion;
    public string buildDate;
    public string buildNumber;
    public string unityVersion;
    public string platform;
}

[Serializable]
public class AnalyticsSettings
{
    public bool enableAnalytics;
    public string analyticsId;
    public bool trackPerformance;
    public bool trackUsage;
    public bool trackErrors;
}

[Serializable]
public class LocalizationSettings
{
    public string currentLanguage;
    public List<string> availableLanguages;
    public Dictionary<string, string> translations;
}

[Serializable]
public class MultiplayerSettings
{
    public int maxPlayersPerServer;
    public int maxServers;
    public string matchmakingMode;
    public bool enableDedicatedServers;
    public bool enablePeerToPeer;
    public int natPunchthroughTimeout;
}

[Serializable]
public class CloudSettings
{
    public bool enableCloudSave;
    public string cloudProvider;
    public string cloudEndpoint;
    public bool autoSync;
    public int syncInterval;
}

[Serializable]
public class SocialSettingsData
{
    public List<string> friends;
    public List<string> blockedPlayers;
    public List<string> pendingFriendRequests;
    public List<string> pendingPartyInvites;
}

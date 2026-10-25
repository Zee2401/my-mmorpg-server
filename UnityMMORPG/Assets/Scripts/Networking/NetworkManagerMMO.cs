using Mirror;
using System;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Custom NetworkManager for MMORPG with player spawning and world management
/// </summary>
public class NetworkManagerMMO : NetworkManager
{
    [Header("Player Settings")]
    public GameObject playerPrefab;
    public Transform[] spawnPoints;
    
    [Header("World Settings")]
    public string defaultWorldScene = "WorldScene";
    public int maxPlayers = 100;
    
    [Header("Game Settings")]
    public float saveInterval = 300f; // 5 minutes
    
    private Dictionary<NetworkConnection, PlayerController> players = new Dictionary<NetworkConnection, PlayerController>();
    private float lastSaveTime;
    
    public static NetworkManagerMMO Instance { get; private set; }
    
    public override void Awake()
    {
        base.Awake();
        Instance = this;
    }
    
    public override void OnStartServer()
    {
        base.OnStartServer();
        Debug.Log("MMORPG Server Started");
        NetworkServer.RegisterHandler<CreatePlayerMessage>(OnCreatePlayer);
        NetworkServer.RegisterHandler<PlayerMovementMessage>(OnPlayerMovement);
    }
    
    public override void OnStopServer()
    {
        base.OnStopServer();
        Debug.Log("MMORPG Server Stopped");
    }
    
    public override void OnClientConnect()
    {
        base.OnClientConnect();
        Debug.Log("Connected to MMORPG Server");
        
        // Send player creation request
        if (isClientOnly)
        {
            var msg = new CreatePlayerMessage
            {
                playerName = PlayerPrefs.GetString("PlayerName", "Player" + UnityEngine.Random.Range(1000, 9999)),
                characterClass = (CharacterClass)PlayerPrefs.GetInt("CharacterClass", 0)
            };
            NetworkClient.Send(msg);
        }
    }
    
    public override void OnClientDisconnect()
    {
        base.OnClientDisconnect();
        Debug.Log("Disconnected from MMORPG Server");
    }
    
    private void OnCreatePlayer(NetworkConnection conn, CreatePlayerMessage msg)
    {
        Debug.Log($
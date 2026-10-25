using Mirror;
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

/// <summary>
/// Main player controller with movement, combat, and networking
/// </summary>
[RequireComponent(typeof(CharacterController))]
[RequireComponent(typeof(NavMeshAgent))]
[RequireComponent(typeof(Animator))]
public class PlayerController : NetworkBehaviour
{
    [Header("Player Settings")]
    public string playerName = "Player";
    public CharacterClass characterClass = CharacterClass.Bard;
    
    [Header("Movement Settings")]
    public float walkSpeed = 3f;
    public float runSpeed = 6f;
    public float rotationSpeed = 10f;
    public float gravity = -9.81f;
    public float jumpHeight = 1.5f;
    
    [Header("Combat Settings")]
    public float attackRange = 3f;
    public float attackCooldown = 0.5f;
    public int baseDamage = 10;
    
    [Header("References")]
    public CharacterController characterController;
    public NavMeshAgent navMeshAgent;
    public Animator animator;
    public Transform cameraTransform;
    public GameObject[] classSpecificModels;
    
    [SyncVar(hook = nameof(OnNameChanged))]
    public string syncPlayerName;
    
    [SyncVar(hook = nameof(OnClassChanged))]
    public CharacterClass syncCharacterClass;
    
    [SyncVar(hook = nameof(OnPositionChanged))]
    public Vector3 syncPosition;
    
    [SyncVar(hook = nameof(OnRotationChanged))]
    public Quaternion syncRotation;
    
    [SyncVar(hook = nameof(OnStatsChanged))]
    public PlayerStats syncStats;
    
    [SyncVar]
    public int level = 1;
    
    [SyncVar]
    public long experience = 0;
    
    [SyncVar]
    public int harmonyGems = 100;
    
    [SyncVar]
    public string currentZone = "SoundCity";
    
    private PlayerData playerData;
    private Vector3 velocity;
    private bool isGrounded;
    private float lastAttackTime;
    private float lastSyncTime;
    private const float syncInterval = 0.1f;
    
    // Input states
    private float horizontalInput;
    private float verticalInput;
    private bool isRunning;
    private bool isJumping;
    private bool isAttacking;
    
    // Network state
    private bool isInitialized = false;
    
    public override void Awake()
    {
        base.Awake();
        
        if (characterController == null)
            characterController = GetComponent<CharacterController>();
        
        if (navMeshAgent == null)
            navMeshAgent = GetComponent<NavMeshAgent>();
            
        if (animator == null)
            animator = GetComponent<Animator>();
        
        // Initialize default stats
        syncStats = new PlayerStats
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
    }
    
    public void InitializePlayer(string name, CharacterClass charClass)
    {
        playerName = name;
        characterClass = charClass;
        syncPlayerName = name;
        syncCharacterClass = charClass;
        
        // Apply class-specific stats
        ApplyClassStats(charClass);
        
        // Setup class model
        SetupClassModel(charClass);
        
        isInitialized = true;
        
        Debug.Log($"Player initialized: {name} ({charClass})");
    }
    
    private void ApplyClassStats(CharacterClass charClass)
    {
        // Base stats already set, add class bonuses
        switch (charClass)
        {
            case CharacterClass.Bard:
                syncStats.pitch += 5;
                syncStats.harmony += 5;
                break;
            case CharacterClass.Beatmaker:
                syncStats.rhythm += 5;
                syncStats.power += 5;
                break;
            case CharacterClass.Vocalist:
                syncStats.power += 5;
                syncStats.harmony += 5;
                break;
            case CharacterClass.Guitarist:
                syncStats.rhythm += 5;
                syncStats.pitch += 5;
                break;
            case CharacterClass.DJ:
                syncStats.rhythm += 3;
                syncStats.power += 3;
                syncStats.harmony += 3;
                syncStats.pitch += 3;
                break;
        }
    }
    
    private void SetupClassModel(CharacterClass charClass)
    {
        // Hide all class models first
        foreach (var model in classSpecificModels)
        {
            if (model != null)
                model.SetActive(false);
        }
        
        // Show the selected class model
        int classIndex = (int)charClass;
        if (classIndex < classSpecificModels.Length && classSpecificModels[classIndex] != null)
        {
            classSpecificModels[classIndex].SetActive(true);
        }
    }
    
    public override void OnStartLocalPlayer()
    {
        base.OnStartLocalPlayer();
        
        // Setup camera for local player
        if (cameraTransform != null)
        {
            Camera.main.transform.SetParent(cameraTransform);
            Camera.main.transform.localPosition = Vector3.zero;
            Camera.main.transform.localRotation = Quaternion.identity;
        }
        
        // Register input handlers
        InputManager.Instance.OnMovementInput += HandleMovementInput;
        InputManager.Instance.OnJumpInput += HandleJumpInput;
        InputManager.Instance.OnAttackInput += HandleAttackInput;
        InputManager.Instance.OnRunInput += HandleRunInput;
        
        // Load saved data
        LoadPlayerData();
        
        Debug.Log("Local player started");
    }
    
    public override void OnStopLocalPlayer()
    {
        base.OnStopLocalPlayer();
        
        // Unregister input handlers
        if (InputManager.Instance != null)
        {
            InputManager.Instance.OnMovementInput -= HandleMovementInput;
            InputManager.Instance.OnJumpInput -= HandleJumpInput;
            InputManager.Instance.OnAttackInput -= HandleAttackInput;
            InputManager.Instance.OnRunInput -= HandleRunInput;
        }
        
        // Save player data
        SavePlayerData();
        
        Debug.Log("Local player stopped");
    }
    
    private void HandleMovementInput(Vector2 input)
    {
        horizontalInput = input.x;
        verticalInput = input.y;
    }
    
    private void HandleJumpInput(bool jump)
    {
        isJumping = jump;
    }
    
    private void HandleAttackInput(bool attack)
    {
        isAttacking = attack;
    }
    
    private void HandleRunInput(bool run)
    {
        isRunning = run;
    }
    
    private void Update()
    {
        if (!isLocalPlayer || !isInitialized)
            return;
        
        // Handle movement
        HandleMovement();
        
        // Handle attack
        if (isAttacking && Time.time - lastAttackTime > attackCooldown)
        {
            PerformAttack();
            lastAttackTime = Time.time;
        }
        
        // Sync position to server periodically
        if (Time.time - lastSyncTime > syncInterval)
        {
            SyncPositionToServer();
            lastSyncTime = Time.time;
        }
    }
    
    private void HandleMovement()
    {
        // Calculate movement direction
        Vector3 moveDirection = new Vector3(horizontalInput, 0, verticalInput).normalized;
        
        // Apply gravity
        if (!characterController.isGrounded)
        {
            velocity.y += gravity * Time.deltaTime;
        }
        else
        {
            velocity.y = -2f; // Small downward force to keep grounded
            isGrounded = true;
        }
        
        // Jump
        if (isJumping && isGrounded)
        {
            velocity.y = Mathf.Sqrt(jumpHeight * -2f * gravity);
            isGrounded = false;
            isJumping = false;
            
            if (animator != null)
                animator.SetTrigger("Jump");
        }
        
        // Calculate speed
        float currentSpeed = isRunning ? runSpeed : walkSpeed;
        
        // Move character
        if (moveDirection.magnitude > 0.1f)
        {
            // Calculate target rotation
            float targetAngle = Mathf.Atan2(moveDirection.x, moveDirection.z) * Mathf.Rad2Deg;
            Quaternion targetRotation = Quaternion.Euler(0, targetAngle, 0);
            
            // Smooth rotation
            transform.rotation = Quaternion.Slerp(transform.rotation, targetRotation, rotationSpeed * Time.deltaTime);
            
            // Move
            Vector3 moveVector = moveDirection * currentSpeed * Time.deltaTime;
            characterController.Move(moveVector + velocity * Time.deltaTime);
            
            // Update animator
            if (animator != null)
            {
                animator.SetBool("IsMoving", true);
                animator.SetBool("IsRunning", isRunning);
                animator.SetFloat("MoveSpeed", currentSpeed);
            }
            
            // Update navmesh agent
            if (navMeshAgent != null)
            {
                navMeshAgent.SetDestination(transform.position + moveDirection * 2f);
            }
        }
        else
        {
            // Stop moving
            if (animator != null)
            {
                animator.SetBool("IsMoving", false);
                animator.SetBool("IsRunning", false);
            }
            
            if (navMeshAgent != null)
            {
                navMeshAgent.SetDestination(transform.position);
            }
        }
    }
    
    private void PerformAttack()
    {
        if (animator != null)
        {
            animator.SetTrigger("Attack");
        }
        
        // Raycast to find targets
        RaycastHit[] hits = Physics.SphereCastAll(
            transform.position + Vector3.up * 1f,
            attackRange,
            transform.forward,
            attackRange,
            LayerMask.GetMask("Enemy")
        );
        
        foreach (var hit in hits)
        {
            IDamageable damageable = hit.collider.GetComponent<IDamageable>();
            if (damageable != null)
            {
                int damage = CalculateDamage();
                damageable.TakeDamage(damage, this);
            }
        }
        
        // Network attack
        if (isServer)
        {
            // Handle server-side attack logic
        }
        else
        {
            // Send attack to server
            CmdPerformAttack();
        }
    }
    
    [Command]
    private void CmdPerformAttack()
    {
        // Server handles attack validation and damage
        RpcPerformAttack();
    }
    
    [ClientRpc]
    private void RpcPerformAttack()
    {
        // All clients play attack animation
        if (animator != null && !isLocalPlayer)
        {
            animator.SetTrigger("Attack");
        }
    }
    
    private int CalculateDamage()
    {
        int damage = baseDamage + syncStats.rhythm + syncStats.power;
        
        // Random variation
        damage += UnityEngine.Random.Range(-2, 3);
        
        return Mathf.Max(1, damage);
    }
    
    private void SyncPositionToServer()
    {
        if (NetworkClient.active && isLocalPlayer)
        {
            CmdSyncPosition(transform.position, transform.rotation);
        }
    }
    
    [Command]
    private void CmdSyncPosition(Vector3 position, Quaternion rotation)
    {
        syncPosition = position;
        syncRotation = rotation;
    }
    
    public void SetMovement(Vector3 direction, float velocity)
    {
        if (!isLocalPlayer)
        {
            // For remote players, just update position
            transform.position = syncPosition;
            transform.rotation = syncRotation;
        }
    }
    
    public void TakeDamage(int damage, PlayerController attacker = null)
    {
        if (isServer)
        {
            int defense = syncStats.harmony / 2;
            int actualDamage = Mathf.Max(1, damage - defense);
            
            syncStats.hp = Mathf.Max(0, syncStats.hp - actualDamage);
            
            if (syncStats.hp <= 0)
            {
                Die(attacker);
            }
            
            // Notify all clients
            RpcTakeDamage(actualDamage);
        }
    }
    
    [ClientRpc]
    private void RpcTakeDamage(int damage)
    {
        if (animator != null)
        {
            animator.SetTrigger("TakeDamage");
        }
        
        // Show damage text
        if (DamageTextManager.Instance != null)
        {
            DamageTextManager.Instance.ShowDamageText(transform.position + Vector3.up * 2f, damage.ToString());
        }
    }
    
    private void Die(PlayerController killer = null)
    {
        if (isServer)
        {
            // Handle death
            Debug.Log($"{playerName} has died!");
            
            // Respawn after delay
            Invoke(nameof(Respawn), 5f);
        }
    }
    
    private void Respawn()
    {
        if (isServer)
        {
            // Find spawn point
            Transform spawnPoint = NetworkManagerMMO.Instance.spawnPoints.Length > 0
                ? NetworkManagerMMO.Instance.spawnPoints[UnityEngine.Random.Range(0, NetworkManagerMMO.Instance.spawnPoints.Length)]
                : null;
            
            Vector3 spawnPosition = spawnPoint != null ? spawnPoint.position : Vector3.zero;
            
            // Reset stats
            syncStats.hp = syncStats.maxHp;
            syncStats.energy = syncStats.maxEnergy;
            
            // Teleport player
            syncPosition = spawnPosition;
            
            RpcRespawn(spawnPosition);
        }
    }
    
    [ClientRpc]
    private void RpcRespawn(Vector3 position)
    {
        if (isLocalPlayer)
        {
            transform.position = position;
            characterController.enabled = false;
            characterController.enabled = true;
        }
        else
        {
            transform.position = position;
        }
        
        if (animator != null)
        {
            animator.SetTrigger("Respawn");
        }
    }
    
    public void GainExperience(int amount)
    {
        if (isServer)
        {
            experience += amount;
            int nextLevelXp = CalculateNextLevelXP();
            
            while (experience >= nextLevelXp)
            {
                experience -= nextLevelXp;
                level++;
                LevelUp();
                nextLevelXp = CalculateNextLevelXP();
            }
            
            RpcGainExperience(amount);
        }
    }
    
    [ClientRpc]
    private void RpcGainExperience(int amount)
    {
        // Show experience gain
        if (ExperienceTextManager.Instance != null && isLocalPlayer)
        {
            ExperienceTextManager.Instance.ShowExperienceText(transform.position + Vector3.up * 2f, $"+{amount} XP");
        }
    }
    
    private int CalculateNextLevelXP()
    {
        return Mathf.FloorToInt(100 * Mathf.Pow(1.5f, level - 1));
    }
    
    private void LevelUp()
    {
        syncStats.maxHp += 20;
        syncStats.hp = syncStats.maxHp;
        syncStats.maxEnergy += 10;
        syncStats.energy = syncStats.maxEnergy;
        syncStats.rhythm += 5;
        syncStats.pitch += 3;
        syncStats.power += 4;
        syncStats.harmony += 3;
        
        Debug.Log($"{playerName} leveled up to {level}!");
    }
    
    public void AddHarmonyGems(int amount)
    {
        if (isServer)
        {
            harmonyGems += amount;
        }
    }
    
    public bool SpendHarmonyGems(int amount)
    {
        if (isServer && harmonyGems >= amount)
        {
            harmonyGems -= amount;
            return true;
        }
        return false;
    }
    
    public void SavePlayerData()
    {
        if (!isLocalPlayer) return;
        
        playerData = new PlayerData
        {
            playerName = playerName,
            characterClass = characterClass,
            position = transform.position,
            rotation = transform.rotation,
            level = level,
            experience = experience,
            harmonyGems = harmonyGems,
            stats = syncStats,
            currentZone = currentZone
        };
        
        // Save to PlayerPrefs (or better storage system)
        string jsonData = JsonUtility.ToJson(playerData);
        PlayerPrefs.SetString("PlayerData", jsonData);
        PlayerPrefs.Save();
        
        Debug.Log("Player data saved");
    }
    
    public void LoadPlayerData()
    {
        if (!isLocalPlayer) return;
        
        if (PlayerPrefs.HasKey("PlayerData"))
        {
            string jsonData = PlayerPrefs.GetString("PlayerData");
            playerData = JsonUtility.FromJson<PlayerData>(jsonData);
            
            if (playerData != null)
            {
                playerName = playerData.playerName;
                characterClass = playerData.characterClass;
                level = playerData.level;
                experience = playerData.experience;
                harmonyGems = playerData.harmonyGems;
                syncStats = playerData.stats;
                currentZone = playerData.currentZone;
                
                // Initialize on server
                if (isServer)
                {
                    syncPlayerName = playerName;
                    syncCharacterClass = characterClass;
                }
                
                Debug.Log("Player data loaded");
            }
        }
    }
    
    // SyncVar hooks
    private void OnNameChanged(string oldName, string newName)
    {
        playerName = newName;
        if (nameText != null)
        {
            nameText.text = newName;
        }
    }
    
    private void OnClassChanged(CharacterClass oldClass, CharacterClass newClass)
    {
        characterClass = newClass;
        SetupClassModel(newClass);
    }
    
    private void OnPositionChanged(Vector3 oldPos, Vector3 newPos)
    {
        if (!isLocalPlayer)
        {
            transform.position = newPos;
        }
    }
    
    private void OnRotationChanged(Quaternion oldRot, Quaternion newRot)
    {
        if (!isLocalPlayer)
        {
            transform.rotation = newRot;
        }
    }
    
    private void OnStatsChanged(PlayerStats oldStats, PlayerStats newStats)
    {
        syncStats = newStats;
        
        // Update UI
        if (UIManager.Instance != null && isLocalPlayer)
        {
            UIManager.Instance.UpdatePlayerStats(syncStats);
        }
    }
    
    // UI reference
    private TMPro.TextMeshProUGUI nameText;
    
    public void SetNameText(TMPro.TextMeshProUGUI text)
    {
        nameText = text;
        if (nameText != null)
        {
            nameText.text = playerName;
        }
    }
}

public interface IDamageable
{
    void TakeDamage(int damage, PlayerController attacker);
}

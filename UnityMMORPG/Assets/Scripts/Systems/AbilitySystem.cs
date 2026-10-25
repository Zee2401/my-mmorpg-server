using Mirror;
using System;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Manages player abilities and skills
/// </summary>
public class AbilitySystem : NetworkBehaviour
{
    public static AbilitySystem Instance { get; private set; }
    
    [Header("Ability Settings")]
    public List<AbilityData> abilities = new List<AbilityData>();
    public int maxAbilities = 4;
    
    [Header("Cooldown Settings")]
    public float globalCooldown = 0.5f;
    public bool showCooldownUI = true;
    
    [SyncVar(hook = nameof(OnAbilitiesChanged))]
    public List<AbilitySlot> abilitySlots = new List<AbilitySlot>();
    
    private Dictionary<string, AbilityData> abilityDatabase = new Dictionary<string, AbilityData>();
    private PlayerController localPlayer;
    private float lastGlobalCooldownTime;
    
    public override void Awake()
    {
        base.Awake();
        Instance = this;
        
        // Initialize ability database
        foreach (var ability in abilities)
        {
            abilityDatabase[ability.id] = ability;
        }
    }
    
    private void Start()
    {
        // Find local player
        localPlayer = FindObjectOfType<PlayerController>();
        if (localPlayer != null && localPlayer.isLocalPlayer)
        {
            InitializeAbilities(localPlayer);
        }
    }
    
    private void InitializeAbilities(PlayerController player)
    {
        // Initialize ability slots
        abilitySlots.Clear();
        for (int i = 0; i < maxAbilities; i++)
        {
            abilitySlots.Add(new AbilitySlot
            {
                slotIndex = i,
                abilityId = string.Empty,
                cooldownEndTime = 0f
            });
        }
        
        // Assign default abilities based on class
        AssignClassAbilities(player.characterClass);
    }
    
    private void AssignClassAbilities(CharacterClass characterClass)
    {
        // Clear existing abilities
        foreach (var slot in abilitySlots)
        {
            slot.abilityId = string.Empty;
        }
        
        // Assign class-specific abilities
        switch (characterClass)
        {
            case CharacterClass.Bard:
                AssignAbility(0, "ability_bard_sonata");
                AssignAbility(1, "ability_bard_harmony");
                AssignAbility(2, "ability_bard_crescendo");
                AssignAbility(3, "ability_bard_encore");
                break;
            case CharacterClass.Beatmaker:
                AssignAbility(0, "ability_beatmaker_drop");
                AssignAbility(1, "ability_beatmaker_remix");
                AssignAbility(2, "ability_beatmaker_scratch");
                AssignAbility(3, "ability_beatmaker_loop");
                break;
            case CharacterClass.Vocalist:
                AssignAbility(0, "ability_vocalist_scream");
                AssignAbility(1, "ability_vocalist_echo");
                AssignAbility(2, "ability_vocalist_harmonize");
                AssignAbility(3, "ability_vocalist_resonate");
                break;
            case CharacterClass.Guitarist:
                AssignAbility(0, "ability_guitarist_riff");
                AssignAbility(1, "ability_guitarist_solo");
                AssignAbility(2, "ability_guitarist_powerchord");
                AssignAbility(3, "ability_guitarist_feedback");
                break;
            case CharacterClass.DJ:
                AssignAbility(0, "ability_dj_spin");
                AssignAbility(1, "ability_dj_mix");
                AssignAbility(2, "ability_dj_fade");
                AssignAbility(3, "ability_dj_boost");
                break;
        }
    }
    
    public void AssignAbility(int slotIndex, string abilityId)
    {
        if (slotIndex < 0 || slotIndex >= abilitySlots.Count) return;
        
        var ability = GetAbility(abilityId);
        if (ability == null)
        {
            Debug.LogWarning($"Ability not found: {abilityId}");
            return;
        }
        
        abilitySlots[slotIndex].abilityId = abilityId;
        abilitySlots[slotIndex].cooldownEndTime = 0f;
    }
    
    public AbilityData GetAbility(string abilityId)
    {
        abilityDatabase.TryGetValue(abilityId, out AbilityData ability);
        return ability;
    }
    
    public AbilitySlot GetAbilitySlot(int slotIndex)
    {
        if (slotIndex < 0 || slotIndex >= abilitySlots.Count) return null;
        return abilitySlots[slotIndex];
    }
    
    public bool CanUseAbility(int slotIndex)
    {
        if (!isLocalPlayer) return false;
        
        var slot = GetAbilitySlot(slotIndex);
        if (slot == null || string.IsNullOrEmpty(slot.abilityId)) return false;
        
        var ability = GetAbility(slot.abilityId);
        if (ability == null) return false;
        
        // Check cooldown
        if (Time.time < slot.cooldownEndTime) return false;
        
        // Check global cooldown
        if (Time.time - lastGlobalCooldownTime < globalCooldown) return false;
        
        // Check mana/energy cost
        if (localPlayer != null && localPlayer.syncStats.energy < ability.manaCost) return false;
        
        return true;
    }
    
    public void UseAbility(int slotIndex, Vector3 targetPosition = default)
    {
        if (!isLocalPlayer) return;
        
        var slot = GetAbilitySlot(slotIndex);
        if (slot == null || string.IsNullOrEmpty(slot.abilityId)) return;
        
        var ability = GetAbility(slot.abilityId);
        if (ability == null) return;
        
        if (!CanUseAbility(slotIndex)) return;
        
        // Check cooldown
        if (Time.time < slot.cooldownEndTime) return;
        
        // Check global cooldown
        if (Time.time - lastGlobalCooldownTime < globalCooldown) return;
        
        // Check energy cost
        if (localPlayer.syncStats.energy < ability.manaCost) return;
        
        // Use ability on server
        if (isServer)
        {
            InternalUseAbility(slotIndex, targetPosition);
        }
        else
        {
            CmdUseAbility(slotIndex, targetPosition);
        }
    }
    
    [Command]
    private void CmdUseAbility(int slotIndex, Vector3 targetPosition)
    {
        InternalUseAbility(slotIndex, targetPosition);
    }
    
    private void InternalUseAbility(int slotIndex, Vector3 targetPosition)
    {
        var slot = abilitySlots[slotIndex];
        var ability = GetAbility(slot.abilityId);
        
        if (ability == null || localPlayer == null) return;
        
        // Check energy
        if (localPlayer.syncStats.energy < ability.manaCost) return;
        
        // Deduct energy
        localPlayer.syncStats.energy -= ability.manaCost;
        
        // Start cooldown
        slot.cooldownEndTime = Time.time + ability.cooldown;
        lastGlobalCooldownTime = Time.time;
        
        // Execute ability
        ExecuteAbility(ability, localPlayer, targetPosition);
        
        // Sync to all clients
        RpcUseAbility(slotIndex, localPlayer.netId, targetPosition);
    }
    
    [ClientRpc]
    private void RpcUseAbility(int slotIndex, uint playerNetId, Vector3 targetPosition)
    {
        // Find player
        NetworkIdentity identity = NetworkServer.spawned[playerNetId];
        if (identity == null) return;
        
        PlayerController player = identity.GetComponent<PlayerController>();
        if (player == null) return;
        
        var slot = abilitySlots[slotIndex];
        var ability = GetAbility(slot.abilityId);
        
        if (ability == null) return;
        
        // Execute ability (visual effects only on clients)
        ExecuteAbilityVisuals(ability, player, targetPosition);
    }
    
    private void ExecuteAbility(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Execute ability effects
        switch (ability.id)
        {
            // Bard abilities
            case "ability_bard_sonata":
                ExecuteSonata(ability, player, targetPosition);
                break;
            case "ability_bard_harmony":
                ExecuteHarmony(ability, player, targetPosition);
                break;
            case "ability_bard_crescendo":
                ExecuteCrescendo(ability, player, targetPosition);
                break;
            case "ability_bard_encore":
                ExecuteEncore(ability, player, targetPosition);
                break;
                
            // Beatmaker abilities
            case "ability_beatmaker_drop":
                ExecuteBassDrop(ability, player, targetPosition);
                break;
            case "ability_beatmaker_remix":
                ExecuteRemix(ability, player, targetPosition);
                break;
            case "ability_beatmaker_scratch":
                ExecuteScratch(ability, player, targetPosition);
                break;
            case "ability_beatmaker_loop":
                ExecuteLoop(ability, player, targetPosition);
                break;
                
            // Vocalist abilities
            case "ability_vocalist_scream":
                ExecuteScream(ability, player, targetPosition);
                break;
            case "ability_vocalist_echo":
                ExecuteEcho(ability, player, targetPosition);
                break;
            case "ability_vocalist_harmonize":
                ExecuteHarmonize(ability, player, targetPosition);
                break;
            case "ability_vocalist_resonate":
                ExecuteResonate(ability, player, targetPosition);
                break;
                
            // Guitarist abilities
            case "ability_guitarist_riff":
                ExecuteRiff(ability, player, targetPosition);
                break;
            case "ability_guitarist_solo":
                ExecuteSolo(ability, player, targetPosition);
                break;
            case "ability_guitarist_powerchord":
                ExecutePowerChord(ability, player, targetPosition);
                break;
            case "ability_guitarist_feedback":
                ExecuteFeedback(ability, player, targetPosition);
                break;
                
            // DJ abilities
            case "ability_dj_spin":
                ExecuteSpin(ability, player, targetPosition);
                break;
            case "ability_dj_mix":
                ExecuteMix(ability, player, targetPosition);
                break;
            case "ability_dj_fade":
                ExecuteFade(ability, player, targetPosition);
                break;
            case "ability_dj_boost":
                ExecuteBoost(ability, player, targetPosition);
                break;
                
            default:
                ExecuteGenericAbility(ability, player, targetPosition);
                break;
        }
        
        // Play ability sound
        if (AudioManager.Instance != null)
        {
            AudioManager.Instance.PlayAbilitySound();
        }
    }
    
    private void ExecuteAbilityVisuals(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Play animation
        if (player.animator != null)
        {
            player.animator.SetTrigger("Ability");
        }
        
        // Play sound
        if (AudioManager.Instance != null)
        {
            AudioManager.Instance.PlaySound(ability.icon, null, 0.5f, false);
        }
    }
    
    // Ability implementations
    private void ExecuteSonata(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Heal self and nearby allies
        int healAmount = Mathf.FloorToInt(player.syncStats.power * 2f);
        player.syncStats.hp = Mathf.Min(player.syncStats.maxHp, player.syncStats.hp + healAmount);
        
        // Find nearby allies
        Collider[] allies = Physics.OverlapSphere(player.transform.position, 10f, LayerMask.GetMask("Player"));
        foreach (var ally in allies)
        {
            PlayerController allyPlayer = ally.GetComponent<PlayerController>();
            if (allyPlayer != null && allyPlayer != player)
            {
                allyPlayer.syncStats.hp = Mathf.Min(allyPlayer.syncStats.maxHp, allyPlayer.syncStats.hp + healAmount / 2);
            }
        }
    }
    
    private void ExecuteHarmony(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Increase defense
        player.syncStats.harmony += Mathf.FloorToInt(player.syncStats.pitch * 0.5f);
    }
    
    private void ExecuteCrescendo(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Damage in area
        int damage = Mathf.FloorToInt(player.syncStats.rhythm * 3f);
        Collider[] enemies = Physics.OverlapSphere(targetPosition, 8f, LayerMask.GetMask("Enemy"));
        
        foreach (var enemy in enemies)
        {
            IDamageable damageable = enemy.GetComponent<IDamageable>();
            if (damageable != null)
            {
                damageable.TakeDamage(damage, player);
            }
        }
    }
    
    private void ExecuteEncore(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Reset all cooldowns
        foreach (var slot in abilitySlots)
        {
            slot.cooldownEndTime = 0f;
        }
    }
    
    private void ExecuteBassDrop(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Stun enemies in area
        int damage = Mathf.FloorToInt(player.syncStats.rhythm * 4f);
        Collider[] enemies = Physics.OverlapSphere(targetPosition, 6f, LayerMask.GetMask("Enemy"));
        
        foreach (var enemy in enemies)
        {
            BossController boss = enemy.GetComponent<BossController>();
            if (boss != null)
            {
                boss.TakeDamage(damage, player);
                // Stun boss for 2 seconds
                // boss.Stun(2f);
            }
        }
    }
    
    private void ExecuteRemix(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Random effect
        int randomEffect = UnityEngine.Random.Range(0, 3);
        switch (randomEffect)
        {
            case 0:
                // Heal
                player.syncStats.hp = Mathf.Min(player.syncStats.maxHp, player.syncStats.hp + player.syncStats.power * 3);
                break;
            case 1:
                // Damage boost
                player.syncStats.rhythm += 10;
                break;
            case 2:
                // Energy restore
                player.syncStats.energy = Mathf.Min(player.syncStats.maxEnergy, player.syncStats.energy + 30);
                break;
        }
    }
    
    private void ExecuteScratch(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Single target high damage
        int damage = Mathf.FloorToInt(player.syncStats.rhythm * 5f);
        
        // Find nearest enemy
        Collider[] enemies = Physics.OverlapSphere(player.transform.position, 15f, LayerMask.GetMask("Enemy"));
        if (enemies.Length > 0)
        {
            IDamageable damageable = enemies[0].GetComponent<IDamageable>();
            if (damageable != null)
            {
                damageable.TakeDamage(damage, player);
            }
        }
    }
    
    private void ExecuteLoop(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Damage over time
        // Create damage over time effect
        // This would be implemented with a separate DoT system
    }
    
    private void ExecuteScream(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Fear effect on enemies
        Collider[] enemies = Physics.OverlapSphere(targetPosition, 10f, LayerMask.GetMask("Enemy"));
        foreach (var enemy in enemies)
        {
            // Cause fear (run away)
            BossController boss = enemy.GetComponent<BossController>();
            if (boss != null)
            {
                // boss.Fear(3f, player.transform.position);
            }
        }
    }
    
    private void ExecuteEcho(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Repeat last ability
        // This would need tracking of last used ability
    }
    
    private void ExecuteHarmonize(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Group buff
        Collider[] allies = Physics.OverlapSphere(player.transform.position, 15f, LayerMask.GetMask("Player"));
        foreach (var ally in allies)
        {
            PlayerController allyPlayer = ally.GetComponent<PlayerController>();
            if (allyPlayer != null)
            {
                allyPlayer.syncStats.rhythm += 5;
                allyPlayer.syncStats.pitch += 5;
            }
        }
    }
    
    private void ExecuteResonate(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Shield allies
        Collider[] allies = Physics.OverlapSphere(player.transform.position, 10f, LayerMask.GetMask("Player"));
        foreach (var ally in allies)
        {
            PlayerController allyPlayer = ally.GetComponent<PlayerController>();
            if (allyPlayer != null)
            {
                allyPlayer.syncStats.harmony += 10;
            }
        }
    }
    
    private void ExecuteRiff(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Multi-hit attack
        for (int i = 0; i < 3; i++)
        {
            int damage = Mathf.FloorToInt(player.syncStats.rhythm * 1.5f);
            Collider[] enemies = Physics.OverlapSphere(player.transform.position, 5f, LayerMask.GetMask("Enemy"));
            
            if (enemies.Length > 0)
            {
                IDamageable damageable = enemies[UnityEngine.Random.Range(0, enemies.Length)].GetComponent<IDamageable>();
                if (damageable != null)
                {
                    damageable.TakeDamage(damage, player);
                }
            }
        }
    }
    
    private void ExecuteSolo(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // High damage single target
        int damage = Mathf.FloorToInt(player.syncStats.rhythm * 6f);
        
        // Find nearest enemy
        Collider[] enemies = Physics.OverlapSphere(player.transform.position, 20f, LayerMask.GetMask("Enemy"));
        if (enemies.Length > 0)
        {
            IDamageable damageable = enemies[0].GetComponent<IDamageable>();
            if (damageable != null)
            {
                damageable.TakeDamage(damage, player);
            }
        }
    }
    
    private void ExecutePowerChord(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Stun and damage
        int damage = Mathf.FloorToInt(player.syncStats.rhythm * 3f);
        Collider[] enemies = Physics.OverlapSphere(targetPosition, 8f, LayerMask.GetMask("Enemy"));
        
        foreach (var enemy in enemies)
        {
            IDamageable damageable = enemy.GetComponent<IDamageable>();
            if (damageable != null)
            {
                damageable.TakeDamage(damage, player);
            }
            
            BossController boss = enemy.GetComponent<BossController>();
            if (boss != null)
            {
                // boss.Stun(1.5f);
            }
        }
    }
    
    private void ExecuteFeedback(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Damage based on missing health
        float healthPercent = (float)player.syncStats.hp / player.syncStats.maxHp;
        int damage = Mathf.FloorToInt(player.syncStats.power * (1f - healthPercent) * 10f);
        
        Collider[] enemies = Physics.OverlapSphere(targetPosition, 10f, LayerMask.GetMask("Enemy"));
        foreach (var enemy in enemies)
        {
            IDamageable damageable = enemy.GetComponent<IDamageable>();
            if (damageable != null)
            {
                damageable.TakeDamage(damage, player);
            }
        }
    }
    
    private void ExecuteSpin(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Dodge and counter
        // Increase dodge chance
        player.syncStats.harmony += 15;
    }
    
    private void ExecuteMix(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Random stat boost
        int randomStat = UnityEngine.Random.Range(0, 4);
        switch (randomStat)
        {
            case 0: player.syncStats.rhythm += 10; break;
            case 1: player.syncStats.pitch += 10; break;
            case 2: player.syncStats.power += 10; break;
            case 3: player.syncStats.harmony += 10; break;
        }
    }
    
    private void ExecuteFade(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Become invulnerable briefly
        // player.SetInvulnerable(2f);
    }
    
    private void ExecuteBoost(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Increase all stats
        player.syncStats.rhythm += 5;
        player.syncStats.pitch += 5;
        player.syncStats.power += 5;
        player.syncStats.harmony += 5;
    }
    
    private void ExecuteGenericAbility(AbilityData ability, PlayerController player, Vector3 targetPosition)
    {
        // Default ability effect
        int damage = Mathf.FloorToInt(player.syncStats.rhythm * 2f);
        Collider[] enemies = Physics.OverlapSphere(targetPosition, 5f, LayerMask.GetMask("Enemy"));
        
        foreach (var enemy in enemies)
        {
            IDamageable damageable = enemy.GetComponent<IDamageable>();
            if (damageable != null)
            {
                damageable.TakeDamage(damage, player);
            }
        }
    }
    
    public float GetCooldownProgress(int slotIndex)
    {
        var slot = GetAbilitySlot(slotIndex);
        if (slot == null) return 0f;
        
        var ability = GetAbility(slot.abilityId);
        if (ability == null) return 0f;
        
        if (slot.cooldownEndTime <= Time.time) return 0f;
        
        return 1f - ((slot.cooldownEndTime - Time.time) / ability.cooldown);
    }
    
    public void ResetCooldown(int slotIndex)
    {
        var slot = GetAbilitySlot(slotIndex);
        if (slot != null)
        {
            slot.cooldownEndTime = 0f;
        }
    }
    
    public void ResetAllCooldowns()
    {
        foreach (var slot in abilitySlots)
        {
            slot.cooldownEndTime = 0f;
        }
    }
    
    // SyncVar hook
    private void OnAbilitiesChanged(List<AbilitySlot> oldSlots, List<AbilitySlot> newSlots)
    {
        abilitySlots = newSlots;
        
        // Update UI
        if (UIManager.Instance != null)
        {
            UIManager.Instance.UpdateAbilityUI();
        }
    }
}

[Serializable]
public class AbilitySlot
{
    public int slotIndex;
    public string abilityId;
    public float cooldownEndTime;
}

[Serializable]
public class AbilityData
{
    public string id;
    public string name;
    public string description;
    public int cooldown;
    public int manaCost;
    public string icon;
    public string animation;
    public float damageMultiplier;
    public float healMultiplier;
    public float duration;
    public string[] tags;
    public CharacterClass requiredClass;
    public int requiredLevel;
    public string targetType; // Self, Ally, Enemy, Area
    public float range;
    public float radius;
}

public enum AbilityType
{
    Damage,
    Heal,
    Buff,
    Debuff,
    Shield,
    Cleanse,
    Teleport,
    Summon,
    Transform
}

public enum AbilityTarget
{
    Self,
    Ally,
    Enemy,
    Area,
    All
}

public enum AbilityActivation
{
    Instant,
    Channeled,
    Charged,
    Toggle,
    Passive
}

public enum AbilityEffect
{
    None,
    Damage,
    Heal,
    Buff,
    Debuff,
    Stun,
    Fear,
    Silence,
    Taunt,
    Cleanse,
    Shield,
    Absorb,
    Reflect,
    Lifesteal,
    Manasteal
}

public enum AbilityScaling
{
    None,
    Rhythm,
    Pitch,
    Power,
    Harmony,
    AllStats,
    MissingHealth,
    MissingEnergy
}

public enum AbilityCostType
{
    None,
    Energy,
    Health,
    HarmonyGems,
    Item,
    ComboPoints,
    Charge
}

public enum AbilityResource
{
    None,
    Energy,
    Health,
    HarmonyGems,
    Stamina,
    Mana,
    Rage,
    Focus
}

public enum AbilitySchool
{
    None,
    Physical,
    Magical,
    Rhythm,
    Melody,
    Harmony,
    Bass,
    Treble,
    Percussion
}

public enum AbilityElement
{
    None,
    Fire,
    Ice,
    Lightning,
    Poison,
    Holy,
    Shadow,
    Sound,
    Vibration,
    Frequency
}

public enum AbilityCategory
{
    None,
    Basic,
    Special,
    Ultimate,
    Passive,
    Talent
}

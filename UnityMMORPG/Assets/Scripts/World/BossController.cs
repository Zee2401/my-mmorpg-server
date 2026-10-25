using Mirror;
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

/// <summary>
/// Controls boss behavior and combat
/// </summary>
[RequireComponent(typeof(NavMeshAgent))]
[RequireComponent(typeof(Animator))]
public class BossController : NetworkBehaviour, IDamageable
{
    [Header("Boss Settings")]
    public BossData bossData;
    
    [Header("Combat Settings")]
    public float attackRange = 5f;
    public float attackCooldown = 2f;
    public float detectionRange = 20f;
    public int baseDamage = 20;
    
    [Header("Movement Settings")]
    public float moveSpeed = 3f;
    public float rotationSpeed = 5f;
    
    [Header("References")]
    public NavMeshAgent navMeshAgent;
    public Animator animator;
    public Transform targetTransform;
    
    [SyncVar(hook = nameof(OnHpChanged))]
    public int currentHp;
    
    [SyncVar]
    public bool isInBattle = false;
    
    [SyncVar]
    public string currentTargetId;
    
    private PlayerController currentTarget;
    private float lastAttackTime;
    private BossState currentState = BossState.Idle;
    
    public enum BossState
    {
        Idle,
        Patrolling,
        Chasing,
        Attacking,
        Casting,
        Stunned,
        Dead
    }
    
    public override void Awake()
    {
        base.Awake();
        
        if (navMeshAgent == null)
            navMeshAgent = GetComponent<NavMeshAgent>();
            
        if (animator == null)
            animator = GetComponent<Animator>();
    }
    
    public override void OnStartServer()
    {
        base.OnStartServer();
        
        // Initialize HP
        currentHp = bossData.maxHp;
        
        // Setup navmesh agent
        navMeshAgent.speed = moveSpeed;
        navMeshAgent.stoppingDistance = attackRange - 1f;
        
        // Start idle behavior
        currentState = BossState.Idle;
        
        Debug.Log($"Boss initialized: {bossData.name} with {currentHp} HP");
    }
    
    public void Initialize(BossData data)
    {
        bossData = data;
        currentHp = data.maxHp;
        
        Debug.Log($"Boss initialized with data: {data.name}");
    }
    
    public BossData GetBossData()
    {
        return bossData;
    }
    
    private void Update()
    {
        if (!isServer) return;
        
        switch (currentState)
        {
            case BossState.Idle:
                UpdateIdle();
                break;
            case BossState.Patrolling:
                UpdatePatrolling();
                break;
            case BossState.Chasing:
                UpdateChasing();
                break;
            case BossState.Attacking:
                UpdateAttacking();
                break;
            case BossState.Casting:
                UpdateCasting();
                break;
            case BossState.Stunned:
                UpdateStunned();
                break;
            case BossState.Dead:
                UpdateDead();
                break;
        }
    }
    
    private void UpdateIdle()
    {
        // Look for players
        PlayerController player = FindNearestPlayer();
        if (player != null && Vector3.Distance(transform.position, player.transform.position) <= detectionRange)
        {
            SetTarget(player);
            currentState = BossState.Chasing;
        }
        else
        {
            // Random patrol
            if (UnityEngine.Random.value < 0.01f)
            {
                FindRandomPatrolPoint();
                currentState = BossState.Patrolling;
            }
        }
    }
    
    private void UpdatePatrolling()
    {
        // Check if reached destination
        if (navMeshAgent.remainingDistance <= navMeshAgent.stoppingDistance)
        {
            currentState = BossState.Idle;
        }
        
        // Look for players while patrolling
        PlayerController player = FindNearestPlayer();
        if (player != null && Vector3.Distance(transform.position, player.transform.position) <= detectionRange)
        {
            SetTarget(player);
            currentState = BossState.Chasing;
        }
    }
    
    private void UpdateChasing()
    {
        if (currentTarget == null || !currentTarget.isActiveAndEnabled)
        {
            currentState = BossState.Idle;
            return;
        }
        
        // Check distance to target
        float distance = Vector3.Distance(transform.position, currentTarget.transform.position);
        
        if (distance <= attackRange)
        {
            // In attack range
            navMeshAgent.SetDestination(transform.position);
            
            // Face target
            Vector3 direction = (currentTarget.transform.position - transform.position).normalized;
            Quaternion lookRotation = Quaternion.LookRotation(direction);
            transform.rotation = Quaternion.Slerp(transform.rotation, lookRotation, rotationSpeed * Time.deltaTime);
            
            // Attack
            if (Time.time - lastAttackTime > attackCooldown)
            {
                currentState = BossState.Attacking;
                lastAttackTime = Time.time;
            }
        }
        else
        {
            // Chase target
            navMeshAgent.SetDestination(currentTarget.transform.position);
        }
        
        // Give up if target is too far
        if (distance > detectionRange * 1.5f)
        {
            currentState = BossState.Idle;
        }
    }
    
    private void UpdateAttacking()
    {
        if (animator != null)
        {
            animator.SetTrigger("Attack");
        }
        
        // Deal damage to target
        if (currentTarget != null && Vector3.Distance(transform.position, currentTarget.transform.position) <= attackRange)
        {
            int damage = CalculateDamage();
            currentTarget.TakeDamage(damage, this);
            
            Debug.Log($"{bossData.name} attacks {currentTarget.playerName} for {damage} damage");
        }
        
        // Return to chasing after attack animation
        currentState = BossState.Chasing;
    }
    
    private void UpdateCasting()
    {
        // Special ability casting
        // TODO: Implement special boss abilities
        
        currentState = BossState.Chasing;
    }
    
    private void UpdateStunned()
    {
        // Wait for stun to end
        // TODO: Implement stun duration
        
        currentState = BossState.Chasing;
    }
    
    private void UpdateDead()
    {
        // Wait for respawn
        // Handled by ZoneManager
    }
    
    private PlayerController FindNearestPlayer()
    {
        PlayerController nearest = null;
        float nearestDistance = float.MaxValue;
        
        foreach (var player in ZoneManager.Instance.GetPlayersInCurrentZone())
        {
            if (player.isActiveAndEnabled && !player.IsDead())
            {
                float distance = Vector3.Distance(transform.position, player.transform.position);
                if (distance < nearestDistance)
                {
                    nearest = player;
                    nearestDistance = distance;
                }
            }
        }
        
        return nearest;
    }
    
    private void FindRandomPatrolPoint()
    {
        // Find random point around boss
        Vector3 randomDirection = UnityEngine.Random.insideUnitSphere * 10f;
        randomDirection += transform.position;
        
        NavMeshHit hit;
        if (NavMesh.SamplePosition(randomDirection, out hit, 10f, NavMesh.AllAreas))
        {
            navMeshAgent.SetDestination(hit.position);
        }
    }
    
    public void SetTarget(PlayerController target)
    {
        currentTarget = target;
        currentTargetId = target != null ? target.netId.ToString() : string.Empty;
        targetTransform = target != null ? target.transform : null;
    }
    
    public void StartBattle(PlayerController player)
    {
        if (isServer)
        {
            isInBattle = true;
            SetTarget(player);
            currentState = BossState.Chasing;
            
            Debug.Log($"Boss battle started with {player.playerName}");
        }
    }
    
    public void EndBattle(bool victory)
    {
        if (isServer)
        {
            isInBattle = false;
            currentTarget = null;
            currentTargetId = string.Empty;
            targetTransform = null;
            
            if (victory)
            {
                currentState = BossState.Dead;
            }
            else
            {
                currentState = BossState.Idle;
            }
            
            Debug.Log($"Boss battle ended: {(victory ? "Victory" : "Defeat")}");
        }
    }
    
    public void TakeDamage(int damage, PlayerController attacker = null)
    {
        if (isServer)
        {
            currentHp = Mathf.Max(0, currentHp - damage);
            
            if (currentHp <= 0)
            {
                Die(attacker);
            }
            else
            {
                // React to damage
                if (animator != null)
                {
                    animator.SetTrigger("TakeDamage");
                }
                
                // If not already in battle, start battle with attacker
                if (!isInBattle && attacker != null)
                {
                    StartBattle(attacker);
                }
            }
            
            Debug.Log($"{bossData.name} takes {damage} damage. HP: {currentHp}/{bossData.maxHp}");
        }
    }
    
    private int CalculateDamage()
    {
        int damage = baseDamage + (bossData.level * 5);
        damage += UnityEngine.Random.Range(-3, 4);
        return Mathf.Max(1, damage);
    }
    
    private void Die(PlayerController killer = null)
    {
        if (isServer)
        {
            currentState = BossState.Dead;
            
            if (animator != null)
            {
                animator.SetTrigger("Die");
            }
            
            // Notify zone manager
            ZoneManager.Instance.EndBossBattle(killer, ZoneManager.Instance.currentZoneId, true);
            
            Debug.Log($"{bossData.name} has been defeated!");
        }
    }
    
    public bool IsDead()
    {
        return currentState == BossState.Dead;
    }
    
    public bool IsInBattle()
    {
        return isInBattle;
    }
    
    // SyncVar hook
    private void OnHpChanged(int oldHp, int newHp)
    {
        // Update HP bar
        if (BossHealthBarManager.Instance != null)
        {
            BossHealthBarManager.Instance.UpdateBossHealth(this, newHp, bossData.maxHp);
        }
    }
}

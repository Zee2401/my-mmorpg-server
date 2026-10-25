using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

/// <summary>
/// Manages floating damage text and other combat UI elements
/// </summary>
public class DamageTextManager : MonoBehaviour
{
    public static DamageTextManager Instance { get; private set; }
    
    [Header("Damage Text Settings")]
    public GameObject damageTextPrefab;
    public GameObject healingTextPrefab;
    public GameObject criticalTextPrefab;
    public GameObject xpTextPrefab;
    public GameObject gemTextPrefab;
    public GameObject statusTextPrefab;
    
    [Header("Pool Settings")]
    public int initialPoolSize = 50;
    public int maxPoolSize = 200;
    
    [Header("Movement Settings")]
    public float moveSpeed = 2f;
    public float fadeSpeed = 1f;
    public float lifetime = 2f;
    
    [Header("Appearance Settings")]
    public Color damageColor = Color.red;
    public Color healingColor = Color.green;
    public Color criticalColor = Color.yellow;
    public Color xpColor = Color.cyan;
    public Color gemColor = Color.magenta;
    public Color statusColor = Color.white;
    
    private Queue<GameObject> damageTextPool = new Queue<GameObject>();
    private Queue<GameObject> healingTextPool = new Queue<GameObject>();
    private Queue<GameObject> criticalTextPool = new Queue<GameObject>();
    private Queue<GameObject> xpTextPool = new Queue<GameObject>();
    private Queue<GameObject> gemTextPool = new Queue<GameObject>();
    private Queue<GameObject> statusTextPool = new Queue<GameObject>();
    
    public override void Awake()
    {
        base.Awake();
        Instance = this;
        
        // Initialize pools
        InitializePools();
    }
    
    private void InitializePools()
    {
        // Damage text pool
        for (int i = 0; i < initialPoolSize; i++)
        {
            GameObject obj = CreateTextObject(damageTextPrefab, "DamageTextPool");
            damageTextPool.Enqueue(obj);
        }
        
        // Healing text pool
        for (int i = 0; i < initialPoolSize; i++)
        {
            GameObject obj = CreateTextObject(healingTextPrefab, "HealingTextPool");
            healingTextPool.Enqueue(obj);
        }
        
        // Critical text pool
        for (int i = 0; i < initialPoolSize; i++)
        {
            GameObject obj = CreateTextObject(criticalTextPrefab, "CriticalTextPool");
            criticalTextPool.Enqueue(obj);
        }
        
        // XP text pool
        for (int i = 0; i < initialPoolSize; i++)
        {
            GameObject obj = CreateTextObject(xpTextPrefab, "XPTextPool");
            xpTextPool.Enqueue(obj);
        }
        
        // Gem text pool
        for (int i = 0; i < initialPoolSize; i++)
        {
            GameObject obj = CreateTextObject(gemTextPrefab, "GemTextPool");
            gemTextPool.Enqueue(obj);
        }
        
        // Status text pool
        for (int i = 0; i < initialPoolSize; i++)
        {
            GameObject obj = CreateTextObject(statusTextPrefab, "StatusTextPool");
            statusTextPool.Enqueue(obj);
        }
    }
    
    private GameObject CreateTextObject(GameObject prefab, string poolName)
    {
        if (prefab == null) return null;
        
        GameObject obj = Instantiate(prefab, transform);
        obj.name = $"{poolName}_{damageTextPool.Count}";
        obj.SetActive(false);
        
        return obj;
    }
    
    private GameObject GetTextObjectFromPool(Queue<GameObject> pool, GameObject prefab, string poolName)
    {
        // Try to get from pool
        if (pool.Count > 0)
        {
            GameObject obj = pool.Dequeue();
            obj.SetActive(true);
            return obj;
        }
        
        // Create new if pool is empty and we haven't reached max
        if (transform.childCount < maxPoolSize)
        {
            GameObject obj = CreateTextObject(prefab, poolName);
            obj.SetActive(true);
            return obj;
        }
        
        // Reuse oldest active object
        foreach (Transform child in transform)
        {
            if (child.gameObject.activeSelf)
            {
                // Check if it's from this pool
                if (prefab != null && child.name.StartsWith(poolName))
                {
                    child.gameObject.SetActive(false);
                    child.gameObject.SetActive(true);
                    return child.gameObject;
                }
            }
        }
        
        return null;
    }
    
    public void ShowDamageText(Vector3 position, string text, bool isCritical = false)
    {
        Queue<GameObject> pool = isCritical ? criticalTextPool : damageTextPool;
        GameObject prefab = isCritical ? criticalTextPrefab : damageTextPrefab;
        Color color = isCritical ? criticalColor : damageColor;
        
        GameObject obj = GetTextObjectFromPool(pool, prefab, isCritical ? "CriticalTextPool" : "DamageTextPool");
        if (obj == null) return;
        
        SetupTextObject(obj, position, text, color);
    }
    
    public void ShowHealingText(Vector3 position, string text)
    {
        GameObject obj = GetTextObjectFromPool(healingTextPool, healingTextPrefab, "HealingTextPool");
        if (obj == null) return;
        
        SetupTextObject(obj, position, text, healingColor);
    }
    
    public void ShowXPText(Vector3 position, string text)
    {
        GameObject obj = GetTextObjectFromPool(xpTextPool, xpTextPrefab, "XPTextPool");
        if (obj == null) return;
        
        SetupTextObject(obj, position, text, xpColor);
    }
    
    public void ShowGemText(Vector3 position, string text)
    {
        GameObject obj = GetTextObjectFromPool(gemTextPool, gemTextPrefab, "GemTextPool");
        if (obj == null) return;
        
        SetupTextObject(obj, position, text, gemColor);
    }
    
    public void ShowStatusText(Vector3 position, string text, Color color)
    {
        GameObject obj = GetTextObjectFromPool(statusTextPool, statusTextPrefab, "StatusTextPool");
        if (obj == null) return;
        
        SetupTextObject(obj, position, text, color);
    }
    
    private void SetupTextObject(GameObject obj, Vector3 worldPosition, string text, Color color)
    {
        // Set position
        obj.transform.position = worldPosition;
        
        // Set text
        TextMeshProUGUI textComponent = obj.GetComponent<TextMeshProUGUI>();
        if (textComponent != null)
        {
            textComponent.text = text;
            textComponent.color = color;
        }
        
        // Start movement coroutine
        StartCoroutine(MoveAndFade(obj, textComponent));
    }
    
    private System.Collections.IEnumerator MoveAndFade(GameObject obj, TextMeshProUGUI textComponent)
    {
        float elapsed = 0f;
        Vector3 startPosition = obj.transform.position;
        
        while (elapsed < lifetime)
        {
            // Move upward
            obj.transform.position = startPosition + Vector3.up * (moveSpeed * elapsed);
            
            // Fade out
            if (textComponent != null)
            {
                Color currentColor = textComponent.color;
                currentColor.a = Mathf.Lerp(1f, 0f, elapsed / lifetime);
                textComponent.color = currentColor;
            }
            
            elapsed += Time.deltaTime;
            yield return null;
        }
        
        // Return to pool
        ReturnToPool(obj);
    }
    
    private void ReturnToPool(GameObject obj)
    {
        if (obj == null) return;
        
        obj.SetActive(false);
        
        // Add back to appropriate pool
        if (obj.name.StartsWith("DamageTextPool"))
        {
            damageTextPool.Enqueue(obj);
        }
        else if (obj.name.StartsWith("HealingTextPool"))
        {
            healingTextPool.Enqueue(obj);
        }
        else if (obj.name.StartsWith("CriticalTextPool"))
        {
            criticalTextPool.Enqueue(obj);
        }
        else if (obj.name.StartsWith("XPTextPool"))
        {
            xpTextPool.Enqueue(obj);
        }
        else if (obj.name.StartsWith("GemTextPool"))
        {
            gemTextPool.Enqueue(obj);
        }
        else if (obj.name.StartsWith("StatusTextPool"))
        {
            statusTextPool.Enqueue(obj);
        }
    }
    
    public void ShowCombatText(Vector3 position, CombatTextType type, string text, bool isCritical = false)
    {
        switch (type)
        {
            case CombatTextType.Damage:
                ShowDamageText(position, text, isCritical);
                break;
            case CombatTextType.Healing:
                ShowHealingText(position, text);
                break;
            case CombatTextType.XP:
                ShowXPText(position, text);
                break;
            case CombatTextType.Gems:
                ShowGemText(position, text);
                break;
            case CombatTextType.Miss:
                ShowStatusText(position, text, Color.gray);
                break;
            case CombatTextType.Dodge:
                ShowStatusText(position, text, Color.cyan);
                break;
            case CombatTextType.Critical:
                ShowDamageText(position, text, true);
                break;
            case CombatTextType.Status:
                ShowStatusText(position, text, statusColor);
                break;
        }
    }
    
    public void ShowNumber(Vector3 position, int number, CombatTextType type, bool isCritical = false)
    {
        ShowCombatText(position, type, number.ToString(), isCritical);
    }
    
    public void ClearAllText()
    {
        // Deactivate all active text objects
        foreach (Transform child in transform)
        {
            if (child.gameObject.activeSelf)
            {
                child.gameObject.SetActive(false);
                
                // Add back to appropriate pool
                ReturnToPool(child.gameObject);
            }
        }
    }
    
    public int GetActiveTextCount()
    {
        int count = 0;
        foreach (Transform child in transform)
        {
            if (child.gameObject.activeSelf)
            {
                count++;
            }
        }
        return count;
    }
}

public enum CombatTextType
{
    Damage,
    Healing,
    XP,
    Gems,
    Miss,
    Dodge,
    Critical,
    Status
}

public enum CombatTextStyle
{
    Normal,
    Bold,
    Italic,
    Outline,
    Shadow,
    Glow
}

public enum CombatTextAnimation
{
    None,
    FloatUp,
    FloatDown,
    Shake,
    Bounce,
    Spin,
    Fade,
    SlideIn
}

public enum CombatTextSize
{
    Small,
    Normal,
    Large,
    Huge
}

public enum CombatTextDuration
{
    Short,
    Normal,
    Long,
    Permanent
}

public enum CombatTextPriority
{
    Low,
    Normal,
    High,
    Critical
}

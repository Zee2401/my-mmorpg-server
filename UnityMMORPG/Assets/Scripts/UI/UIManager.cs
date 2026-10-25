using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Manages all UI elements and screens
/// </summary>
public class UIManager : MonoBehaviour
{
    public static UIManager Instance { get; private set; }
    
    [Header("UI References")]
    public GameObject mainCanvas;
    public GameObject hudPanel;
    public GameObject menuPanel;
    public GameObject inventoryPanel;
    public GameObject mapPanel;
    public GameObject shopPanel;
    public GameObject characterPanel;
    public GameObject bandPanel;
    public GameObject settingsPanel;
    public GameObject battlePanel;
    public GameObject questPanel;
    public GameObject socialPanel;
    
    [Header("HUD Elements")]
    public TextMeshProUGUI playerNameText;
    public TextMeshProUGUI playerLevelText;
    public Slider healthSlider;
    public TextMeshProUGUI healthText;
    public Slider energySlider;
    public TextMeshProUGUI energyText;
    public Slider experienceSlider;
    public TextMeshProUGUI experienceText;
    public TextMeshProUGUI harmonyGemsText;
    public TextMeshProUGUI zoneNameText;
    
    [Header("Minimap")]
    public RawImage minimapImage;
    public RectTransform playerMarker;
    public RectTransform[] questMarkers;
    public RectTransform[] playerMarkers;
    
    [Header("Ability Bar")]
    public Image[] abilityIcons;
    public Image[] abilityCooldowns;
    public TextMeshProUGUI[] abilityHotkeys;
    
    [Header("Target Info")]
    public GameObject targetInfoPanel;
    public TextMeshProUGUI targetNameText;
    public Slider targetHealthSlider;
    public TextMeshProUGUI targetHealthText;
    
    [Header("Damage Text")]
    public DamageTextPool damageTextPool;
    
    [Header("Notifications")]
    public NotificationPanel notificationPanel;
    
    [Header("Dialogue")]
    public DialoguePanel dialoguePanel;
    
    [Header("Loading Screen")]
    public LoadingScreen loadingScreen;
    
    [Header("Chat")]
    public ChatPanel chatPanel;
    
    [Header("Party")]
    public PartyPanel partyPanel;
    
    [Header("Boss Health Bar")]
    public BossHealthBar bossHealthBar;
    
    private Stack<GameObject> panelStack = new Stack<GameObject>();
    private PlayerController localPlayer;
    
    public override void Awake()
    {
        base.Awake();
        Instance = this;
    }
    
    private void Start()
    {
        // Hide all panels except HUD
        HideAllPanels();
        ShowHUD(true);
        
        // Find local player
        localPlayer = FindObjectOfType<PlayerController>();
        if (localPlayer != null && localPlayer.isLocalPlayer)
        {
            UpdatePlayerInfo(localPlayer);
        }
    }
    
    private void Update()
    {
        // Update HUD every frame
        if (localPlayer != null)
        {
            UpdatePlayerStats(localPlayer.syncStats);
        }
    }
    
    public void SetLocalPlayer(PlayerController player)
    {
        localPlayer = player;
        UpdatePlayerInfo(player);
    }
    
    public void UpdatePlayerInfo(PlayerController player)
    {
        if (playerNameText != null)
        {
            playerNameText.text = player.playerName;
        }
        
        if (playerLevelText != null)
        {
            playerLevelText.text = "Lv. " + player.level;
        }
        
        UpdatePlayerStats(player.syncStats);
    }
    
    public void UpdatePlayerStats(PlayerStats stats)
    {
        if (healthSlider != null)
        {
            healthSlider.value = (float)stats.hp / stats.maxHp;
        }
        
        if (healthText != null)
        {
            healthText.text = $"{stats.hp}/{stats.maxHp}";
        }
        
        if (energySlider != null)
        {
            energySlider.value = (float)stats.energy / stats.maxEnergy;
        }
        
        if (energyText != null)
        {
            energyText.text = $"{stats.energy}/{stats.maxEnergy}";
        }
        
        if (localPlayer != null)
        {
            long nextLevelXp = CalculateNextLevelXP(localPlayer.level);
            float xpProgress = (float)localPlayer.experience / nextLevelXp;
            
            if (experienceSlider != null)
            {
                experienceSlider.value = xpProgress;
            }
            
            if (experienceText != null)
            {
                experienceText.text = $"{localPlayer.experience}/{nextLevelXp}";
            }
            
            if (harmonyGemsText != null)
            {
                harmonyGemsText.text = localPlayer.harmonyGems.ToString();
            }
        }
    }
    
    private long CalculateNextLevelXP(int level)
    {
        return Mathf.FloorToInt(100 * Mathf.Pow(1.5f, level - 1));
    }
    
    public void UpdateZoneInfo(ZoneData zone)
    {
        if (zoneNameText != null)
        {
            zoneNameText.text = zone.name;
        }
    }
    
    public void ShowHUD(bool show)
    {
        if (hudPanel != null)
        {
            hudPanel.SetActive(show);
        }
    }
    
    public void HideAllPanels()
    {
        menuPanel?.SetActive(false);
        inventoryPanel?.SetActive(false);
        mapPanel?.SetActive(false);
        shopPanel?.SetActive(false);
        characterPanel?.SetActive(false);
        bandPanel?.SetActive(false);
        settingsPanel?.SetActive(false);
        battlePanel?.SetActive(false);
        questPanel?.SetActive(false);
        socialPanel?.SetActive(false);
        
        panelStack.Clear();
    }
    
    public void ShowPanel(GameObject panel)
    {
        if (panel == null) return;
        
        // Hide current panel if any
        if (panelStack.Count > 0)
        {
            panelStack.Peek().SetActive(false);
        }
        
        // Show new panel
        panel.SetActive(true);
        panelStack.Push(panel);
    }
    
    public void HideCurrentPanel()
    {
        if (panelStack.Count > 0)
        {
            panelStack.Pop().SetActive(false);
        }
        
        if (panelStack.Count > 0)
        {
            panelStack.Peek().SetActive(true);
        }
    }
    
    public void TogglePanel(GameObject panel)
    {
        if (panel.activeSelf)
        {
            HideCurrentPanel();
        }
        else
        {
            ShowPanel(panel);
        }
    }
    
    public void ShowMenu()
    {
        ShowPanel(menuPanel);
    }
    
    public void HideMenu()
    {
        HideCurrentPanel();
    }
    
    public void ShowInventory()
    {
        ShowPanel(inventoryPanel);
    }
    
    public void HideInventory()
    {
        HideCurrentPanel();
    }
    
    public void ShowMap()
    {
        ShowPanel(mapPanel);
    }
    
    public void HideMap()
    {
        HideCurrentPanel();
    }
    
    public void ShowShop()
    {
        ShowPanel(shopPanel);
    }
    
    public void HideShop()
    {
        HideCurrentPanel();
    }
    
    public void ShowCharacter()
    {
        ShowPanel(characterPanel);
    }
    
    public void HideCharacter()
    {
        HideCurrentPanel();
    }
    
    public void ShowBand()
    {
        ShowPanel(bandPanel);
    }
    
    public void HideBand()
    {
        HideCurrentPanel();
    }
    
    public void ShowSettings()
    {
        ShowPanel(settingsPanel);
    }
    
    public void HideSettings()
    {
        HideCurrentPanel();
    }
    
    public void ShowBattle(bool show)
    {
        if (battlePanel != null)
        {
            battlePanel.SetActive(show);
        }
    }
    
    public void ShowQuestLog()
    {
        ShowPanel(questPanel);
    }
    
    public void HideQuestLog()
    {
        HideCurrentPanel();
    }
    
    public void ShowSocial()
    {
        ShowPanel(socialPanel);
    }
    
    public void HideSocial()
    {
        HideCurrentPanel();
    }
    
    public void SetTargetInfo(string name, int currentHp, int maxHp)
    {
        if (targetInfoPanel != null)
        {
            targetInfoPanel.SetActive(true);
            
            if (targetNameText != null)
            {
                targetNameText.text = name;
            }
            
            if (targetHealthSlider != null)
            {
                targetHealthSlider.value = (float)currentHp / maxHp;
            }
            
            if (targetHealthText != null)
            {
                targetHealthText.text = $"{currentHp}/{maxHp}";
            }
        }
    }
    
    public void ClearTargetInfo()
    {
        if (targetInfoPanel != null)
        {
            targetInfoPanel.SetActive(false);
        }
    }
    
    public void ShowDamageText(Vector3 position, string text, Color color)
    {
        if (damageTextPool != null)
        {
            damageTextPool.SpawnDamageText(position, text, color);
        }
    }
    
    public void ShowNotification(string title, string message, NotificationType type = NotificationType.Info)
    {
        if (notificationPanel != null)
        {
            notificationPanel.ShowNotification(title, message, type);
        }
    }
    
    public void ShowDialogue(string speaker, string message, Sprite portrait = null, Action onComplete = null)
    {
        if (dialoguePanel != null)
        {
            dialoguePanel.ShowDialogue(speaker, message, portrait, onComplete);
        }
    }
    
    public void ShowLoadingScreen(bool show, string message = "Loading...")
    {
        if (loadingScreen != null)
        {
            loadingScreen.Show(show, message);
        }
    }
    
    public void UpdateLoadingProgress(float progress)
    {
        if (loadingScreen != null)
        {
            loadingScreen.UpdateProgress(progress);
        }
    }
    
    public void AddChatMessage(string sender, string message, string channel = "General")
    {
        if (chatPanel != null)
        {
            chatPanel.AddMessage(sender, message, channel);
        }
    }
    
    public void UpdatePartyInfo(List<PlayerController> partyMembers)
    {
        if (partyPanel != null)
        {
            partyPanel.UpdateParty(partyMembers);
        }
    }
    
    public void ShowBossHealthBar(BossController boss)
    {
        if (bossHealthBar != null)
        {
            bossHealthBar.Show(boss);
        }
    }
    
    public void HideBossHealthBar()
    {
        if (bossHealthBar != null)
        {
            bossHealthBar.Hide();
        }
    }
    
    public void UpdateBossHealth(BossController boss, int currentHp, int maxHp)
    {
        if (bossHealthBar != null)
        {
            bossHealthBar.UpdateHealth(currentHp, maxHp);
        }
    }
}

public enum NotificationType
{
    Info,
    Success,
    Warning,
    Error
}

// Damage Text Pool
public class DamageTextPool : MonoBehaviour
{
    public GameObject damageTextPrefab;
    public int poolSize = 20;
    private Queue<GameObject> pool = new Queue<GameObject>();
    
    private void Awake()
    {
        for (int i = 0; i < poolSize; i++)
        {
            GameObject obj = Instantiate(damageTextPrefab, transform);
            obj.SetActive(false);
            pool.Enqueue(obj);
        }
    }
    
    public void SpawnDamageText(Vector3 worldPosition, string text, Color color)
    {
        if (pool.Count == 0) return;
        
        GameObject obj = pool.Dequeue();
        obj.SetActive(true);
        obj.transform.position = worldPosition;
        
        DamageText damageText = obj.GetComponent<DamageText>();
        if (damageText != null)
        {
            damageText.SetText(text, color);
        }
        
        StartCoroutine(ReturnToPool(obj, 2f));
    }
    
    private System.Collections.IEnumerator ReturnToPool(GameObject obj, float delay)
    {
        yield return new WaitForSeconds(delay);
        obj.SetActive(false);
        pool.Enqueue(obj);
    }
}

public class DamageText : MonoBehaviour
{
    public TextMeshProUGUI textComponent;
    public float moveSpeed = 2f;
    public float fadeSpeed = 1f;
    
    private void Update()
    {
        transform.position += Vector3.up * moveSpeed * Time.deltaTime;
        
        if (textComponent != null)
        {
            Color color = textComponent.color;
            color.a = Mathf.MoveTowards(color.a, 0, fadeSpeed * Time.deltaTime);
            textComponent.color = color;
            
            if (color.a <= 0)
            {
                gameObject.SetActive(false);
            }
        }
    }
    
    public void SetText(string text, Color color)
    {
        if (textComponent != null)
        {
            textComponent.text = text;
            textComponent.color = color;
        }
    }
}

// Notification Panel
public class NotificationPanel : MonoBehaviour
{
    public GameObject notificationPrefab;
    public Transform notificationContainer;
    public float displayDuration = 5f;
    
    public void ShowNotification(string title, string message, NotificationType type)
    {
        GameObject notificationObj = Instantiate(notificationPrefab, notificationContainer);
        Notification notification = notificationObj.GetComponent<Notification>();
        
        if (notification != null)
        {
            notification.Setup(title, message, type);
            Destroy(notificationObj, displayDuration);
        }
    }
}

public class Notification : MonoBehaviour
{
    public TextMeshProUGUI titleText;
    public TextMeshProUGUI messageText;
    public Image iconImage;
    public Sprite infoIcon;
    public Sprite successIcon;
    public Sprite warningIcon;
    public Sprite errorIcon;
    
    public void Setup(string title, string message, NotificationType type)
    {
        if (titleText != null)
        {
            titleText.text = title;
        }
        
        if (messageText != null)
        {
            messageText.text = message;
        }
        
        if (iconImage != null)
        {
            switch (type)
            {
                case NotificationType.Info:
                    iconImage.sprite = infoIcon;
                    iconImage.color = Color.blue;
                    break;
                case NotificationType.Success:
                    iconImage.sprite = successIcon;
                    iconImage.color = Color.green;
                    break;
                case NotificationType.Warning:
                    iconImage.sprite = warningIcon;
                    iconImage.color = Color.yellow;
                    break;
                case NotificationType.Error:
                    iconImage.sprite = errorIcon;
                    iconImage.color = Color.red;
                    break;
            }
        }
    }
}

// Dialogue Panel
public class DialoguePanel : MonoBehaviour
{
    public GameObject dialogueBox;
    public TextMeshProUGUI speakerText;
    public TextMeshProUGUI messageText;
    public Image portraitImage;
    public GameObject continueButton;
    
    private Action onComplete;
    
    public void ShowDialogue(string speaker, string message, Sprite portrait, Action onComplete)
    {
        this.onComplete = onComplete;
        
        if (dialogueBox != null)
        {
            dialogueBox.SetActive(true);
        }
        
        if (speakerText != null)
        {
            speakerText.text = speaker;
        }
        
        if (messageText != null)
        {
            messageText.text = message;
        }
        
        if (portraitImage != null)
        {
            portraitImage.sprite = portrait;
            portraitImage.enabled = portrait != null;
        }
        
        if (continueButton != null)
        {
            continueButton.SetActive(true);
        }
    }
    
    public void HideDialogue()
    {
        if (dialogueBox != null)
        {
            dialogueBox.SetActive(false);
        }
        
        onComplete?.Invoke();
        onComplete = null;
    }
    
    public void OnContinueButton()
    {
        HideDialogue();
    }
}

// Loading Screen
public class LoadingScreen : MonoBehaviour
{
    public GameObject loadingPanel;
    public Slider progressSlider;
    public TextMeshProUGUI progressText;
    public TextMeshProUGUI messageText;
    
    public void Show(bool show, string message)
    {
        if (loadingPanel != null)
        {
            loadingPanel.SetActive(show);
        }
        
        if (messageText != null)
        {
            messageText.text = message;
        }
    }
    
    public void UpdateProgress(float progress)
    {
        progress = Mathf.Clamp01(progress);
        
        if (progressSlider != null)
        {
            progressSlider.value = progress;
        }
        
        if (progressText != null)
        {
            progressText.text = $"{(int)(progress * 100)}%";
        }
    }
}

// Chat Panel
public class ChatPanel : MonoBehaviour
{
    public GameObject chatBox;
    public TextMeshProUGUI chatText;
    public TMP_InputField chatInput;
    public ScrollRect chatScroll;
    
    public void AddMessage(string sender, string message, string channel)
    {
        if (chatText != null)
        {
            chatText.text += $"[{channel}] {sender}: {message}\n";
        }
        
        // Auto-scroll to bottom
        if (chatScroll != null)
        {
            Canvas.ForceUpdateCanvases();
            chatScroll.verticalNormalizedPosition = 0f;
        }
    }
    
    public void SendMessage()
    {
        if (chatInput != null && !string.IsNullOrEmpty(chatInput.text))
        {
            string message = chatInput.text;
            chatInput.text = string.Empty;
            
            if (NetworkManagerMMO.Instance != null)
            {
                // Send message to server
                NetworkClient.Send(new ChatMessage
                {
                    senderId = NetworkClient.connection.identity.netId.ToString(),
                    senderName = localPlayer?.playerName ?? "Player",
                    message = message,
                    channel = "General"
                });
            }
            
            // Show message locally
            AddMessage(localPlayer?.playerName ?? "Player", message, "General");
        }
    }
    
    public void ToggleChat()
    {
        if (chatBox != null)
        {
            chatBox.SetActive(!chatBox.activeSelf);
            
            if (chatBox.activeSelf && chatInput != null)
            {
                chatInput.ActivateInputField();
            }
        }
    }
}

// Party Panel
public class PartyPanel : MonoBehaviour
{
    public GameObject partyMemberPrefab;
    public Transform partyContainer;
    private List<PartyMemberUI> partyMembers = new List<PartyMemberUI>();
    
    public void UpdateParty(List<PlayerController> members)
    {
        // Clear existing
        foreach (var member in partyMembers)
        {
            Destroy(member.gameObject);
        }
        partyMembers.Clear();
        
        // Create new
        foreach (var player in members)
        {
            GameObject memberObj = Instantiate(partyMemberPrefab, partyContainer);
            PartyMemberUI memberUI = memberObj.GetComponent<PartyMemberUI>();
            
            if (memberUI != null)
            {
                memberUI.Setup(player);
                partyMembers.Add(memberUI);
            }
        }
    }
}

public class PartyMemberUI : MonoBehaviour
{
    public TextMeshProUGUI nameText;
    public TextMeshProUGUI levelText;
    public TextMeshProUGUI classText;
    public Slider healthSlider;
    
    public void Setup(PlayerController player)
    {
        if (nameText != null)
        {
            nameText.text = player.playerName;
        }
        
        if (levelText != null)
        {
            levelText.text = "Lv. " + player.level;
        }
        
        if (classText != null)
        {
            classText.text = player.characterClass.ToString();
        }
        
        if (healthSlider != null)
        {
            healthSlider.value = (float)player.syncStats.hp / player.syncStats.maxHp;
        }
    }
}

// Boss Health Bar
public class BossHealthBar : MonoBehaviour
{
    public GameObject bossHealthPanel;
    public TextMeshProUGUI bossNameText;
    public Slider bossHealthSlider;
    public TextMeshProUGUI bossHealthText;
    
    private BossController currentBoss;
    
    public void Show(BossController boss)
    {
        currentBoss = boss;
        
        if (bossHealthPanel != null)
        {
            bossHealthPanel.SetActive(true);
        }
        
        if (bossNameText != null)
        {
            bossNameText.text = boss.bossData.name;
        }
        
        UpdateHealth(boss.currentHp, boss.bossData.maxHp);
    }
    
    public void Hide()
    {
        if (bossHealthPanel != null)
        {
            bossHealthPanel.SetActive(false);
        }
        
        currentBoss = null;
    }
    
    public void UpdateHealth(int currentHp, int maxHp)
    {
        if (bossHealthSlider != null)
        {
            bossHealthSlider.value = (float)currentHp / maxHp;
        }
        
        if (bossHealthText != null)
        {
            bossHealthText.text = $"{currentHp}/{maxHp}";
        }
    }
}

public class BossHealthBarManager : MonoBehaviour
{
    public static BossHealthBarManager Instance { get; private set; }
    
    public BossHealthBar bossHealthBarPrefab;
    private BossHealthBar currentBossHealthBar;
    
    public override void Awake()
    {
        base.Awake();
        Instance = this;
    }
    
    public void ShowBossHealth(BossController boss)
    {
        if (currentBossHealthBar == null)
        {
            currentBossHealthBar = Instantiate(bossHealthBarPrefab, transform);
        }
        
        currentBossHealthBar.Show(boss);
    }
    
    public void HideBossHealth()
    {
        if (currentBossHealthBar != null)
        {
            currentBossHealthBar.Hide();
        }
    }
    
    public void UpdateBossHealth(BossController boss, int currentHp, int maxHp)
    {
        if (currentBossHealthBar != null && currentBossHealthBar.currentBoss == boss)
        {
            currentBossHealthBar.UpdateHealth(currentHp, maxHp);
        }
    }
}

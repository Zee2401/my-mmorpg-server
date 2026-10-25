pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MMORPGHUD.generated.h"

// Forward declarations
class AMusicMMORPGPlayerController;
class AMusicMMORPGCharacter;
class AMusicMMORPGPlayerState;

/**
 * HUD for Music MMORPG
 * Displays game information and UI elements
 */
UCLASS()
class MUSICMMORPG_API UMMORPGHUD : public UHUD
{
    GENERATED_BODY()

public:
    UMMORPGHUD();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void DrawHUD() override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void InitializeHUD(AMusicMMORPGPlayerController* InPlayerController);
    
    // UI Widgets
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> MainHUDWidgetClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> InventoryWidgetClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> MapWidgetClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> MenuWidgetClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> ChatWidgetClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> SocialWidgetClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> QuestWidgetClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> ShopWidgetClass;
    
    // Widget instances
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    class UUserWidget* MainHUDWidget;
    
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    class UUserWidget* InventoryWidget;
    
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    class UUserWidget* MapWidget;
    
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    class UUserWidget* MenuWidget;
    
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    class UUserWidget* ChatWidget;
    
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    class UUserWidget* SocialWidget;
    
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    class UUserWidget* QuestWidget;
    
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    class UUserWidget* ShopWidget;
    
    // Player controller reference
    UPROPERTY(BlueprintReadOnly, Category = "HUD")
    AMusicMMORPGPlayerController* PlayerController;
    
    // Chat messages
    UPROPERTY(BlueprintReadOnly, Category = "Chat")
    TArray<FText> ChatMessages;
    
    UPROPERTY(BlueprintReadOnly, Category = "Chat")
    TArray<FString> ChatMessageSenders;
    
    // UI Toggle
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ToggleHUD();
    
    // Widget toggles
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ToggleInventory();
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ToggleMap();
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ToggleMenu();
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ToggleChat();
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ToggleSocial();
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ToggleQuest();
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ToggleShop();
    
    // Chat functions
    UFUNCTION(BlueprintCallable, Category = "Chat")
    void AddChatMessage(const FText& Message, const FString& Sender);
    
    UFUNCTION(BlueprintCallable, Category = "Chat")
    void SendChatMessage(const FText& Message);
    
    // Notification functions
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowNotification(const FText& Message);
    
    // Zone functions
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void OnZoneChanged(FName NewZone);
    
    // Player info updates
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdatePlayerName(const FString& NewName);
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdatePlayerLevel(int32 NewLevel);
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdatePlayerHealth(int32 NewHealth, int32 MaxHealth);
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdatePlayerEnergy(int32 NewEnergy, int32 MaxEnergy);
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdatePlayerExperience(int64 NewExperience, int64 ExperienceToNextLevel);
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHarmonyGems(int32 NewAmount);
    
    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatMessageAddedSignature, FText, Message);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotificationShownSignature, FText, Message);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZoneChangedSignature, FName, NewZone);
    
    UPROPERTY(BlueprintAssignable, Category = "HUD")
    FOnChatMessageAddedSignature OnChatMessageAdded;
    
    UPROPERTY(BlueprintAssignable, Category = "HUD")
    FOnNotificationShownSignature OnNotificationShown;
    
    UPROPERTY(BlueprintAssignable, Category = "HUD")
    FOnZoneChangedSignature OnZoneChangedEvent;
    
protected:
    // Draw functions
    void DrawHealthBar();
    void DrawEnergyBar();
    void DrawExperienceBar();
    void DrawChat();
    void DrawNotifications();
    void DrawPlayerInfo();
    void DrawTargetInfo();
    void DrawMinimap();
    
    // Helper functions
    void UpdatePlayerInfo();
};

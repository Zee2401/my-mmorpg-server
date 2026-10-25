pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MMORPGGameMode.generated.h"

// Forward declarations
class AMusicMMORPGPlayerController;
class AMusicMMORPGCharacter;
class AMusicMMORPGPlayerState;
class AMusicMMORPGGameState;
class UWorldData;
class UZoneManager;
class USocialSystem;
class UQuestSystem;
class UShopSystem;
class UCraftingSystem;
class USaveSystem;

/**
 * Main GameMode for Music MMORPG
 * Handles game rules, player spawning, and game state management
 */
UCLASS()
class MUSICMMORPG_API AMusicMMORPGGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMusicMMORPGGameMode();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void StartPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    // Player spawning
    virtual void SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot) override;
    virtual void RestartPlayer(AController* NewPlayer) override;
    virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, const FString& InStartTag) override;
    virtual void RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) override;
    
    // Player management
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;
    virtual void PreLogout(AController* Exiting) override;
    
    // Game state
    virtual void InitGame(const FString& MapName, const FString& Options) override;
    virtual void InitGameState() override;
    virtual void HandleMatchHasStarted() override;
    virtual void HandleMatchHasEnded() override;
    
    // Networking
    virtual void PreInitializeComponents() override;
    virtual void PostInitializeComponents() override;
    
    // Game systems
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Systems")
    class UWorldData* WorldData;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Systems")
    class UZoneManager* ZoneManager;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Systems")
    class USocialSystem* SocialSystem;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Systems")
    class UQuestSystem* QuestSystem;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Systems")
    class UShopSystem* ShopSystem;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Systems")
    class UCraftingSystem* CraftingSystem;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Systems")
    class USaveSystem* SaveSystem;
    
    // Default classes
    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<AMusicMMORPGPlayerController> PlayerControllerClass;
    
    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<AMusicMMORPGCharacter> DefaultPawnClass;
    
    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<AMusicMMORPGPlayerState> PlayerStateClass;
    
    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<AMusicMMORPGGameState> GameStateClass;
    
    UPROPERTY(EditDefaultsOnly, Category = "Classes")
    TSubclassOf<APlayerStart> PlayerStartClass;
    
    // Game settings
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings")
    FString GameName = TEXT("Music MMORPG");
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings")
    FString GameVersion = TEXT("1.0.0");
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings")
    int32 MaxPlayers = 100;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings")
    float AutoSaveInterval = 300.0f; // 5 minutes
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings")
    bool bAutoSaveEnabled = true;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings")
    bool bFriendlyFire = false;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings")
    bool bPvPEnabled = true;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings")
    bool bAllowSpectating = false;
    
    // Spawn settings
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn Settings")
    TArray<FName> SpawnTags;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn Settings")
    TArray<AActor*> SpawnPoints;
    
    // Zone settings
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zone Settings")
    FName DefaultZone = NAME_SoundCity;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zone Settings")
    TArray<FName> AvailableZones;
    
    // Getters
    UFUNCTION(BlueprintPure, Category = "Game Mode")
    AMusicMMORPGGameState* GetMMORPGGameState() const;
    
    UFUNCTION(BlueprintPure, Category = "Game Mode")
    UWorldData* GetWorldData() const { return WorldData; }
    
    UFUNCTION(BlueprintPure, Category = "Game Mode")
    UZoneManager* GetZoneManager() const { return ZoneManager; }
    
    UFUNCTION(BlueprintPure, Category = "Game Mode")
    USocialSystem* GetSocialSystem() const { return SocialSystem; }
    
    UFUNCTION(BlueprintPure, Category = "Game Mode")
    UQuestSystem* GetQuestSystem() const { return QuestSystem; }
    
    UFUNCTION(BlueprintPure, Category = "Game Mode")
    UShopSystem* GetShopSystem() const { return ShopSystem; }
    
    UFUNCTION(BlueprintPure, Category = "Game Mode")
    UCraftingSystem* GetCraftingSystem() const { return CraftingSystem; }
    
    UFUNCTION(BlueprintPure, Category = "Game Mode")
    USaveSystem* GetSaveSystem() const { return SaveSystem; }
    
    // Player management
    UFUNCTION(BlueprintCallable, Category = "Game Mode")
    void KickPlayer(APlayerController* PlayerController, const FText& Reason = FText::GetEmptyText());
    
    UFUNCTION(BlueprintCallable, Category = "Game Mode")
    void BanPlayer(APlayerController* PlayerController, const FText& Reason = FText::GetEmptyText());
    
    UFUNCTION(BlueprintCallable, Category = "Game Mode")
    void MutePlayer(APlayerController* PlayerController, bool bMute);
    
    UFUNCTION(BlueprintCallable, Category = "Game Mode")
    void SendGlobalMessage(const FText& Message, APlayerController* Sender = nullptr);
    
    // Zone management
    UFUNCTION(BlueprintCallable, Category = "Game Mode")
    void ChangePlayerZone(AMusicMMORPGCharacter* PlayerCharacter, const FName& ZoneName);
    
    UFUNCTION(BlueprintCallable, Category = "Game Mode")
    void TeleportPlayerToZone(AMusicMMORPGCharacter* PlayerCharacter, const FName& ZoneName, const FVector& Location = FVector::ZeroVector, const FRotator& Rotation = FRotator::ZeroRotator);
    
    // Save/Load
    UFUNCTION(BlueprintCallable, Category = "Game Mode")
    void SaveGame();
    
    UFUNCTION(BlueprintCallable, Category = "Game Mode")
    void LoadGame();
    
    UFUNCTION(BlueprintCallable, Category = "Game Mode")
    void AutoSave();
    
    // Network functions
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerKickPlayer(APlayerController* PlayerController, const FText& Reason);
    
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerBanPlayer(APlayerController* PlayerController, const FText& Reason);
    
    UFUNCTION(Server, Reliable)
    void ServerSendGlobalMessage(const FText& Message, APlayerController* Sender);
    
    UFUNCTION(Server, Reliable)
    void ServerChangePlayerZone(AMusicMMORPGCharacter* PlayerCharacter, const FName& ZoneName);
    
    UFUNCTION(Server, Reliable)
    void ServerTeleportPlayerToZone(AMusicMMORPGCharacter* PlayerCharacter, const FName& ZoneName, const FVector& Location, const FRotator& Rotation);
    
    // Static instance
    static AMusicMMORPGGameMode* GetInstance(const UWorld* World);
    
protected:
    virtual void InitGameState() override;
    virtual void CreateGameState() override;
    
    // Auto-save timer
    FTimerHandle AutoSaveTimerHandle;
    
    void OnAutoSaveTimer();
    
    // Initialize game systems
    void InitializeGameSystems();
    void InitializeWorldData();
    void InitializeZoneManager();
    void InitializeSocialSystem();
    void InitializeQuestSystem();
    void InitializeShopSystem();
    void InitializeCraftingSystem();
    void InitializeSaveSystem();
    
    // Spawn default pawn
    void SpawnDefaultPawn(AMusicMMORPGPlayerController* PlayerController);
    
    // Find spawn location
    FTransform FindSpawnTransform(AController* Player, const FString& InStartTag = FString());
};

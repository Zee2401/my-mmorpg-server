pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MMORPGGameState.generated.h"

// Forward declarations
class AMusicMMORPGPlayerState;
class AMusicMMORPGCharacter;
class AMMORPGBossCharacter;

/**
 * Game state for Music MMORPG
 * Tracks all players, zones, and game-wide information
 */
UCLASS()
class MUSICMMORPG_API AMusicMMORPGGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AMusicMMORPGGameState();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    // Player management
    UFUNCTION(BlueprintCallable, Category = "Game State")
    void OnPlayerLoggedIn(AMusicMMORPGPlayerController* PlayerController);
    
    UFUNCTION(BlueprintCallable, Category = "Game State")
    void OnPlayerLoggedOut(AMusicMMORPGPlayerController* PlayerController);
    
    UFUNCTION(BlueprintCallable, Category = "Game State")
    void OnPlayerSpawned(AMusicMMORPGCharacter* PlayerCharacter);
    
    UFUNCTION(BlueprintCallable, Category = "Game State")
    void OnPlayerDestroyed(AMusicMMORPGCharacter* PlayerCharacter);
    
    // Player tracking
    UFUNCTION(BlueprintPure, Category = "Game State")
    TArray<AMusicMMORPGPlayerState*> GetAllPlayerStates() const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    TArray<AMusicMMORPGCharacter*> GetAllPlayerCharacters() const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    AMusicMMORPGPlayerState* GetPlayerStateByName(const FString& PlayerName) const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    AMusicMMORPGCharacter* GetPlayerCharacterByName(const FString& PlayerName) const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    AMusicMMORPGPlayerState* GetPlayerStateByID(const FString& PlayerID) const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    AMusicMMORPGCharacter* GetPlayerCharacterByID(const FString& PlayerID) const;
    
    // Zone tracking
    UFUNCTION(BlueprintPure, Category = "Game State")
    TArray<FName> GetAvailableZones() const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    TArray<AMusicMMORPGCharacter*> GetPlayersInZone(const FName& ZoneName) const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    int32 GetPlayerCountInZone(const FName& ZoneName) const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    FName GetZoneForPlayer(const AMusicMMORPGCharacter* PlayerCharacter) const;
    
    // Boss tracking
    UFUNCTION(BlueprintPure, Category = "Game State")
    TArray<AMMORPGBossCharacter*> GetAllBosses() const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    AMMORPGBossCharacter* GetBossInZone(const FName& ZoneName) const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    bool IsBossActiveInZone(const FName& ZoneName) const;
    
    // Game statistics
    UFUNCTION(BlueprintPure, Category = "Game State")
    int32 GetTotalPlayerCount() const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    int32 GetMaxPlayers() const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    FString GetServerName() const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    FString GetGameVersion() const;
    
    // Banned players
    UFUNCTION(BlueprintCallable, Category = "Game State")
    void AddBannedPlayer(const FString& PlayerID);
    
    UFUNCTION(BlueprintCallable, Category = "Game State")
    void RemoveBannedPlayer(const FString& PlayerID);
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    bool IsPlayerBanned(const FString& PlayerID) const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    TArray<FString> GetBannedPlayers() const;
    
    // Server info
    UFUNCTION(BlueprintPure, Category = "Game State")
    FDateTime GetServerStartTime() const;
    
    UFUNCTION(BlueprintPure, Category = "Game State")
    float GetServerUptime() const;
    
    // Replication
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
    FString ServerName;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
    FString GameVersion;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
    int32 CurrentPlayerCount;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
    int32 MaxPlayers;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
    FDateTime ServerStartTime;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
    TArray<FString> AvailableZones;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
    TMap<FString, FName> PlayerZoneMap;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
    TArray<FString> BannedPlayers;
    
    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLoggedInSignature, AMusicMMORPGPlayerController*, PlayerController);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLoggedOutSignature, AMusicMMORPGPlayerController*, PlayerController);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSpawnedSignature, AMusicMMORPGCharacter*, PlayerCharacter);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDestroyedSignature, AMusicMMORPGCharacter*, PlayerCharacter);
    
    UPROPERTY(BlueprintAssignable, Category = "Game State")
    FOnPlayerLoggedInSignature OnPlayerLoggedIn;
    
    UPROPERTY(BlueprintAssignable, Category = "Game State")
    FOnPlayerLoggedOutSignature OnPlayerLoggedOut;
    
    UPROPERTY(BlueprintAssignable, Category = "Game State")
    FOnPlayerSpawnedSignature OnPlayerSpawned;
    
    UPROPERTY(BlueprintAssignable, Category = "Game State")
    FOnPlayerDestroyedSignature OnPlayerDestroyed;
    
protected:
    // Track all player characters
    UPROPERTY(Replicated)
    TArray<AMusicMMORPGCharacter*> PlayerCharacters;
    
    // Track all bosses
    UPROPERTY(Replicated)
    TArray<AMMORPGBossCharacter*> BossCharacters;
    
    // Track zone bosses
    UPROPERTY(Replicated)
    TMap<FName, AMMORPGBossCharacter*> ZoneBossMap;
    
    // Track player zones
    UPROPERTY(Replicated)
    TMap<AMusicMMORPGCharacter*, FName> CharacterZoneMap;
};

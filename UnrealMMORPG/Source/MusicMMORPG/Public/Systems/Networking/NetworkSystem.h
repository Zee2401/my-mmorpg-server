pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NetworkSystem.generated.h"

// Forward declarations
class AMusicMMORPGCharacter;
class AMusicMMORPGPlayerController;
class AMusicMMORPGPlayerState;
class AMusicMMORPGGameMode;

/**
 * Network System for Music MMORPG
 * Handles all networking functionality including replication, RPCs, and connection management
 */
UCLASS(Blueprintable)
class MUSICMMORPG_API UNetworkSystem : public UObject
{
    GENERATED_BODY()

public:
    UNetworkSystem();
    
    virtual void Initialize();
    virtual void Tick(float DeltaTime);
    virtual void Shutdown();
    
    // Connection management
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool ConnectToServer(const FString& ServerAddress, int32 Port);
    
    UFUNCTION(BlueprintCallable, Category = "Network")
    void DisconnectFromServer();
    
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool CreateServer(int32 Port, int32 MaxPlayers);
    
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool CreateListenServer(int32 Port, int32 MaxPlayers);
    
    UFUNCTION(BlueprintCallable, Category = "Network")
    void DestroyServer();
    
    // Connection state
    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsConnected() const;
    
    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsServer() const;
    
    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsClient() const;
    
    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsListenServer() const;
    
    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsDedicatedServer() const;
    
    UFUNCTION(BlueprintPure, Category = "Network")
    int32 GetCurrentPlayerCount() const;
    
    UFUNCTION(BlueprintPure, Category = "Network")
    int32 GetMaxPlayers() const;
    
    UFUNCTION(BlueprintPure, Category = "Network")
    FString GetServerAddress() const;
    
    UFUNCTION(BlueprintPure, Category = "Network")
    int32 GetServerPort() const;
    
    // Player management
    UFUNCTION(BlueprintCallable, Category = "Network")
    void RegisterPlayer(AMusicMMORPGPlayerController* PlayerController);
    
    UFUNCTION(BlueprintCallable, Category = "Network")
    void UnregisterPlayer(AMusicMMORPGPlayerController* PlayerController);
    
    UFUNCTION(BlueprintPure, Category = "Network")
    TArray<AMusicMMORPGPlayerController*> GetAllPlayers() const;
    
    UFUNCTION(BlueprintPure, Category = "Network")
    AMusicMMORPGPlayerController* GetPlayerByName(const FString& PlayerName) const;
    
    UFUNCTION(BlueprintPure, Category = "Network")
    AMusicMMORPGPlayerController* GetPlayerByID(const FString& PlayerID) const;
    
    // Replication management
    UFUNCTION(BlueprintCallable, Category = "Network")
    void ForceNetUpdate(AActor* Actor);
    
    UFUNCTION(BlueprintCallable, Category = "Network")
    void SetReplicationInterval(AActor* Actor, float Interval);
    
    // RPC helpers
    UFUNCTION(BlueprintCallable, Category = "Network")
    void ExecuteServerRPC(UObject* Object, FName FunctionName, void* Parameters);
    
    UFUNCTION(BlueprintCallable, Category = "Network")
    void ExecuteClientRPC(UObject* Object, FName FunctionName, void* Parameters);
    
    UFUNCTION(BlueprintCallable, Category = "Network")
    void ExecuteMulticastRPC(UObject* Object, FName FunctionName, void* Parameters);
    
    // Network statistics
    UFUNCTION(BlueprintPure, Category = "Network")
    float GetPing() const;
    
    UFUNCTION(BlueprintPure, Category = "Network")
    float GetPacketLoss() const;
    
    UFUNCTION(BlueprintPure, Category = "Network")
    float GetBandwidth() const;
    
    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnectedSignature);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisconnectedSignature);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerConnectedSignature, AMusicMMORPGPlayerController*, PlayerController);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDisconnectedSignature, AMusicMMORPGPlayerController*, PlayerController);
    
    UPROPERTY(BlueprintAssignable, Category = "Network")
    FOnConnectedSignature OnConnected;
    
    UPROPERTY(BlueprintAssignable, Category = "Network")
    FOnDisconnectedSignature OnDisconnected;
    
    UPROPERTY(BlueprintAssignable, Category = "Network")
    FOnPlayerConnectedSignature OnPlayerConnected;
    
    UPROPERTY(BlueprintAssignable, Category = "Network")
    FOnPlayerDisconnectedSignature OnPlayerDisconnected;
    
protected:
    // Network state
    UPROPERTY()
    bool bIsConnected;
    
    UPROPERTY()
    bool bIsServer;
    
    UPROPERTY()
    bool bIsListenServer;
    
    UPROPERTY()
    bool bIsDedicatedServer;
    
    // Server info
    UPROPERTY()
    FString ServerAddress;
    
    UPROPERTY()
    int32 ServerPort;
    
    UPROPERTY()
    int32 MaxPlayers;
    
    UPROPERTY()
    int32 CurrentPlayerCount;
    
    // Player tracking
    UPROPERTY()
    TArray<AMusicMMORPGPlayerController*> Players;
    
    UPROPERTY()
    TMap<FString, AMusicMMORPGPlayerController*> PlayerMap;
    
    // Helper functions
    void UpdateNetworkState();
};

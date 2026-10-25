#include "NetworkSystem.h"
#include "MusicMMORPGPlayerController.h"
#include "MusicMMORPGCharacter.h"
#include "MusicMMORPGGameMode.h"
#include "Engine/World.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/OnlineSession.h"

UNetworkSystem::UNetworkSystem()
{
    bIsConnected = false;
    bIsServer = false;
    bIsListenServer = false;
    bIsDedicatedServer = false;
    ServerAddress = TEXT("");
    ServerPort = 7777;
    MaxPlayers = 100;
    CurrentPlayerCount = 0;
}

void UNetworkSystem::Initialize()
{
    UpdateNetworkState();
}

void UNetworkSystem::Tick(float DeltaTime)
{
    UpdateNetworkState();
}

void UNetworkSystem::Shutdown()
{
    Players.Empty();
    PlayerMap.Empty();
    bIsConnected = false;
    bIsServer = false;
    bIsListenServer = false;
    bIsDedicatedServer = false;
}

bool UNetworkSystem::ConnectToServer(const FString& ServerAddress, int32 Port)
{
    // In a real implementation, this would use the engine's networking system
    // For now, we'll just update the state
    ServerAddress = ServerAddress;
    ServerPort = Port;
    
    // Try to connect using Unreal's networking
    if (UWorld* World = GetOuter()->GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            FString URL = FString::Printf(TEXT("%s:%d"), *ServerAddress, Port);
            PlayerController->ClientTravel(URL, ETravelType::TRAVEL_Absolute);
            
            bIsConnected = true;
            OnConnected.Broadcast();
            return true;
        }
    }
    
    return false;
}

void UNetworkSystem::DisconnectFromServer()
{
    if (UWorld* World = GetOuter()->GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            PlayerController->ConsoleCommand("Disconnect");
            bIsConnected = false;
            OnDisconnected.Broadcast();
        }
    }
}

bool UNetworkSystem::CreateServer(int32 Port, int32 MaxPlayers)
{
    ServerPort = Port;
    this->MaxPlayers = MaxPlayers;
    
    // Create a dedicated server
    if (UWorld* World = GetOuter()->GetWorld())
    {
        // In a real implementation, this would use the engine's server creation
        // For now, we'll just update the state
        bIsServer = true;
        bIsDedicatedServer = true;
        bIsConnected = true;
        
        return true;
    }
    
    return false;
}

bool UNetworkSystem::CreateListenServer(int32 Port, int32 MaxPlayers)
{
    ServerPort = Port;
    this->MaxPlayers = MaxPlayers;
    
    // Create a listen server
    if (UWorld* World = GetOuter()->GetWorld())
    {
        // In a real implementation, this would use the engine's listen server creation
        // For now, we'll just update the state
        bIsServer = true;
        bIsListenServer = true;
        bIsConnected = true;
        
        return true;
    }
    
    return false;
}

void UNetworkSystem::DestroyServer()
{
    if (bIsServer)
    {
        // In a real implementation, this would destroy the server
        bIsServer = false;
        bIsListenServer = false;
        bIsDedicatedServer = false;
        bIsConnected = false;
    }
}

bool UNetworkSystem::IsConnected() const
{
    return bIsConnected;
}

bool UNetworkSystem::IsServer() const
{
    return bIsServer;
}

bool UNetworkSystem::IsClient() const
{
    return bIsConnected && !bIsServer;
}

bool UNetworkSystem::IsListenServer() const
{
    return bIsListenServer;
}

bool UNetworkSystem::IsDedicatedServer() const
{
    return bIsDedicatedServer;
}

int32 UNetworkSystem::GetCurrentPlayerCount() const
{
    return CurrentPlayerCount;
}

int32 UNetworkSystem::GetMaxPlayers() const
{
    return MaxPlayers;
}

FString UNetworkSystem::GetServerAddress() const
{
    return ServerAddress;
}

int32 UNetworkSystem::GetServerPort() const
{
    return ServerPort;
}

void UNetworkSystem::RegisterPlayer(AMusicMMORPGPlayerController* PlayerController)
{
    if (PlayerController && !Players.Contains(PlayerController))
    {
        Players.Add(PlayerController);
        
        if (AMusicMMORPGCharacter* Character = PlayerController->GetMMORPGCharacter())
        {
            if (AMusicMMORPGPlayerState* PlayerState = Character->GetMMORPGPlayerState())
            {
                PlayerMap.Add(PlayerState->GetPlayerName(), PlayerController);
            }
        }
        
        CurrentPlayerCount = Players.Num();
        OnPlayerConnected.Broadcast(PlayerController);
    }
}

void UNetworkSystem::UnregisterPlayer(AMusicMMORPGPlayerController* PlayerController)
{
    if (PlayerController)
    {
        Players.Remove(PlayerController);
        
        if (AMusicMMORPGCharacter* Character = PlayerController->GetMMORPGCharacter())
        {
            if (AMusicMMORPGPlayerState* PlayerState = Character->GetMMORPGPlayerState())
            {
                PlayerMap.Remove(PlayerState->GetPlayerName());
            }
        }
        
        CurrentPlayerCount = Players.Num();
        OnPlayerDisconnected.Broadcast(PlayerController);
    }
}

TArray<AMusicMMORPGPlayerController*> UNetworkSystem::GetAllPlayers() const
{
    return Players;
}

AMusicMMORPGPlayerController* UNetworkSystem::GetPlayerByName(const FString& PlayerName) const
{
    if (const AMusicMMORPGPlayerController* Player = PlayerMap.Find(PlayerName))
    {
        return *Player;
    }
    return nullptr;
}

AMusicMMORPGPlayerController* UNetworkSystem::GetPlayerByID(const FString& PlayerID) const
{
    // In a real implementation, this would use the player's unique ID
    // For now, we'll just search by name
    return GetPlayerByName(PlayerID);
}

void UNetworkSystem::ForceNetUpdate(AActor* Actor)
{
    if (Actor)
    {
        Actor->ForceNetUpdate();
    }
}

void UNetworkSystem::SetReplicationInterval(AActor* Actor, float Interval)
{
    if (Actor)
    {
        Actor->NetUpdateFrequency = Interval;
    }
}

void UNetworkSystem::ExecuteServerRPC(UObject* Object, FName FunctionName, void* Parameters)
{
    // In a real implementation, this would execute a server RPC
    // For now, we'll just log it
}

void UNetworkSystem::ExecuteClientRPC(UObject* Object, FName FunctionName, void* Parameters)
{
    // In a real implementation, this would execute a client RPC
    // For now, we'll just log it
}

void UNetworkSystem::ExecuteMulticastRPC(UObject* Object, FName FunctionName, void* Parameters)
{
    // In a real implementation, this would execute a multicast RPC
    // For now, we'll just log it
}

float UNetworkSystem::GetPing() const
{
    // In a real implementation, this would get the actual ping
    return 0.0f;
}

float UNetworkSystem::GetPacketLoss() const
{
    // In a real implementation, this would get the actual packet loss
    return 0.0f;
}

float UNetworkSystem::GetBandwidth() const
{
    // In a real implementation, this would get the actual bandwidth usage
    return 0.0f;
}

void UNetworkSystem::UpdateNetworkState()
{
    if (UWorld* World = GetOuter()->GetWorld())
    {
        if (World->GetNetMode() == NM_DedicatedServer)
        {
            bIsServer = true;
            bIsDedicatedServer = true;
            bIsListenServer = false;
            bIsConnected = true;
        }
        else if (World->GetNetMode() == NM_ListenServer)
        {
            bIsServer = true;
            bIsDedicatedServer = false;
            bIsListenServer = true;
            bIsConnected = true;
        }
        else if (World->GetNetMode() == NM_Client)
        {
            bIsServer = false;
            bIsDedicatedServer = false;
            bIsListenServer = false;
            bIsConnected = true;
        }
        else
        {
            bIsServer = false;
            bIsDedicatedServer = false;
            bIsListenServer = false;
            bIsConnected = false;
        }
        
        // Update player count
        if (bIsServer)
        {
            CurrentPlayerCount = World->GetNumPlayers();
        }
    }
}

#include "MusicMMORPGGameState.h"
#include "MusicMMORPGPlayerState.h"
#include "MusicMMORPGCharacter.h"
#include "MusicMMORPGPlayerController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"

AMusicMMORPGGameState::AMusicMMORPGGameState()
{
    // Set default values
    ServerName = TEXT("Music MMORPG Server");
    GameVersion = TEXT("1.0.0");
    CurrentPlayerCount = 0;
    MaxPlayers = 100;
    ServerStartTime = FDateTime::Now();
    
    // Initialize zone list
    AvailableZones.Add(NAME_SoundCity);
    AvailableZones.Add(NAME_BassCaverns);
    AvailableZones.Add(NAME_SynthCitadel);
    
    // Enable replication
    bReplicates = true;
    
    // Set replication policy
    SetReplicateMovement(true);
}

void AMusicMMORPGGameState::BeginPlay()
{
    Super::BeginPlay();
}

void AMusicMMORPGGameState::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMusicMMORPGGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    // Replicate these properties
    DOREPLIFETIME(AMusicMMORPGGameState, ServerName);
    DOREPLIFETIME(AMusicMMORPGGameState, GameVersion);
    DOREPLIFETIME(AMusicMMORPGGameState, CurrentPlayerCount);
    DOREPLIFETIME(AMusicMMORPGGameState, MaxPlayers);
    DOREPLIFETIME(AMusicMMORPGGameState, ServerStartTime);
    DOREPLIFETIME(AMusicMMORPGGameState, AvailableZones);
    DOREPLIFETIME(AMusicMMORPGGameState, PlayerZoneMap);
    DOREPLIFETIME(AMusicMMORPGGameState, BannedPlayers);
    DOREPLIFETIME(AMusicMMORPGGameState, PlayerCharacters);
    DOREPLIFETIME(AMusicMMORPGGameState, BossCharacters);
    DOREPLIFETIME(AMusicMMORPGGameState, ZoneBossMap);
    DOREPLIFETIME(AMusicMMORPGGameState, CharacterZoneMap);
}

void AMusicMMORPGGameState::OnPlayerLoggedIn(AMusicMMORPGPlayerController* PlayerController)
{
    if (!PlayerController) return;
    
    // Add to player list
    if (AMusicMMORPGCharacter* Character = PlayerController->GetMMORPGCharacter())
    {
        PlayerCharacters.Add(Character);
        
        // Track zone
        if (AMusicMMORPGPlayerState* PlayerState = PlayerController->GetMMORPGPlayerState())
        {
            FName Zone = PlayerState->GetCurrentZone();
            CharacterZoneMap.Add(Character, Zone);
            PlayerZoneMap.Add(PlayerState->GetPlayerName(), Zone);
        }
    }
    
    // Update player count
    CurrentPlayerCount = PlayerCharacters.Num();
    
    // Broadcast event
    OnPlayerLoggedIn.Broadcast(PlayerController);
}

void AMusicMMORPGGameState::OnPlayerLoggedOut(AMusicMMORPGPlayerController* PlayerController)
{
    if (!PlayerController) return;
    
    // Remove from player list
    if (AMusicMMORPGCharacter* Character = PlayerController->GetMMORPGCharacter())
    {
        PlayerCharacters.Remove(Character);
        CharacterZoneMap.Remove(Character);
        
        if (AMusicMMORPGPlayerState* PlayerState = PlayerController->GetMMORPGPlayerState())
        {
            PlayerZoneMap.Remove(PlayerState->GetPlayerName());
        }
    }
    
    // Update player count
    CurrentPlayerCount = PlayerCharacters.Num();
    
    // Broadcast event
    OnPlayerLoggedOut.Broadcast(PlayerController);
}

void AMusicMMORPGGameState::OnPlayerSpawned(AMusicMMORPGCharacter* PlayerCharacter)
{
    if (!PlayerCharacter) return;
    
    // Add to player list if not already there
    if (!PlayerCharacters.Contains(PlayerCharacter))
    {
        PlayerCharacters.Add(PlayerCharacter);
        CurrentPlayerCount = PlayerCharacters.Num();
    }
    
    // Track zone
    if (AMusicMMORPGPlayerState* PlayerState = PlayerCharacter->GetMMORPGPlayerState())
    {
        FName Zone = PlayerState->GetCurrentZone();
        CharacterZoneMap.Add(PlayerCharacter, Zone);
        PlayerZoneMap.Add(PlayerState->GetPlayerName(), Zone);
    }
    
    // Broadcast event
    OnPlayerSpawned.Broadcast(PlayerCharacter);
}

void AMusicMMORPGGameState::OnPlayerDestroyed(AMusicMMORPGCharacter* PlayerCharacter)
{
    if (!PlayerCharacter) return;
    
    // Remove from player list
    PlayerCharacters.Remove(PlayerCharacter);
    CharacterZoneMap.Remove(PlayerCharacter);
    
    if (AMusicMMORPGPlayerState* PlayerState = PlayerCharacter->GetMMORPGPlayerState())
    {
        PlayerZoneMap.Remove(PlayerState->GetPlayerName());
    }
    
    // Update player count
    CurrentPlayerCount = PlayerCharacters.Num();
    
    // Broadcast event
    OnPlayerDestroyed.Broadcast(PlayerCharacter);
}

TArray<AMusicMMORPGPlayerState*> AMusicMMORPGGameState::GetAllPlayerStates() const
{
    TArray<AMusicMMORPGPlayerState*> PlayerStates;
    
    for (AMusicMMORPGCharacter* Character : PlayerCharacters)
    {
        if (AMusicMMORPGPlayerState* PlayerState = Character->GetMMORPGPlayerState())
        {
            PlayerStates.Add(PlayerState);
        }
    }
    
    return PlayerStates;
}

TArray<AMusicMMORPGCharacter*> AMusicMMORPGGameState::GetAllPlayerCharacters() const
{
    return PlayerCharacters;
}

AMusicMMORPGPlayerState* AMusicMMORPGGameState::GetPlayerStateByName(const FString& PlayerName) const
{
    for (AMusicMMORPGCharacter* Character : PlayerCharacters)
    {
        if (AMusicMMORPGPlayerState* PlayerState = Character->GetMMORPGPlayerState())
        {
            if (PlayerState->GetPlayerName() == PlayerName)
            {
                return PlayerState;
            }
        }
    }
    return nullptr;
}

AMusicMMORPGCharacter* AMusicMMORPGGameState::GetPlayerCharacterByName(const FString& PlayerName) const
{
    for (AMusicMMORPGCharacter* Character : PlayerCharacters)
    {
        if (AMusicMMORPGPlayerState* PlayerState = Character->GetMMORPGPlayerState())
        {
            if (PlayerState->GetPlayerName() == PlayerName)
            {
                return Character;
            }
        }
    }
    return nullptr;
}

AMusicMMORPGPlayerState* AMusicMMORPGGameState::GetPlayerStateByID(const FString& PlayerID) const
{
    // In a real implementation, this would use the unique player ID
    // For now, we'll just search by name
    return GetPlayerStateByName(PlayerID);
}

AMusicMMORPGCharacter* AMusicMMORPGGameState::GetPlayerCharacterByID(const FString& PlayerID) const
{
    // In a real implementation, this would use the unique player ID
    // For now, we'll just search by name
    return GetPlayerCharacterByName(PlayerID);
}

TArray<FName> AMusicMMORPGGameState::GetAvailableZones() const
{
    return AvailableZones;
}

TArray<AMusicMMORPGCharacter*> AMusicMMORPGGameState::GetPlayersInZone(const FName& ZoneName) const
{
    TArray<AMusicMMORPGCharacter*> PlayersInZone;
    
    for (const auto& Pair : CharacterZoneMap)
    {
        if (Pair.Value == ZoneName)
        {
            PlayersInZone.Add(Pair.Key);
        }
    }
    
    return PlayersInZone;
}

int32 AMusicMMORPGGameState::GetPlayerCountInZone(const FName& ZoneName) const
{
    int32 Count = 0;
    
    for (const auto& Pair : CharacterZoneMap)
    {
        if (Pair.Value == ZoneName)
        {
            Count++;
        }
    }
    
    return Count;
}

FName AMusicMMORPGGameState::GetZoneForPlayer(const AMusicMMORPGCharacter* PlayerCharacter) const
{
    if (const FName* Zone = CharacterZoneMap.Find(PlayerCharacter))
    {
        return *Zone;
    }
    return NAME_None;
}

TArray<AMMORPGBossCharacter*> AMusicMMORPGGameState::GetAllBosses() const
{
    return BossCharacters;
}

AMMORPGBossCharacter* AMusicMMORPGGameState::GetBossInZone(const FName& ZoneName) const
{
    if (const AMMORPGBossCharacter* Boss = ZoneBossMap.Find(ZoneName))
    {
        return *Boss;
    }
    return nullptr;
}

bool AMusicMMORPGGameState::IsBossActiveInZone(const FName& ZoneName) const
{
    return ZoneBossMap.Contains(ZoneName) && GetBossInZone(ZoneName) != nullptr;
}

int32 AMusicMMORPGGameState::GetTotalPlayerCount() const
{
    return CurrentPlayerCount;
}

int32 AMusicMMORPGGameState::GetMaxPlayers() const
{
    return MaxPlayers;
}

FString AMusicMMORPGGameState::GetServerName() const
{
    return ServerName;
}

FString AMusicMMORPGGameState::GetGameVersion() const
{
    return GameVersion;
}

void AMusicMMORPGGameState::AddBannedPlayer(const FString& PlayerID)
{
    if (!BannedPlayers.Contains(PlayerID))
    {
        BannedPlayers.Add(PlayerID);
    }
}

void AMusicMMORPGGameState::RemoveBannedPlayer(const FString& PlayerID)
{
    BannedPlayers.Remove(PlayerID);
}

bool AMusicMMORPGGameState::IsPlayerBanned(const FString& PlayerID) const
{
    return BannedPlayers.Contains(PlayerID);
}

TArray<FString> AMusicMMORPGGameState::GetBannedPlayers() const
{
    return BannedPlayers;
}

FDateTime AMusicMMORPGGameState::GetServerStartTime() const
{
    return ServerStartTime;
}

float AMusicMMORPGGameState::GetServerUptime() const
{
    FDateTime Now = FDateTime::Now();
    FTimespan Uptime = Now - ServerStartTime;
    return Uptime.GetTotalSeconds();
}

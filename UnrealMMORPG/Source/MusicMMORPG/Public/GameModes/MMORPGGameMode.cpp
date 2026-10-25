#include "MMORPGGameMode.h"
#include "MusicMMORPGPlayerController.h"
#include "MusicMMORPGCharacter.h"
#include "MusicMMORPGPlayerState.h"
#include "MusicMMORPGGameState.h"
#include "WorldData.h"
#include "ZoneManager.h"
#include "SocialSystem.h"
#include "QuestSystem.h"
#include "ShopSystem.h"
#include "CraftingSystem.h"
#include "SaveSystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

AMusicMMORPGGameMode::AMusicMMORPGGameMode()
{
    // Set default classes
    PlayerControllerClass = AMusicMMORPGPlayerController::StaticClass();
    DefaultPawnClass = AMusicMMORPGCharacter::StaticClass();
    PlayerStateClass = AMusicMMORPGPlayerState::StaticClass();
    GameStateClass = AMusicMMORPGGameState::StaticClass();
    
    // Set default spawn tags
    SpawnTags.Add(NAME_PlayerStart);
    
    // Set default zones
    AvailableZones.Add(NAME_SoundCity);
    AvailableZones.Add(NAME_BassCaverns);
    AvailableZones.Add(NAME_SynthCitadel);
    
    // Initialize pointers
    WorldData = nullptr;
    ZoneManager = nullptr;
    SocialSystem = nullptr;
    QuestSystem = nullptr;
    ShopSystem = nullptr;
    CraftingSystem = nullptr;
    SaveSystem = nullptr;
}

void AMusicMMORPGGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize game systems
    InitializeGameSystems();
    
    // Start auto-save timer
    if (bAutoSaveEnabled && AutoSaveInterval > 0.0f)
    {
        GetWorldTimerManager().SetTimer(AutoSaveTimerHandle, this, &AMusicMMORPGGameMode::OnAutoSaveTimer, AutoSaveInterval, true);
    }
}

void AMusicMMORPGGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMusicMMORPGGameMode::StartPlay()
{
    Super::StartPlay();
}

void AMusicMMORPGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Clear auto-save timer
    if (AutoSaveTimerHandle.IsValid())
    {
        GetWorldTimerManager().ClearTimer(AutoSaveTimerHandle);
    }
    
    Super::EndPlay(EndPlayReason);
}

void AMusicMMORPGGameMode::SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot)
{
    if (AMusicMMORPGPlayerController* PlayerController = Cast<AMusicMMORPGPlayerController>(NewPlayer))
    {
        SpawnDefaultPawn(PlayerController);
    }
    else
    {
        Super::SpawnDefaultPawnFor(NewPlayer, StartSpot);
    }
}

void AMusicMMORPGGameMode::RestartPlayer(AController* NewPlayer)
{
    if (AMusicMMORPGPlayerController* PlayerController = Cast<AMusicMMORPGPlayerController>(NewPlayer))
    {
        SpawnDefaultPawn(PlayerController);
    }
    else
    {
        Super::RestartPlayer(NewPlayer);
    }
}

void AMusicMMORPGGameMode::RestartPlayerAtPlayerStart(AController* NewPlayer, const FString& InStartTag)
{
    if (AMusicMMORPGPlayerController* PlayerController = Cast<AMusicMMORPGPlayerController>(NewPlayer))
    {
        SpawnDefaultPawn(PlayerController);
    }
    else
    {
        Super::RestartPlayerAtPlayerStart(NewPlayer, InStartTag);
    }
}

void AMusicMMORPGGameMode::RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform)
{
    if (AMusicMMORPGPlayerController* PlayerController = Cast<AMusicMMORPGPlayerController>(NewPlayer))
    {
        SpawnDefaultPawn(PlayerController);
    }
    else
    {
        Super::RestartPlayerAtTransform(NewPlayer, SpawnTransform);
    }
}

void AMusicMMORPGGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    
    // Initialize player
    if (AMusicMMORPGPlayerController* PlayerController = Cast<AMusicMMORPGPlayerController>(NewPlayer))
    {
        // Set player state
        if (AMusicMMORPGPlayerState* PlayerState = PlayerController->GetPlayerState<AMusicMMORPGPlayerState>())
        {
            PlayerState->InitializePlayer();
        }
        
        // Spawn pawn
        SpawnDefaultPawn(PlayerController);
        
        // Notify game state
        if (AMusicMMORPGGameState* GameState = GetMMORPGGameState())
        {
            GameState->OnPlayerLoggedIn(PlayerController);
        }
    }
}

void AMusicMMORPGGameMode::Logout(AController* Exiting)
{
    // Notify game state
    if (AMusicMMORPGGameState* GameState = GetMMORPGGameState())
    {
        if (AMusicMMORPGPlayerController* PlayerController = Cast<AMusicMMORPGPlayerController>(Exiting))
        {
            GameState->OnPlayerLoggedOut(PlayerController);
        }
    }
    
    Super::Logout(Exiting);
}

void AMusicMMORPGGameMode::PreLogout(AController* Exiting)
{
    // Save player data before logout
    if (AMusicMMORPGPlayerController* PlayerController = Cast<AMusicMMORPGPlayerController>(Exiting))
    {
        if (SaveSystem)
        {
            SaveSystem->SavePlayerData(PlayerController);
        }
    }
    
    Super::PreLogout(Exiting);
}

void AMusicMMORPGGameMode::InitGame(const FString& MapName, const FString& Options)
{
    Super::InitGame(MapName, Options);
}

void AMusicMMORPGGameMode::InitGameState()
{
    Super::InitGameState();
    
    // Create game state if it doesn't exist
    CreateGameState();
}

void AMusicMMORPGGameMode::HandleMatchHasStarted()
{
    Super::HandleMatchHasStarted();
}

void AMusicMMORPGGameMode::HandleMatchHasEnded()
{
    Super::HandleMatchHasEnded();
}

void AMusicMMORPGGameMode::PreInitializeComponents()
{
    Super::PreInitializeComponents();
}

void AMusicMMORPGGameMode::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

void AMusicMMORPGGameMode::CreateGameState()
{
    if (!GameState)
    {
        GameState = GetWorld()->SpawnActor<AMusicMMORPGGameState>();
    }
}

AMusicMMORPGGameState* AMusicMMORPGGameMode::GetMMORPGGameState() const
{
    return Cast<AMusicMMORPGGameState>(GameState);
}

void AMusicMMORPGGameMode::KickPlayer(APlayerController* PlayerController, const FText& Reason)
{
    if (HasAuthority())
    {
        ServerKickPlayer(PlayerController, Reason);
    }
    else
    {
        // Request server to kick player
        AMusicMMORPGPlayerController* MMOPlayerController = Cast<AMusicMMORPGPlayerController>(PlayerController);
        if (MMOPlayerController)
        {
            MMOPlayerController->ServerRequestKick(PlayerController, Reason);
        }
    }
}

void AMusicMMORPGGameMode::BanPlayer(APlayerController* PlayerController, const FText& Reason)
{
    if (HasAuthority())
    {
        ServerBanPlayer(PlayerController, Reason);
    }
    else
    {
        // Request server to ban player
        AMusicMMORPGPlayerController* MMOPlayerController = Cast<AMusicMMORPGPlayerController>(PlayerController);
        if (MMOPlayerController)
        {
            MMOPlayerController->ServerRequestBan(PlayerController, Reason);
        }
    }
}

void AMusicMMORPGGameMode::MutePlayer(APlayerController* PlayerController, bool bMute)
{
    // Implementation for muting players
}

void AMusicMMORPGGameMode::SendGlobalMessage(const FText& Message, APlayerController* Sender)
{
    if (HasAuthority())
    {
        ServerSendGlobalMessage(Message, Sender);
    }
    else
    {
        AMusicMMORPGPlayerController* MMOPlayerController = Cast<AMusicMMORPGPlayerController>(Sender);
        if (MMOPlayerController)
        {
            MMOPlayerController->ServerSendGlobalMessage(Message);
        }
    }
}

void AMusicMMORPGGameMode::ChangePlayerZone(AMusicMMORPGCharacter* PlayerCharacter, const FName& ZoneName)
{
    if (HasAuthority())
    {
        ServerChangePlayerZone(PlayerCharacter, ZoneName);
    }
    else
    {
        AMusicMMORPGPlayerController* PlayerController = PlayerCharacter ? Cast<AMusicMMORPGPlayerController>(PlayerCharacter->GetController()) : nullptr;
        if (PlayerController)
        {
            PlayerController->ServerChangeZone(ZoneName);
        }
    }
}

void AMusicMMORPGGameMode::TeleportPlayerToZone(AMusicMMORPGCharacter* PlayerCharacter, const FName& ZoneName, const FVector& Location, const FRotator& Rotation)
{
    if (HasAuthority())
    {
        ServerTeleportPlayerToZone(PlayerCharacter, ZoneName, Location, Rotation);
    }
    else
    {
        AMusicMMORPGPlayerController* PlayerController = PlayerCharacter ? Cast<AMusicMMORPGPlayerController>(PlayerCharacter->GetController()) : nullptr;
        if (PlayerController)
        {
            PlayerController->ServerTeleportToZone(ZoneName, Location, Rotation);
        }
    }
}

void AMusicMMORPGGameMode::SaveGame()
{
    if (SaveSystem)
    {
        SaveSystem->SaveGame();
    }
}

void AMusicMMORPGGameMode::LoadGame()
{
    if (SaveSystem)
    {
        SaveSystem->LoadGame();
    }
}

void AMusicMMORPGGameMode::AutoSave()
{
    if (SaveSystem)
    {
        SaveSystem->AutoSave();
    }
}

void AMusicMMORPGGameMode::ServerKickPlayer_Implementation(APlayerController* PlayerController, const FText& Reason)
{
    if (PlayerController)
    {
        // Notify player
        if (AMusicMMORPGPlayerController* MMOPlayerController = Cast<AMusicMMORPGPlayerController>(PlayerController))
        {
            MMOPlayerController->ClientKickNotification(Reason);
        }
        
        // Kick player
        GetWorld()->GetAuthGameMode()->KickPlayer(PlayerController, Reason);
    }
}

bool AMusicMMORPGGameMode::ServerKickPlayer_Validate(APlayerController* PlayerController, const FText& Reason)
{
    return PlayerController != nullptr;
}

void AMusicMMORPGGameMode::ServerBanPlayer_Implementation(APlayerController* PlayerController, const FText& Reason)
{
    if (PlayerController)
    {
        // Add to ban list
        if (AMusicMMORPGGameState* GameState = GetMMORPGGameState())
        {
            GameState->AddBannedPlayer(PlayerController->GetUniqueId());
        }
        
        // Kick player
        ServerKickPlayer(PlayerController, Reason);
    }
}

bool AMusicMMORPGGameMode::ServerBanPlayer_Validate(APlayerController* PlayerController, const FText& Reason)
{
    return PlayerController != nullptr;
}

void AMusicMMORPGGameMode::ServerSendGlobalMessage_Implementation(const FText& Message, APlayerController* Sender)
{
    // Broadcast message to all players
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        APlayerController* PlayerController = Iterator->Get();
        if (PlayerController)
        {
            if (AMusicMMORPGPlayerController* MMOPlayerController = Cast<AMusicMMORPGPlayerController>(PlayerController))
            {
                MMOPlayerController->ClientReceiveGlobalMessage(Message, Sender);
            }
        }
    }
}

void AMusicMMORPGGameMode::ServerChangePlayerZone_Implementation(AMusicMMORPGCharacter* PlayerCharacter, const FName& ZoneName)
{
    if (PlayerCharacter && ZoneManager)
    {
        ZoneManager->ChangePlayerZone(PlayerCharacter, ZoneName);
    }
}

bool AMusicMMORPGGameMode::ServerChangePlayerZone_Validate(AMusicMMORPGCharacter* PlayerCharacter, const FName& ZoneName)
{
    return PlayerCharacter != nullptr && ZoneManager != nullptr;
}

void AMusicMMORPGGameMode::ServerTeleportPlayerToZone_Implementation(AMusicMMORPGCharacter* PlayerCharacter, const FName& ZoneName, const FVector& Location, const FRotator& Rotation)
{
    if (PlayerCharacter && ZoneManager)
    {
        ZoneManager->TeleportPlayerToZone(PlayerCharacter, ZoneName, Location, Rotation);
    }
}

bool AMusicMMORPGGameMode::ServerTeleportPlayerToZone_Validate(AMusicMMORPGCharacter* PlayerCharacter, const FName& ZoneName, const FVector& Location, const FRotator& Rotation)
{
    return PlayerCharacter != nullptr && ZoneManager != nullptr;
}

AMusicMMORPGGameMode* AMusicMMORPGGameMode::GetInstance(const UWorld* World)
{
    if (World)
    {
        return Cast<AMusicMMORPGGameMode>(World->GetAuthGameMode());
    }
    return nullptr;
}

void AMusicMMORPGGameMode::OnAutoSaveTimer()
{
    AutoSave();
}

void AMusicMMORPGGameMode::InitializeGameSystems()
{
    // Initialize world data
    InitializeWorldData();
    
    // Initialize zone manager
    InitializeZoneManager();
    
    // Initialize social system
    InitializeSocialSystem();
    
    // Initialize quest system
    InitializeQuestSystem();
    
    // Initialize shop system
    InitializeShopSystem();
    
    // Initialize crafting system
    InitializeCraftingSystem();
    
    // Initialize save system
    InitializeSaveSystem();
}

void AMusicMMORPGGameMode::InitializeWorldData()
{
    if (!WorldData)
    {
        WorldData = NewObject<UWorldData>(this, TEXT("WorldData"));
        if (WorldData)
        {
            WorldData->Initialize();
        }
    }
}

void AMusicMMORPGGameMode::InitializeZoneManager()
{
    if (!ZoneManager)
    {
        ZoneManager = NewObject<UZoneManager>(this, TEXT("ZoneManager"));
        if (ZoneManager)
        {
            ZoneManager->Initialize();
        }
    }
}

void AMusicMMORPGGameMode::InitializeSocialSystem()
{
    if (!SocialSystem)
    {
        SocialSystem = NewObject<USocialSystem>(this, TEXT("SocialSystem"));
        if (SocialSystem)
        {
            SocialSystem->Initialize();
        }
    }
}

void AMusicMMORPGGameMode::InitializeQuestSystem()
{
    if (!QuestSystem)
    {
        QuestSystem = NewObject<UQuestSystem>(this, TEXT("QuestSystem"));
        if (QuestSystem)
        {
            QuestSystem->Initialize();
        }
    }
}

void AMusicMMORPGGameMode::InitializeShopSystem()
{
    if (!ShopSystem)
    {
        ShopSystem = NewObject<UShopSystem>(this, TEXT("ShopSystem"));
        if (ShopSystem)
        {
            ShopSystem->Initialize();
        }
    }
}

void AMusicMMORPGGameMode::InitializeCraftingSystem()
{
    if (!CraftingSystem)
    {
        CraftingSystem = NewObject<UCraftingSystem>(this, TEXT("CraftingSystem"));
        if (CraftingSystem)
        {
            CraftingSystem->Initialize();
        }
    }
}

void AMusicMMORPGGameMode::InitializeSaveSystem()
{
    if (!SaveSystem)
    {
        SaveSystem = NewObject<USaveSystem>(this, TEXT("SaveSystem"));
        if (SaveSystem)
        {
            SaveSystem->Initialize();
        }
    }
}

void AMusicMMORPGGameMode::SpawnDefaultPawn(AMusicMMORPGPlayerController* PlayerController)
{
    if (!PlayerController || !DefaultPawnClass)
    {
        return;
    }
    
    // Find spawn transform
    FTransform SpawnTransform = FindSpawnTransform(PlayerController);
    
    // Spawn pawn
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    SpawnParams.Owner = PlayerController;
    SpawnParams.Instigator = PlayerController->GetPawn();
    
    AMusicMMORPGCharacter* NewPawn = GetWorld()->SpawnActor<AMusicMMORPGCharacter>(
        DefaultPawnClass,
        SpawnTransform.GetLocation(),
        SpawnTransform.GetRotation().Rotator(),
        SpawnParams
    );
    
    if (NewPawn)
    {
        // Possess pawn
        PlayerController->Possess(NewPawn);
        
        // Initialize pawn
        NewPawn->InitializePlayer();
        
        // Set player state
        if (AMusicMMORPGPlayerState* PlayerState = PlayerController->GetPlayerState<AMusicMMORPGPlayerState>())
        {
            NewPawn->SetPlayerState(PlayerState);
            PlayerState->SetPawn(NewPawn);
        }
    }
}

FTransform AMusicMMORPGGameMode::FindSpawnTransform(AController* Player, const FString& InStartTag)
{
    // Try to find a player start with the specified tag
    if (!InStartTag.IsEmpty())
    {
        for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
        {
            APlayerStart* PlayerStart = *It;
            if (PlayerStart && PlayerStart->PlayerStartTag == InStartTag)
            {
                return PlayerStart->GetActorTransform();
            }
        }
    }
    
    // Try to find any player start
    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        APlayerStart* PlayerStart = *It;
        if (PlayerStart)
        {
            return PlayerStart->GetActorTransform();
        }
    }
    
    // Default spawn at origin
    return FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
}

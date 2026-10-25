#include "MusicMMORPGPlayerController.h"
#include "MusicMMORPGCharacter.h"
#include "MusicMMORPGPlayerState.h"
#include "MusicMMORPGGameMode.h"
#include "MusicMMORPGGameState.h"
#include "MMORPGInputComponent.h"
#include "MMORPGHUD.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

AMusicMMORPGPlayerController::AMusicMMORPGPlayerController()
{
    // Set default values
    bIsReady = false;
    PlayerName = TEXT("Player");
    CurrentZone = NAME_SoundCity;
    
    // Enable replication
    bReplicates = true;
    
    // Set input mode
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void AMusicMMORPGPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize player
    InitializePlayer();
}

void AMusicMMORPGPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Update rotation
    UpdateRotation(DeltaTime);
}

void AMusicMMORPGPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
}

void AMusicMMORPGPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    // Create MMO input component
    if (!MMOInputComponent)
    {
        MMOInputComponent = NewObject<UMMORPGInputComponent>(this, TEXT("MMOInputComponent"));
    }
    
    // Setup input bindings
    SetupInputBindings();
    BindActionEvents();
}

void AMusicMMORPGPlayerController::Possess(APawn* InPawn)
{
    Super::Possess(InPawn);
    
    // Initialize HUD
    ShowHUD();
    
    // Initialize player
    if (AMusicMMORPGCharacter* Character = Cast<AMusicMMORPGCharacter>(InPawn))
    {
        Character->InitializePlayer();
    }
}

void AMusicMMORPGPlayerController::UnPossess()
{
    // Hide HUD
    HideHUD();
    
    Super::UnPossess();
}

void AMusicMMORPGPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    // Replicate these properties
    DOREPLIFETIME(AMusicMMORPGPlayerController, bIsReady);
    DOREPLIFETIME(AMusicMMORPGPlayerController, PlayerName);
    DOREPLIFETIME(AMusicMMORPGPlayerController, CurrentZone);
}

void AMusicMMORPGPlayerController::InitializePlayer()
{
    // Set player name from player state
    if (AMusicMMORPGPlayerState* PlayerState = GetMMORPGPlayerState())
    {
        PlayerName = PlayerState->GetPlayerName();
    }
    
    // Set ready state
    bIsReady = true;
    
    // Initialize HUD
    ShowHUD();
}

void AMusicMMORPGPlayerController::ShowHUD()
{
    if (!MMOHUD && HUDClass)
    {
        MMOHUD = CreateWidget<UMMORPGHUD>(this, HUDClass);
        if (MMOHUD)
        {
            MMOHUD->AddToViewport();
            MMOHUD->InitializeHUD(this);
        }
    }
}

void AMusicMMORPGPlayerController::HideHUD()
{
    if (MMOHUD)
    {
        MMOHUD->RemoveFromViewport();
        MMOHUD = nullptr;
    }
}

void AMusicMMORPGPlayerController::ToggleHUD()
{
    if (MMOHUD)
    {
        HideHUD();
    }
    else
    {
        ShowHUD();
    }
}

void AMusicMMORPGPlayerController::SetupInputBindings()
{
    if (!InputComponent) return;
    
    // Movement
    InputComponent->BindAxis("MoveForward", this, &AMusicMMORPGPlayerController::OnMoveForward);
    InputComponent->BindAxis("MoveRight", this, &AMusicMMORPGPlayerController::OnMoveRight);
    InputComponent->BindAxis("LookUp", this, &AMusicMMORPGPlayerController::OnLookUp);
    InputComponent->BindAxis("LookRight", this, &AMusicMMORPGPlayerController::OnLookRight);
    
    // Actions
    InputComponent->BindAction("Jump", IE_Pressed, this, &AMusicMMORPGPlayerController::OnJumpStart);
    InputComponent->BindAction("Jump", IE_Released, this, &AMusicMMORPGPlayerController::OnJumpStop);
    InputComponent->BindAction("Run", IE_Pressed, this, &AMusicMMORPGPlayerController::OnRunStart);
    InputComponent->BindAction("Run", IE_Released, this, &AMusicMMORPGPlayerController::OnRunStop);
    InputComponent->BindAction("Sprint", IE_Pressed, this, &AMusicMMORPGPlayerController::OnSprintStart);
    InputComponent->BindAction("Sprint", IE_Released, this, &AMusicMMORPGPlayerController::OnSprintStop);
    InputComponent->BindAction("Crouch", IE_Pressed, this, &AMusicMMORPGPlayerController::OnCrouchStart);
    InputComponent->BindAction("Crouch", IE_Released, this, &AMusicMMORPGPlayerController::OnCrouchStop);
    
    // Combat
    InputComponent->BindAction("Attack", IE_Pressed, this, &AMusicMMORPGPlayerController::OnAttack);
    InputComponent->BindAction("Ability1", IE_Pressed, this, &AMusicMMORPGPlayerController::OnAbility1);
    InputComponent->BindAction("Ability2", IE_Pressed, this, &AMusicMMORPGPlayerController::OnAbility2);
    InputComponent->BindAction("Ability3", IE_Pressed, this, &AMusicMMORPGPlayerController::OnAbility3);
    InputComponent->BindAction("Ability4", IE_Pressed, this, &AMusicMMORPGPlayerController::OnAbility4);
    
    // UI
    InputComponent->BindAction("Interact", IE_Pressed, this, &AMusicMMORPGPlayerController::OnInteract);
    InputComponent->BindAction("Inventory", IE_Pressed, this, &AMusicMMORPGPlayerController::OnInventory);
    InputComponent->BindAction("Map", IE_Pressed, this, &AMusicMMORPGPlayerController::OnMap);
    InputComponent->BindAction("Menu", IE_Pressed, this, &AMusicMMORPGPlayerController::OnMenu);
    InputComponent->BindAction("Chat", IE_Pressed, this, &AMusicMMORPGPlayerController::OnChat);
    InputComponent->BindAction("Social", IE_Pressed, this, &AMusicMMORPGPlayerController::OnSocial);
    InputComponent->BindAction("Quest", IE_Pressed, this, &AMusicMMORPGPlayerController::OnQuest);
    InputComponent->BindAction("Shop", IE_Pressed, this, &AMusicMMORPGPlayerController::OnShop);
}

void AMusicMMORPGPlayerController::BindActionEvents()
{
    // Bind MMO input component events
    if (MMOInputComponent)
    {
        MMOInputComponent->OnMoveForward.AddDynamic(this, &AMusicMMORPGPlayerController::OnMoveForward);
        MMOInputComponent->OnMoveRight.AddDynamic(this, &AMusicMMORPGPlayerController::OnMoveRight);
        MMOInputComponent->OnLookUp.AddDynamic(this, &AMusicMMORPGPlayerController::OnLookUp);
        MMOInputComponent->OnLookRight.AddDynamic(this, &AMusicMMORPGPlayerController::OnLookRight);
        MMOInputComponent->OnJump.AddDynamic(this, &AMusicMMORPGPlayerController::OnJumpStart);
        MMOInputComponent->OnAttack.AddDynamic(this, &AMusicMMORPGPlayerController::OnAttack);
    }
}

void AMusicMMORPGPlayerController::ServerRequestSpawn_Implementation(const FTransform& SpawnTransform)
{
    // Request spawn from game mode
    if (AMusicMMORPGGameMode* GameMode = GetMMORPGGameMode())
    {
        GameMode->SpawnDefaultPawn(this);
    }
}

bool AMusicMMORPGPlayerController::ServerRequestSpawn_Validate(const FTransform& SpawnTransform)
{
    return true;
}

void AMusicMMORPGPlayerController::ServerRequestKick_Implementation(APlayerController* PlayerController, const FText& Reason)
{
    if (AMusicMMORPGGameMode* GameMode = GetMMORPGGameMode())
    {
        GameMode->KickPlayer(PlayerController, Reason);
    }
}

bool AMusicMMORPGPlayerController::ServerRequestKick_Validate(APlayerController* PlayerController, const FText& Reason)
{
    return PlayerController != nullptr;
}

void AMusicMMORPGPlayerController::ServerRequestBan_Implementation(APlayerController* PlayerController, const FText& Reason)
{
    if (AMusicMMORPGGameMode* GameMode = GetMMORPGGameMode())
    {
        GameMode->BanPlayer(PlayerController, Reason);
    }
}

bool AMusicMMORPGPlayerController::ServerRequestBan_Validate(APlayerController* PlayerController, const FText& Reason)
{
    return PlayerController != nullptr;
}

void AMusicMMORPGPlayerController::ServerSendGlobalMessage_Implementation(const FText& Message)
{
    if (AMusicMMORPGGameMode* GameMode = GetMMORPGGameMode())
    {
        GameMode->SendGlobalMessage(Message, this);
    }
}

void AMusicMMORPGPlayerController::ServerChangeZone_Implementation(const FName& ZoneName)
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        if (AMusicMMORPGGameMode* GameMode = GetMMORPGGameMode())
        {
            GameMode->ChangePlayerZone(Character, ZoneName);
        }
    }
}

bool AMusicMMORPGPlayerController::ServerChangeZone_Validate(const FName& ZoneName)
{
    return true;
}

void AMusicMMORPGPlayerController::ServerTeleportToZone_Implementation(const FName& ZoneName, const FVector& Location, const FRotator& Rotation)
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        if (AMusicMMORPGGameMode* GameMode = GetMMORPGGameMode())
        {
            GameMode->TeleportPlayerToZone(Character, ZoneName, Location, Rotation);
        }
    }
}

bool AMusicMMORPGPlayerController::ServerTeleportToZone_Validate(const FName& ZoneName, const FVector& Location, const FRotator& Rotation)
{
    return true;
}

void AMusicMMORPGPlayerController::ServerUseAbility_Implementation(int32 AbilityIndex)
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->UseAbility(AbilityIndex);
    }
}

bool AMusicMMORPGPlayerController::ServerUseAbility_Validate(int32 AbilityIndex)
{
    return AbilityIndex >= 0 && AbilityIndex < 4;
}

void AMusicMMORPGPlayerController::ServerInteract_Implementation(AActor* TargetActor)
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->SetTarget(TargetActor);
    }
}

bool AMusicMMORPGPlayerController::ServerInteract_Validate(AActor* TargetActor)
{
    return TargetActor != nullptr;
}

void AMusicMMORPGPlayerController::ClientKickNotification_Implementation(const FText& Reason)
{
    // Show kick notification
    ClientShowNotification(FText::Format(NSLOCTEXT("MMORPG", "KickNotification", "You have been kicked: {0}"), Reason));
    
    // Return to main menu or disconnect
    if (GetWorld())
    {
        GetWorld()->GetFirstPlayerController()->ConsoleCommand("Disconnect");
    }
}

void AMusicMMORPGPlayerController::ClientReceiveGlobalMessage_Implementation(const FText& Message, APlayerController* Sender)
{
    // Display global message
    if (MMOHUD)
    {
        MMOHUD->AddChatMessage(Message, Sender ? Sender->GetName() : TEXT("Server"));
    }
}

void AMusicMMORPGPlayerController::ClientChangeZone_Implementation(const FName& ZoneName)
{
    CurrentZone = ZoneName;
    
    // Notify HUD
    if (MMOHUD)
    {
        MMOHUD->OnZoneChanged(ZoneName);
    }
}

void AMusicMMORPGPlayerController::ClientTeleportToZone_Implementation(const FName& ZoneName, const FVector& Location, const FRotator& Rotation)
{
    CurrentZone = ZoneName;
    
    // Teleport player
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->SetActorLocationAndRotation(Location, Rotation);
    }
    
    // Notify HUD
    if (MMOHUD)
    {
        MMOHUD->OnZoneChanged(ZoneName);
    }
}

void AMusicMMORPGPlayerController::ClientShowNotification_Implementation(const FText& Message)
{
    // Show notification in HUD
    if (MMOHUD)
    {
        MMOHUD->ShowNotification(Message);
    }
}

AMusicMMORPGPlayerState* AMusicMMORPGPlayerController::GetMMORPGPlayerState() const
{
    return Cast<AMusicMMORPGPlayerState>(PlayerState);
}

AMusicMMORPGCharacter* AMusicMMORPGPlayerController::GetMMORPGCharacter() const
{
    return Cast<AMusicMMORPGCharacter>(GetPawn());
}

AMusicMMORPGGameMode* AMusicMMORPGPlayerController::GetMMORPGGameMode() const
{
    return Cast<AMusicMMORPGGameMode>(GetWorld()->GetAuthGameMode());
}

AMusicMMORPGGameState* AMusicMMORPGPlayerController::GetMMORPGGameState() const
{
    return Cast<AMusicMMORPGGameState>(GetWorld()->GetGameState());
}

bool AMusicMMORPGPlayerController::IsLocalPlayerController() const
{
    return IsLocalController();
}

bool AMusicMMORPGPlayerController::IsServer() const
{
    return GetWorld() && GetWorld()->GetNetMode() == NM_DedicatedServer;
}

bool AMusicMMORPGPlayerController::IsClient() const
{
    return GetWorld() && GetWorld()->GetNetMode() != NM_DedicatedServer;
}

AMusicMMORPGPlayerController* AMusicMMORPGPlayerController::GetInstance(const UWorld* World)
{
    if (World)
    {
        for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
        {
            APlayerController* PlayerController = Iterator->Get();
            if (AMusicMMORPGPlayerController* MMOPlayerController = Cast<AMusicMMORPGPlayerController>(PlayerController))
            {
                return MMOPlayerController;
            }
        }
    }
    return nullptr;
}

void AMusicMMORPGPlayerController::OnRep_PlayerName()
{
    // Update HUD with new player name
    if (MMOHUD)
    {
        MMOHUD->UpdatePlayerName(PlayerName);
    }
}

void AMusicMMORPGPlayerController::OnRep_CurrentZone()
{
    // Update HUD with new zone
    if (MMOHUD)
    {
        MMOHUD->OnZoneChanged(CurrentZone);
    }
}

void AMusicMMORPGPlayerController::OnRep_bIsReady()
{
    // Handle ready state change
}

void AMusicMMORPGPlayerController::OnMoveForward(float Value)
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->Move(FVector2D(Value, 0.0f));
    }
}

void AMusicMMORPGPlayerController::OnMoveRight(float Value)
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->Move(FVector2D(0.0f, Value));
    }
}

void AMusicMMORPGPlayerController::OnLookUp(float Value)
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->Look(FVector2D(Value, 0.0f));
    }
}

void AMusicMMORPGPlayerController::OnLookRight(float Value)
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->Look(FVector2D(0.0f, Value));
    }
}

void AMusicMMORPGPlayerController::OnJumpStart()
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->Jump();
    }
}

void AMusicMMORPGPlayerController::OnJumpStop()
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->StopJumping();
    }
}

void AMusicMMORPGPlayerController::OnRunStart()
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->StartRunning();
    }
}

void AMusicMMORPGPlayerController::OnRunStop()
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->StopRunning();
    }
}

void AMusicMMORPGPlayerController::OnSprintStart()
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->StartSprinting();
    }
}

void AMusicMMORPGPlayerController::OnSprintStop()
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->StopSprinting();
    }
}

void AMusicMMORPGPlayerController::OnCrouchStart()
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->StartCrouching();
    }
}

void AMusicMMORPGPlayerController::OnCrouchStop()
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->StopCrouching();
    }
}

void AMusicMMORPGPlayerController::OnAttack()
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->Attack();
    }
}

void AMusicMMORPGPlayerController::OnAbility1()
{
    ServerUseAbility(0);
}

void AMusicMMORPGPlayerController::OnAbility2()
{
    ServerUseAbility(1);
}

void AMusicMMORPGPlayerController::OnAbility3()
{
    ServerUseAbility(2);
}

void AMusicMMORPGPlayerController::OnAbility4()
{
    ServerUseAbility(3);
}

void AMusicMMORPGPlayerController::OnInteract()
{
    // Find interactable actor
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(Character);
        
        FVector Start = Character->GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);
        FVector End = Start + (Character->GetActorForwardVector() * 500.0f);
        
        if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
        {
            ServerInteract(HitResult.GetActor());
        }
    }
}

void AMusicMMORPGPlayerController::OnInventory()
{
    if (MMOHUD)
    {
        MMOHUD->ToggleInventory();
    }
}

void AMusicMMORPGPlayerController::OnMap()
{
    if (MMOHUD)
    {
        MMOHUD->ToggleMap();
    }
}

void AMusicMMORPGPlayerController::OnMenu()
{
    if (MMOHUD)
    {
        MMOHUD->ToggleMenu();
    }
}

void AMusicMMORPGPlayerController::OnChat()
{
    if (MMOHUD)
    {
        MMOHUD->ToggleChat();
    }
}

void AMusicMMORPGPlayerController::OnSocial()
{
    if (MMOHUD)
    {
        MMOHUD->ToggleSocial();
    }
}

void AMusicMMORPGPlayerController::OnQuest()
{
    if (MMOHUD)
    {
        MMOHUD->ToggleQuest();
    }
}

void AMusicMMORPGPlayerController::OnShop()
{
    if (MMOHUD)
    {
        MMOHUD->ToggleShop();
    }
}

void AMusicMMORPGPlayerController::UpdateRotation(float DeltaTime)
{
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        // Update character rotation based on input
        FVector2D LookInput = Character->LookInput;
        if (!LookInput.IsNearlyZero())
        {
            FRotator NewRotation = Character->GetActorRotation();
            NewRotation.Yaw += LookInput.X * DeltaTime * 100.0f;
            NewRotation.Pitch += LookInput.Y * DeltaTime * 100.0f;
            Character->SetActorRotation(NewRotation);
        }
    }
}

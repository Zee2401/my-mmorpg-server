#include "MusicMMORPGCharacter.h"
#include "MusicMMORPGPlayerController.h"
#include "MusicMMORPGPlayerState.h"
#include "MusicMMORPGGameMode.h"
#include "MusicMMORPGGameState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArm.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AMusicMMORPGCharacter::AMusicMMORPGCharacter()
{
    // Set this character to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;
    
    // Enable replication
    bReplicates = true;
    SetReplicateMovement(true);
    
    // Create and setup components
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->SetUsingAbsoluteRotation(true);
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->TargetArmLength = 800.0f;
    SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
    SpringArm->bEnableCameraLag = true;
    SpringArm->CameraLagSpeed = 3.0f;
    
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;
    
    // Set default movement settings
    WalkSpeed = 200.0f;
    RunSpeed = 400.0f;
    SprintSpeed = 600.0f;
    RotationRate = 1.0f;
    JumpVelocity = 600.0f;
    
    // Set default character info
    PlayerName = TEXT("Player");
    CharacterClass = ECharacterClass::Bard;
    Level = 1;
    Experience = 0;
    HarmonyGems = 0;
    CurrentZone = NAME_SoundCity;
    
    // Set default state
    CurrentState = EMMORPGCharacterState::Idle;
    CurrentMovementMode = EMMORPGMovementMode::Walking;
    bIsRunning = false;
    bIsSprinting = false;
    bIsCrouching = false;
    bIsDead = false;
    
    // Set default targeting
    CurrentTarget = nullptr;
    CurrentBossTarget = nullptr;
    
    // Set default input
    MovementInput = FVector2D::ZeroVector;
    LookInput = FVector2D::ZeroVector;
    bJumpInput = false;
    bAttackInput = false;
    bRunInput = false;
    bSprintInput = false;
    bInteractInput = false;
    bInventoryInput = false;
    bMapInput = false;
    bMenuInput = false;
    
    // Initialize ability inputs
    AbilityInputs.Init(false, 4);
}

void AMusicMMORPGCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize components
    InitializeComponents();
    
    // Update character based on class
    UpdateCharacterClass();
    
    // Set movement settings
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->JumpZVelocity = JumpVelocity;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, RotationRate, 0.0f);
}

void AMusicMMORPGCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMusicMMORPGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMusicMMORPGCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    
    // Initialize player
    InitializePlayer();
}

void AMusicMMORPGCharacter::UnPossess()
{
    Super::UnPossess();
}

void AMusicMMORPGCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    // Replicate these properties
    DOREPLIFETIME(AMusicMMORPGCharacter, PlayerName);
    DOREPLIFETIME(AMusicMMORPGCharacter, CharacterClass);
    DOREPLIFETIME(AMusicMMORPGCharacter, Level);
    DOREPLIFETIME(AMusicMMORPGCharacter, Experience);
    DOREPLIFETIME(AMusicMMORPGCharacter, HarmonyGems);
    DOREPLIFETIME(AMusicMMORPGCharacter, CurrentZone);
    DOREPLIFETIME(AMusicMMORPGCharacter, CurrentState);
    DOREPLIFETIME(AMusicMMORPGCharacter, CurrentMovementMode);
    DOREPLIFETIME(AMusicMMORPGCharacter, bIsRunning);
    DOREPLIFETIME(AMusicMMORPGCharacter, bIsSprinting);
    DOREPLIFETIME(AMusicMMORPGCharacter, bIsCrouching);
    DOREPLIFETIME(AMusicMMORPGCharacter, bIsDead);
    DOREPLIFETIME(AMusicMMORPGCharacter, CurrentTarget);
    DOREPLIFETIME(AMusicMMORPGCharacter, CurrentBossTarget);
    DOREPLIFETIME(AMusicMMORPGCharacter, WalkSpeed);
    DOREPLIFETIME(AMusicMMORPGCharacter, RunSpeed);
    DOREPLIFETIME(AMusicMMORPGCharacter, SprintSpeed);
    DOREPLIFETIME(AMusicMMORPGCharacter, RotationRate);
    DOREPLIFETIME(AMusicMMORPGCharacter, JumpVelocity);
}

void AMusicMMORPGCharacter::InitializePlayer()
{
    // Set player name from player state
    if (AMusicMMORPGPlayerState* PlayerState = GetMMORPGPlayerState())
    {
        PlayerName = PlayerState->GetPlayerName();
        CharacterClass = PlayerState->GetCharacterClass();
        Level = PlayerState->GetPlayerStats().Level;
        Experience = PlayerState->GetPlayerStats().Experience;
        HarmonyGems = PlayerState->GetPlayerStats().HarmonyGems;
        CurrentZone = PlayerState->GetCurrentZone();
    }
    
    // Set player state reference
    if (AMusicMMORPGPlayerController* PlayerController = GetMMORPGPlayerController())
    {
        if (AMusicMMORPGPlayerState* PlayerState = PlayerController->GetMMORPGPlayerState())
        {
            PlayerState->SetCharacter(this);
        }
    }
    
    // Update character based on class
    UpdateCharacterClass();
    
    // Notify game state
    if (AMusicMMORPGGameState* GameState = GetWorld()->GetGameState<AMusicMMORPGGameState>())
    {
        GameState->OnPlayerSpawned(this);
    }
}

void AMusicMMORPGCharacter::SetPlayerState(AMusicMMORPGPlayerState* InPlayerState)
{
    // This is called from player state to set the reference
}

AMusicMMORPGPlayerState* AMusicMMORPGCharacter::GetMMORPGPlayerState() const
{
    return Cast<AMusicMMORPGPlayerState>(PlayerState);
}

AMusicMMORPGPlayerController* AMusicMMORPGCharacter::GetMMORPGPlayerController() const
{
    return Cast<AMusicMMORPGPlayerController>(Controller);
}

void AMusicMMORPGCharacter::Move(const FVector2D& InputVector)
{
    if (!CanMove()) return;
    
    MovementInput = InputVector;
    
    // Calculate movement direction
    if (MovementInput.Size() > 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        
        // Get current speed based on movement mode
        float CurrentSpeed = WalkSpeed;
        if (bIsRunning)
        {
            CurrentSpeed = RunSpeed;
        }
        else if (bIsSprinting)
        {
            CurrentSpeed = SprintSpeed;
        }
        
        // Add movement
        AddMovementInput(ForwardDirection, MovementInput.Y * CurrentSpeed);
        AddMovementInput(RightDirection, MovementInput.X * CurrentSpeed);
        
        // Update state
        if (CurrentState != EMMORPGCharacterState::Moving)
        {
            CurrentState = EMMORPGCharacterState::Moving;
        }
    }
    else
    {
        // Update state
        if (CurrentState != EMMORPGCharacterState::Idle)
        {
            CurrentState = EMMORPGCharacterState::Idle;
        }
    }
    
    // Server RPC
    if (HasAuthority())
    {
        ServerMove(MovementInput);
    }
    else if (IsLocallyControlled())
    {
        ServerMove(MovementInput);
    }
}

void AMusicMMORPGCharacter::Look(const FVector2D& InputVector)
{
    LookInput = InputVector;
    
    // Server RPC
    if (HasAuthority())
    {
        ServerLook(LookInput);
    }
    else if (IsLocallyControlled())
    {
        ServerLook(LookInput);
    }
}

void AMusicMMORPGCharacter::Jump()
{
    if (!CanMove()) return;
    
    bJumpInput = true;
    
    Super::Jump();
    
    // Server RPC
    if (HasAuthority())
    {
        ServerJump();
    }
    else if (IsLocallyControlled())
    {
        ServerJump();
    }
}

void AMusicMMORPGCharacter::StopJumping()
{
    bJumpInput = false;
    
    Super::StopJumping();
    
    // Server RPC
    if (HasAuthority())
    {
        ServerStopJumping();
    }
    else if (IsLocallyControlled())
    {
        ServerStopJumping();
    }
}

void AMusicMMORPGCharacter::StartRunning()
{
    if (!CanMove()) return;
    
    bIsRunning = true;
    bRunInput = true;
    
    // Update movement speed
    GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
    
    // Update movement mode
    CurrentMovementMode = EMMORPGMovementMode::Running;
    
    // Server RPC
    if (HasAuthority())
    {
        // No need to call server RPC for this, it's handled by replication
    }
}

void AMusicMMORPGCharacter::StopRunning()
{
    bIsRunning = false;
    bRunInput = false;
    
    // Update movement speed
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    
    // Update movement mode
    CurrentMovementMode = EMMORPGMovementMode::Walking;
}

void AMusicMMORPGCharacter::StartSprinting()
{
    if (!CanMove()) return;
    
    bIsSprinting = true;
    bSprintInput = true;
    
    // Update movement speed
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    
    // Update movement mode
    CurrentMovementMode = EMMORPGMovementMode::Sprinting;
}

void AMusicMMORPGCharacter::StopSprinting()
{
    bIsSprinting = false;
    bSprintInput = false;
    
    // Update movement speed based on current state
    if (bIsRunning)
    {
        GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
        CurrentMovementMode = EMMORPGMovementMode::Running;
    }
    else
    {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
        CurrentMovementMode = EMMORPGMovementMode::Walking;
    }
}

void AMusicMMORPGCharacter::StartCrouching()
{
    if (!CanMove()) return;
    
    bIsCrouching = true;
    
    // Crouch character
    Crouch();
    
    // Update movement mode
    CurrentMovementMode = EMMORPGMovementMode::Crouching;
}

void AMusicMMORPGCharacter::StopCrouching()
{
    bIsCrouching = false;
    
    // Uncrouch character
    UnCrouch();
    
    // Update movement mode based on current state
    if (bIsSprinting)
    {
        CurrentMovementMode = EMMORPGMovementMode::Sprinting;
    }
    else if (bIsRunning)
    {
        CurrentMovementMode = EMMORPGMovementMode::Running;
    }
    else
    {
        CurrentMovementMode = EMMORPGMovementMode::Walking;
    }
}

void AMusicMMORPGCharacter::Attack()
{
    if (!CanAttack()) return;
    
    bAttackInput = true;
    CurrentState = EMMORPGCharacterState::Attacking;
    
    // Server RPC
    if (HasAuthority())
    {
        ServerAttack();
    }
    else if (IsLocallyControlled())
    {
        ServerAttack();
    }
}

void AMusicMMORPGCharacter::StopAttacking()
{
    bAttackInput = false;
    CurrentState = EMMORPGCharacterState::Idle;
}

void AMusicMMORPGCharacter::UseAbility(int32 AbilityIndex)
{
    if (!CanUseAbility(AbilityIndex)) return;
    
    AbilityInputs[AbilityIndex] = true;
    CurrentState = EMMORPGCharacterState::UsingAbility;
    
    // Server RPC
    if (HasAuthority())
    {
        ServerUseAbility(AbilityIndex);
    }
    else if (IsLocallyControlled())
    {
        ServerUseAbility(AbilityIndex);
    }
}

void AMusicMMORPGCharacter::SetTarget(AActor* NewTarget)
{
    CurrentTarget = NewTarget;
    CurrentBossTarget = Cast<AMMORPGBossCharacter>(NewTarget);
    
    // Server RPC
    if (HasAuthority())
    {
        // No need to call server RPC for this, it's handled by replication
    }
    else if (IsLocallyControlled())
    {
        AMusicMMORPGPlayerController* PlayerController = GetMMORPGPlayerController();
        if (PlayerController)
        {
            PlayerController->ServerInteract(NewTarget);
        }
    }
}

AActor* AMusicMMORPGCharacter::GetTarget() const
{
    return CurrentTarget;
}

void AMusicMMORPGCharacter::ChangeZone(const FName& ZoneName)
{
    if (HasAuthority())
    {
        ServerChangeZone(ZoneName);
    }
    else if (IsLocallyControlled())
    {
        AMusicMMORPGPlayerController* PlayerController = GetMMORPGPlayerController();
        if (PlayerController)
        {
            PlayerController->ServerChangeZone(ZoneName);
        }
    }
}

void AMusicMMORPGCharacter::TeleportToZone(const FName& ZoneName, const FVector& Location, const FRotator& Rotation)
{
    if (HasAuthority())
    {
        ServerTeleportToZone(ZoneName, Location, Rotation);
    }
    else if (IsLocallyControlled())
    {
        AMusicMMORPGPlayerController* PlayerController = GetMMORPGPlayerController();
        if (PlayerController)
        {
            PlayerController->ServerTeleportToZone(ZoneName, Location, Rotation);
        }
    }
}

void AMusicMMORPGCharacter::GainExperience(int64 Amount)
{
    if (HasAuthority())
    {
        ServerGainExperience(Amount);
    }
    else if (IsLocallyControlled())
    {
        AMusicMMORPGPlayerController* PlayerController = GetMMORPGPlayerController();
        if (PlayerController)
        {
            PlayerController->ServerGainExperience(Amount);
        }
    }
}

void AMusicMMORPGCharacter::AddHarmonyGems(int32 Amount)
{
    if (HasAuthority())
    {
        // Directly update
        HarmonyGems += Amount;
    }
    else if (IsLocallyControlled())
    {
        AMusicMMORPGPlayerController* PlayerController = GetMMORPGPlayerController();
        if (PlayerController)
        {
            PlayerController->ServerAddHarmonyGems(Amount);
        }
    }
}

bool AMusicMMORPGCharacter::SpendHarmonyGems(int32 Amount)
{
    if (HarmonyGems >= Amount)
    {
        if (HasAuthority())
        {
            HarmonyGems -= Amount;
            return true;
        }
        else if (IsLocallyControlled())
        {
            AMusicMMORPGPlayerController* PlayerController = GetMMORPGPlayerController();
            if (PlayerController)
            {
                PlayerController->ServerSpendHarmonyGems(Amount);
                return true;
            }
        }
    }
    return false;
}

void AMusicMMORPGCharacter::TakeDamage(float Damage, AController* InstigatedBy)
{
    if (bIsDead) return;
    
    if (HasAuthority())
    {
        ServerTakeDamage(Damage, InstigatedBy);
    }
    else if (IsLocallyControlled())
    {
        // Only server can handle damage
    }
}

void AMusicMMORPGCharacter::Heal(float Amount)
{
    if (HasAuthority())
    {
        // Directly heal
        if (AMusicMMORPGPlayerState* PlayerState = GetMMORPGPlayerState())
        {
            PlayerState->Heal(FMath::RoundToInt(Amount));
        }
    }
    else if (IsLocallyControlled())
    {
        AMusicMMORPGPlayerController* PlayerController = GetMMORPGPlayerController();
        if (PlayerController)
        {
            PlayerController->ServerHeal(FMath::RoundToInt(Amount));
        }
    }
}

void AMusicMMORPGCharacter::Die(AController* Killer)
{
    if (bIsDead) return;
    
    bIsDead = true;
    CurrentState = EMMORPGCharacterState::Dead;
    
    if (HasAuthority())
    {
        // Handle death on server
        if (AMusicMMORPGPlayerState* PlayerState = GetMMORPGPlayerState())
        {
            PlayerState->TakeDamage(PlayerState->GetPlayerStats().Health);
        }
    }
}

void AMusicMMORPGCharacter::Respawn()
{
    if (!bIsDead) return;
    
    bIsDead = false;
    CurrentState = EMMORPGCharacterState::Idle;
    
    // Find spawn location
    FTransform SpawnTransform = FindSpawnLocationInZone(CurrentZone);
    
    // Teleport to spawn location
    SetActorLocationAndRotation(SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator());
    
    // Reset health
    if (AMusicMMORPGPlayerState* PlayerState = GetMMORPGPlayerState())
    {
        PlayerState->Heal(PlayerState->GetPlayerStats().MaxHealth);
    }
}

// Server RPC implementations
void AMusicMMORPGCharacter::ServerMove_Implementation(const FVector2D& InputVector)
{
    if (IsLocallyControlled())
    {
        Move(InputVector);
    }
}

bool AMusicMMORPGCharacter::ServerMove_Validate(const FVector2D& InputVector)
{
    return true;
}

void AMusicMMORPGCharacter::ServerLook_Implementation(const FVector2D& InputVector)
{
    if (IsLocallyControlled())
    {
        Look(InputVector);
    }
}

bool AMusicMMORPGCharacter::ServerLook_Validate(const FVector2D& InputVector)
{
    return true;
}

void AMusicMMORPGCharacter::ServerJump_Implementation()
{
    if (IsLocallyControlled())
    {
        Jump();
    }
}

bool AMusicMMORPGCharacter::ServerJump_Validate()
{
    return true;
}

void AMusicMMORPGCharacter::ServerStopJumping_Implementation()
{
    if (IsLocallyControlled())
    {
        StopJumping();
    }
}

bool AMusicMMORPGCharacter::ServerStopJumping_Validate()
{
    return true;
}

void AMusicMMORPGCharacter::ServerAttack_Implementation()
{
    if (IsLocallyControlled())
    {
        Attack();
    }
}

bool AMusicMMORPGCharacter::ServerAttack_Validate()
{
    return true;
}

void AMusicMMORPGCharacter::ServerUseAbility_Implementation(int32 AbilityIndex)
{
    if (IsLocallyControlled())
    {
        UseAbility(AbilityIndex);
    }
}

bool AMusicMMORPGCharacter::ServerUseAbility_Validate(int32 AbilityIndex)
{
    return AbilityIndex >= 0 && AbilityIndex < 4;
}

void AMusicMMORPGCharacter::ServerChangeZone_Implementation(const FName& ZoneName)
{
    if (HasAuthority())
    {
        CurrentZone = ZoneName;
        
        // Find spawn location
        FTransform SpawnTransform = FindSpawnLocationInZone(ZoneName);
        
        // Teleport character
        SetActorLocationAndRotation(SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator());
        
        // Update player state
        if (AMusicMMORPGPlayerState* PlayerState = GetMMORPGPlayerState())
        {
            PlayerState->SetCurrentZone(ZoneName);
        }
        
        // Update game state
        if (AMusicMMORPGGameState* GameState = GetWorld()->GetGameState<AMusicMMORPGGameState>())
        {
            GameState->CharacterZoneMap.Add(this, ZoneName);
        }
    }
}

bool AMusicMMORPGCharacter::ServerChangeZone_Validate(const FName& ZoneName)
{
    return true;
}

void AMusicMMORPGCharacter::ServerTeleportToZone_Implementation(const FName& ZoneName, const FVector& Location, const FRotator& Rotation)
{
    if (HasAuthority())
    {
        CurrentZone = ZoneName;
        
        // Teleport character
        SetActorLocationAndRotation(Location, Rotation);
        
        // Update player state
        if (AMusicMMORPGPlayerState* PlayerState = GetMMORPGPlayerState())
        {
            PlayerState->SetCurrentZone(ZoneName);
        }
        
        // Update game state
        if (AMusicMMORPGGameState* GameState = GetWorld()->GetGameState<AMusicMMORPGGameState>())
        {
            GameState->CharacterZoneMap.Add(this, ZoneName);
        }
    }
}

bool AMusicMMORPGCharacter::ServerTeleportToZone_Validate(const FName& ZoneName, const FVector& Location, const FRotator& Rotation)
{
    return true;
}

void AMusicMMORPGCharacter::ServerTakeDamage_Implementation(float Damage, AController* InstigatedBy)
{
    if (HasAuthority())
    {
        if (AMusicMMORPGPlayerState* PlayerState = GetMMORPGPlayerState())
        {
            PlayerState->TakeDamage(FMath::RoundToInt(Damage));
        }
    }
}

bool AMusicMMORPGCharacter::ServerTakeDamage_Validate(float Damage, AController* InstigatedBy)
{
    return Damage > 0;
}

void AMusicMMORPGCharacter::ServerGainExperience_Implementation(int64 Amount)
{
    if (HasAuthority())
    {
        if (AMusicMMORPGPlayerState* PlayerState = GetMMORPGPlayerState())
        {
            PlayerState->GainExperience(Amount);
        }
        else
        {
            Experience += Amount;
        }
    }
}

bool AMusicMMORPGCharacter::ServerGainExperience_Validate(int64 Amount)
{
    return Amount > 0;
}

void AMusicMMORPGCharacter::ServerAddHarmonyGems_Implementation(int32 Amount)
{
    if (HasAuthority())
    {
        HarmonyGems += Amount;
    }
}

bool AMusicMMORPGCharacter::ServerAddHarmonyGems_Validate(int32 Amount)
{
    return true;
}

void AMusicMMORPGCharacter::ServerSpendHarmonyGems_Implementation(int32 Amount)
{
    if (HasAuthority())
    {
        if (HarmonyGems >= Amount)
        {
            HarmonyGems -= Amount;
        }
    }
}

bool AMusicMMORPGCharacter::ServerSpendHarmonyGems_Validate(int32 Amount)
{
    return Amount > 0;
}

// Client RPC implementations
void AMusicMMORPGCharacter::ClientSetTarget_Implementation(AActor* NewTarget)
{
    SetTarget(NewTarget);
}

void AMusicMMORPGCharacter::ClientChangeZone_Implementation(const FName& ZoneName)
{
    CurrentZone = ZoneName;
}

void AMusicMMORPGCharacter::ClientTeleportToZone_Implementation(const FName& ZoneName, const FVector& Location, const FRotator& Rotation)
{
    CurrentZone = ZoneName;
    SetActorLocationAndRotation(Location, Rotation);
}

void AMusicMMORPGCharacter::ClientTakeDamage_Implementation(float Damage, AController* InstigatedBy)
{
    // Play damage effects on client
    TakeDamage(Damage, InstigatedBy);
}

void AMusicMMORPGCharacter::ClientGainExperience_Implementation(int64 Amount)
{
    // Update experience on client
    Experience += Amount;
}

void AMusicMMORPGCharacter::ClientAddHarmonyGems_Implementation(int32 Amount)
{
    // Update harmony gems on client
    HarmonyGems += Amount;
}

// Helper functions
bool AMusicMMORPGCharacter::IsLocalPlayer() const
{
    return IsLocallyControlled();
}

bool AMusicMMORPGCharacter::IsDead() const
{
    return bIsDead;
}

bool AMusicMMORPGCharacter::CanMove() const
{
    return !bIsDead && CurrentState != EMMORPGCharacterState::Attacking && 
           CurrentState != EMMORPGCharacterState::Casting && 
           CurrentState != EMMORPGCharacterState::UsingAbility &&
           CurrentState != EMMORPGCharacterState::TakingDamage;
}

bool AMusicMMORPGCharacter::CanAttack() const
{
    return !bIsDead && CurrentState != EMMORPGCharacterState::Attacking && 
           CurrentState != EMMORPGCharacterState::Casting && 
           CurrentState != EMMORPGCharacterState::UsingAbility &&
           CurrentState != EMMORPGCharacterState::TakingDamage &&
           CurrentState != EMMORPGCharacterState::Dead &&
           CurrentState != EMMORPGCharacterState::InMenu;
}

bool AMusicMMORPGCharacter::CanUseAbility(int32 AbilityIndex) const
{
    return CanAttack() && AbilityIndex >= 0 && AbilityIndex < 4;
}

AMusicMMORPGCharacter* AMusicMMORPGCharacter::GetInstance(const UWorld* World)
{
    if (World)
    {
        for (FConstPawnIterator Iterator = World->GetPawnIterator(); Iterator; ++Iterator)
        {
            APawn* Pawn = Iterator->Get();
            if (AMusicMMORPGCharacter* Character = Cast<AMusicMMORPGCharacter>(Pawn))
            {
                return Character;
            }
        }
    }
    return nullptr;
}

void AMusicMMORPGCharacter::OnRep_PlayerName()
{
    // Player name changed
}

void AMusicMMORPGCharacter::OnRep_CharacterClass()
{
    UpdateCharacterClass();
}

void AMusicMMORPGCharacter::OnRep_Level()
{
    // Level changed
}

void AMusicMMORPGCharacter::OnRep_Experience()
{
    // Experience changed
}

void AMusicMMORPGCharacter::OnRep_HarmonyGems()
{
    // Harmony gems changed
}

void AMusicMMORPGCharacter::OnRep_CurrentZone()
{
    // Zone changed
}

void AMusicMMORPGCharacter::OnRep_CurrentState()
{
    // State changed
}

void AMusicMMORPGCharacter::OnRep_CurrentMovementMode()
{
    UpdateMovementMode();
}

void AMusicMMORPGCharacter::OnRep_bIsRunning()
{
    if (bIsRunning)
    {
        GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
    }
    else
    {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
}

void AMusicMMORPGCharacter::OnRep_bIsSprinting()
{
    if (bIsSprinting)
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
    else if (bIsRunning)
    {
        GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
    }
    else
    {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
}

void AMusicMMORPGCharacter::OnRep_bIsCrouching()
{
    if (bIsCrouching)
    {
        Crouch();
    }
    else
    {
        UnCrouch();
    }
}

void AMusicMMORPGCharacter::OnRep_bIsDead()
{
    if (bIsDead)
    {
        CurrentState = EMMORPGCharacterState::Dead;
    }
}

void AMusicMMORPGCharacter::OnRep_CurrentTarget()
{
    // Target changed
}

void AMusicMMORPGCharacter::OnRep_CurrentBossTarget()
{
    // Boss target changed
}

void AMusicMMORPGCharacter::UpdateCharacterClass()
{
    // Update character appearance and stats based on class
    switch (CharacterClass)
    {
        case ECharacterClass::Bard:
            // Bard specific setup
            break;
        case ECharacterClass::Beatmaker:
            // Beatmaker specific setup
            break;
        case ECharacterClass::Vocalist:
            // Vocalist specific setup
            break;
        case ECharacterClass::Guitarist:
            // Guitarist specific setup
            break;
        case ECharacterClass::DJ:
            // DJ specific setup
            break;
        default:
            break;
    }
}

void AMusicMMORPGCharacter::HandleLevelUp()
{
    // Handle level up logic
    Level++;
}

FTransform AMusicMMORPGCharacter::FindSpawnLocationInZone(const FName& ZoneName)
{
    // Try to find a player start in the zone
    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        APlayerStart* PlayerStart = *It;
        if (PlayerStart && PlayerStart->PlayerStartTag == ZoneName.ToString())
        {
            return PlayerStart->GetActorTransform();
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

void AMusicMMORPGCharacter::UpdateMovementMode()
{
    // Update movement mode based on current state
    if (bIsSprinting)
    {
        CurrentMovementMode = EMMORPGMovementMode::Sprinting;
    }
    else if (bIsRunning)
    {
        CurrentMovementMode = EMMORPGMovementMode::Running;
    }
    else if (bIsCrouching)
    {
        CurrentMovementMode = EMMORPGMovementMode::Crouching;
    }
    else
    {
        CurrentMovementMode = EMMORPGMovementMode::Walking;
    }
}

void AMusicMMORPGCharacter::UpdateCamera()
{
    // Update camera settings
    if (SpringArm)
    {
        SpringArm->TargetArmLength = 800.0f;
    }
}

void AMusicMMORPGCharacter::InitializeComponents()
{
    // Initialize all character components
    if (InventoryComponent)
    {
        InventoryComponent->InitializeComponent();
    }
    
    if (AbilityComponent)
    {
        AbilityComponent->InitializeComponent();
    }
    
    if (CombatComponent)
    {
        CombatComponent->InitializeComponent();
    }
    
    if (StatsComponent)
    {
        StatsComponent->InitializeComponent();
    }
    
    if (MMOInputComponent)
    {
        MMOInputComponent->InitializeComponent();
    }
}

void AMusicMMORPGCharacter::OnStatsUpdated()
{
    // Update character stats from player state
    if (AMusicMMORPGPlayerState* PlayerState = GetMMORPGPlayerState())
    {
        FPlayerStats Stats = PlayerState->GetPlayerStats();
        Level = Stats.Level;
        Experience = Stats.Experience;
        HarmonyGems = Stats.HarmonyGems;
    }
}

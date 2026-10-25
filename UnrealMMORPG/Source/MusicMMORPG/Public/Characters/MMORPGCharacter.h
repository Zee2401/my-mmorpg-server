pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MMORPGCharacter.generated.h"

// Forward declarations
class AMusicMMORPGPlayerController;
class AMusicMMORPGPlayerState;
class UCharacterMovementComponent;
class USpringArmComponent;
class UCameraComponent;
class UMMORPGInventoryComponent;
class UMMORPGAbilityComponent;
class UMMORPGCombatComponent;
class UMMORPGStatsComponent;
class UMMORPGInputComponent;
class AMMORPGBossCharacter;

// Character classes
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
    Bard UMETA(DisplayName = "Bard"),
    Beatmaker UMETA(DisplayName = "Beatmaker"),
    Vocalist UMETA(DisplayName = "Vocalist"),
    Guitarist UMETA(DisplayName = "Guitarist"),
    DJ UMETA(DisplayName = "DJ"),
    MAX UMETA(Hidden)
};

// Character movement modes
UENUM(BlueprintType)
enum class EMMORPGMovementMode : uint8
{
    Walking UMETA(DisplayName = "Walking"),
    Running UMETA(DisplayName = "Running"),
    Sprinting UMETA(DisplayName = "Sprinting"),
    Crouching UMETA(DisplayName = "Crouching"),
    Sliding UMETA(DisplayName = "Sliding"),
    Swimming UMETA(DisplayName = "Swimming"),
    Flying UMETA(DisplayName = "Flying"),
    MAX UMETA(Hidden)
};

// Character states
UENUM(BlueprintType)
enum class EMMORPGCharacterState : uint8
{
    Idle UMETA(DisplayName = "Idle"),
    Moving UMETA(DisplayName = "Moving"),
    Attacking UMETA(DisplayName = "Attacking"),
    Casting UMETA(DisplayName = "Casting"),
    UsingAbility UMETA(DisplayName = "Using Ability"),
    TakingDamage UMETA(DisplayName = "Taking Damage"),
    Dead UMETA(DisplayName = "Dead"),
    Interacting UMETA(DisplayName = "Interacting"),
    InDialogue UMETA(DisplayName = "In Dialogue"),
    InMenu UMETA(DisplayName = "In Menu"),
    MAX UMETA(Hidden)
};

/**
 * Main character class for Music MMORPG
 * Handles player movement, combat, abilities, and networking
 */
UCLASS()
class MUSICMMORPG_API AMusicMMORPGCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMusicMMORPGCharacter();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void UnPossess() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Character")
    virtual void InitializePlayer();
    
    UFUNCTION(BlueprintCallable, Category = "Character")
    void SetPlayerState(AMusicMMORPGPlayerState* InPlayerState);
    
    UFUNCTION(BlueprintPure, Category = "Character")
    AMusicMMORPGPlayerState* GetMMORPGPlayerState() const;
    
    UFUNCTION(BlueprintPure, Category = "Character")
    AMusicMMORPGPlayerController* GetMMORPGPlayerController() const;
    
    // Character info
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Info", Replicated)
    FString PlayerName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Info", Replicated)
    ECharacterClass CharacterClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Info", Replicated)
    int32 Level;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Info", Replicated)
    int64 Experience;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Info", Replicated)
    int32 HarmonyGems;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Info", Replicated)
    FName CurrentZone;
    
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Camera;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UMMORPGInventoryComponent* InventoryComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UMMORPGAbilityComponent* AbilityComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UMMORPGCombatComponent* CombatComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UMMORPGStatsComponent* StatsComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UMMORPGInputComponent* MMOInputComponent;
    
    // Movement settings
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Replicated)
    float WalkSpeed;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Replicated)
    float RunSpeed;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Replicated)
    float SprintSpeed;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Replicated)
    float RotationRate;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Replicated)
    float JumpVelocity;
    
    // State
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "State")
    EMMORPGCharacterState CurrentState;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "State")
    EMMORPGMovementMode CurrentMovementMode;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "State")
    bool bIsRunning;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "State")
    bool bIsSprinting;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "State")
    bool bIsCrouching;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "State")
    bool bIsDead;
    
    // Targeting
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Targeting")
    AActor* CurrentTarget;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Targeting")
    AMMORPGBossCharacter* CurrentBossTarget;
    
    // Input
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    FVector2D MovementInput;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    FVector2D LookInput;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bJumpInput;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bAttackInput;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bRunInput;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bSprintInput;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bInteractInput;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bInventoryInput;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bMapInput;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bMenuInput;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    TArray<bool> AbilityInputs;
    
    // Movement
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Move(const FVector2D& InputVector);
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Look(const FVector2D& InputVector);
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Jump();
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopJumping();
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartRunning();
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopRunning();
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartSprinting();
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopSprinting();
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartCrouching();
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopCrouching();
    
    // Combat
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Attack();
    
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StopAttacking();
    
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void UseAbility(int32 AbilityIndex);
    
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void SetTarget(AActor* NewTarget);
    
    UFUNCTION(BlueprintPure, Category = "Combat")
    AActor* GetTarget() const;
    
    // Zone management
    UFUNCTION(BlueprintCallable, Category = "Zone")
    void ChangeZone(const FName& ZoneName);
    
    UFUNCTION(BlueprintCallable, Category = "Zone")
    void TeleportToZone(const FName& ZoneName, const FVector& Location = FVector::ZeroVector, const FRotator& Rotation = FRotator::ZeroRotator);
    
    // Stats
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void GainExperience(int64 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void AddHarmonyGems(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Stats")
    bool SpendHarmonyGems(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void TakeDamage(float Damage, AController* InstigatedBy);
    
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void Heal(float Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void Die(AController* Killer);
    
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void Respawn();
    
    // Network functions
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerMove(const FVector2D& InputVector);
    
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerLook(const FVector2D& InputVector);
    
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerJump();
    
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerStopJumping();
    
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerAttack();
    
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerUseAbility(int32 AbilityIndex);
    
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerChangeZone(const FName& ZoneName);
    
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerTeleportToZone(const FName& ZoneName, const FVector& Location, const FRotator& Rotation);
    
    UFUNCTION(Server, Reliable)
    void ServerTakeDamage(float Damage, AController* InstigatedBy);
    
    UFUNCTION(Server, Reliable)
    void ServerGainExperience(int64 Amount);
    
    UFUNCTION(Server, Reliable)
    void ServerAddHarmonyGems(int32 Amount);
    
    UFUNCTION(Server, Reliable)
    void ServerSpendHarmonyGems(int32 Amount);
    
    // Client functions
    UFUNCTION(Client, Reliable)
    void ClientSetTarget(AActor* NewTarget);
    
    UFUNCTION(Client, Reliable)
    void ClientChangeZone(const FName& ZoneName);
    
    UFUNCTION(Client, Reliable)
    void ClientTeleportToZone(const FName& ZoneName, const FVector& Location, const FRotator& Rotation);
    
    UFUNCTION(Client, Reliable)
    void ClientTakeDamage(float Damage, AController* InstigatedBy);
    
    UFUNCTION(Client, Reliable)
    void ClientGainExperience(int64 Amount);
    
    UFUNCTION(Client, Reliable)
    void ClientAddHarmonyGems(int32 Amount);
    
    // Helper functions
    UFUNCTION(BlueprintPure, Category = "Character")
    bool IsLocalPlayer() const;
    
    UFUNCTION(BlueprintPure, Category = "Character")
    bool IsDead() const;
    
    UFUNCTION(BlueprintPure, Category = "Character")
    bool CanMove() const;
    
    UFUNCTION(BlueprintPure, Category = "Character")
    bool CanAttack() const;
    
    UFUNCTION(BlueprintPure, Category = "Character")
    bool CanUseAbility(int32 AbilityIndex) const;
    
    // Static instance
    static AMusicMMORPGCharacter* GetInstance(const UWorld* World);
    
protected:
    virtual void OnRep_PlayerName();
    virtual void OnRep_CharacterClass();
    virtual void OnRep_Level();
    virtual void OnRep_Experience();
    virtual void OnRep_HarmonyGems();
    virtual void OnRep_CurrentZone();
    virtual void OnRep_CurrentState();
    virtual void OnRep_CurrentMovementMode();
    virtual void OnRep_bIsRunning();
    virtual void OnRep_bIsSprinting();
    virtual void OnRep_bIsCrouching();
    virtual void OnRep_bIsDead();
    virtual void OnRep_CurrentTarget();
    virtual void OnRep_CurrentBossTarget();
    
    // Update character based on class
    void UpdateCharacterClass();
    
    // Handle level up
    void HandleLevelUp();
    
    // Find spawn location in zone
    FTransform FindSpawnLocationInZone(const FName& ZoneName);
    
    // Update movement mode
    void UpdateMovementMode();
    
    // Update camera
    void UpdateCamera();
    
    // Initialize components
    void InitializeComponents();
};

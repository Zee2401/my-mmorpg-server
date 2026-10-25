pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MusicMMORPGPlayerState.generated.h"

// Forward declarations
class AMusicMMORPGCharacter;
class AMusicMMORPGPlayerController;

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

// Player statistics structure
USTRUCT(BlueprintType)
struct FPlayerStats
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 Level;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int64 Experience;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int64 ExperienceToNextLevel;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 HarmonyGems;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 Health;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 MaxHealth;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 Energy;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 MaxEnergy;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 AttackPower;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 DefensePower;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 RhythmAccuracy;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 PitchAccuracy;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 SpecialPower;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 HarmonyDefense;
    
    // Attribute points
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 AvailableAttributePoints;
    
    // Skill points
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    int32 AvailableSkillPoints;
    
    // Constructor
    FPlayerStats()
        : Level(1)
        , Experience(0)
        , ExperienceToNextLevel(1000)
        , HarmonyGems(0)
        , Health(100)
        , MaxHealth(100)
        , Energy(100)
        , MaxEnergy(100)
        , AttackPower(10)
        , DefensePower(5)
        , RhythmAccuracy(50)
        , PitchAccuracy(50)
        , SpecialPower(10)
        , HarmonyDefense(5)
        , AvailableAttributePoints(0)
        , AvailableSkillPoints(0)
    {
    }
};

// Character progression structure
USTRUCT(BlueprintType)
struct FCharacterProgression
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progression")
    TArray<int64> ExperiencePerLevel;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progression")
    TArray<int32> HealthPerLevel;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progression")
    TArray<int32> EnergyPerLevel;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progression")
    TArray<int32> AttributePointsPerLevel;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progression")
    TArray<int32> SkillPointsPerLevel;
    
    // Constructor
    FCharacterProgression()
    {
        // Initialize experience curve
        for (int32 i = 1; i <= 100; i++)
        {
            ExperiencePerLevel.Add(i * 1000);
        }
        
        // Initialize health and energy curves
        for (int32 i = 1; i <= 100; i++)
        {
            HealthPerLevel.Add(100 + (i - 1) * 10);
            EnergyPerLevel.Add(100 + (i - 1) * 5);
        }
        
        // Initialize attribute and skill points
        for (int32 i = 1; i <= 100; i++)
        {
            AttributePointsPerLevel.Add(5);
            SkillPointsPerLevel.Add(3);
        }
    }
};

/**
 * Player State for Music MMORPG
 * Tracks player statistics, progression, and persistent data
 */
UCLASS()
class MUSICMMORPG_API AMusicMMORPGPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AMusicMMORPGPlayerState();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void CopyProperties(APlayerState* PlayerState) override;
    virtual void OverrideWith(APlayerState* PlayerState) override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Player State")
    virtual void InitializePlayer();
    
    // Player info
    UFUNCTION(BlueprintPure, Category = "Player State")
    FString GetPlayerName() const;
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void SetPlayerName(const FString& NewName);
    
    UFUNCTION(BlueprintPure, Category = "Player State")
    ECharacterClass GetCharacterClass() const;
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void SetCharacterClass(ECharacterClass NewClass);
    
    // Statistics
    UFUNCTION(BlueprintPure, Category = "Player State")
    FPlayerStats GetPlayerStats() const;
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void SetPlayerStats(const FPlayerStats& NewStats);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void GainExperience(int64 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void AddHarmonyGems(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    bool SpendHarmonyGems(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void TakeDamage(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void Heal(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void UseEnergy(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void RestoreEnergy(int32 Amount);
    
    // Level up
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void LevelUp();
    
    UFUNCTION(BlueprintPure, Category = "Player State")
    bool CanLevelUp() const;
    
    // Attribute management
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void AddAttributePoints(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    bool SpendAttributePoints(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void AddAttackPower(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void AddDefensePower(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void AddRhythmAccuracy(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void AddPitchAccuracy(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void AddSpecialPower(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void AddHarmonyDefense(int32 Amount);
    
    // Skill management
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void AddSkillPoints(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    bool SpendSkillPoints(int32 Amount);
    
    // Zone management
    UFUNCTION(BlueprintPure, Category = "Player State")
    FName GetCurrentZone() const;
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void SetCurrentZone(FName NewZone);
    
    // Progression data
    UFUNCTION(BlueprintPure, Category = "Player State")
    FCharacterProgression GetCharacterProgression() const;
    
    // Character access
    UFUNCTION(BlueprintPure, Category = "Player State")
    AMusicMMORPGCharacter* GetMMORPGCharacter() const;
    
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void SetCharacter(AMusicMMORPGCharacter* NewCharacter);
    
    // Network functions
    UFUNCTION(Server, Reliable)
    void ServerGainExperience(int64 Amount);
    
    UFUNCTION(Server, Reliable)
    void ServerAddHarmonyGems(int32 Amount);
    
    UFUNCTION(Server, Reliable)
    void ServerSpendHarmonyGems(int32 Amount);
    
    UFUNCTION(Server, Reliable)
    void ServerTakeDamage(int32 Amount);
    
    UFUNCTION(Server, Reliable)
    void ServerHeal(int32 Amount);
    
    UFUNCTION(Server, Reliable)
    void ServerLevelUp();
    
    // Replicated properties
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player State")
    FString PlayerName;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player State")
    ECharacterClass CharacterClass;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player State")
    FPlayerStats Stats;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player State")
    FName CurrentZone;
    
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player State")
    FCharacterProgression Progression;
    
    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExperienceGainedSignature, int64, Amount);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpSignature, int32, NewLevel);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHarmonyGemsChangedSignature, int32, NewAmount);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, int32, NewHealth);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnergyChangedSignature, int32, NewEnergy);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZoneChangedSignature, FName, NewZone);
    
    UPROPERTY(BlueprintAssignable, Category = "Player State")
    FOnExperienceGainedSignature OnExperienceGained;
    
    UPROPERTY(BlueprintAssignable, Category = "Player State")
    FOnLevelUpSignature OnLevelUp;
    
    UPROPERTY(BlueprintAssignable, Category = "Player State")
    FOnHarmonyGemsChangedSignature OnHarmonyGemsChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Player State")
    FOnHealthChangedSignature OnHealthChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Player State")
    FOnEnergyChangedSignature OnEnergyChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Player State")
    FOnZoneChangedSignature OnZoneChanged;
    
protected:
    // Character reference
    UPROPERTY()
    AMusicMMORPGCharacter* CachedCharacter;
    
    // Replication callbacks
    virtual void OnRep_PlayerName();
    virtual void OnRep_CharacterClass();
    virtual void OnRep_Stats();
    virtual void OnRep_CurrentZone();
    
    // Helper functions
    void UpdateExperienceToNextLevel();
    void HandleLevelUp();
    
    // Static instance
    static AMusicMMORPGPlayerState* GetInstance(const UWorld* World);
};

#include "MusicMMORPGPlayerState.h"
#include "MusicMMORPGCharacter.h"
#include "MusicMMORPGPlayerController.h"
#include "Engine/World.h"

AMusicMMORPGPlayerState::AMusicMMORPGPlayerState()
{
    // Set default values
    PlayerName = TEXT("Player");
    CharacterClass = ECharacterClass::Bard;
    CurrentZone = NAME_SoundCity;
    
    // Initialize stats
    Stats = FPlayerStats();
    
    // Initialize progression
    Progression = FCharacterProgression();
    
    // Enable replication
    bReplicates = true;
    
    // Set replication policy
    SetReplicateMovement(true);
}

void AMusicMMORPGPlayerState::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize player
    InitializePlayer();
}

void AMusicMMORPGPlayerState::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMusicMMORPGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    // Replicate these properties
    DOREPLIFETIME(AMusicMMORPGPlayerState, PlayerName);
    DOREPLIFETIME(AMusicMMORPGPlayerState, CharacterClass);
    DOREPLIFETIME(AMusicMMORPGPlayerState, Stats);
    DOREPLIFETIME(AMusicMMORPGPlayerState, CurrentZone);
    DOREPLIFETIME(AMusicMMORPGPlayerState, Progression);
}

void AMusicMMORPGPlayerState::CopyProperties(APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);
    
    if (AMusicMMORPGPlayerState* MMORPGPlayerState = Cast<AMusicMMORPGPlayerState>(PlayerState))
    {
        PlayerName = MMORPGPlayerState->PlayerName;
        CharacterClass = MMORPGPlayerState->CharacterClass;
        Stats = MMORPGPlayerState->Stats;
        CurrentZone = MMORPGPlayerState->CurrentZone;
        Progression = MMORPGPlayerState->Progression;
    }
}

void AMusicMMORPGPlayerState::OverrideWith(APlayerState* PlayerState)
{
    Super::OverrideWith(PlayerState);
    
    if (AMusicMMORPGPlayerState* MMORPGPlayerState = Cast<AMusicMMORPGPlayerState>(PlayerState))
    {
        PlayerName = MMORPGPlayerState->PlayerName;
        CharacterClass = MMORPGPlayerState->CharacterClass;
        Stats = MMORPGPlayerState->Stats;
        CurrentZone = MMORPGPlayerState->CurrentZone;
        Progression = MMORPGPlayerState->Progression;
    }
}

void AMusicMMORPGPlayerState::InitializePlayer()
{
    // Set player name from parent class
    if (PlayerName.IsEmpty())
    {
        PlayerName = GetPlayerName();
    }
    
    // Initialize character reference
    if (AMusicMMORPGCharacter* Character = GetMMORPGCharacter())
    {
        Character->SetPlayerState(this);
    }
}

FString AMusicMMORPGPlayerState::GetPlayerName() const
{
    return PlayerName;
}

void AMusicMMORPGPlayerState::SetPlayerName(const FString& NewName)
{
    if (HasAuthority())
    {
        PlayerName = NewName;
        OnRep_PlayerName();
    }
    else
    {
        // Request server to change name
        AMusicMMORPGPlayerController* PlayerController = GetOwner<AMusicMMORPGPlayerController>();
        if (PlayerController && PlayerController->IsLocalController())
        {
            // In a real implementation, this would call a server RPC
        }
    }
}

ECharacterClass AMusicMMORPGPlayerState::GetCharacterClass() const
{
    return CharacterClass;
}

void AMusicMMORPGPlayerState::SetCharacterClass(ECharacterClass NewClass)
{
    if (HasAuthority())
    {
        CharacterClass = NewClass;
        OnRep_CharacterClass();
    }
}

FPlayerStats AMusicMMORPGPlayerState::GetPlayerStats() const
{
    return Stats;
}

void AMusicMMORPGPlayerState::SetPlayerStats(const FPlayerStats& NewStats)
{
    if (HasAuthority())
    {
        Stats = NewStats;
        OnRep_Stats();
    }
}

void AMusicMMORPGPlayerState::GainExperience(int64 Amount)
{
    if (HasAuthority())
    {
        ServerGainExperience(Amount);
    }
    else
    {
        AMusicMMORPGPlayerController* PlayerController = GetOwner<AMusicMMORPGPlayerController>();
        if (PlayerController && PlayerController->IsLocalController())
        {
            PlayerController->ServerGainExperience(Amount);
        }
    }
}

void AMusicMMORPGPlayerState::AddHarmonyGems(int32 Amount)
{
    if (HasAuthority())
    {
        ServerAddHarmonyGems(Amount);
    }
    else
    {
        AMusicMMORPGPlayerController* PlayerController = GetOwner<AMusicMMORPGPlayerController>();
        if (PlayerController && PlayerController->IsLocalController())
        {
            PlayerController->ServerAddHarmonyGems(Amount);
        }
    }
}

bool AMusicMMORPGPlayerState::SpendHarmonyGems(int32 Amount)
{
    if (Stats.HarmonyGems >= Amount)
    {
        if (HasAuthority())
        {
            ServerSpendHarmonyGems(Amount);
            return true;
        }
        else
        {
            AMusicMMORPGPlayerController* PlayerController = GetOwner<AMusicMMORPGPlayerController>();
            if (PlayerController && PlayerController->IsLocalController())
            {
                PlayerController->ServerSpendHarmonyGems(Amount);
                return true;
            }
        }
    }
    return false;
}

void AMusicMMORPGPlayerState::TakeDamage(int32 Amount)
{
    if (HasAuthority())
    {
        ServerTakeDamage(Amount);
    }
    else
    {
        AMusicMMORPGPlayerController* PlayerController = GetOwner<AMusicMMORPGPlayerController>();
        if (PlayerController && PlayerController->IsLocalController())
        {
            PlayerController->ServerTakeDamage(Amount, nullptr);
        }
    }
}

void AMusicMMORPGPlayerState::Heal(int32 Amount)
{
    if (HasAuthority())
    {
        ServerHeal(Amount);
    }
    else
    {
        AMusicMMORPGPlayerController* PlayerController = GetOwner<AMusicMMORPGPlayerController>();
        if (PlayerController && PlayerController->IsLocalController())
        {
            PlayerController->ServerHeal(Amount);
        }
    }
}

void AMusicMMORPGPlayerState::UseEnergy(int32 Amount)
{
    if (Stats.Energy >= Amount)
    {
        Stats.Energy = FMath::Max(0, Stats.Energy - Amount);
        OnEnergyChanged.Broadcast(Stats.Energy);
    }
}

void AMusicMMORPGPlayerState::RestoreEnergy(int32 Amount)
{
    Stats.Energy = FMath::Min(Stats.MaxEnergy, Stats.Energy + Amount);
    OnEnergyChanged.Broadcast(Stats.Energy);
}

void AMusicMMORPGPlayerState::LevelUp()
{
    if (HasAuthority())
    {
        ServerLevelUp();
    }
    else
    {
        AMusicMMORPGPlayerController* PlayerController = GetOwner<AMusicMMORPGPlayerController>();
        if (PlayerController && PlayerController->IsLocalController())
        {
            PlayerController->ServerGainExperience(Progression.ExperiencePerLevel[Stats.Level - 1] - Stats.Experience);
        }
    }
}

bool AMusicMMORPGPlayerState::CanLevelUp() const
{
    return Stats.Experience >= Progression.ExperiencePerLevel[Stats.Level - 1];
}

void AMusicMMORPGPlayerState::AddAttributePoints(int32 Amount)
{
    Stats.AvailableAttributePoints += Amount;
}

bool AMusicMMORPGPlayerState::SpendAttributePoints(int32 Amount)
{
    if (Stats.AvailableAttributePoints >= Amount)
    {
        Stats.AvailableAttributePoints -= Amount;
        return true;
    }
    return false;
}

void AMusicMMORPGPlayerState::AddAttackPower(int32 Amount)
{
    Stats.AttackPower += Amount;
}

void AMusicMMORPGPlayerState::AddDefensePower(int32 Amount)
{
    Stats.DefensePower += Amount;
}

void AMusicMMORPGPlayerState::AddRhythmAccuracy(int32 Amount)
{
    Stats.RhythmAccuracy = FMath::Min(100, Stats.RhythmAccuracy + Amount);
}

void AMusicMMORPGPlayerState::AddPitchAccuracy(int32 Amount)
{
    Stats.PitchAccuracy = FMath::Min(100, Stats.PitchAccuracy + Amount);
}

void AMusicMMORPGPlayerState::AddSpecialPower(int32 Amount)
{
    Stats.SpecialPower += Amount;
}

void AMusicMMORPGPlayerState::AddHarmonyDefense(int32 Amount)
{
    Stats.HarmonyDefense += Amount;
}

void AMusicMMORPGPlayerState::AddSkillPoints(int32 Amount)
{
    Stats.AvailableSkillPoints += Amount;
}

bool AMusicMMORPGPlayerState::SpendSkillPoints(int32 Amount)
{
    if (Stats.AvailableSkillPoints >= Amount)
    {
        Stats.AvailableSkillPoints -= Amount;
        return true;
    }
    return false;
}

FName AMusicMMORPGPlayerState::GetCurrentZone() const
{
    return CurrentZone;
}

void AMusicMMORPGPlayerState::SetCurrentZone(FName NewZone)
{
    if (HasAuthority())
    {
        CurrentZone = NewZone;
        OnRep_CurrentZone();
    }
}

FCharacterProgression AMusicMMORPGPlayerState::GetCharacterProgression() const
{
    return Progression;
}

AMusicMMORPGCharacter* AMusicMMORPGPlayerState::GetMMORPGCharacter() const
{
    return CachedCharacter;
}

void AMusicMMORPGPlayerState::SetCharacter(AMusicMMORPGCharacter* NewCharacter)
{
    CachedCharacter = NewCharacter;
}

void AMusicMMORPGPlayerState::ServerGainExperience_Implementation(int64 Amount)
{
    if (Amount <= 0) return;
    
    Stats.Experience += Amount;
    
    // Check for level up
    while (Stats.Level < Progression.ExperiencePerLevel.Num() && 
           Stats.Experience >= Progression.ExperiencePerLevel[Stats.Level - 1])
    {
        HandleLevelUp();
    }
    
    UpdateExperienceToNextLevel();
    OnExperienceGained.Broadcast(Amount);
}

void AMusicMMORPGPlayerState::ServerAddHarmonyGems_Implementation(int32 Amount)
{
    Stats.HarmonyGems += Amount;
    OnHarmonyGemsChanged.Broadcast(Stats.HarmonyGems);
}

void AMusicMMORPGPlayerState::ServerSpendHarmonyGems_Implementation(int32 Amount)
{
    if (Stats.HarmonyGems >= Amount)
    {
        Stats.HarmonyGems -= Amount;
        OnHarmonyGemsChanged.Broadcast(Stats.HarmonyGems);
    }
}

void AMusicMMORPGPlayerState::ServerTakeDamage_Implementation(int32 Amount)
{
    if (Amount <= 0) return;
    
    // Calculate actual damage based on defense
    int32 ActualDamage = FMath::Max(1, Amount - Stats.DefensePower / 2);
    Stats.Health = FMath::Max(0, Stats.Health - ActualDamage);
    
    OnHealthChanged.Broadcast(Stats.Health);
    
    // Check if player died
    if (Stats.Health <= 0)
    {
        if (CachedCharacter)
        {
            CachedCharacter->Die(nullptr);
        }
    }
}

void AMusicMMORPGPlayerState::ServerHeal_Implementation(int32 Amount)
{
    if (Amount <= 0) return;
    
    Stats.Health = FMath::Min(Stats.MaxHealth, Stats.Health + Amount);
    OnHealthChanged.Broadcast(Stats.Health);
}

void AMusicMMORPGPlayerState::ServerLevelUp_Implementation()
{
    if (Stats.Level >= Progression.ExperiencePerLevel.Num()) return;
    
    // Increase level
    Stats.Level++;
    
    // Update stats based on level
    Stats.MaxHealth = Progression.HealthPerLevel[Stats.Level - 1];
    Stats.MaxEnergy = Progression.EnergyPerLevel[Stats.Level - 1];
    Stats.Health = Stats.MaxHealth;
    Stats.Energy = Stats.MaxEnergy;
    
    // Add attribute and skill points
    Stats.AvailableAttributePoints += Progression.AttributePointsPerLevel[Stats.Level - 1];
    Stats.AvailableSkillPoints += Progression.SkillPointsPerLevel[Stats.Level - 1];
    
    UpdateExperienceToNextLevel();
    OnLevelUp.Broadcast(Stats.Level);
}

void AMusicMMORPGPlayerState::OnRep_PlayerName()
{
    // Broadcast player name change
    // This could update UI or other systems
}

void AMusicMMORPGPlayerState::OnRep_CharacterClass()
{
    // Update character based on class
    if (CachedCharacter)
    {
        CachedCharacter->UpdateCharacterClass();
    }
}

void AMusicMMORPGPlayerState::OnRep_Stats()
{
    // Stats changed, update character
    if (CachedCharacter)
    {
        CachedCharacter->OnStatsUpdated();
    }
}

void AMusicMMORPGPlayerState::OnRep_CurrentZone()
{
    // Zone changed
    OnZoneChanged.Broadcast(CurrentZone);
}

void AMusicMMORPGPlayerState::UpdateExperienceToNextLevel()
{
    if (Stats.Level < Progression.ExperiencePerLevel.Num())
    {
        Stats.ExperienceToNextLevel = Progression.ExperiencePerLevel[Stats.Level] - Stats.Experience;
    }
    else
    {
        Stats.ExperienceToNextLevel = 0;
    }
}

void AMusicMMORPGPlayerState::HandleLevelUp()
{
    // Increase level
    Stats.Level++;
    
    // Update stats based on level
    Stats.MaxHealth = Progression.HealthPerLevel[Stats.Level - 1];
    Stats.MaxEnergy = Progression.EnergyPerLevel[Stats.Level - 1];
    Stats.Health = Stats.MaxHealth;
    Stats.Energy = Stats.MaxEnergy;
    
    // Add attribute and skill points
    Stats.AvailableAttributePoints += Progression.AttributePointsPerLevel[Stats.Level - 1];
    Stats.AvailableSkillPoints += Progression.SkillPointsPerLevel[Stats.Level - 1];
    
    UpdateExperienceToNextLevel();
    OnLevelUp.Broadcast(Stats.Level);
}

AMusicMMORPGPlayerState* AMusicMMORPGPlayerState::GetInstance(const UWorld* World)
{
    if (World)
    {
        for (FConstPawnIterator Iterator = World->GetPawnIterator(); Iterator; ++Iterator)
        {
            APawn* Pawn = Iterator->Get();
            if (AMusicMMORPGCharacter* Character = Cast<AMusicMMORPGCharacter>(Pawn))
            {
                if (AMusicMMORPGPlayerState* PlayerState = Character->GetMMORPGPlayerState())
                {
                    return PlayerState;
                }
            }
        }
    }
    return nullptr;
}

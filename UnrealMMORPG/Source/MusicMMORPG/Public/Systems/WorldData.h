pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorldData.generated.h"

// Forward declarations
class AMusicMMORPGCharacter;
class AMMORPGBossCharacter;

// Zone information structure
USTRUCT(BlueprintType)
struct FZoneInfo
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zone")
    FName ZoneName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zone")
    FString DisplayName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zone")
    FString Description;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zone")
    FVector SpawnLocation;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zone")
    FRotator SpawnRotation;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zone")
    int32 MinLevel;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zone")
    int32 MaxLevel;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zone")
    bool bIsPvPEnabled;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zone")
    bool bIsBossZone;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zone")
    TSubclassOf<AMMORPGBossCharacter> BossClass;
    
    // Constructor
    FZoneInfo()
        : ZoneName(NAME_None)
        , DisplayName(TEXT("Unknown"))
        , Description(TEXT(""))
        , SpawnLocation(FVector::ZeroVector)
        , SpawnRotation(FRotator::ZeroRotator)
        , MinLevel(1)
        , MaxLevel(100)
        , bIsPvPEnabled(false)
        , bIsBossZone(false)
        , BossClass(nullptr)
    {
    }
};

// Boss information structure
USTRUCT(BlueprintType)
struct FBossInfo
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
    FName BossName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
    FString DisplayName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
    FString Description;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
    int32 Level;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
    int32 Health;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
    int32 AttackPower;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
    int32 DefensePower;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
    FName ZoneName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
    int64 ExperienceReward;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
    int32 HarmonyGemsReward;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
    TArray<FName> LootTable;
    
    // Constructor
    FBossInfo()
        : BossName(NAME_None)
        , DisplayName(TEXT("Unknown"))
        , Description(TEXT(""))
        , Level(1)
        , Health(1000)
        , AttackPower(50)
        , DefensePower(30)
        , ZoneName(NAME_None)
        , ExperienceReward(1000)
        , HarmonyGemsReward(100)
    {
    }
};

// Item information structure
USTRUCT(BlueprintType)
struct FItemInfo
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    FName ItemName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    FString DisplayName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    FString Description;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    FName ItemType;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    FName ItemCategory;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    int32 ItemLevel;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    int32 Rarity;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    int32 BuyPrice;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    int32 SellPrice;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    bool bIsStackable;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    int32 MaxStackSize;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    TSubclassOf<AActor> ItemClass;
    
    // Constructor
    FItemInfo()
        : ItemName(NAME_None)
        , DisplayName(TEXT("Unknown"))
        , Description(TEXT(""))
        , ItemType(NAME_None)
        , ItemCategory(NAME_None)
        , ItemLevel(1)
        , Rarity(1)
        , BuyPrice(0)
        , SellPrice(0)
        , bIsStackable(false)
        , MaxStackSize(1)
        , ItemClass(nullptr)
    {
    }
};

// Quest information structure
USTRUCT(BlueprintType)
struct FQuestInfo
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
    FName QuestName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
    FString DisplayName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
    FString Description;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
    FName QuestType;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
    int32 QuestLevel;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
    FName StartingZone;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
    FName TargetZone;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
    int32 RequiredKills;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
    int32 RequiredItems;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
    FName RequiredItem;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
    int64 ExperienceReward;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
    int32 HarmonyGemsReward;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
    TArray<FName> ItemRewards;
    
    // Constructor
    FQuestInfo()
        : QuestName(NAME_None)
        , DisplayName(TEXT("Unknown"))
        , Description(TEXT(""))
        , QuestType(NAME_None)
        , QuestLevel(1)
        , StartingZone(NAME_None)
        , TargetZone(NAME_None)
        , RequiredKills(0)
        , RequiredItems(0)
        , RequiredItem(NAME_None)
        , ExperienceReward(0)
        , HarmonyGemsReward(0)
    {
    }
};

/**
 * World Data for Music MMORPG
 * Contains all game world information, zones, bosses, items, and quests
 */
UCLASS(Blueprintable)
class MUSICMMORPG_API UWorldData : public UObject
{
    GENERATED_BODY()

public:
    UWorldData();
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "World Data")
    virtual void Initialize();
    
    // Zone management
    UFUNCTION(BlueprintPure, Category = "World Data")
    FZoneInfo GetZoneInfo(FName ZoneName) const;
    
    UFUNCTION(BlueprintPure, Category = "World Data")
    TArray<FZoneInfo> GetAllZoneInfos() const;
    
    UFUNCTION(BlueprintPure, Category = "World Data")
    bool IsValidZone(FName ZoneName) const;
    
    UFUNCTION(BlueprintPure, Category = "World Data")
    FName GetDefaultZone() const;
    
    // Boss management
    UFUNCTION(BlueprintPure, Category = "World Data")
    FBossInfo GetBossInfo(FName BossName) const;
    
    UFUNCTION(BlueprintPure, Category = "World Data")
    TArray<FBossInfo> GetAllBossInfos() const;
    
    UFUNCTION(BlueprintPure, Category = "World Data")
    TArray<FBossInfo> GetBossesInZone(FName ZoneName) const;
    
    // Item management
    UFUNCTION(BlueprintPure, Category = "World Data")
    FItemInfo GetItemInfo(FName ItemName) const;
    
    UFUNCTION(BlueprintPure, Category = "World Data")
    TArray<FItemInfo> GetAllItemInfos() const;
    
    UFUNCTION(BlueprintPure, Category = "World Data")
    TArray<FItemInfo> GetItemsByCategory(FName Category) const;
    
    UFUNCTION(BlueprintPure, Category = "World Data")
    TArray<FItemInfo> GetItemsByRarity(int32 Rarity) const;
    
    // Quest management
    UFUNCTION(BlueprintPure, Category = "World Data")
    FQuestInfo GetQuestInfo(FName QuestName) const;
    
    UFUNCTION(BlueprintPure, Category = "World Data")
    TArray<FQuestInfo> GetAllQuestInfos() const;
    
    UFUNCTION(BlueprintPure, Category = "World Data")
    TArray<FQuestInfo> GetQuestsByZone(FName ZoneName) const;
    
    UFUNCTION(BlueprintPure, Category = "World Data")
    TArray<FQuestInfo> GetAvailableQuests(int32 PlayerLevel) const;
    
    // Zone data
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "World Data")
    TMap<FName, FZoneInfo> ZoneData;
    
    // Boss data
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "World Data")
    TMap<FName, FBossInfo> BossData;
    
    // Item data
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "World Data")
    TMap<FName, FItemInfo> ItemData;
    
    // Quest data
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "World Data")
    TMap<FName, FQuestInfo> QuestData;
    
    // Default zone
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "World Data")
    FName DefaultZoneName;
    
protected:
    // Initialize default data
    void InitializeDefaultZones();
    void InitializeDefaultBosses();
    void InitializeDefaultItems();
    void InitializeDefaultQuests();
};

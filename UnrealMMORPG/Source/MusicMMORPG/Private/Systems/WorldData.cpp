#include "WorldData.h"

UWorldData::UWorldData()
{
    // Set default zone
    DefaultZoneName = NAME_SoundCity;
    
    // Initialize data
    Initialize();
}

void UWorldData::Initialize()
{
    // Initialize default data
    InitializeDefaultZones();
    InitializeDefaultBosses();
    InitializeDefaultItems();
    InitializeDefaultQuests();
}

FZoneInfo UWorldData::GetZoneInfo(FName ZoneName) const
{
    if (const FZoneInfo* ZoneInfo = ZoneData.Find(ZoneName))
    {
        return *ZoneInfo;
    }
    return FZoneInfo();
}

TArray<FZoneInfo> UWorldData::GetAllZoneInfos() const
{
    TArray<FZoneInfo> Zones;
    for (const auto& Pair : ZoneData)
    {
        Zones.Add(Pair.Value);
    }
    return Zones;
}

bool UWorldData::IsValidZone(FName ZoneName) const
{
    return ZoneData.Contains(ZoneName);
}

FName UWorldData::GetDefaultZone() const
{
    return DefaultZoneName;
}

FBossInfo UWorldData::GetBossInfo(FName BossName) const
{
    if (const FBossInfo* BossInfo = BossData.Find(BossName))
    {
        return *BossInfo;
    }
    return FBossInfo();
}

TArray<FBossInfo> UWorldData::GetAllBossInfos() const
{
    TArray<FBossInfo> Bosses;
    for (const auto& Pair : BossData)
    {
        Bosses.Add(Pair.Value);
    }
    return Bosses;
}

TArray<FBossInfo> UWorldData::GetBossesInZone(FName ZoneName) const
{
    TArray<FBossInfo> ZoneBosses;
    for (const auto& Pair : BossData)
    {
        if (Pair.Value.ZoneName == ZoneName)
        {
            ZoneBosses.Add(Pair.Value);
        }
    }
    return ZoneBosses;
}

FItemInfo UWorldData::GetItemInfo(FName ItemName) const
{
    if (const FItemInfo* ItemInfo = ItemData.Find(ItemName))
    {
        return *ItemInfo;
    }
    return FItemInfo();
}

TArray<FItemInfo> UWorldData::GetAllItemInfos() const
{
    TArray<FItemInfo> Items;
    for (const auto& Pair : ItemData)
    {
        Items.Add(Pair.Value);
    }
    return Items;
}

TArray<FItemInfo> UWorldData::GetItemsByCategory(FName Category) const
{
    TArray<FItemInfo> CategoryItems;
    for (const auto& Pair : ItemData)
    {
        if (Pair.Value.ItemCategory == Category)
        {
            CategoryItems.Add(Pair.Value);
        }
    }
    return CategoryItems;
}

TArray<FItemInfo> UWorldData::GetItemsByRarity(int32 Rarity) const
{
    TArray<FItemInfo> RarityItems;
    for (const auto& Pair : ItemData)
    {
        if (Pair.Value.Rarity == Rarity)
        {
            RarityItems.Add(Pair.Value);
        }
    }
    return RarityItems;
}

FQuestInfo UWorldData::GetQuestInfo(FName QuestName) const
{
    if (const FQuestInfo* QuestInfo = QuestData.Find(QuestName))
    {
        return *QuestInfo;
    }
    return FQuestInfo();
}

TArray<FQuestInfo> UWorldData::GetAllQuestInfos() const
{
    TArray<FQuestInfo> Quests;
    for (const auto& Pair : QuestData)
    {
        Quests.Add(Pair.Value);
    }
    return Quests;
}

TArray<FQuestInfo> UWorldData::GetQuestsByZone(FName ZoneName) const
{
    TArray<FQuestInfo> ZoneQuests;
    for (const auto& Pair : QuestData)
    {
        if (Pair.Value.StartingZone == ZoneName || Pair.Value.TargetZone == ZoneName)
        {
            ZoneQuests.Add(Pair.Value);
        }
    }
    return ZoneQuests;
}

TArray<FQuestInfo> UWorldData::GetAvailableQuests(int32 PlayerLevel) const
{
    TArray<FQuestInfo> AvailableQuests;
    for (const auto& Pair : QuestData)
    {
        if (Pair.Value.QuestLevel <= PlayerLevel)
        {
            AvailableQuests.Add(Pair.Value);
        }
    }
    return AvailableQuests;
}

void UWorldData::InitializeDefaultZones()
{
    // Sound City
    FZoneInfo SoundCity;
    SoundCity.ZoneName = NAME_SoundCity;
    SoundCity.DisplayName = TEXT("Sound City");
    SoundCity.Description = TEXT("The bustling capital of the Music MMORPG world. A place where musicians gather and perform.");
    SoundCity.SpawnLocation = FVector(0.0f, 0.0f, 0.0f);
    SoundCity.SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
    SoundCity.MinLevel = 1;
    SoundCity.MaxLevel = 20;
    SoundCity.bIsPvPEnabled = false;
    SoundCity.bIsBossZone = false;
    ZoneData.Add(SoundCity.ZoneName, SoundCity);
    
    // Bass Caverns
    FZoneInfo BassCaverns;
    BassCaverns.ZoneName = NAME_BassCaverns;
    BassCaverns.DisplayName = TEXT("Bass Caverns");
    BassCaverns.Description = TEXT("A dark underground cavern filled with powerful bass frequencies. Home to the Subwoofer Titan.");
    BassCaverns.SpawnLocation = FVector(1000.0f, 0.0f, 0.0f);
    BassCaverns.SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
    BassCaverns.MinLevel = 15;
    BassCaverns.MaxLevel = 30;
    BassCaverns.bIsPvPEnabled = false;
    BassCaverns.bIsBossZone = true;
    ZoneData.Add(BassCaverns.ZoneName, BassCaverns);
    
    // Synth Citadel
    FZoneInfo SynthCitadel;
    SynthCitadel.ZoneName = NAME_SynthCitadel;
    SynthCitadel.DisplayName = TEXT("Synth Citadel");
    SynthCitadel.Description = TEXT("A futuristic fortress of synthesis. The domain of the Silence Overlord.");
    SynthCitadel.SpawnLocation = FVector(2000.0f, 0.0f, 0.0f);
    SynthCitadel.SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
    SynthCitadel.MinLevel = 25;
    SynthCitadel.MaxLevel = 50;
    SynthCitadel.bIsPvPEnabled = false;
    SynthCitadel.bIsBossZone = true;
    ZoneData.Add(SynthCitadel.ZoneName, SynthCitadel);
    
    // PvP Arena
    FZoneInfo PvPArena;
    PvPArena.ZoneName = NAME_PvPArena;
    PvPArena.DisplayName = TEXT("PvP Arena");
    PvPArena.Description = TEXT("A battleground where players can test their skills against each other.");
    PvPArena.SpawnLocation = FVector(3000.0f, 0.0f, 0.0f);
    PvPArena.SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
    PvPArena.MinLevel = 1;
    PvPArena.MaxLevel = 100;
    PvPArena.bIsPvPEnabled = true;
    PvPArena.bIsBossZone = false;
    ZoneData.Add(PvPArena.ZoneName, PvPArena);
}

void UWorldData::InitializeDefaultBosses()
{
    // Static Spectre
    FBossInfo StaticSpectre;
    StaticSpectre.BossName = NAME_StaticSpectre;
    StaticSpectre.DisplayName = TEXT("Static Spectre");
    StaticSpectre.Description = TEXT("A ghostly entity that disrupts sound waves. The first boss of Sound City.");
    StaticSpectre.Level = 10;
    StaticSpectre.Health = 2000;
    StaticSpectre.AttackPower = 30;
    StaticSpectre.DefensePower = 15;
    StaticSpectre.ZoneName = NAME_SoundCity;
    StaticSpectre.ExperienceReward = 500;
    StaticSpectre.HarmonyGemsReward = 50;
    StaticSpectre.LootTable.Add(NAME_Guitar);
    StaticSpectre.LootTable.Add(NAME_Microphone);
    BossData.Add(StaticSpectre.BossName, StaticSpectre);
    
    // Subwoofer Titan
    FBossInfo SubwooferTitan;
    SubwooferTitan.BossName = NAME_SubwooferTitan;
    SubwooferTitan.DisplayName = TEXT("Subwoofer Titan");
    SubwooferTitan.Description = TEXT("A massive creature that generates earth-shaking bass frequencies. The guardian of Bass Caverns.");
    SubwooferTitan.Level = 25;
    SubwooferTitan.Health = 5000;
    SubwooferTitan.AttackPower = 60;
    SubwooferTitan.DefensePower = 30;
    SubwooferTitan.ZoneName = NAME_BassCaverns;
    SubwooferTitan.ExperienceReward = 1500;
    SubwooferTitan.HarmonyGemsReward = 150;
    SubwooferTitan.LootTable.Add(NAME_BassGuitar);
    SubwooferTitan.LootTable.Add(NAME_Subwoofer);
    BossData.Add(SubwooferTitan.BossName, SubwooferTitan);
    
    // Silence Overlord
    FBossInfo SilenceOverlord;
    SilenceOverlord.BossName = NAME_SilenceOverlord;
    SilenceOverlord.DisplayName = TEXT("Silence Overlord");
    SilenceOverlord.Description = TEXT("The ruler of Synth Citadel. A being that seeks to silence all music in the world.");
    SilenceOverlord.Level = 40;
    SilenceOverlord.Health = 10000;
    SilenceOverlord.AttackPower = 100;
    SilenceOverlord.DefensePower = 50;
    SilenceOverlord.ZoneName = NAME_SynthCitadel;
    SilenceOverlord.ExperienceReward = 3000;
    SilenceOverlord.HarmonyGemsReward = 300;
    SilenceOverlord.LootTable.Add(NAME_Synthesizer);
    SilenceOverlord.LootTable.Add(NAME_MasterKeytar);
    BossData.Add(SilenceOverlord.BossName, SilenceOverlord);
}

void UWorldData::InitializeDefaultItems()
{
    // Weapons - Guitars
    FItemInfo Guitar;
    Guitar.ItemName = NAME_Guitar;
    Guitar.DisplayName = TEXT("Standard Guitar");
    Guitar.Description = TEXT("A basic electric guitar. Perfect for beginners.");
    Guitar.ItemType = NAME_Weapon;
    Guitar.ItemCategory = NAME_Guitar;
    Guitar.ItemLevel = 1;
    Guitar.Rarity = 1;
    Guitar.BuyPrice = 100;
    Guitar.SellPrice = 50;
    Guitar.bIsStackable = false;
    Guitar.MaxStackSize = 1;
    ItemData.Add(Guitar.ItemName, Guitar);
    
    FItemInfo ElectricGuitar;
    ElectricGuitar.ItemName = NAME_ElectricGuitar;
    ElectricGuitar.DisplayName = TEXT("Electric Guitar");
    ElectricGuitar.Description = TEXT("A powerful electric guitar with excellent sound quality.");
    ElectricGuitar.ItemType = NAME_Weapon;
    ElectricGuitar.ItemCategory = NAME_Guitar;
    ElectricGuitar.ItemLevel = 5;
    ElectricGuitar.Rarity = 2;
    ElectricGuitar.BuyPrice = 500;
    ElectricGuitar.SellPrice = 250;
    ElectricGuitar.bIsStackable = false;
    ElectricGuitar.MaxStackSize = 1;
    ItemData.Add(ElectricGuitar.ItemName, ElectricGuitar);
    
    FItemInfo BassGuitar;
    BassGuitar.ItemName = NAME_BassGuitar;
    BassGuitar.DisplayName = TEXT("Bass Guitar");
    BassGuitar.Description = TEXT("A deep-sounding bass guitar. Essential for rhythm sections.");
    BassGuitar.ItemType = NAME_Weapon;
    BassGuitar.ItemCategory = NAME_Guitar;
    BassGuitar.ItemLevel = 10;
    BassGuitar.Rarity = 3;
    BassGuitar.BuyPrice = 1000;
    BassGuitar.SellPrice = 500;
    BassGuitar.bIsStackable = false;
    BassGuitar.MaxStackSize = 1;
    ItemData.Add(BassGuitar.ItemName, BassGuitar);
    
    // Weapons - Microphones
    FItemInfo Microphone;
    Microphone.ItemName = NAME_Microphone;
    Microphone.DisplayName = TEXT("Standard Microphone");
    Microphone.Description = TEXT("A basic microphone for vocal performances.");
    Microphone.ItemType = NAME_Weapon;
    Microphone.ItemCategory = NAME_Microphone;
    Microphone.ItemLevel = 1;
    Microphone.Rarity = 1;
    Microphone.BuyPrice = 100;
    Microphone.SellPrice = 50;
    Microphone.bIsStackable = false;
    Microphone.MaxStackSize = 1;
    ItemData.Add(Microphone.ItemName, Microphone);
    
    FItemInfo CondenserMic;
    CondenserMic.ItemName = NAME_CondenserMic;
    CondenserMic.DisplayName = TEXT("Condenser Microphone");
    CondenserMic.Description = TEXT("A high-quality condenser microphone for professional vocalists.");
    CondenserMic.ItemType = NAME_Weapon;
    CondenserMic.ItemCategory = NAME_Microphone;
    CondenserMic.ItemLevel = 5;
    CondenserMic.Rarity = 2;
    CondenserMic.BuyPrice = 500;
    CondenserMic.SellPrice = 250;
    CondenserMic.bIsStackable = false;
    CondenserMic.MaxStackSize = 1;
    ItemData.Add(CondenserMic.ItemName, CondenserMic);
    
    // Weapons - Keytars
    FItemInfo Keytar;
    Keytar.ItemName = NAME_Keytar;
    Keytar.DisplayName = TEXT("Standard Keytar");
    Keytar.Description = TEXT("A keyboard guitar combination. Perfect for stage performances.");
    Keytar.ItemType = NAME_Weapon;
    Keytar.ItemCategory = NAME_Keytar;
    Keytar.ItemLevel = 1;
    Keytar.Rarity = 1;
    Keytar.BuyPrice = 150;
    Keytar.SellPrice = 75;
    Keytar.bIsStackable = false;
    Keytar.MaxStackSize = 1;
    ItemData.Add(Keytar.ItemName, Keytar);
    
    FItemInfo MasterKeytar;
    MasterKeytar.ItemName = NAME_MasterKeytar;
    MasterKeytar.DisplayName = TEXT("Master Keytar");
    MasterKeytar.Description = TEXT("The ultimate keytar. Used by the greatest performers.");
    MasterKeytar.ItemType = NAME_Weapon;
    MasterKeytar.ItemCategory = NAME_Keytar;
    MasterKeytar.ItemLevel = 15;
    MasterKeytar.Rarity = 4;
    MasterKeytar.BuyPrice = 5000;
    MasterKeytar.SellPrice = 2500;
    MasterKeytar.bIsStackable = false;
    MasterKeytar.MaxStackSize = 1;
    ItemData.Add(MasterKeytar.ItemName, MasterKeytar);
    
    // Weapons - DJ Equipment
    FItemInfo Turntables;
    Turntables.ItemName = NAME_Turntables;
    Turntables.DisplayName = TEXT("Standard Turntables");
    Turntables.Description = TEXT("A basic set of turntables for DJ performances.");
    Turntables.ItemType = NAME_Weapon;
    Turntables.ItemCategory = NAME_Turntables;
    Turntables.ItemLevel = 1;
    Turntables.Rarity = 1;
    Turntables.BuyPrice = 120;
    Turntables.SellPrice = 60;
    Turntables.bIsStackable = false;
    Turntables.MaxStackSize = 1;
    ItemData.Add(Turntables.ItemName, Turntables);
    
    FItemInfo Subwoofer;
    Subwoofer.ItemName = NAME_Subwoofer;
    Subwoofer.DisplayName = TEXT("Powerful Subwoofer");
    Subwoofer.Description = TEXT("A massive subwoofer that produces deep bass frequencies.");
    Subwoofer.ItemType = NAME_Weapon;
    Subwoofer.ItemCategory = NAME_Subwoofer;
    Subwoofer.ItemLevel = 10;
    Subwoofer.Rarity = 3;
    Subwoofer.BuyPrice = 1200;
    Subwoofer.SellPrice = 600;
    Subwoofer.bIsStackable = false;
    Subwoofer.MaxStackSize = 1;
    ItemData.Add(Subwoofer.ItemName, Subwoofer);
    
    // Weapons - Synthesizers
    FItemInfo Synthesizer;
    Synthesizer.ItemName = NAME_Synthesizer;
    Synthesizer.DisplayName = TEXT("Digital Synthesizer");
    Synthesizer.Description = TEXT("A powerful digital synthesizer for creating electronic music.");
    Synthesizer.ItemType = NAME_Weapon;
    Synthesizer.ItemCategory = NAME_Synthesizer;
    Synthesizer.ItemLevel = 10;
    Synthesizer.Rarity = 3;
    Synthesizer.BuyPrice = 1500;
    Synthesizer.SellPrice = 750;
    Synthesizer.bIsStackable = false;
    Synthesizer.MaxStackSize = 1;
    ItemData.Add(Synthesizer.ItemName, Synthesizer);
    
    // Consumables - Health
    FItemInfo EnergyElixir;
    EnergyElixir.ItemName = NAME_EnergyElixir;
    EnergyElixir.DisplayName = TEXT("Energy Elixir");
    EnergyElixir.Description = TEXT("Restores 50 energy points.");
    EnergyElixir.ItemType = NAME_Consumable;
    EnergyElixir.ItemCategory = NAME_Health;
    EnergyElixir.ItemLevel = 1;
    EnergyElixir.Rarity = 1;
    EnergyElixir.BuyPrice = 50;
    EnergyElixir.SellPrice = 25;
    EnergyElixir.bIsStackable = true;
    EnergyElixir.MaxStackSize = 10;
    ItemData.Add(EnergyElixir.ItemName, EnergyElixir);
    
    FItemInfo GreaterEnergyElixir;
    GreaterEnergyElixir.ItemName = NAME_GreaterEnergyElixir;
    GreaterEnergyElixir.DisplayName = TEXT("Greater Energy Elixir");
    GreaterEnergyElixir.Description = TEXT("Restores 100 energy points.");
    GreaterEnergyElixir.ItemType = NAME_Consumable;
    GreaterEnergyElixir.ItemCategory = NAME_Health;
    GreaterEnergyElixir.ItemLevel = 5;
    GreaterEnergyElixir.Rarity = 2;
    GreaterEnergyElixir.BuyPrice = 150;
    GreaterEnergyElixir.SellPrice = 75;
    GreaterEnergyElixir.bIsStackable = true;
    GreaterEnergyElixir.MaxStackSize = 10;
    ItemData.Add(GreaterEnergyElixir.ItemName, GreaterEnergyElixir);
    
    // Consumables - Harmony Gems
    FItemInfo HarmonyGem;
    HarmonyGem.ItemName = NAME_HarmonyGem;
    HarmonyGem.DisplayName = TEXT("Harmony Gem");
    HarmonyGem.Description = TEXT("A valuable gem that can be used for various purposes.");
    HarmonyGem.ItemType = NAME_Currency;
    HarmonyGem.ItemCategory = NAME_Gem;
    HarmonyGem.ItemLevel = 1;
    HarmonyGem.Rarity = 1;
    HarmonyGem.BuyPrice = 100;
    HarmonyGem.SellPrice = 50;
    HarmonyGem.bIsStackable = true;
    HarmonyGem.MaxStackSize = 100;
    ItemData.Add(HarmonyGem.ItemName, HarmonyGem);
    
    // Armor
    FItemInfo ThunderAmp;
    ThunderAmp.ItemName = NAME_ThunderAmp;
    ThunderAmp.DisplayName = TEXT("Thunder Amp");
    ThunderAmp.Description = TEXT("A powerful amplifier that boosts your attack power.");
    ThunderAmp.ItemType = NAME_Armor;
    ThunderAmp.ItemCategory = NAME_Amp;
    ThunderAmp.ItemLevel = 5;
    ThunderAmp.Rarity = 2;
    ThunderAmp.BuyPrice = 300;
    ThunderAmp.SellPrice = 150;
    ThunderAmp.bIsStackable = false;
    ThunderAmp.MaxStackSize = 1;
    ItemData.Add(ThunderAmp.ItemName, ThunderAmp);
    
    FItemInfo DistortionPedal;
    DistortionPedal.ItemName = NAME_DistortionPedal;
    DistortionPedal.DisplayName = TEXT("Distortion Pedal");
    DistortionPedal.Description = TEXT("A pedal that adds distortion to your sound, increasing damage.");
    DistortionPedal.ItemType = NAME_Armor;
    DistortionPedal.ItemCategory = NAME_Pedal;
    DistortionPedal.ItemLevel = 5;
    DistortionPedal.Rarity = 2;
    DistortionPedal.BuyPrice = 250;
    DistortionPedal.SellPrice = 125;
    DistortionPedal.bIsStackable = false;
    DistortionPedal.MaxStackSize = 1;
    ItemData.Add(DistortionPedal.ItemName, DistortionPedal);
}

void UWorldData::InitializeDefaultQuests()
{
    // Sound City Quests
    FQuestInfo FirstPerformance;
    FirstPerformance.QuestName = NAME_FirstPerformance;
    FirstPerformance.DisplayName = TEXT("First Performance");
    FirstPerformance.Description = TEXT("Perform your first song in Sound City to prove your worth as a musician.");
    FirstPerformance.QuestType = NAME_Performance;
    FirstPerformance.QuestLevel = 1;
    FirstPerformance.StartingZone = NAME_SoundCity;
    FirstPerformance.TargetZone = NAME_SoundCity;
    FirstPerformance.RequiredKills = 0;
    FirstPerformance.RequiredItems = 0;
    FirstPerformance.ExperienceReward = 100;
    FirstPerformance.HarmonyGemsReward = 10;
    FirstPerformance.ItemRewards.Add(NAME_Guitar);
    QuestData.Add(FirstPerformance.QuestName, FirstPerformance);
    
    FQuestInfo MeetTheBand;
    MeetTheBand.QuestName = NAME_MeetTheBand;
    MeetTheBand.DisplayName = TEXT("Meet the Band");
    MeetTheBand.Description = TEXT("Find and talk to the members of the Cyber Beats Guild in Sound City.");
    MeetTheBand.QuestType = NAME_Social;
    MeetTheBand.QuestLevel = 2;
    MeetTheBand.StartingZone = NAME_SoundCity;
    MeetTheBand.TargetZone = NAME_SoundCity;
    MeetTheBand.RequiredKills = 0;
    MeetTheBand.RequiredItems = 0;
    MeetTheBand.ExperienceReward = 200;
    MeetTheBand.HarmonyGemsReward = 20;
    QuestData.Add(MeetTheBand.QuestName, MeetTheBand);
    
    FQuestInfo StaticSpectreQuest;
    StaticSpectreQuest.QuestName = NAME_StaticSpectreQuest;
    StaticSpectreQuest.DisplayName = TEXT("Silence the Static");
    StaticSpectreQuest.Description = TEXT("Defeat the Static Spectre that has been disrupting performances in Sound City.");
    StaticSpectreQuest.QuestType = NAME_Combat;
    StaticSpectreQuest.QuestLevel = 5;
    StaticSpectreQuest.StartingZone = NAME_SoundCity;
    StaticSpectreQuest.TargetZone = NAME_SoundCity;
    StaticSpectreQuest.RequiredKills = 1;
    StaticSpectreQuest.RequiredItems = 0;
    StaticSpectreQuest.ExperienceReward = 500;
    StaticSpectreQuest.HarmonyGemsReward = 50;
    StaticSpectreQuest.ItemRewards.Add(NAME_ElectricGuitar);
    QuestData.Add(StaticSpectreQuest.QuestName, StaticSpectreQuest);
    
    // Bass Caverns Quests
    FQuestInfo ExploreBassCaverns;
    ExploreBassCaverns.QuestName = NAME_ExploreBassCaverns;
    ExploreBassCaverns.DisplayName = TEXT("Explore Bass Caverns");
    ExploreBassCaverns.Description = TEXT("Explore the dark Bass Caverns and discover its secrets.");
    ExploreBassCaverns.QuestType = NAME_Exploration;
    ExploreBassCaverns.QuestLevel = 10;
    ExploreBassCaverns.StartingZone = NAME_SoundCity;
    ExploreBassCaverns.TargetZone = NAME_BassCaverns;
    ExploreBassCaverns.RequiredKills = 0;
    ExploreBassCaverns.RequiredItems = 0;
    ExploreBassCaverns.ExperienceReward = 800;
    ExploreBassCaverns.HarmonyGemsReward = 80;
    QuestData.Add(ExploreBassCaverns.QuestName, ExploreBassCaverns);
    
    FQuestInfo SubwooferTitanQuest;
    SubwooferTitanQuest.QuestName = NAME_SubwooferTitanQuest;
    SubwooferTitanQuest.DisplayName = TEXT("Tame the Titan");
    SubwooferTitanQuest.Description = TEXT("Defeat the Subwoofer Titan in Bass Caverns.");
    SubwooferTitanQuest.QuestType = NAME_Combat;
    SubwooferTitanQuest.QuestLevel = 15;
    SubwooferTitanQuest.StartingZone = NAME_BassCaverns;
    SubwooferTitanQuest.TargetZone = NAME_BassCaverns;
    SubwooferTitanQuest.RequiredKills = 1;
    SubwooferTitanQuest.RequiredItems = 0;
    SubwooferTitanQuest.ExperienceReward = 1500;
    SubwooferTitanQuest.HarmonyGemsReward = 150;
    SubwooferTitanQuest.ItemRewards.Add(NAME_BassGuitar);
    QuestData.Add(SubwooferTitanQuest.QuestName, SubwooferTitanQuest);
    
    // Synth Citadel Quests
    FQuestInfo EnterSynthCitadel;
    EnterSynthCitadel.QuestName = NAME_EnterSynthCitadel;
    EnterSynthCitadel.DisplayName = TEXT("Enter Synth Citadel");
    EnterSynthCitadel.Description = TEXT("Find a way to enter the fortified Synth Citadel.");
    EnterSynthCitadel.QuestType = NAME_Exploration;
    EnterSynthCitadel.QuestLevel = 20;
    EnterSynthCitadel.StartingZone = NAME_SoundCity;
    EnterSynthCitadel.TargetZone = NAME_SynthCitadel;
    EnterSynthCitadel.RequiredKills = 0;
    EnterSynthCitadel.RequiredItems = 0;
    EnterSynthCitadel.ExperienceReward = 1000;
    EnterSynthCitadel.HarmonyGemsReward = 100;
    QuestData.Add(EnterSynthCitadel.QuestName, EnterSynthCitadel);
    
    FQuestInfo SilenceOverlordQuest;
    SilenceOverlordQuest.QuestName = NAME_SilenceOverlordQuest;
    SilenceOverlordQuest.DisplayName = TEXT("Defeat the Silence Overlord");
    SilenceOverlordQuest.Description = TEXT("Defeat the Silence Overlord and save the world of music from eternal silence.");
    SilenceOverlordQuest.QuestType = NAME_Combat;
    SilenceOverlordQuest.QuestLevel = 30;
    SilenceOverlordQuest.StartingZone = NAME_SynthCitadel;
    SilenceOverlordQuest.TargetZone = NAME_SynthCitadel;
    SilenceOverlordQuest.RequiredKills = 1;
    SilenceOverlordQuest.RequiredItems = 0;
    SilenceOverlordQuest.ExperienceReward = 3000;
    SilenceOverlordQuest.HarmonyGemsReward = 300;
    SilenceOverlordQuest.ItemRewards.Add(NAME_Synthesizer);
    SilenceOverlordQuest.ItemRewards.Add(NAME_MasterKeytar);
    QuestData.Add(SilenceOverlordQuest.QuestName, SilenceOverlordQuest);
}

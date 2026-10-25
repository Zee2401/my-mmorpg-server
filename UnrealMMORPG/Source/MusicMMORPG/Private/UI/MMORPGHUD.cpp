#include "MMORPGHUD.h"
#include "MusicMMORPGPlayerController.h"
#include "MusicMMORPGCharacter.h"
#include "MusicMMORPGPlayerState.h"
#include "MusicMMORPGGameState.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

UMMORPGHUD::UMMORPGHUD()
{
    // Set default widget classes (these should be set in the editor)
    static ConstructorHelpers::FClassFinder<UUserWidget> MainHUDWidgetClassFinder(TEXT("/Game/UI/WBP_MainHUD"));
    if (MainHUDWidgetClassFinder.Succeeded())
    {
        MainHUDWidgetClass = MainHUDWidgetClassFinder.Class;
    }
    
    static ConstructorHelpers::FClassFinder<UUserWidget> InventoryWidgetClassFinder(TEXT("/Game/UI/WBP_Inventory"));
    if (InventoryWidgetClassFinder.Succeeded())
    {
        InventoryWidgetClass = InventoryWidgetClassFinder.Class;
    }
    
    static ConstructorHelpers::FClassFinder<UUserWidget> MapWidgetClassFinder(TEXT("/Game/UI/WBP_Map"));
    if (MapWidgetClassFinder.Succeeded())
    {
        MapWidgetClass = MapWidgetClassFinder.Class;
    }
    
    static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetClassFinder(TEXT("/Game/UI/WBP_Menu"));
    if (MenuWidgetClassFinder.Succeeded())
    {
        MenuWidgetClass = MenuWidgetClassFinder.Class;
    }
    
    static ConstructorHelpers::FClassFinder<UUserWidget> ChatWidgetClassFinder(TEXT("/Game/UI/WBP_Chat"));
    if (ChatWidgetClassFinder.Succeeded())
    {
        ChatWidgetClass = ChatWidgetClassFinder.Class;
    }
    
    static ConstructorHelpers::FClassFinder<UUserWidget> SocialWidgetClassFinder(TEXT("/Game/UI/WBP_Social"));
    if (SocialWidgetClassFinder.Succeeded())
    {
        SocialWidgetClass = SocialWidgetClassFinder.Class;
    }
    
    static ConstructorHelpers::FClassFinder<UUserWidget> QuestWidgetClassFinder(TEXT("/Game/UI/WBP_Quest"));
    if (QuestWidgetClassFinder.Succeeded())
    {
        QuestWidgetClass = QuestWidgetClassFinder.Class;
    }
    
    static ConstructorHelpers::FClassFinder<UUserWidget> ShopWidgetClassFinder(TEXT("/Game/UI/WBP_Shop"));
    if (ShopWidgetClassFinder.Succeeded())
    {
        ShopWidgetClass = ShopWidgetClassFinder.Class;
    }
    
    // Initialize widget instances
    MainHUDWidget = nullptr;
    InventoryWidget = nullptr;
    MapWidget = nullptr;
    MenuWidget = nullptr;
    ChatWidget = nullptr;
    SocialWidget = nullptr;
    QuestWidget = nullptr;
    ShopWidget = nullptr;
    
    PlayerController = nullptr;
}

void UMMORPGHUD::BeginPlay()
{
    Super::BeginPlay();
}

void UMMORPGHUD::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void UMMORPGHUD::DrawHUD()
{
    Super::DrawHUD();
    
    // Draw HUD elements
    DrawHealthBar();
    DrawEnergyBar();
    DrawExperienceBar();
    DrawChat();
    DrawNotifications();
    DrawPlayerInfo();
    DrawTargetInfo();
    DrawMinimap();
}

void UMMORPGHUD::InitializeHUD(AMusicMMORPGPlayerController* InPlayerController)
{
    PlayerController = InPlayerController;
    
    // Create main HUD widget
    if (MainHUDWidgetClass)
    {
        MainHUDWidget = CreateWidget(GetWorld(), MainHUDWidgetClass);
        if (MainHUDWidget)
        {
            MainHUDWidget->AddToViewport();
        }
    }
    
    // Update player info
    UpdatePlayerInfo();
}

void UMMORPGHUD::ToggleHUD()
{
    if (MainHUDWidget)
    {
        if (MainHUDWidget->IsInViewport())
        {
            MainHUDWidget->RemoveFromViewport();
        }
        else
        {
            MainHUDWidget->AddToViewport();
        }
    }
}

void UMMORPGHUD::ToggleInventory()
{
    if (!InventoryWidget && InventoryWidgetClass)
    {
        InventoryWidget = CreateWidget(GetWorld(), InventoryWidgetClass);
    }
    
    if (InventoryWidget)
    {
        if (InventoryWidget->IsInViewport())
        {
            InventoryWidget->RemoveFromViewport();
        }
        else
        {
            InventoryWidget->AddToViewport();
        }
    }
}

void UMMORPGHUD::ToggleMap()
{
    if (!MapWidget && MapWidgetClass)
    {
        MapWidget = CreateWidget(GetWorld(), MapWidgetClass);
    }
    
    if (MapWidget)
    {
        if (MapWidget->IsInViewport())
        {
            MapWidget->RemoveFromViewport();
        }
        else
        {
            MapWidget->AddToViewport();
        }
    }
}

void UMMORPGHUD::ToggleMenu()
{
    if (!MenuWidget && MenuWidgetClass)
    {
        MenuWidget = CreateWidget(GetWorld(), MenuWidgetClass);
    }
    
    if (MenuWidget)
    {
        if (MenuWidget->IsInViewport())
        {
            MenuWidget->RemoveFromViewport();
        }
        else
        {
            MenuWidget->AddToViewport();
        }
    }
}

void UMMORPGHUD::ToggleChat()
{
    if (!ChatWidget && ChatWidgetClass)
    {
        ChatWidget = CreateWidget(GetWorld(), ChatWidgetClass);
    }
    
    if (ChatWidget)
    {
        if (ChatWidget->IsInViewport())
        {
            ChatWidget->RemoveFromViewport();
        }
        else
        {
            ChatWidget->AddToViewport();
        }
    }
}

void UMMORPGHUD::ToggleSocial()
{
    if (!SocialWidget && SocialWidgetClass)
    {
        SocialWidget = CreateWidget(GetWorld(), SocialWidgetClass);
    }
    
    if (SocialWidget)
    {
        if (SocialWidget->IsInViewport())
        {
            SocialWidget->RemoveFromViewport();
        }
        else
        {
            SocialWidget->AddToViewport();
        }
    }
}

void UMMORPGHUD::ToggleQuest()
{
    if (!QuestWidget && QuestWidgetClass)
    {
        QuestWidget = CreateWidget(GetWorld(), QuestWidgetClass);
    }
    
    if (QuestWidget)
    {
        if (QuestWidget->IsInViewport())
        {
            QuestWidget->RemoveFromViewport();
        }
        else
        {
            QuestWidget->AddToViewport();
        }
    }
}

void UMMORPGHUD::ToggleShop()
{
    if (!ShopWidget && ShopWidgetClass)
    {
        ShopWidget = CreateWidget(GetWorld(), ShopWidgetClass);
    }
    
    if (ShopWidget)
    {
        if (ShopWidget->IsInViewport())
        {
            ShopWidget->RemoveFromViewport();
        }
        else
        {
            ShopWidget->AddToViewport();
        }
    }
}

void UMMORPGHUD::AddChatMessage(const FText& Message, const FString& Sender)
{
    ChatMessages.Add(Message);
    ChatMessageSenders.Add(Sender);
    
    // Limit chat history
    if (ChatMessages.Num() > 100)
    {
        ChatMessages.RemoveAt(0);
        ChatMessageSenders.RemoveAt(0);
    }
    
    OnChatMessageAdded.Broadcast(Message);
}

void UMMORPGHUD::SendChatMessage(const FText& Message)
{
    if (PlayerController)
    {
        PlayerController->ServerSendGlobalMessage(Message);
    }
}

void UMMORPGHUD::ShowNotification(const FText& Message)
{
    OnNotificationShown.Broadcast(Message);
}

void UMMORPGHUD::OnZoneChanged(FName NewZone)
{
    OnZoneChangedEvent.Broadcast(NewZone);
}

void UMMORPGHUD::UpdatePlayerName(const FString& NewName)
{
    // Update player name in UI
}

void UMMORPGHUD::UpdatePlayerLevel(int32 NewLevel)
{
    // Update player level in UI
}

void UMMORPGHUD::UpdatePlayerHealth(int32 NewHealth, int32 MaxHealth)
{
    // Update health in UI
}

void UMMORPGHUD::UpdatePlayerEnergy(int32 NewEnergy, int32 MaxEnergy)
{
    // Update energy in UI
}

void UMMORPGHUD::UpdatePlayerExperience(int64 NewExperience, int64 ExperienceToNextLevel)
{
    // Update experience in UI
}

void UMMORPGHUD::UpdateHarmonyGems(int32 NewAmount)
{
    // Update harmony gems in UI
}

void UMMORPGHUD::DrawHealthBar()
{
    if (!PlayerController) return;
    
    if (AMusicMMORPGCharacter* Character = PlayerController->GetMMORPGCharacter())
    {
        if (AMusicMMORPGPlayerState* PlayerState = Character->GetMMORPGPlayerState())
        {
            FPlayerStats Stats = PlayerState->GetPlayerStats();
            
            float HealthPercent = Stats.Health / (float)Stats.MaxHealth;
            
            // Draw health bar
            FCanvasTextItem TextItem(FText::FromString(FString::Printf(TEXT("HP: %d/%d"), Stats.Health, Stats.MaxHealth)), GEngine->GetSmallFont(), FLinearColor::White);
            TextItem.Scale = FVector2D(0.8f, 0.8f);
            Canvas->DrawItem(TextItem, 50.0f, 50.0f);
            
            // Draw health bar background
            Canvas->DrawBox(FVector2D(50.0f, 70.0f), FVector2D(200.0f, 20.0f), FLinearColor::Black);
            
            // Draw health bar fill
            Canvas->DrawBox(FVector2D(50.0f, 70.0f), FVector2D(200.0f * HealthPercent, 20.0f), FLinearColor::Green);
        }
    }
}

void UMMORPGHUD::DrawEnergyBar()
{
    if (!PlayerController) return;
    
    if (AMusicMMORPGCharacter* Character = PlayerController->GetMMORPGCharacter())
    {
        if (AMusicMMORPGPlayerState* PlayerState = Character->GetMMORPGPlayerState())
        {
            FPlayerStats Stats = PlayerState->GetPlayerStats();
            
            float EnergyPercent = Stats.Energy / (float)Stats.MaxEnergy;
            
            // Draw energy bar
            FCanvasTextItem TextItem(FText::FromString(FString::Printf(TEXT("Energy: %d/%d"), Stats.Energy, Stats.MaxEnergy)), GEngine->GetSmallFont(), FLinearColor::White);
            TextItem.Scale = FVector2D(0.8f, 0.8f);
            Canvas->DrawItem(TextItem, 50.0f, 100.0f);
            
            // Draw energy bar background
            Canvas->DrawBox(FVector2D(50.0f, 120.0f), FVector2D(200.0f, 20.0f), FLinearColor::Black);
            
            // Draw energy bar fill
            Canvas->DrawBox(FVector2D(50.0f, 120.0f), FVector2D(200.0f * EnergyPercent, 20.0f), FLinearColor::Blue);
        }
    }
}

void UMMORPGHUD::DrawExperienceBar()
{
    if (!PlayerController) return;
    
    if (AMusicMMORPGCharacter* Character = PlayerController->GetMMORPGCharacter())
    {
        if (AMusicMMORPGPlayerState* PlayerState = Character->GetMMORPGPlayerState())
        {
            FPlayerStats Stats = PlayerState->GetPlayerStats();
            
            float ExperiencePercent = Stats.Experience / (float)Stats.ExperienceToNextLevel;
            
            // Draw experience bar
            FCanvasTextItem TextItem(FText::FromString(FString::Printf(TEXT("Level: %d, EXP: %lld/%lld"), Stats.Level, Stats.Experience, Stats.ExperienceToNextLevel)), GEngine->GetSmallFont(), FLinearColor::White);
            TextItem.Scale = FVector2D(0.8f, 0.8f);
            Canvas->DrawItem(TextItem, 50.0f, 150.0f);
            
            // Draw experience bar background
            Canvas->DrawBox(FVector2D(50.0f, 170.0f), FVector2D(200.0f, 20.0f), FLinearColor::Black);
            
            // Draw experience bar fill
            Canvas->DrawBox(FVector2D(50.0f, 170.0f), FVector2D(200.0f * ExperiencePercent, 20.0f), FLinearColor::Yellow);
        }
    }
}

void UMMORPGHUD::DrawChat()
{
    // Draw chat messages
    float YPos = Canvas->SizeY - 200.0f;
    
    for (int32 i = FMath::Max(0, ChatMessages.Num() - 10); i < ChatMessages.Num(); i++)
    {
        FText Message = FText::Format(NSLOCTEXT("MMORPG", "ChatMessage", "{0}: {1}"), 
                                       FText::FromString(ChatMessageSenders[i]), 
                                       ChatMessages[i]);
        
        FCanvasTextItem TextItem(Message, GEngine->GetSmallFont(), FLinearColor::White);
        TextItem.Scale = FVector2D(0.7f, 0.7f);
        Canvas->DrawItem(TextItem, 50.0f, YPos);
        
        YPos -= 20.0f;
    }
}

void UMMORPGHUD::DrawNotifications()
{
    // Notifications would be drawn here
    // In a real implementation, this would show temporary notifications
}

void UMMORPGHUD::DrawPlayerInfo()
{
    if (!PlayerController) return;
    
    if (AMusicMMORPGCharacter* Character = PlayerController->GetMMORPGCharacter())
    {
        if (AMusicMMORPGPlayerState* PlayerState = Character->GetMMORPGPlayerState())
        {
            FPlayerStats Stats = PlayerState->GetPlayerStats();
            
            // Draw player name
            FCanvasTextItem NameText(FText::FromString(PlayerState->GetPlayerName()), GEngine->GetMediumFont(), FLinearColor::White);
            NameText.Scale = FVector2D(1.0f, 1.0f);
            Canvas->DrawItem(NameText, Canvas->SizeX - 200.0f, 50.0f);
            
            // Draw class
            FString ClassName;
            switch (PlayerState->GetCharacterClass())
            {
                case ECharacterClass::Bard: ClassName = TEXT("Bard"); break;
                case ECharacterClass::Beatmaker: ClassName = TEXT("Beatmaker"); break;
                case ECharacterClass::Vocalist: ClassName = TEXT("Vocalist"); break;
                case ECharacterClass::Guitarist: ClassName = TEXT("Guitarist"); break;
                case ECharacterClass::DJ: ClassName = TEXT("DJ"); break;
                default: ClassName = TEXT("Unknown"); break;
            }
            
            FCanvasTextItem ClassText(FText::FromString(ClassName), GEngine->GetSmallFont(), FLinearColor::Yellow);
            ClassText.Scale = FVector2D(0.8f, 0.8f);
            Canvas->DrawItem(ClassText, Canvas->SizeX - 200.0f, 80.0f);
            
            // Draw harmony gems
            FCanvasTextItem GemsText(FText::FromString(FString::Printf(TEXT("Harmony Gems: %d"), Stats.HarmonyGems)), GEngine->GetSmallFont(), FLinearColor::Cyan);
            GemsText.Scale = FVector2D(0.8f, 0.8f);
            Canvas->DrawItem(GemsText, Canvas->SizeX - 200.0f, 110.0f);
        }
    }
}

void UMMORPGHUD::DrawTargetInfo()
{
    if (!PlayerController) return;
    
    if (AMusicMMORPGCharacter* Character = PlayerController->GetMMORPGCharacter())
    {
        if (AActor* Target = Character->GetTarget())
        {
            // Draw target info
            FCanvasTextItem TargetText(FText::FromString(Target->GetName()), GEngine->GetSmallFont(), FLinearColor::Red);
            TargetText.Scale = FVector2D(0.8f, 0.8f);
            Canvas->DrawItem(TargetText, Canvas->SizeX / 2 - 50.0f, Canvas->SizeY / 2 - 50.0f);
        }
    }
}

void UMMORPGHUD::DrawMinimap()
{
    // Draw minimap background
    Canvas->DrawBox(FVector2D(Canvas->SizeX - 250.0f, Canvas->SizeY - 250.0f), 
                   FVector2D(200.0f, 200.0f), 
                   FLinearColor::Black);
    
    // Draw minimap border
    Canvas->DrawBox(FVector2D(Canvas->SizeX - 250.0f, Canvas->SizeY - 250.0f), 
                   FVector2D(200.0f, 200.0f), 
                   FLinearColor::White, 
                   1.0f);
}

void UMMORPGHUD::UpdatePlayerInfo()
{
    if (!PlayerController) return;
    
    if (AMusicMMORPGCharacter* Character = PlayerController->GetMMORPGCharacter())
    {
        if (AMusicMMORPGPlayerState* PlayerState = Character->GetMMORPGPlayerState())
        {
            FPlayerStats Stats = PlayerState->GetPlayerStats();
            
            UpdatePlayerName(PlayerState->GetPlayerName());
            UpdatePlayerLevel(Stats.Level);
            UpdatePlayerHealth(Stats.Health, Stats.MaxHealth);
            UpdatePlayerEnergy(Stats.Energy, Stats.MaxEnergy);
            UpdatePlayerExperience(Stats.Experience, Stats.ExperienceToNextLevel);
            UpdateHarmonyGems(Stats.HarmonyGems);
        }
    }
}

pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MusicMMORPGPlayerController.generated.h"

// Forward declarations
class AMusicMMORPGCharacter;
class AMusicMMORPGPlayerState;
class AMusicMMORPGGameMode;
class AMusicMMORPGGameState;

/**
 * Player Controller for Music MMORPG
 * Handles player input, UI, and client-server communication
 */
UCLASS()
class MUSICMMORPG_API AMusicMMORPGPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AMusicMMORPGPlayerController();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;
    virtual void PlayerTick(float DeltaTime) override;
    virtual void Possess(APawn* InPawn) override;
    virtual void UnPossess() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Player Controller")
    virtual void InitializePlayer();
    
    // UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UMMORPGHUD> HUDClass;
    
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    class UMMORPGHUD* MMOHUD;
    
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowHUD();
    
    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideHUD();
    
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ToggleHUD();
    
    // Input
    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetupInputBindings();
    
    UFUNCTION(BlueprintCallable, Category = "Input")
    void BindActionEvents();
    
    // Network functions
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerRequestSpawn(const FTransform& SpawnTransform);
    
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerRequestKick(APlayerController* PlayerController, const FText& Reason);
    
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerRequestBan(APlayerController* PlayerController, const FText& Reason);
    
    UFUNCTION(Server, Reliable)
    void ServerSendGlobalMessage(const FText& Message);
    
    UFUNCTION(Server, Reliable)
    void ServerChangeZone(const FName& ZoneName);
    
    UFUNCTION(Server, Reliable)
    void ServerTeleportToZone(const FName& ZoneName, const FVector& Location, const FRotator& Rotation);
    
    UFUNCTION(Server, Reliable)
    void ServerUseAbility(int32 AbilityIndex);
    
    UFUNCTION(Server, Reliable)
    void ServerInteract(AActor* TargetActor);
    
    // Client functions
    UFUNCTION(Client, Reliable)
    void ClientKickNotification(const FText& Reason);
    
    UFUNCTION(Client, Reliable)
    void ClientReceiveGlobalMessage(const FText& Message, APlayerController* Sender);
    
    UFUNCTION(Client, Reliable)
    void ClientChangeZone(const FName& ZoneName);
    
    UFUNCTION(Client, Reliable)
    void ClientTeleportToZone(const FName& ZoneName, const FVector& Location, const FRotator& Rotation);
    
    UFUNCTION(Client, Reliable)
    void ClientShowNotification(const FText& Message);
    
    // Player state access
    UFUNCTION(BlueprintPure, Category = "Player Controller")
    AMusicMMORPGPlayerState* GetMMORPGPlayerState() const;
    
    UFUNCTION(BlueprintPure, Category = "Player Controller")
    AMusicMMORPGCharacter* GetMMORPGCharacter() const;
    
    UFUNCTION(BlueprintPure, Category = "Player Controller")
    AMusicMMORPGGameMode* GetMMORPGGameMode() const;
    
    UFUNCTION(BlueprintPure, Category = "Player Controller")
    AMusicMMORPGGameState* GetMMORPGGameState() const;
    
    // Helper functions
    UFUNCTION(BlueprintPure, Category = "Player Controller")
    bool IsLocalPlayerController() const;
    
    UFUNCTION(BlueprintPure, Category = "Player Controller")
    bool IsServer() const;
    
    UFUNCTION(BlueprintPure, Category = "Player Controller")
    bool IsClient() const;
    
    // Static instance
    static AMusicMMORPGPlayerController* GetInstance(const UWorld* World);
    
protected:
    // Input component
    UPROPERTY()
    class UMMORPGInputComponent* MMOInputComponent;
    
    // Replicated data
    UPROPERTY(Replicated)
    bool bIsReady;
    
    UPROPERTY(Replicated)
    FString PlayerName;
    
    UPROPERTY(Replicated)
    FName CurrentZone;
    
    // Input callbacks
    void OnMoveForward(float Value);
    void OnMoveRight(float Value);
    void OnLookUp(float Value);
    void OnLookRight(float Value);
    void OnJumpStart();
    void OnJumpStop();
    void OnRunStart();
    void OnRunStop();
    void OnSprintStart();
    void OnSprintStop();
    void OnCrouchStart();
    void OnCrouchStop();
    void OnAttack();
    void OnAbility1();
    void OnAbility2();
    void OnAbility3();
    void OnAbility4();
    void OnInteract();
    void OnInventory();
    void OnMap();
    void OnMenu();
    void OnChat();
    void OnSocial();
    void OnQuest();
    void OnShop();
    
    // Replication callbacks
    virtual void OnRep_PlayerName();
    virtual void OnRep_CurrentZone();
    virtual void OnRep_bIsReady();
    
    // Helper functions
    void UpdateRotation(float DeltaTime);
};

pragma once

#include "CoreMinimal.h"
#include "Components/InputComponent.h"
#include "MMORPGInputComponent.generated.h"

// Delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveSignature, FVector2D, InputVector);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLookSignature, FVector2D, InputVector);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJumpSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilitySignature, int32, AbilityIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractSignature, AActor*, TargetActor);

/**
 * Custom Input Component for Music MMORPG
 * Handles input binding and processing
 */
UCLASS()
class MUSICMMORPG_API UMMORPGInputComponent : public UInputComponent
{
    GENERATED_BODY()

public:
    UMMORPGInputComponent();
    
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    // Input binding
    UFUNCTION(BlueprintCallable, Category = "Input")
    void BindInputActions();
    
    // Input processing
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ProcessMovementInput(float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ProcessLookInput(float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ProcessActionInput(float DeltaTime);
    
    // Input state
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    FVector2D MovementInput;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    FVector2D LookInput;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bJumpPressed;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bJumpReleased;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bAttackPressed;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bRunPressed;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bSprintPressed;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    bool bCrouchPressed;
    
    UPROPERTY(BlueprintReadOnly, Category = "Input")
    TArray<bool> AbilityPressed;
    
    // Input events
    UPROPERTY(BlueprintAssignable, Category = "Input")
    FOnMoveSignature OnMoveForward;
    
    UPROPERTY(BlueprintAssignable, Category = "Input")
    FOnMoveSignature OnMoveRight;
    
    UPROPERTY(BlueprintAssignable, Category = "Input")
    FOnLookSignature OnLookUp;
    
    UPROPERTY(BlueprintAssignable, Category = "Input")
    FOnLookSignature OnLookRight;
    
    UPROPERTY(BlueprintAssignable, Category = "Input")
    FOnJumpSignature OnJump;
    
    UPROPERTY(BlueprintAssignable, Category = "Input")
    FOnAttackSignature OnAttack;
    
    UPROPERTY(BlueprintAssignable, Category = "Input")
    FOnAbilitySignature OnAbility;
    
    UPROPERTY(BlueprintAssignable, Category = "Input")
    FOnInteractSignature OnInteract;
    
protected:
    // Input callbacks
    void OnMoveForwardCallback(float Value);
    void OnMoveRightCallback(float Value);
    void OnLookUpCallback(float Value);
    void OnLookRightCallback(float Value);
    void OnJumpCallback();
    void OnAttackCallback();
    void OnAbility1Callback();
    void OnAbility2Callback();
    void OnAbility3Callback();
    void OnAbility4Callback();
    void OnRunCallback();
    void OnSprintCallback();
    void OnCrouchCallback();
    
    // Helper functions
    void ClearInputState();
};

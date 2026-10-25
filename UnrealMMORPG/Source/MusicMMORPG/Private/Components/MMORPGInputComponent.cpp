#include "MMORPGInputComponent.h"
#include "MusicMMORPGCharacter.h"
#include "Components/InputComponent.h"

UMMORPGInputComponent::UMMORPGInputComponent()
{
    // Initialize input state
    MovementInput = FVector2D::ZeroVector;
    LookInput = FVector2D::ZeroVector;
    bJumpPressed = false;
    bJumpReleased = false;
    bAttackPressed = false;
    bRunPressed = false;
    bSprintPressed = false;
    bCrouchPressed = false;
    
    // Initialize ability inputs
    AbilityPressed.Init(false, 4);
    
    // Set component tick enabled
    PrimaryComponentTick.bCanEverTick = true;
}

void UMMORPGInputComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Bind input actions
    BindInputActions();
}

void UMMORPGInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // Process input
    ProcessMovementInput(DeltaTime);
    ProcessLookInput(DeltaTime);
    ProcessActionInput(DeltaTime);
    
    // Clear input state at the end of the frame
    ClearInputState();
}

void UMMORPGInputComponent::BindInputActions()
{
    // Movement
    BindAxis("MoveForward", this, &UMMORPGInputComponent::OnMoveForwardCallback);
    BindAxis("MoveRight", this, &UMMORPGInputComponent::OnMoveRightCallback);
    
    // Look
    BindAxis("LookUp", this, &UMMORPGInputComponent::OnLookUpCallback);
    BindAxis("LookRight", this, &UMMORPGInputComponent::OnLookRightCallback);
    
    // Actions
    BindAction("Jump", IE_Pressed, this, &UMMORPGInputComponent::OnJumpCallback);
    BindAction("Attack", IE_Pressed, this, &UMMORPGInputComponent::OnAttackCallback);
    BindAction("Ability1", IE_Pressed, this, &UMMORPGInputComponent::OnAbility1Callback);
    BindAction("Ability2", IE_Pressed, this, &UMMORPGInputComponent::OnAbility2Callback);
    BindAction("Ability3", IE_Pressed, this, &UMMORPGInputComponent::OnAbility3Callback);
    BindAction("Ability4", IE_Pressed, this, &UMMORPGInputComponent::OnAbility4Callback);
    BindAction("Run", IE_Pressed, this, &UMMORPGInputComponent::OnRunCallback);
    BindAction("Sprint", IE_Pressed, this, &UMMORPGInputComponent::OnSprintCallback);
    BindAction("Crouch", IE_Pressed, this, &UMMORPGInputComponent::OnCrouchCallback);
}

void UMMORPGInputComponent::ProcessMovementInput(float DeltaTime)
{
    // Process movement input
    if (!MovementInput.IsNearlyZero())
    {
        OnMoveForward.Broadcast(FVector2D(MovementInput.Y, 0.0f));
        OnMoveRight.Broadcast(FVector2D(0.0f, MovementInput.X));
    }
}

void UMMORPGInputComponent::ProcessLookInput(float DeltaTime)
{
    // Process look input
    if (!LookInput.IsNearlyZero())
    {
        OnLookUp.Broadcast(FVector2D(LookInput.Y, 0.0f));
        OnLookRight.Broadcast(FVector2D(0.0f, LookInput.X));
    }
}

void UMMORPGInputComponent::ProcessActionInput(float DeltaTime)
{
    // Process jump input
    if (bJumpPressed)
    {
        OnJump.Broadcast();
    }
    
    // Process attack input
    if (bAttackPressed)
    {
        OnAttack.Broadcast();
    }
    
    // Process ability inputs
    for (int32 i = 0; i < AbilityPressed.Num(); i++)
    {
        if (AbilityPressed[i])
        {
            OnAbility.Broadcast(i);
        }
    }
}

void UMMORPGInputComponent::OnMoveForwardCallback(float Value)
{
    MovementInput.Y = Value;
}

void UMMORPGInputComponent::OnMoveRightCallback(float Value)
{
    MovementInput.X = Value;
}

void UMMORPGInputComponent::OnLookUpCallback(float Value)
{
    LookInput.Y = Value;
}

void UMMORPGInputComponent::OnLookRightCallback(float Value)
{
    LookInput.X = Value;
}

void UMMORPGInputComponent::OnJumpCallback()
{
    bJumpPressed = true;
}

void UMMORPGInputComponent::OnAttackCallback()
{
    bAttackPressed = true;
}

void UMMORPGInputComponent::OnAbility1Callback()
{
    AbilityPressed[0] = true;
}

void UMMORPGInputComponent::OnAbility2Callback()
{
    AbilityPressed[1] = true;
}

void UMMORPGInputComponent::OnAbility3Callback()
{
    AbilityPressed[2] = true;
}

void UMMORPGInputComponent::OnAbility4Callback()
{
    AbilityPressed[3] = true;
}

void UMMORPGInputComponent::OnRunCallback()
{
    bRunPressed = true;
}

void UMMORPGInputComponent::OnSprintCallback()
{
    bSprintPressed = true;
}

void UMMORPGInputComponent::OnCrouchCallback()
{
    bCrouchPressed = true;
}

void UMMORPGInputComponent::ClearInputState()
{
    // Reset input state
    MovementInput = FVector2D::ZeroVector;
    LookInput = FVector2D::ZeroVector;
    bJumpPressed = false;
    bAttackPressed = false;
    bRunPressed = false;
    bSprintPressed = false;
    bCrouchPressed = false;
    
    for (bool& Pressed : AbilityPressed)
    {
        Pressed = false;
    }
}

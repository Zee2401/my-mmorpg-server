using System;
using UnityEngine;
using UnityEngine.InputSystem;

/// <summary>
/// Handles input for both PC and mobile (iOS) platforms
/// </summary>
public class InputManager : MonoBehaviour
{
    public static InputManager Instance { get; private set; }
    
    // Input actions
    public event Action<Vector2> OnMovementInput;
    public event Action<bool> OnJumpInput;
    public event Action<bool> OnAttackInput;
    public event Action<bool> OnRunInput;
    public event Action<Vector2> OnLookInput;
    public event Action<int> OnAbilityInput;
    public event Action OnInteractInput;
    public event Action OnInventoryInput;
    public event Action OnMapInput;
    public event Action OnMenuInput;
    
    // Touch controls
    private bool isTouching = false;
    private Vector2 touchStartPosition;
    private Vector2 touchCurrentPosition;
    private int touchId = -1;
    
    // Virtual joystick
    public RectTransform joystickArea;
    public RectTransform joystickKnob;
    private Vector2 joystickInput;
    private bool joystickActive = false;
    
    // Buttons
    public RectTransform jumpButton;
    public RectTransform attackButton;
    public RectTransform runButton;
    
    // Settings
    public float joystickRadius = 100f;
    public float deadzone = 0.1f;
    
    // Keyboard controls
    private Controls controls;
    
    private void Awake()
    {
        Instance = this;
        
        // Initialize input system
        controls = new Controls();
    }
    
    private void OnEnable()
    {
        // Enable input system
        if (controls != null)
        {
            controls.Enable();
            
            // Setup callbacks
            controls.Player.Move.performed += OnMovePerformed;
            controls.Player.Move.canceled += OnMoveCanceled;
            controls.Player.Look.performed += OnLookPerformed;
            controls.Player.Jump.performed += OnJumpPerformed;
            controls.Player.Attack.performed += OnAttackPerformed;
            controls.Player.Run.performed += OnRunPerformed;
            controls.Player.Run.canceled += OnRunCanceled;
            controls.Player.Interact.performed += OnInteractPerformed;
            controls.Player.Inventory.performed += OnInventoryPerformed;
            controls.Player.Map.performed += OnMapPerformed;
            controls.Player.Menu.performed += OnMenuPerformed;
            controls.Player.Ability1.performed += OnAbility1Performed;
            controls.Player.Ability2.performed += OnAbility2Performed;
            controls.Player.Ability3.performed += OnAbility3Performed;
            controls.Player.Ability4.performed += OnAbility4Performed;
        }
    }
    
    private void OnDisable()
    {
        // Disable input system
        if (controls != null)
        {
            controls.Player.Move.performed -= OnMovePerformed;
            controls.Player.Move.canceled -= OnMoveCanceled;
            controls.Player.Look.performed -= OnLookPerformed;
            controls.Player.Jump.performed -= OnJumpPerformed;
            controls.Player.Attack.performed -= OnAttackPerformed;
            controls.Player.Run.performed -= OnRunPerformed;
            controls.Player.Run.canceled -= OnRunCanceled;
            controls.Player.Interact.performed -= OnInteractPerformed;
            controls.Player.Inventory.performed -= OnInventoryPerformed;
            controls.Player.Map.performed -= OnMapPerformed;
            controls.Player.Menu.performed -= OnMenuPerformed;
            controls.Player.Ability1.performed -= OnAbility1Performed;
            controls.Player.Ability2.performed -= OnAbility2Performed;
            controls.Player.Ability3.performed -= OnAbility3Performed;
            controls.Player.Ability4.performed -= OnAbility4Performed;
            
            controls.Disable();
        }
    }
    
    // Input system callbacks
    private void OnMovePerformed(InputAction.CallbackContext context)
    {
        Vector2 input = context.ReadValue<Vector2>();
        OnMovementInput?.Invoke(input);
    }
    
    private void OnMoveCanceled(InputAction.CallbackContext context)
    {
        OnMovementInput?.Invoke(Vector2.zero);
    }
    
    private void OnLookPerformed(InputAction.CallbackContext context)
    {
        Vector2 input = context.ReadValue<Vector2>();
        OnLookInput?.Invoke(input);
    }
    
    private void OnJumpPerformed(InputAction.CallbackContext context)
    {
        OnJumpInput?.Invoke(true);
    }
    
    private void OnAttackPerformed(InputAction.CallbackContext context)
    {
        OnAttackInput?.Invoke(true);
    }
    
    private void OnRunPerformed(InputAction.CallbackContext context)
    {
        OnRunInput?.Invoke(true);
    }
    
    private void OnRunCanceled(InputAction.CallbackContext context)
    {
        OnRunInput?.Invoke(false);
    }
    
    private void OnInteractPerformed(InputAction.CallbackContext context)
    {
        OnInteractInput?.Invoke();
    }
    
    private void OnInventoryPerformed(InputAction.CallbackContext context)
    {
        OnInventoryInput?.Invoke();
    }
    
    private void OnMapPerformed(InputAction.CallbackContext context)
    {
        OnMapInput?.Invoke();
    }
    
    private void OnMenuPerformed(InputAction.CallbackContext context)
    {
        OnMenuInput?.Invoke();
    }
    
    private void OnAbility1Performed(InputAction.CallbackContext context)
    {
        OnAbilityInput?.Invoke(0);
    }
    
    private void OnAbility2Performed(InputAction.CallbackContext context)
    {
        OnAbilityInput?.Invoke(1);
    }
    
    private void OnAbility3Performed(InputAction.CallbackContext context)
    {
        OnAbilityInput?.Invoke(2);
    }
    
    private void OnAbility4Performed(InputAction.CallbackContext context)
    {
        OnAbilityInput?.Invoke(3);
    }
    
    // Touch input handling for mobile
    private void Update()
    {
        // Handle touch input for mobile
        if (Application.isMobilePlatform || Application.platform == RuntimePlatform.IPhonePlayer)
        {
            HandleTouchInput();
        }
        
        // Handle virtual joystick
        HandleVirtualJoystick();
    }
    
    private void HandleTouchInput()
    {
        if (Touchscreen.current == null) return;
        
        // Check for touch start
        if (Touchscreen.current.primaryTouch.press.wasPressedThisFrame)
        {
            Vector2 touchPosition = Touchscreen.current.primaryTouch.position.ReadValue();
            
            // Check if touching joystick area
            if (joystickArea != null && RectTransformUtility.RectangleContainsScreenPoint(
                joystickArea, touchPosition, Camera.main))
            {
                isTouching = true;
                touchId = Touchscreen.current.primaryTouch.touchId.ReadValue();
                touchStartPosition = touchPosition;
                joystickActive = true;
            }
            
            // Check for button presses
            CheckButtonPress(touchPosition);
        }
        
        // Check for touch end
        if (Touchscreen.current.primaryTouch.press.wasReleasedThisFrame)
        {
            isTouching = false;
            joystickActive = false;
            joystickInput = Vector2.zero;
            OnMovementInput?.Invoke(Vector2.zero);
        }
        
        // Handle touch movement
        if (isTouching && joystickActive)
        {
            touchCurrentPosition = Touchscreen.current.primaryTouch.position.ReadValue();
            Vector2 delta = touchCurrentPosition - touchStartPosition;
            
            // Clamp to joystick radius
            if (delta.magnitude > joystickRadius)
            {
                delta = delta.normalized * joystickRadius;
            }
            
            joystickInput = delta / joystickRadius;
            
            // Apply deadzone
            if (joystickInput.magnitude < deadzone)
            {
                joystickInput = Vector2.zero;
            }
            
            // Update joystick knob position
            if (joystickKnob != null)
            {
                joystickKnob.anchoredPosition = delta;
            }
            
            OnMovementInput?.Invoke(joystickInput);
        }
    }
    
    private void CheckButtonPress(Vector2 touchPosition)
    {
        // Check jump button
        if (jumpButton != null && RectTransformUtility.RectangleContainsScreenPoint(
            jumpButton, touchPosition, Camera.main))
        {
            OnJumpInput?.Invoke(true);
        }
        
        // Check attack button
        if (attackButton != null && RectTransformUtility.RectangleContainsScreenPoint(
            attackButton, touchPosition, Camera.main))
        {
            OnAttackInput?.Invoke(true);
        }
        
        // Check run button
        if (runButton != null && RectTransformUtility.RectangleContainsScreenPoint(
            runButton, touchPosition, Camera.main))
        {
            OnRunInput?.Invoke(true);
        }
    }
    
    private void HandleVirtualJoystick()
    {
        // Reset joystick when not touching
        if (!isTouching && joystickKnob != null)
        {
            joystickKnob.anchoredPosition = Vector2.zero;
        }
    }
    
    // Public methods to simulate input (for UI buttons)
    public void SimulateMovementInput(Vector2 input)
    {
        OnMovementInput?.Invoke(input);
    }
    
    public void SimulateJumpInput(bool jump)
    {
        OnJumpInput?.Invoke(jump);
    }
    
    public void SimulateAttackInput(bool attack)
    {
        OnAttackInput?.Invoke(attack);
    }
    
    public void SimulateRunInput(bool run)
    {
        OnRunInput?.Invoke(run);
    }
    
    public void SimulateAbilityInput(int abilityIndex)
    {
        OnAbilityInput?.Invoke(abilityIndex);
    }
    
    public void SimulateInteractInput()
    {
        OnInteractInput?.Invoke();
    }
    
    public void SimulateInventoryInput()
    {
        OnInventoryInput?.Invoke();
    }
    
    public void SimulateMapInput()
    {
        OnMapInput?.Invoke();
    }
    
    public void SimulateMenuInput()
    {
        OnMenuInput?.Invoke();
    }
    
    // Get current input state
    public Vector2 GetMovementInput()
    {
        return joystickInput;
    }
    
    public bool IsRunning()
    {
        // Check if run button is pressed or shift key
        return runButton != null && runButton.gameObject.activeSelf || 
               (controls != null && controls.Player.Run.IsPressed());
    }
    
    public bool IsAttacking()
    {
        return (controls != null && controls.Player.Attack.IsPressed()) ||
               (attackButton != null && attackButton.gameObject.activeSelf);
    }
    
    public bool IsJumping()
    {
        return (controls != null && controls.Player.Jump.IsPressed()) ||
               (jumpButton != null && jumpButton.gameObject.activeSelf);
    }
}

// Input system controls
public class Controls : InputActionAsset
{
    public InputActionMap Player;
    
    public Controls()
    {
        Player = new InputActionMap("Player");
        
        // Movement
        Player.AddAction("Move", InputActionType.Value, "WASD", "WASD movement");
        Player.AddAction("Move", InputActionType.Value, "LeftStick", "Gamepad left stick");
        
        // Look
        Player.AddAction("Look", InputActionType.Value, "Mouse/Delta", "Mouse look");
        Player.AddAction("Look", InputActionType.Value, "RightStick", "Gamepad right stick");
        
        // Jump
        Player.AddAction("Jump", InputActionType.Button, "Space", "Jump");
        Player.AddAction("Jump", InputActionType.Button, "Gamepad/South", "Gamepad jump");
        
        // Attack
        Player.AddAction("Attack", InputActionType.Button, "Mouse/LeftClick", "Left click attack");
        Player.AddAction("Attack", InputActionType.Button, "Gamepad/RightTrigger", "Gamepad attack");
        
        // Run
        Player.AddAction("Run", InputActionType.Button, "LeftShift", "Run");
        Player.AddAction("Run", InputActionType.Button, "Gamepad/LeftShift", "Gamepad run");
        
        // Interact
        Player.AddAction("Interact", InputActionType.Button, "E", "Interact");
        Player.AddAction("Interact", InputActionType.Button, "Gamepad/West", "Gamepad interact");
        
        // Inventory
        Player.AddAction("Inventory", InputActionType.Button, "Tab", "Inventory");
        Player.AddAction("Inventory", InputActionType.Button, "Gamepad/Select", "Gamepad inventory");
        
        // Map
        Player.AddAction("Map", InputActionType.Button, "M", "Map");
        Player.AddAction("Map", InputActionType.Button, "Gamepad/Start", "Gamepad map");
        
        // Menu
        Player.AddAction("Menu", InputActionType.Button, "Escape", "Menu");
        Player.AddAction("Menu", InputActionType.Button, "Gamepad/Options", "Gamepad menu");
        
        // Abilities
        Player.AddAction("Ability1", InputActionType.Button, "1", "Ability 1");
        Player.AddAction("Ability2", InputActionType.Button, "2", "Ability 2");
        Player.AddAction("Ability3", InputActionType.Button, "3", "Ability 3");
        Player.AddAction("Ability4", InputActionType.Button, "4", "Ability 4");
    }
}

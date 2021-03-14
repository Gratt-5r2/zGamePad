// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
#define STICK_MAX   32768
#define STICK_MAX_D 32768.0
#define STICK_MAX_F 32768.0f
#define DEADZONE_R 10000.0f

  typedef int JOYKEY, DXKEY;
  typedef bool( *LPCONDFUNC )();

  struct zTCombination {
    Array<JOYKEY>     Combination;
    Array<DXKEY>      Emulation;
    Array<bool (*)()> AllowConditions;
    Array<DXKEY>      AllowButtons;
    Array<DXKEY>      AllowCombinations;
    Array<bool( *)()> DenyConditions;
    Array<DXKEY>      DenyButtons;
    Array<DXKEY>      DenyCombinations;
    bool              Enabled;
    bool              ToggleMode;
    bool              Toggled;
    JOYKEY            KeyStates;

    zTCombination();
    bool operator == ( const zTCombination& other ) const;
    bool operator <  ( const zTCombination& other ) const;
    bool operator >  ( const zTCombination& other ) const;

    void CheckDisable( JOYKEY& keys );
    void Disable();
    bool CheckEnable( JOYKEY& keys );
    bool HasToggledKeys();
    bool CheckCondition();
    bool CheckKeyStateCondition();
    void Enable();
    void AddCombination( JOYKEY keys ... );
    void AddEmulation( DXKEY keys ... );
    void AddAllowFunctions( LPCONDFUNC conditions ... );
    void AddAllowButtons( DXKEY keys ... );
    void AddAllowCombinations( JOYKEY keys ... );
    void AddDenyFunctions( LPCONDFUNC conditions ... );
    void AddDenyButtons( DXKEY keys ... );
    void AddDenyCombinations( JOYKEY keys ... );
    void SetEmulationState( bool_t state );
    void Clear();
  };



  struct zTVibrationMessage {
    Timer  Timer;
    uint   Index;
    string Pattern;
  };




  enum zEXInputKey : int {
    ArrowUp          = 1 << 0,
    ArrowDown        = 1 << 1,
    ArrowLeft        = 1 << 2,
    ArrowRight       = 1 << 3,

    // Dualshock names
    ButtonStart      = 1 << 4,
    ButtonSelect     = 1 << 5,
    // xbox names
    ButtonMenu       = ButtonStart,
    ButtonView       = ButtonSelect,

    ButtonLeftStick  = 1 << 6,
    ButtonRightStick = 1 << 7,
    ButtonL1         = 1 << 8,
    ButtonR1         = 1 << 9,

    // Dualshock names
    ButtonCross      = 1 << 12,
    ButtonCircle     = 1 << 13,
    ButtonSquare     = 1 << 14,
    ButtonTriangle   = 1 << 15,
    // xbox names
    ButtonA          = ButtonCross,
    ButtonB          = ButtonCircle,
    ButtonX          = ButtonSquare,
    ButtonY          = ButtonTriangle,

    GameWalk         = 1 << 16,
    GameForward      = 1 << 17,
    GameBackward     = 1 << 18,
    GameLeftStrafe   = 1 << 19,
    GameRightStrafe  = 1 << 20,
    GamePunch        = 1 << 21,
    GameParade       = 1 << 22
  };

  struct zTStickState {
    int X;
    int Y;
    int Length() {
      return (int)sqrt( double(X * X + Y * Y) );
    }
  };




  class zCXInputDevice {
    friend class zCInput_Win32;
    friend class zCGamepadSpellBook;

    JOYKEY KeyStates;
    zTStickState LeftStick;
    zTStickState RightStick;
    int LeftTrigger;
    int RightTrigger;
    bool DeviceConnected;

    void InitCombinations();
    void UpdateVibration();
    void UpdateLeftSticksState();
    void UpdateRightSticksState();
    void UpdateSticksState();
    void UpdateKeyState();
    void UpdateLastKeyState();
    bool ForceVideoSkipping();
    bool ParseControlFile();
    void ParseControlsCombination( zTCombination& combination, string row );
    void ParseControlsEmulation( zTCombination& combination, string row );
    void ParseControlsEndRecord( zTCombination& combination );
    void ParseControlsCondition( zTCombination& combination, string row );
    void ParseDescriptionsFile( string fileName );
    Array<zTCombination> KeyCombinations;
    zTVibrationMessage VibrationMessage;
  public:

    XINPUT_STATE Gamepad;
    PXINPUT_CAPABILITIES Capabilities;
    XINPUT_VIBRATION Vibration;

    void InitDevice();
    void UpdateControls();
    void UpdateGamePad();
    void StartVibration( string ptr );
    uint GetBatteryLevel();
    bool IsBatteryLow();
    bool IsConnected();
    bool HasToggledKeys( JOYKEY& keys );
    void GetStickStatesSquare( zTStickState& stateLeft, zTStickState& stateRight );
    void GetStickStatesCircle( zTStickState& stateLeft, zTStickState& stateRight );
  };

  zCXInputDevice XInputDevice;




  bool IsFightMode();
  bool IsFightModeRange();
  bool IsCanShoot();
  bool IsFightModeMelee();
  bool IsNotFightMode();
  bool IsSneak();
  bool IsDive();
  DXKEY GetBinded( uint16 bind );



  bool Cond_FightMode();
  bool Cond_FightModeMelee();
  bool Cond_FightModeRange();
  bool Cond_FightModeMagic();
  bool Cond_CanShoot();
  bool Cond_CanSneaking();
  bool Cond_Diving();
  bool Cond_HasFocusVob();
  bool Cond_HasFocusNpc();
  bool Cond_TargetIsLocked();
  bool Cond_OnChooseWeapon();
  bool Cond_OnSpellBook();
  bool Cond_InventoryIsOpen();
  bool Cond_InterfaceIsOpen();
  bool Cond_IsOverlayTop();
  bool Cond_InTransformation();
  bool Cond_CanQuickPotionDrink();
  bool Cond_VideoIsOpen();
  bool Cond_CanLockTarget();
  bool Cond_G1();
  bool Cond_G2();




#define KEYRECORD_BEGIN { zTCombination _comb;
#define COMBINATION                     _comb.AddCombination(
#define EMULATION                 , 0); _comb.AddEmulation(
#define ALLOWCONDITION            , 0); _comb.AddAllowFunctions(
#define DENYCONDITION             , 0); _comb.AddDenyFunctions(
#define KEYRECORD_END             , 0); KeyCombinations.InsertSorted( _comb ); }




  static float GetScaleMultiplier() {
    float scale = 1.0f;
    Union.GetSysPackOption().Read( scale, "INTERFACE", "Scale" );
    return scale;
  }

  static float GetStickSensitivity() {
    float scale = 1.0f;
    Union.GetSysPackOption().Read( scale, "ZGAMEPAD", "StickSensitivity", scale );
    return scale;
  }

  static float CalculateInterfaceScale() {
    static float scale = GetScaleMultiplier();

    if( scale == 0 )
      return 1.0f;

    if( scale != 1 )
      return scale;

    int ydim = zrenderer->vid_ydim;
    if( ydim < 2048 )
      return 1.0f;

    return (float)ydim / 1080.0f;
  }







  // XInput 1.3 - 1.4 compatible code
  struct XINPUT_BATTERY_INFORMATION {
    BYTE BatteryType;
    BYTE BatteryLevel;
  };

  typedef DWORD( WINAPI* LPXINPUTGETSTATE ) (DWORD, XINPUT_STATE*);
  typedef DWORD( WINAPI* LPXINPUTSETSTATE ) (DWORD, XINPUT_VIBRATION*);
  typedef DWORD( WINAPI* LPXINPUTGETBATTERYINFORMATION )(DWORD, BYTE, XINPUT_BATTERY_INFORMATION*);

#define BATTERY_DEVTYPE_GAMEPAD    0x00
#define BATTERY_DEVTYPE_HEADSET    0x01
#define BATTERY_LEVEL_EMPTY        0x00
#define BATTERY_LEVEL_LOW          0x01
#define BATTERY_LEVEL_MEDIUM       0x02
#define BATTERY_LEVEL_FULL         0x03
#define BATTERY_LEVEL_FAKE         0x04

  static DWORD WINAPI XInputgetBatteryInformation_Fake( DWORD device, BYTE flags, XINPUT_BATTERY_INFORMATION* info ) {
    info->BatteryLevel = BATTERY_LEVEL_FAKE;
    return ERROR_SUCCESS;
  }


  LPXINPUTGETSTATE XINPUTGETSTATE = &XInputGetState;
  LPXINPUTSETSTATE XINPUTSETSTATE = &XInputSetState;
  LPXINPUTGETBATTERYINFORMATION XINPUTGETBATTERYINFORMATION = &XInputgetBatteryInformation_Fake;


  static HMODULE LoadHighestXInput() {
    HMODULE XInput = LoadLibrary( "XInput1_4.dll" );
    if( !XInput ) {
      XInput = LoadLibrary( "XInput1_3.dll" );
    }

    return XInput;
  }

  static void InitializeCompatibleXInput() {
    HMODULE XInput = LoadHighestXInput();

    if( XInput ) {
      XINPUTGETSTATE = (LPXINPUTGETSTATE)GetProcAddress( XInput, "XInputGetState" );
      XINPUTSETSTATE = (LPXINPUTSETSTATE)GetProcAddress( XInput, "XInputSetState" );
      XINPUTGETBATTERYINFORMATION = (LPXINPUTGETBATTERYINFORMATION)GetProcAddress( XInput, "XInputGetBatteryInformation" );
    }
  }



  struct zTMouseState {
    int xpos;
    int ypos;
    int zpos;
    int buttonPressedLeft;
    int buttonPressedMid;
    int buttonPressedRight;
  };

  static zTMouseState& GetWrapperMouseState() {
    HMODULE zMouseFix = GetModuleHandle( "zMouseFix.dll" );
    if( zMouseFix != Null ) {
      zTMouseState* mouseState = (zTMouseState*)GetProcAddress( zMouseFix, "wrapperMouseState" );
      if( mouseState != Null )
        return *mouseState;
    }

    // Default gothic mouse state
    return *(zTMouseState*)ZenDef( 0x0086CCAC, 0x008B27A8, 0x008C3004, 0x008D165C );
  }

  static zTMouseState& wrapperMouseState = GetWrapperMouseState();
}
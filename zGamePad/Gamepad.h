// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  typedef int JOYKEY, DXKEY;
  typedef bool( *LPCONDFUNC )();

  struct zTCombination {
    Array<JOYKEY>     Combination;
    Array<DXKEY>      Emulation;
    Array<bool (*)()> AllowConditions;
    Array<bool( *)()> DenyConditions;
    bool              Enabled;

    zTCombination();
    bool operator == ( const zTCombination& other ) const;
    bool operator <  ( const zTCombination& other ) const;
    bool operator >  ( const zTCombination& other ) const;

    void CheckDisable( JOYKEY& keys );
    void Disable();
    bool CheckEnable( JOYKEY& keys );
    bool CheckCondition();
    void Enable();
    void AddCombination( JOYKEY keys ... );
    void AddEmulation( DXKEY keys ... );
    void AddAllowFunctions( LPCONDFUNC conditions ... );
    void AddDenyFunctions( LPCONDFUNC conditions ... );
    void Clear();
  };



  struct zTVibrationMessage {
    CTimer Timer;
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
  };




  class zCXInputDevice {
    friend class zCInput_Win32;

    JOYKEY KeyStates;
    zTStickState LeftStick;
    zTStickState RightStick;
    int LeftTrigger;
    int RightTrigger;

    void InitCombinations();
    void UpdateVibration();
    void UpdateLeftSticksState();
    void UpdateRightSticksState();
    void UpdateSticksState();
    void UpdateKeyState();
    bool ForceVideoSkipping();
    bool ParseControlFile();
    void ParseControlsCombination( zTCombination& combination, string row );
    void ParseControlsEmulation( zTCombination& combination, string row );
    void ParseControlsEndRecord( zTCombination& combination );
    void ParseControlsCondition( zTCombination& combination, string row );
    Array<zTCombination> KeyCombinations;
    zTVibrationMessage VibrationMessage;
  public:

    XINPUT_STATE Gamepad;
    PXINPUT_CAPABILITIES Capabilities;
    XINPUT_VIBRATION Vibration;

    void InitDevice();
    void UpdateGamePad();
    void StartVibration( string ptr );
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
  bool Cond_InTransformation();
  bool Cond_CanQuickPotionDrink();
  bool Cond_VideoIsOpen();
  bool Cond_CanLockTarget();




#define KEYRECORD_BEGIN { zTCombination _comb;
#define COMBINATION                     _comb.AddCombination(
#define EMULATION                 , 0); _comb.AddEmulation(
#define ALLOWCONDITION            , 0); _comb.AddAllowFunctions(
#define DENYCONDITION             , 0); _comb.AddDenyFunctions(
#define KEYRECORD_END             , 0); KeyCombinations.InsertSorted( _comb ); }
}
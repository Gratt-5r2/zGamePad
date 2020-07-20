// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  typedef int JOYKEY, DXKEY;

  struct zTCombination {
    Array<JOYKEY> Combination;
    Array<DXKEY>  Emulation;
    bool          Enabled;
    bool (*Condition)();

    zTCombination();
    bool operator == ( const zTCombination& other ) const;
    bool operator <  ( const zTCombination& other ) const;
    bool operator >  ( const zTCombination& other ) const;

    void CheckDisable( JOYKEY& keys );
    void Disable();
    bool CheckEnable( JOYKEY& keys );
    void Enable();
    void AddCombination( JOYKEY keys ... );
    void AddEmulation( DXKEY keys ... );
    void AddFunction( bool (*condition)() );
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




#define KEYRECORD_BEGIN { zTCombination _comb;
#define COMBINATION                     _comb.AddCombination(
#define EMULATION                 , 0); _comb.AddEmulation(
#define CONDITION                 , 0); _comb.AddFunction(
#define KEYRECORD_END                ); KeyCombinations.InsertSorted( _comb ); }
}
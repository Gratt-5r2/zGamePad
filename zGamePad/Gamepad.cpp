// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  bool*         keyevent  =  (bool*)        ZenDef( 0x0086CCC8, 0x008B27C0, 0x008C3020, 0x008D1678 );
  bool*         keytoggle =  (bool*)        ZenDef( 0x0086CED0, 0x008B29D4, 0x008C3260, 0x008D18B8 );
  zCArray<int>& keybuffer = *(zCArray<int>*)ZenDef( 0x0086D2DC, 0x008B2E00, 0x008C36F0, 0x008D1D50 );
  float         DeadZoneR = 10000.0f;



  void SetKeyStateAndInsert( DXKEY keyID, bool_t state ) {
    bool keyEventBefore = keyevent[keyID];
    keyevent[keyID] = state != 0;
    if( keyevent[keyID] ) {
      keybuffer.InsertEnd( keyID );
      keytoggle[keyID] = !keyEventBefore;
    }
  }



  DXKEY GetBinded( uint16 bind ) {
    zCArray<uint16> keys;
    if( zinput->GetBinding( bind, keys ) )
      if( keys.GetNum() )
        return keys[0];

    return Invalid;
  }



  zTCombination::zTCombination() {
    Enabled = false;
    Condition = Null;
  }



  bool zTCombination::operator == ( const zTCombination& other ) const {
    return Combination.GetNum() == other.Combination.GetNum();
  }



  bool zTCombination::operator < ( const zTCombination& other ) const {
    return Combination.GetNum() > other.Combination.GetNum();
  }



  bool zTCombination::operator > ( const zTCombination& other ) const {
    return Combination.GetNum() < other.Combination.GetNum();
  }

  

  void zTCombination::CheckDisable( JOYKEY& keys ) {
    for( uint i = 0; i < Combination.GetNum(); i++ ) {
      JOYKEY key = keys & Combination[i];
      if( !key )
        return Disable();
    }
  }



  void zTCombination::Disable() {
    if( !Enabled )
      return;

    Enabled = false;
    for( uint i = 0; i < Emulation.GetNum(); i++ )
      SetKeyStateAndInsert( Emulation[i], False );
  }



  bool zTCombination::CheckEnable( JOYKEY& keys ) {
    if( Condition && !Condition() )
      return false;

    JOYKEY flags = keys;
    bool success = true;

    for( uint i = 0; i < Combination.GetNum(); i++ ) {
      JOYKEY key = keys & Combination[i];
      flags ^= Combination[i];

      if( !key ) {
        success = false;
        break;
      }
    }

    if( success ) {
      keys = flags;
      Enable();
    }

    return success;
  }



  void zTCombination::Enable() {
    if( Enabled )
      return;

    Enabled = true;
    for( uint i = 0; i < Emulation.GetNum(); i++ )
      SetKeyStateAndInsert( Emulation[i], True );
  }



  void zTCombination::AddCombination( JOYKEY keys_first ... ) {
    JOYKEY* keys = &keys_first;

    for( uint i = 0; true; i++ ) {
      if( keys[i] == 0 )
        break;

      Combination += keys[i];
    }
  }



  void zTCombination::AddEmulation( DXKEY keys_first ... ) {
    DXKEY* keys = &keys_first;

    for( uint i = 0; true; i++ ) {
      if( keys[i] == 0 )
        break;

      Emulation += keys[i];
    }
  }



  void zTCombination::AddFunction( bool (*condition)() ) {
    Condition = condition;
  }









  void zCXInputDevice::InitDevice() {
    Gamepad = XINPUT_STATE();
    Capabilities = PXINPUT_CAPABILITIES();
    ZeroMemory( &Gamepad, sizeof( XINPUT_STATE ) );
    KeyStates = (zEXInputKey)0;
    InitCombinations();
  }



  void zCXInputDevice::UpdateVibration() {
    if( VibrationMessage.Index == Invalid )
      return;

    VibrationMessage.Timer.Attach();
    if( VibrationMessage.Timer( 0, 150, TM_PRIMARY ) ) {

      string strength = VibrationMessage.Pattern.GetWordSmart( ++VibrationMessage.Index );
      if( strength.IsEmpty() ) {
        VibrationMessage.Index     = Invalid;
        Vibration.wLeftMotorSpeed  = 0;
        Vibration.wRightMotorSpeed = 0;
        XInputSetState( 0, &Vibration );
        return;
      }

      Vibration.wLeftMotorSpeed  = strength.ToInt32();      // Hight-motor
      Vibration.wRightMotorSpeed = 0;// strength.ToInt32(); // Low-motor
      XInputSetState( 0, &Vibration );
    }
  }



  int sqrti( int a ) {
    return (int)sqrt( (float)a );
  }



  void zCXInputDevice::UpdateLeftSticksState() {
    static bool runActive = false;
    static CTimer timer;
    timer.Attach();

    // Maximum of Triggers or Sticks - 65536
    LeftStick.X = Gamepad.Gamepad.sThumbLX;
    LeftStick.Y = Gamepad.Gamepad.sThumbLY;
    int length  = sqrti( LeftStick.X * LeftStick.X + LeftStick.Y * LeftStick.Y );

    if( length > 15000 ) {
      // Check strafe
      if( abs( LeftStick.X ) > abs( LeftStick.Y ) ) {
        KeyStates |= LeftStick.X > 0 ?
          GameRightStrafe :
          GameLeftStrafe;
      }
      else {
        // Check run
        if( LeftStick.Y > 0 ) {
          if( runActive || timer( 0, 20 ) ) {
            runActive = true;

            // Walk situations
            bool canWalk = length <= 20000 && !(player && player->fmode) && !IsSneak();

            KeyStates |= canWalk ?
              GameWalk :
              GameForward;
          }
        }
        else
          KeyStates |= GameBackward;
      }
    }
    else if( runActive ) {
      runActive = false;
      timer.Delete( 0 );
    }
  }



  void zCXInputDevice::UpdateRightSticksState() {
    LeftTrigger  = Gamepad.Gamepad.bLeftTrigger;
    RightTrigger = Gamepad.Gamepad.bRightTrigger;

    if( LeftTrigger > 50 )
      KeyStates |= GameParade;

    if( RightTrigger > 50 )
      KeyStates |= GamePunch;
  }



  void zCXInputDevice::UpdateSticksState() {
    bool diveMode = IsDive();
    UpdateLeftSticksState();

    if( !diveMode )
      UpdateRightSticksState();

    // Dive inverse
    int leftStick  = (diveMode ? Gamepad.Gamepad.sThumbLX : Gamepad.Gamepad.sThumbRX);
    int rightStick = (diveMode ? Gamepad.Gamepad.sThumbLY : Gamepad.Gamepad.sThumbRY);

    RightStick.X = abs( leftStick  ) > DeadZoneR ? leftStick  : 0;
    RightStick.Y = abs( rightStick ) > DeadZoneR ? rightStick : 0;

    if( RightStick.X ) RightStick.X += leftStick  > 0 ? (int)-DeadZoneR : (int)+DeadZoneR;
    if( RightStick.Y ) RightStick.Y += rightStick > 0 ? (int)-DeadZoneR : (int)+DeadZoneR;
  }



  void zCXInputDevice::UpdateKeyState() {
    XInputGetState( 0, &Gamepad );
    KeyStates = Gamepad.Gamepad.wButtons;
    UpdateSticksState();


    for( uint i = 0; i < KeyCombinations.GetNum(); i++ )
      KeyCombinations[i].CheckDisable( KeyStates );
    
    if( KeyStates ) {
      for( uint i = 0; i < KeyCombinations.GetNum(); i++ )
        if( KeyCombinations[i].CheckEnable( KeyStates ) )
          /*break*/continue;
    }
  }



  void zCXInputDevice::UpdateGamePad() {
    UpdateKeyState();
    UpdateVibration();
  }



  void zCXInputDevice::StartVibration( string ptr ) {
    VibrationMessage.Index = 0;
    VibrationMessage.Pattern = ptr;
    VibrationMessage.Timer.Delete(0);
  }
}
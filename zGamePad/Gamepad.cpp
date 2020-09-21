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
    Clear();
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
    if( !Toggled )
      return;

    Toggled = false;
    SetEmulationState( False );
  }



  bool HasFlag( int value, int flag ) {
    return (value & flag) == flag;
  }



  bool zTCombination::CheckCondition() {
    for( uint i = 0; i < AllowConditions.GetNum(); i++ )
      if( !AllowConditions[i]() )
        return false;

    for( uint i = 0; i < DenyConditions.GetNum(); i++ )
      if( DenyConditions[i]() )
        return false;

    for( uint i = 0; i < AllowButtons.GetNum(); i++ )
      if( !zinput->KeyPressed( AllowButtons[i] ) )
        return false;

    for( uint i = 0; i < DenyButtons.GetNum(); i++ )
      if( zinput->KeyPressed( DenyButtons[i] ) )
        return false;
    
    if( !CheckKeyStateCondition() )
      return false;

    return true;
  }



  bool zTCombination::CheckKeyStateCondition() {
    for( uint i = 0; i < AllowCombinations.GetNum(); i++ )
      if( !HasFlag( KeyStates, AllowCombinations[i] ) )
        return false;

    for( uint i = 0; i < DenyCombinations.GetNum(); i++ )
      if( HasFlag( KeyStates, DenyCombinations[i] ) )
        return false;

    return true;
  }



  bool zTCombination::CheckEnable( JOYKEY& keys ) {
    if( !CheckCondition() )
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
    if( Enabled ) {
      if( ToggleMode && Toggled ) {
        Toggled = false;
        SetEmulationState( False );
      }

      return;
    }

    Enabled = true;
    Toggled = true;
    SetEmulationState( True );
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



  void zTCombination::AddAllowFunctions( bool (*conditions)() ... ) {
    auto* keys = &conditions;

    for( uint i = 0; true; i++ ) {
      if( keys[i] == 0 )
        break;

      AllowConditions += keys[i];
    }
  }



  void zTCombination::AddDenyFunctions( bool( *conditions )() ... ) {
    auto* keys = &conditions;

    for( uint i = 0; true; i++ ) {
      if( keys[i] == 0 )
        break;

      DenyConditions += keys[i];
    }
  }



  void zTCombination::AddAllowButtons( DXKEY keys ... ) {
    auto* __keys = &keys;

    for( uint i = 0; true; i++ ) {
      if( __keys[i] == 0 )
        break;

      AllowButtons += __keys[i];
    }
  }



  void zTCombination::AddDenyButtons( DXKEY keys ... ) {
    auto* __keys = &keys;

    for( uint i = 0; true; i++ ) {
      if( __keys[i] == 0 )
        break;

      DenyButtons += __keys[i];
    }
  }



  void zTCombination::AddAllowCombinations( DXKEY keys ... ) {
    auto* __keys = &keys;

    for( uint i = 0; true; i++ ) {
      if( __keys[i] == 0 )
        break;

      AllowCombinations += __keys[i];
    }
  }



  void zTCombination::AddDenyCombinations( DXKEY keys ... ) {
    auto* __keys = &keys;

    for( uint i = 0; true; i++ ) {
      if( __keys[i] == 0 )
        break;

      DenyCombinations += __keys[i];
    }
  }



  void zTCombination::SetEmulationState( bool_t state ) {
    for( uint i = 0; i < Emulation.GetNum(); i++ )
      SetKeyStateAndInsert( Emulation[i], state );
  }



  void zTCombination::Clear() {
    Combination.Clear();
    Emulation.Clear();
    AllowConditions.Clear();
    DenyConditions.Clear();
    AllowButtons.Clear();
    DenyButtons.Clear();
    AllowCombinations.Clear();
    DenyCombinations.Clear();
    ToggleMode = false;
    Toggled = false;
    KeyStates = None;
  }









  void zCXInputDevice::InitDevice() {
    Gamepad = XINPUT_STATE();
    Capabilities = PXINPUT_CAPABILITIES();
    ZeroMemory( &Gamepad, sizeof( XINPUT_STATE ) );
    KeyStates = (zEXInputKey)0;
    UpdateControls();
  }



  void zCXInputDevice::UpdateControls() {
    KeyCombinations.Clear();
    InitCombinations();

    XInputGetState( 0, &Gamepad );
    for( uint i = 0; i < KeyCombinations.GetNum(); i++ )
      KeyCombinations[i].KeyStates = Gamepad.Gamepad.wButtons;
  }



  void zCXInputDevice::UpdateVibration() {
    if( VibrationMessage.Index == Invalid )
      return;

    VibrationMessage.Timer.ClearUnused();
    if( VibrationMessage.Timer[0u].Await( 150, true ) ) {

      string strength = VibrationMessage.Pattern.GetWordSmart( ++VibrationMessage.Index );
      if( strength.IsEmpty() ) {
        VibrationMessage.Index     = Invalid;
        Vibration.wLeftMotorSpeed  = 0;
        Vibration.wRightMotorSpeed = 0;
        XInputSetState( 0, &Vibration );
        return;
      }

      Vibration.wLeftMotorSpeed  = strength.ToInt32(); // Low-speed motor
      Vibration.wRightMotorSpeed = 0;                  // Hight-speed motor
      XInputSetState( 0, &Vibration );
    }
  }



  int sqrti( int a ) {
    return (int)sqrt( (float)a );
  }



  void zCXInputDevice::UpdateLeftSticksState() {
    static bool runActive = false;
    static Timer timer;
    timer.ClearUnused();

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
          if( runActive || timer[0u].Await( 20 ) ) {
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
      timer[0u].Delete();
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



  bool zCXInputDevice::ForceVideoSkipping() {
    static bool skipReady = false;

    if( !ActiveVideo ) {
      if( skipReady )
        skipReady = false;

      return false;
    }

    if( !KeyStates )
      skipReady = true;

    if( KeyStates && skipReady ) {
      SetKeyStateAndInsert( KEY_ESCAPE, True );
      skipReady = false;
    }

    return true;
  }



  int GetGamepadID() {
    int gamepadID = 0;
    Union.GetSysPackOption().Read( gamepadID, "ZGAMEPAD", "ControllerID", gamepadID );
    return min( 3, max( 0, gamepadID ) );
  }



  void zCXInputDevice::UpdateKeyState() {
    static int gamepadID = GetGamepadID();

    XInputGetState( gamepadID, &Gamepad );
    KeyStates = Gamepad.Gamepad.wButtons;
    UpdateSticksState();

    if( ForceVideoSkipping() )
     return;

    for( uint i = 0; i < KeyCombinations.GetNum(); i++ )
      KeyCombinations[i].CheckDisable( KeyStates );
    
    if( KeyStates )
      for( uint i = 0; i < KeyCombinations.GetNum(); i++ )
        KeyCombinations[i].CheckEnable( KeyStates );

    UpdateLastKeyState();



    if( player ) {
      static Timer helper;
      helper.ClearUnused();
      KeyStates = Gamepad.Gamepad.wButtons;

      if( (KeyStates & JOY_LSTICK) && (KeyStates & JOY_RSTICK) ) {
        if( helper[0u].Await( 2000 ) )
          player->EmergencyResetPos( player->GetPositionWorld() );
      }
      else
        helper[0u].Delete();
    }
  }


  void zCXInputDevice::UpdateLastKeyState() {
    for( uint i = 0; i < KeyCombinations.GetNum(); i++ )
      KeyCombinations[i].KeyStates = Gamepad.Gamepad.wButtons;
  }



  void zCXInputDevice::UpdateGamePad() {
    UpdateKeyState();
    UpdateVibration();
  }



  void zCXInputDevice::StartVibration( string ptr ) {
    VibrationMessage.Index = 0;
    VibrationMessage.Pattern = ptr;
    VibrationMessage.Timer[0u];
  }
}
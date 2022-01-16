// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  bool*         keyevent  =  (bool*)        ZenDef( 0x0086CCC8, 0x008B27C0, 0x008C3020, 0x008D1678 );
  bool*         keytoggle =  (bool*)        ZenDef( 0x0086CED0, 0x008B29D4, 0x008C3260, 0x008D18B8 );
  zCArray<int>& keybuffer = *(zCArray<int>*)ZenDef( 0x0086D2DC, 0x008B2E00, 0x008C36F0, 0x008D1D50 );



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
    Help = Null;
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
    SetHelpEnable( false );
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



  bool zTCombination::CheckAllConditions() {
    for( uint i = 0; i < AllowConditions.GetNum(); i++ )
      if( !Gamepad_GetStaticCondition( AllowConditions[i] ) )
        return false;

    for( uint i = 0; i < DenyConditions.GetNum(); i++ )
      if( Gamepad_GetStaticCondition( DenyConditions[i] ) )
        return false;

    for( uint i = 0; i < AllowButtons.GetNum(); i++ )
      if( !zinput->KeyPressed( AllowButtons[i] ) )
        return false;

    for( uint i = 0; i < DenyButtons.GetNum(); i++ )
      if( zinput->KeyPressed( DenyButtons[i] ) )
        return false;

    SetHelpEnable( true );

    if( !CheckKeyStateConditions() )
      return false;

    return true;
  }



  bool zTCombination::CheckKeyStateConditions() {
    for( uint i = 0; i < AllowCombinations.GetNum(); i++ )
      if( !HasFlag( KeyStates, AllowCombinations[i] ) )
        return false;

    for( uint i = 0; i < DenyCombinations.GetNum(); i++ )
      if( HasFlag( KeyStates, DenyCombinations[i] ) )
        return false;

    return true;
  }



  bool zTCombination::CheckEnable( JOYKEY& keys ) {
    if( !CheckAllConditions() )
      return false;

    if( Emulation.GetNum() == 0 )
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
    else if( HasToggledKeys() )
      return;

    Enabled = true;
    Toggled = true;
    SetEmulationState( True );
  }



  void zTCombination::SetHelpEnable( bool enable ) {
    if( Help != Null )
      Help->Enabled = enable;
  }



  bool zTCombination::HasToggledKeys() {
    JOYKEY joyKeys = 0;
    for( uint i = 0; i < Combination.GetNum(); i++ )
      joyKeys |= Combination[i];

    return XInputDevice.HasToggledKeys( joyKeys );
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
    Help = Null;
  }



  zTCombination::~zTCombination() {
  }









  void zCXInputDevice::InitDevice() {
    InitializeCompatibleXInput();
    Gamepad = XINPUT_STATE();
    Capabilities = PXINPUT_CAPABILITIES();
    ZeroMemory( &Gamepad, sizeof( XINPUT_STATE ) );
    KeyStates = (zEXInputKey)0;
    UpdateControls();
  }



  void zCXInputDevice::UpdateControls() {
    KeyCombinations.Clear();
    InitCombinations();

    XINPUTGETSTATE( 0, &Gamepad );
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
      }
      else {
        Vibration.wLeftMotorSpeed  = strength.ToInt32(); // big motor
        Vibration.wRightMotorSpeed = 0;                  // small motor
      }

      XINPUTSETSTATE( 0, &Vibration );
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
    if (!DeviceConnected)
    {
        LeftStick.X = DS4Device.GetLeftStick().X;
        LeftStick.Y = DS4Device.GetLeftStick().Y;
    }
    else
    {
        LeftStick.X = Gamepad.Gamepad.sThumbLX;
        LeftStick.Y = Gamepad.Gamepad.sThumbLY;
    }

    int length    = sqrti( LeftStick.X * LeftStick.X + LeftStick.Y * LeftStick.Y );
    StrafePressed = false;

    if( length > DEADZONE_L ) {
      StrafePressed = abs( LeftStick.X * 5 ) > abs( LeftStick.Y * 7 );

      // Check strafe
      if( Opt_MotionType == 0 && StrafePressed ) {
        KeyStates |= LeftStick.X > 0 ?
          GameRightStrafe :
          GameLeftStrafe;
      }
      else
      {
        // Check run
        if( LeftStick.Y > 0 || StrafePressed ) {
          WalkBack = false;
          if( runActive || timer[0u].Await( 20 ) ) {
            runActive = true;

            // Walk situations
            bool canWalk = length <= 25000 && !(player && player->fmode) && !Cond_CanSneaking();

            KeyStates |= canWalk ?
              GameWalk :
              GameForward;
          }
        }
        else {
          KeyStates |= GameBackward;
          WalkBack = true;
        }
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
    bool diveMode = Cond_Diving();
    UpdateLeftSticksState();

    if( !diveMode )
      UpdateRightSticksState();

    // Dive inverse
    int leftStick, rightStick;

    if (!DeviceConnected)
    {
        leftStick = (diveMode ? DS4Device.GetLeftStick().X : DS4Device.GetRightStick().X);
        rightStick = (diveMode ? DS4Device.GetLeftStick().Y : DS4Device.GetRightStick().Y);
    }
    else
    {
        leftStick = (diveMode ? Gamepad.Gamepad.sThumbLX : Gamepad.Gamepad.sThumbRX);
        rightStick = (diveMode ? Gamepad.Gamepad.sThumbLY : Gamepad.Gamepad.sThumbRY);
    }

    RightStick.X = abs( leftStick  ) > DEADZONE_R ? leftStick  : 0;
    RightStick.Y = abs( rightStick ) > DEADZONE_R ? rightStick : 0;

    if( RightStick.X ) RightStick.X += leftStick  > 0 ? (int)-DEADZONE_R : (int)+DEADZONE_R;
    if( RightStick.Y ) RightStick.Y += rightStick > 0 ? (int)-DEADZONE_R : (int)+DEADZONE_R;
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



  void zCXInputDevice::DisplayDisconnect() {
    static zCMenu* menu = Null;
    if( menu == Null ) {
      menu = zCMenu::Create( "ZGAMEPAD_MENU_DISCONNECTED" );
      if( menu ) {
        ogame->Pause();
        menu->Run();
        menu->Release();
        ogame->Unpause();
        menu = Null;
      }
    }
  }



  void zCXInputDevice::UpdateKeyState() {
    if( !zinput )
      return;

    bool DSInputDisconnected = !DS4Device.CheckConnection();
    bool XInputDisconnected = XINPUTGETSTATE(Opt_ControllerID, &Gamepad) == ERROR_DEVICE_NOT_CONNECTED;

    if(XInputDisconnected && DSInputDisconnected) {
      if (DeviceConnected || DS4Device.IsConnected()) {
            DisplayDisconnect();
      }
      DeviceConnected = false;
      return;
    }
    
    
    if (!XInputDisconnected && !DeviceConnected) {
        DeviceConnected = true;
    }

    if (!DSInputDisconnected && !DS4Device.IsConnected()) {
        DS4Device.SetConnected(true);
    }

    if (!DeviceConnected) {
        DS4Device.UpdateState();
        KeyStates = DS4Device.GetKeyState();
    }
    else {
        KeyStates = Gamepad.Gamepad.wButtons;
    }
    
    UpdateSticksState();
    KeyStatesReal = KeyStates;

    if( ForceVideoSkipping() || !ogame )
     return;

    if( !Opt_GamepadEnabled )
      return;

#if 0
    static uint lastBattery    = 0;
           uint currentbattery = GetBatteryLevel();

    if( currentbattery == 0 && lastBattery > 0 ) {
      static zCMenu* menu = Null;
      if( menu == Null ) {
        menu = zCMenu::Create( "ZGAMEPAD_MENU_DISCONNECTED" );
        if( menu ) {
          ogame->Pause();
          menu->Run();
          menu->Release();
          ogame->Unpause();
          menu = Null;
        }
      }
    }

    lastBattery = currentbattery;
#endif

    // Important: Update a statick condition
    // information for the faster access !!!
    Gamepad_UpdateStaticConditions();

    for( uint i = 0; i < KeyCombinations.GetNum(); i++ )
      KeyCombinations[i].CheckDisable( KeyStates );
    
    // For correclty hints drawing this
    // condition should be always true
    if( KeyStates || Opt_HintsEnabled )
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
      if (!DeviceConnected)
      {
          WORD DSKeyState = DS4Device.GetKeyState();
          for (uint i = 0; i < KeyCombinations.GetNum(); i++)
              KeyCombinations[i].KeyStates = DSKeyState;
      }
      else
      {
          for (uint i = 0; i < KeyCombinations.GetNum(); i++)
              KeyCombinations[i].KeyStates = Gamepad.Gamepad.wButtons;
      }
  }



  void zCXInputDevice::UpdateGamePad() {
    UpdateKeyState();
    UpdateVibration();
  }



  void zCXInputDevice::StartVibration( string ptr ) {
    VibrationMessage.Index = 0;
    VibrationMessage.Pattern = ptr;
    VibrationMessage.Timer[0u].Delete();
  }



  uint zCXInputDevice::GetBatteryLevel() {
    XINPUT_BATTERY_INFORMATION batteryInformation;
    if( !XINPUTGETBATTERYINFORMATION( Opt_ControllerID, BATTERY_DEVTYPE_GAMEPAD, &batteryInformation ) != ERROR_SUCCESS )
      /*return BATTERY_LEVEL_FAKE*/;

    return batteryInformation.BatteryLevel;
  }



  bool zCXInputDevice::IsBatteryLow() {
    return GetBatteryLevel() <= BATTERY_LEVEL_LOW;
  }



  bool zCXInputDevice::IsConnected() {
    return DeviceConnected;
  }



  bool zCXInputDevice::HasToggledKeys( JOYKEY& keys ) {
    for( uint i = 0; i < KeyCombinations.GetNum(); i++ ) {

      auto& combo = KeyCombinations[i];
      if( combo.ToggleMode && combo.Enabled ) {

        auto& comboKeys = combo.Combination;
        for( uint j = 0; j < comboKeys.GetNum(); j++ )
          if( (keys & comboKeys[j]) != 0 )
            return true;
      }
    }

    return false;
  }



  bool zCXInputDevice::JoyPressed( const JOYKEY& keys ) {
    return KeyStatesReal & keys;
  }



  bool zCXInputDevice::StrafeButtonIsPressed() {
    return StrafePressed;
  }



  bool zCXInputDevice::IsBacKWalk() {
    return WalkBack;
  }



  void zCXInputDevice::GetStickStatesSquare( zTStickState& stateLeft, zTStickState& stateRight ) {
    stateLeft = LeftStick;
    stateRight = RightStick;
  }



  void zCXInputDevice::GetStickStatesCircle( zTStickState& stateLeft, zTStickState& stateRight ) {
    double squareX = double( LeftStick.X ) / STICK_MAX_D;
    double squareY = double( LeftStick.Y ) / STICK_MAX_D;
    stateLeft.X = int( squareX * sqrt( 1.0 - pow( squareY, 2 ) * 0.5 ) * STICK_MAX_D );
    stateLeft.Y = int( squareY * sqrt( 1.0 - pow( squareX, 2 ) * 0.5 ) * STICK_MAX_D );

    squareX = double( RightStick.X ) / STICK_MAX_D;
    squareY = double( RightStick.Y ) / STICK_MAX_D;
    stateRight.X = int( squareX * sqrt( 1.0 - pow( squareY, 2 ) * 0.5 ) * STICK_MAX_D );
    stateRight.Y = int( squareY * sqrt( 1.0 - pow( squareX, 2 ) * 0.5 ) * STICK_MAX_D );
  }



  zCXInputDevice::~zCXInputDevice() {
    StartVibration("0");
    UpdateVibration();
  }
}
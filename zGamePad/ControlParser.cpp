// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  COption& GetOptions() {
    COption& defaultOptions = Union.GetDefaultOption();
    COption& gameOptions = Union.GetGameOption();
    COption& sysPackOptions = Union.GetSysPackOption();

    return &defaultOptions == &gameOptions ?
      sysPackOptions :
      gameOptions;
  }



  void zCXInputDevice::ParseControlsCombination( zTCombination& combination, string row ) {
    for( uint i = 2; true; i++ ) {
      string token = row.GetWordSmart( i );
      if( token.IsEmpty() )
        break;

      // Token-separator
      if( token == "," )
        continue;

      int code = GetCombinationKeyCode( token );
      if( code == None )
        Message::Fatal( "Unknown control combination: " + token );
     
      combination.AddCombination( code, 0 );
    }
  }



  void zCXInputDevice::ParseControlsEmulation( zTCombination& combination, string row ) {
    for( uint i = 2; true; i++ ) {
      string token = row.GetWordSmart( i );
      if( token.IsEmpty() )
        break;

      // Token-separator
      if( token == "," )
        continue;

      int code = GetEmulationKeyCode( token );
      if( code == None )
        Message::Fatal( "Unknown control emulation: " + token );

      combination.AddEmulation( code, 0 );
    }
  }



  void zCXInputDevice::ParseControlsEndRecord( zTCombination& combination ) {
    KeyCombinations.InsertSorted( combination );
  }



  void zCXInputDevice::ParseControlsCondition( zTCombination& combination, string row ) {
    bool isNot = false;
    for( uint i = 2; true; i++ ) {
      string token = row.GetWordSmart( i );
      if( token.IsEmpty() )
        break;

      // Is 'NOT' function ??
      if( token == "!" ) {
        isNot = true;
        continue;
      }

      // Token-separator
      if( token == "," )
        continue;

      // Find function by name
      LPCONDFUNC func = GetConditionFunction( token );
      if( func != Null ) {
        if( !isNot ) combination.AddAllowFunctions( func, 0 );
        else {
          combination.AddDenyFunctions( func, 0 );
          isNot = false;
        }

        continue;
      }

      // 
      DXKEY dxKey = GetEmulationKeyCode( token );
      if( dxKey != None ) {
        if( !isNot ) combination.AddAllowButtons( dxKey, 0 );
        else {
          combination.AddDenyButtons( dxKey, 0 );
          isNot = false;
        }

        continue;
      }

      // 
      JOYKEY joyKey = GetCombinationKeyCode( token );
      if( joyKey != None ) {
        if( !isNot ) combination.AddAllowCombinations( joyKey, 0 );
        else {
          combination.AddDenyCombinations( joyKey, 0 );
          isNot = false;
        }

        continue;
      }

      Message::Fatal( "Unknown control condition: " + token );
    }
  }



  bool zCXInputDevice::ParseControlFile() {
    // Check external control file
    string defaultControlsFileName = "Controls.Gamepad";
    string controlsFileName;
    GetOptions().Read( controlsFileName, "zGamePad", "ControlsFile", defaultControlsFileName );

    if( controlsFileName.IsEmpty() )
      controlsFileName = defaultControlsFileName;

    bool initialized = false;
    zTCombination combination;

    string controlsFile;
    if( !controlsFile.ReadFromVdf( controlsFileName, VDF_DEFAULT | VDF_PHYSICALFIRST ) ) {
      cmd << "Controls not found" << endl;
      return false;
    }

    rowString controlsRows = controlsFile;

    for( uint i = 0; i < controlsRows.GetNum(); i++ ) {
      // Skip empty line
      string& row = controlsRows[i];
      if( row.Shrink().IsEmpty() )
        continue;

      // Skip comment line or blocks
      string command = row.GetWordSmart( 1, true );
      if( command == "//" || command.StartWith( "#" ) )
        continue;
      
      // Check command
      if( command == "KeyRecord" ) {

        // End record and start new
        if( initialized ) {
          ParseControlsEndRecord( combination );
          combination.Clear();
        }

        // One press - one click ??
        string mode = row.GetWordSmart( 2, true );
        if( mode == "Toggled" )
          combination.ToggleMode = true;

        initialized = true;
        continue;
      }

           // Parse commands
           if( command == "Combination" ) ParseControlsCombination( combination, row );
      else if( command == "Emulation" )   ParseControlsEmulation  ( combination, row );
      else if( command == "Condition" )   ParseControlsCondition  ( combination, row );
      else
        // unknown command !!!
        Message::Fatal( "Unknown control command: " + command );
    }

    // End last record
    if( initialized )
      ParseControlsEndRecord( combination );

    return true;
  }
}
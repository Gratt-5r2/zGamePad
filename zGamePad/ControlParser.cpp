// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  Array<zTGamepadControlInfo> zTGamepadControlInfo::GamepadControlsList;

  void zTGamepadControlInfo::CreateGamepadControlsList() {
    Array<string> namesList;

    // Find physical control list
    char** fileTable = Null;
    long count = vdf_filelist_physical( fileTable );
    for( long i = 0; i < count; i++ ) {
      string fileName = fileTable[i];
      if( fileName.EndWith( ".GAMEPAD" ) )
        namesList |= fileName.GetWord( "\\" );
    }

    delete[] fileTable;

    // Find virtual control list
    count = vdf_filelist_physical( fileTable );
    for( long i = 0; i < count; i++ ) {
      string fileName = fileTable[i];
      if( fileName.EndWith( ".GAMEPAD" ) )
        namesList |= fileName.GetWord( "\\" );
    }

    delete[] fileTable;

    for( uint i = 0; i < namesList.GetNum(); i++ ) {
      XInputDevice.ParseControlFileStrings( namesList[i] );
    }
    // for( string fileName : namesList )
    // XInputDevice.ParseControlFileStrings( fileName );
  }

  void zTGamepadControlInfo::RegisterStyleInfo( const string& fileName, const string& styleName ) {
    uint index = GamepadControlsList.SearchEqual( fileName );
    if( index == Invalid ) {
      zTGamepadControlInfo& info = GamepadControlsList.Create();
      info.FileName = fileName;
      info.StyleName = styleName;
    }
  }

  bool zTGamepadControlInfo::operator == ( const string& fileName ) const {
    return FileName == fileName;
  }





  Array<zTHelpString> zTHelpString::HelpStrings;

  zTHelpString::zTHelpString() {
    // pass
  }



  inline TSystemLangID GetLangID( const string& lang ) {
    if( lang == "RUS" ) return Lang_Rus;
    if( lang == "ENG" ) return Lang_Eng;
    if( lang == "GER" ) return Lang_Ger;
    if( lang == "DEU" ) return Lang_Ger;
    if( lang == "POL" ) return Lang_Pol;
    if( lang == "ROU" ) return Lang_Rou;
    if( lang == "ITA" ) return Lang_Ita;
    if( lang == "CZE" ) return Lang_Cze;
    if( lang == "ESP" ) return Lang_Esp;
    return Lang_Eng;
  }



  void zTHelpString::SetText( const string& text, const string& lang ) {
    TSystemLangID langID = GetLangID( lang );

    for( uint i = 0; i < Items.GetNum(); i++ ) {
      if( Items[i].LangID == langID ) {
        Items[i].Text = text;
        return;
      }
    }

    auto& item  = Items.Create();
    item.LangID = langID;
    item.Text   = text;
  }



  string zTHelpString::GetText() {
    TSystemLangID currentLandID = Union.GetSystemLanguage();
    TSystemLangID alterLandID   = Lang_Eng;

    for( uint i = 0; i < Items.GetNum(); i++ )
      if( Items[i].LangID == currentLandID )
        return Items[i].Text;
    
    for( uint i = 0; i < Items.GetNum(); i++ )
      if( Items[i].LangID == alterLandID )
        return Items[i].Text;

    if( Items.GetNum() > 0 )
      return Items.GetFirst().Text;

    return "";
  }



  void zTHelpString::CreateString( const string& name, const string& text, const string& lang ) {
    for( uint i = 0; i < HelpStrings.GetNum(); i++ ) {
      zTHelpString& help = HelpStrings[i];
      if( help.Name == name ) {
        help.SetText( text, lang );
        return;
      }
    }

    zTHelpString& help = HelpStrings.Create();
    help.Name = name;
    help.SetText( text, lang );
  }



  string zTHelpString::GetString( const string& name ) {
    for( uint i = 0; i < HelpStrings.GetNum(); i++ )
      if( HelpStrings[i].Name == name )
        return HelpStrings[i].GetText();
    
    return "???";
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
        isNot = !isNot;
        continue;
      }

      // Token-separator
      if( token == "," )
        continue;

      // Condition by function
      LPCONDFUNC func = GetConditionFunction( token );
      if( func != Null ) {
        if( isNot ) {
          combination.AddDenyFunctions( func, 0 );
          isNot = false;
        }
        else
          combination.AddAllowFunctions( func, 0 );

        continue;
      }

      // Condition by keyboard key
      DXKEY dxKey = GetEmulationKeyCode( token );
      if( dxKey != None ) {
        if( isNot ) {
          combination.AddDenyButtons( dxKey, 0 );
          isNot = false;
        }
        else
          combination.AddAllowButtons( dxKey, 0 );

        continue;
      }

      // Condition by gamepad key
      JOYKEY joyKey = GetCombinationKeyCode( token );
      if( joyKey != None ) {
        if( isNot ) {
          combination.AddDenyCombinations( joyKey, 0 );
          isNot = false;
        }
        else
          combination.AddAllowCombinations( joyKey, 0 );

        continue;
      }

      Message::Fatal( "Unknown control condition: " + token );
    }
  }



  void zCXInputDevice::ParseControlsHelp( zTCombination& combination, string row ) {
    string text = row.GetWordSmart( 2 );
    if( text == "\"" )
      text = row.GetPattern( "\"", "\"" );
    else
      text = zTHelpString::GetString( text );

    combination.Help = zTCombination_Help::Create( text, combination.Combination );
  }



  void zCXInputDevice::ParseControlsStringName( string& stringName, string row ) {
    string name = row.GetWordSmart( 2 );
    if( name.IsEmpty() )
      Message::Fatal( "Empty string name in Gamepad file." );

    stringName = name;
  }



  void zCXInputDevice::ParseControlsStringText( string& stringName, string row ) {
    if( stringName.IsEmpty() )
      Message::Fatal( "Empty string name in Gamepad file." );

    string lang = row.GetWordSmart( 1 );
    string text = row.GetWordSmart( 2 );
    if( text == "\"" )
      text = row.GetPattern( "\"", "\"" );
    else
      text = zTHelpString::GetString( text );

    zTHelpString::CreateString( stringName, text, lang );
  }



  void zCXInputDevice::ParseControlsStyleName( const string& fileName, string row ) {
    string text = row.GetWordSmart( 2 );
    if( text == "\"" )
      text = row.GetPattern( "\"", "\"" );
    else
      text = zTHelpString::GetString( text );

    zTGamepadControlInfo::RegisterStyleInfo( fileName, text );
  }



  bool zCXInputDevice::ParseControlFile() {
    // Check external control file
    if( Opt_ControlsFile.IsEmpty() )
      Opt_ControlsFile = "Controls.Gamepad";

    bool initialized = false;
    zTCombination combination;
    string currentStringName;

    string controlsFile;
    if( !controlsFile.ReadFromVdf( Opt_ControlsFile, VDF_DEFAULT | VDF_PHYSICALFIRST ) ) {
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
      else if( command == "Help" )        ParseControlsHelp       ( combination, row );
      else if( command == "String" )      ParseControlsStringName ( currentStringName, row );
      else if( command == "Rus" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Eng" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Ger" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Deu" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Pol" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Rou" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Ita" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Cze" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Esp" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "ControlName" ) continue;
      else
        // unknown command !!!
        Message::Fatal( "Unknown control command: " + command );
    }

    // End last record
    if( initialized )
      ParseControlsEndRecord( combination );

    return true;
  }






  bool zCXInputDevice::ParseControlFileStrings( const string& fileName ) {
    string currentStringName;
    string controlsFile;
    if( !controlsFile.ReadFromVdf( fileName, VDF_DEFAULT | VDF_PHYSICALFIRST ) ) {
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

           if( command == "String" )      ParseControlsStringName ( currentStringName, row );
      else if( command == "Rus" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Eng" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Ger" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Deu" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Pol" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Rou" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Ita" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Cze" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "Esp" )         ParseControlsStringText ( currentStringName, row );
      else if( command == "ControlName" ) ParseControlsStyleName  ( fileName, row );
      else continue;
    }

    return true;
  }
}
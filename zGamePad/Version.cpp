// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  bool CheckPluginUpdate( const int& curVersion ) {
    if( !zoptions->EntryExists( "ZGAMEPAD", "Enabled" ) ) {
      zoptions->WriteInt( "ZGAMEPAD", "Version", curVersion, 0 );
      return false;
    }

    int oldVersion = zoptions->ReadInt( "ZGAMEPAD", "Version", 0 );
    if( oldVersion >= curVersion )
      return false;

    zoptions->WriteInt( "ZGAMEPAD", "Version", curVersion, 0 );
    return true;
  }


  void PluginWasUpdated() {
    if( !CheckPluginUpdate( 1 ) )
      return;

    static zCMenu* menu = Null;
    if( menu == Null ) {
      menu = zCMenu::Create( "ZGAMEPAD_UPDATE:MENU_UPDATE" );
      if( menu ) {
        menu->Run();
        menu->Release();
      }
    }
  }

  HOOK Hook_CGameManager_Menu PATCH( &CGameManager::Menu, & CGameManager::Menu_Union );

  void CGameManager::Menu_Union( int i ) {
    PluginWasUpdated();
    ApplyGamepadOptions();
    THISCALL( Hook_CGameManager_Menu )(i);
    Hook_CGameManager_Menu.Detach();
  }
}
// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	void zCGamepadQuickKeys::Show( zCView* parent ) {
	}

	void zCGamepadQuickKeys::Close() {
	}

	string zCGamepadQuickKeys::GetDescription() {
		switch( Union.GetSystemLanguage() ) {
      case Lang_Rus: return (W L"Горячие клавиши").   WToA( ANSI_CODEPAGE_CYRILLIC );
      case Lang_Ger: return (W L"Hotkeys").					  WToA( ANSI_COPEDAGE_NORTHORWESTERN_EUROPEAN );
      case Lang_Pol: return (W L"Skróty klawiszowe"). WToA( ANSI_COPEDAGE_CENTRALOREASTERN_EUROPEAN );
      default:       return (W L"Hotkeys" ).				  WToA( ANSI_COPEDAGE_NORTHORWESTERN_EUROPEAN );
    }
	}

	int zCGamepadQuickKeys::HandleEvent( int key ) {
		// TODO
		return False;
	}

	zCGamepadQuickKeys* zCGamepadQuickKeys::GetInstance() {
		static zCGamepadQuickKeys* menu = new zCGamepadQuickKeys();
		return menu;
	}
}
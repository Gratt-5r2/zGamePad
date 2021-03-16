// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	zCGamepadOverlay::zCGamepadOverlay() {
		Parent = Null;
		IsOpened = False;
	}



	bool zCGamepadOverlay::Opened() {
		return IsOpened;
	}



	void zCGamepadOverlay::InsertChild( zCGamepadOverlay* overlay ) {
		Childs += overlay;
	}



	void zCGamepadOverlay::RemoveChild( zCGamepadOverlay* overlay ) {
		if( Childs.HasEqual( overlay ) )
			Childs -= overlay;
	}



  string zCGamepadOverlay::GetDescription() {
		return "Unnamed overlay";
	}




	void OverlaysLoop() {
		if( !player || player->IsDead() || !player->IsHuman() )
			return;

		static zCGamepadQuickMenu* quickMenu = zCGamepadQuickMenu::GetInstance();
		if( zKeyToggled( KEY_3 ) )
			quickMenu->Show( screen );

		static zCGamepadControlsHelp* controlsHelp = zCGamepadControlsHelp::GetInstance();
	}
}
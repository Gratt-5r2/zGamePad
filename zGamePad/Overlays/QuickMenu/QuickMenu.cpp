// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	zTGamepadQuickMenu_Selector::zTGamepadQuickMenu_Selector( zCGamepadQuickMenu* parent ) : zCView( 0, 0, 8192, 8192 ) {
		InsertBack( "BLACK.TGA" );
		SetAlphaBlendFunc( zRND_ALPHA_FUNC_ADD );
		Parent = parent;
		Description = "???";
		SetFont( Parent->GetFontName() );
	}



  void zTGamepadQuickMenu_Selector::Blit() {
		zCView::Blit();
  }



  void zTGamepadQuickMenu_Selector::Show() {
		int fontY = FontY();
		Print( 512, 4196 - fontY / 2, Description );

	  // PrintCY( 512, Description ); // PB have a mistake in the PrintCY function !!! FIXME
  }



  void zTGamepadQuickMenu_Selector::Close() {
		ClrPrintwin();
  }








	zCGamepadQuickMenu::zCGamepadQuickMenu() : zCGamepadOverlay() {
		InsertBack( "BLACK.TGA" );
		SetAlphaBlendFunc( zRND_ALPHA_FUNC_BLEND );
		SetTransparency( 150 );
		ActiveOverlay = Null;
		SetFont( "FONT_OLD_20_WHITE.TGA" );

		InsertChild( zCGamepadQuickBar_Items::GetInstance() );
		// InsertChild( zCGamepadQuickKeys::GetInstance() ); // TODO
		InsertChild( zCGamepadQuickBar_Weapons::GetInstance() );

		LeftSide = new zCView();
		LeftSide->InsertBack( "QUICKMENU_LEFTSIDE.TGA" );
		LeftSide->SetAlphaBlendFunc( zRND_ALPHA_FUNC_ADD );
		InsertItem( LeftSide );
	}



	void zCGamepadQuickMenu::Blit() {
		// SetDaylightColor();
    zCView::Blit();

#if ENGINE < Engine_G2
		float x, y, z;
		Input_GetMousePosReal( x, y, z );
		if( z != 0 )
      SetKeyStateAndInsert( z > 0 ? MOUSE_WHEELUP : MOUSE_WHEELDOWN, True );
#endif
	}



	void zCGamepadQuickMenu::UpdateSelectors () {
		uint selectorsCount = Selectors.GetNum();

		int sy = 512;
		int sx = Parent->anx( Parent->nay( sy ) );
	}



	void zCGamepadQuickMenu::Show( zCView* parent ) {
		if( IsOpened )
			return;

		Parent = parent;
		ztimer->factorMotion /= 3.0f;
		Parent->InsertItem( this );
		SetEnableHandleEvent( True );
		SetHandleEventTop();
		SetSize( 8192, 8192 );
		IsOpened = True;

		SetSelectedMenu( zKeyToggled( MOUSE_UP ) ? Childs.GetNum() - 1 : 0 );

		uint itemsCount			= Selectors.GetNum();
		int itemSizeY				= FontY() * 2;
		int itemSizeY_Total = itemSizeY * itemsCount;
		int itemPosY_Start  = 4196 - itemSizeY_Total / 2;

		for( uint i = 0; i < itemsCount; i++ ) {
			auto selector = Selectors[i];
			selector->SetSize( 2048, itemSizeY );
			selector->SetPos( 512, itemPosY_Start + itemSizeY * i );
			selector->Show();
		}

		int leftSizeSX = 16;
		int leftSizeSY = itemSizeY_Total + 512;
		int leftSizePX = 512 - leftSizeSX / 2;
		int leftSizePY = itemPosY_Start - 256;
		LeftSide->SetPos( leftSizePX, leftSizePY );
		LeftSide->SetSize( leftSizeSX, leftSizeSY );
	}



	void zCGamepadQuickMenu::Close() {
		ztimer->factorMotion *= 3.0f;
		Parent->RemoveItem( this );
		SetEnableHandleEvent( False );
		IsOpened = False;
		SetSelectedMenu( Invalid );
		Parent = Null;
	}



	void zCGamepadQuickMenu::SelectNextMenu() {
		if( !ActiveOverlay )
			return;

		SetSelectedMenu( (SelectedMenu + 1) % Selectors.GetNum() );
	}



	void zCGamepadQuickMenu::SelectPrevMenu() {
		if( !ActiveOverlay )
			return;

		SetSelectedMenu( (SelectedMenu - 1) % Selectors.GetNum() );
	}



	void zCGamepadQuickMenu::SetSelectedMenu( const uint& index ) {
		if( ActiveOverlay ) {
			ActiveOverlay->Close();
			ActiveOverlay = Null;
			Selectors[SelectedMenu]->InsertBack( Null );
		}

		if( index != Invalid ) {
			ActiveOverlay = Childs[index];
			ActiveOverlay->Show( this );
			Selectors[index]->InsertBack( "QUICKMENU_SELECTEDITEM.TGA" );
		}

		SelectedMenu = index;
	}



	void zCGamepadQuickMenu::InsertChild( zCGamepadOverlay* overlay ) {
		zCGamepadOverlay::InsertChild( overlay );
		zTGamepadQuickMenu_Selector* selector = Selectors.Insert( new zTGamepadQuickMenu_Selector( this ) );
		selector->Description = overlay->GetDescription();
		InsertItem( selector );
	}



	void zCGamepadQuickMenu::RemoveChild( zCGamepadOverlay* overlay ) {
		int index = Childs.SearchEqual( overlay );
		if( index != Invalid ) {
			RemoveItem( Selectors[index] );
			Selectors.DeleteAt( index );
		}

		zCGamepadOverlay::RemoveChild( overlay );
	}



	void zCGamepadQuickMenu::ChangeColor( const zCOLOR& color ) {
		zCGamepadOverlay::ChangeColor( color );
		LeftSide->SetColor( color );
		for( uint i = 0; i < Selectors.GetNum(); i++ ) {
			Selectors[i]->SetColor( color );
			auto* textLine = Selectors[i]->textLines.GetNextInList();
			while( textLine ) {
				zCViewText* textData = textLine->GetData();
				textData->color = color;
				textLine = textLine->GetNextInList();
			}
		}

		if( ActiveOverlay )
			ActiveOverlay->ChangeColor( color );
	}

	



	// Can or not change spell in other actions.
	// For example if need to swap spells in
	// hand - player can did it from all body states.
	static bool ForceDrawMagic = false;

	static bool CheckOverlayMagBook( zCGamepadOverlay* overlay ) {
		zCGamepadQuickBar_Weapons* quickBar_magBook = dynamic_cast<zCGamepadQuickBar_Weapons*>(overlay);
		if( quickBar_magBook != Null ) {
			uint circleID, cellID;
			quickBar_magBook->GetSelectedCellID( circleID, cellID );
			uint keyToggled = (circleID == 0 ? KEY_1 : KEY_4) + cellID;

			if( circleID != Invalid && cellID != Invalid ) {
				ForceDrawMagic = keyToggled >= KEY_4;
				zinput->SetKey( keyToggled, True );
				ogame->HandleEvent( keyToggled );
				zinput->SetKey( keyToggled, False );
				ForceDrawMagic = false;
			}
			return true;
		}

		return false;
	}



	static void Npc_UseItem( oCNpc* npc, oCItem* item ) {
		if( !npc || !item )
			return;

		if( npc->anictrl->state != zCAIPlayer::zMV_STATE_STAND ) {
			ogame->GetTextView()->Printwin( Opt_UseItemError );
			return;
		}

		static oCMsgManipulate* useItem = Null;
		if( useItem && npc->GetEM()->messageList.IsInList( useItem ) )
			return;

		oCMsgMovement* standUp = new oCMsgMovement( oCMsgMovement::EV_STANDUP, 0 );
		oCMsgWeapon* removeWeaponMsg = new oCMsgWeapon( oCMsgWeapon::EV_REMOVEWEAPON, 0, 0 );
		useItem = !item->GetSchemeName().IsEmpty() ?
			new oCMsgManipulate( oCMsgManipulate::EV_USEITEMTOSTATE, item, npc->interactItemCurrentState + 1 ) :
			new oCMsgManipulate( oCMsgManipulate::EV_EQUIPITEM, item, 0 );

		npc->GetEM()->OnMessage( standUp, npc );
		npc->GetEM()->OnMessage( removeWeaponMsg, npc );
		npc->GetEM()->OnMessage( useItem, npc );
	}



	static bool CheckOverlayQuickBar( zCGamepadOverlay* overlay ) {
		zCGamepadQuickBar_Items* quickBar_items = dynamic_cast<zCGamepadQuickBar_Items*>(overlay);
		if( quickBar_items != Null ) {
			oCItem* item = quickBar_items->GetSelectedItem();
			Npc_UseItem( player, item );
			return true;
		}

		return false;
	}



	int zCGamepadQuickMenu::HandleEvent( int key ) {
		sizeof( zCMenu );
		if( key == KEY_F1 ) {
			static zCMenu* menu = Null;
			if( menu == Null ) {
				menu = zCMenu::Create( Z "ZGAMEPAD_MENU_OPT" );
				if( menu ) {
					ogame->Pause();
					menu->Run();
					menu->Release();
					ogame->Unpause();
					gameMan->ApplySomeSettings();
					SetHandleEventTop();
					menu = Null;
				}
			}

			return True;
		}

		if( key == KEY_ESCAPE || key == MOUSE_BUTTONRIGHT ) {
			Close();
			return True;
		}

		if( key == KEY_RETURN || key == KEY_LCONTROL || key == MOUSE_BUTTONLEFT ) {
			if( CheckOverlayMagBook( ActiveOverlay ) ) {
				Close();
				return True;
			}

			if( CheckOverlayQuickBar( ActiveOverlay ) ) {
				Close();
				return True;
			}
		}

		if( key == MOUSE_WHEELUP ) {
			SelectPrevMenu();
			return True;
		}

		if( key == MOUSE_WHEELDOWN ) {
			SelectNextMenu();
			return True;
		}

		return True;
	}



	zCGamepadQuickMenu::~zCGamepadQuickMenu() {
		RemoveItem( LeftSide );
		delete LeftSide;

		for( uint i = 0; i < Selectors.GetNum(); i++ ) {
			auto selector = Selectors[i];
			RemoveItem( selector );
			delete selector;
		}
	}



	zCGamepadQuickMenu* zCGamepadQuickMenu::GetInstance() {
		static zCGamepadQuickMenu* menu = new zCGamepadQuickMenu();
		return menu;
	}
}
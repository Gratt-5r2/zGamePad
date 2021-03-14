// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  zTGamepadQuickBarCell::zTGamepadQuickBarCell() {
    Item = Null;
    InstanceID = Invalid;
    Background = new zCView( 0, 0, 8192, 8192 );
    Background_Highlight = new zCView( 0, 0, 8192, 8192 );
    Background_Highlight->SetAlphaBlendFunc( zRND_ALPHA_FUNC_ADD );
    Background_Highlight->SetTransparency( 255 );
    ItemRenderer = new zCView( 0, 0, 8192, 8192 );
    ItemRenderer->SetFont( "FONT_OLD_20_WHITE.TGA" );
    Foreground = new zCView( 0, 0, 8192, 8192 );
    Foreground->SetFont( "FONT_OLD_10_WHITE.TGA" );
    Foreground->SetAlphaBlendFunc( zRND_ALPHA_FUNC_ADD );
    Foreground->SetTransparency( 210 );
    Foreground->InsertBack( "SPELLBOOK_AMOUNT.TGA" );
  }


  void zTGamepadQuickBarCell::SetItem( oCItem* item ) {
    InstanceID = item != Null ? item->instanz : Invalid;
    Item = Null;
  }



  void zTGamepadQuickBarCell::SetViewport( const int& px, const int& py, const int& sx, const int& sy ) {
    ItemRenderer->SetPos( px, py );
    ItemRenderer->SetSize( sx, sy );
    Foreground->SetPos( px, py + sy / 2 );
    Foreground->SetSize( sx, sy );
  }





  zCGamepadQuickBar::zCGamepadQuickBar() : zCGamepadOverlay() {
    Alignment     = zEGamepadQuickBarAlignment_Right;
    LastCircleID = Invalid;
    LastCellID   = Invalid;
    SetFont( "FONT_OLD_20_WHITE.TGA" );
    NeedToMarkEquipedItems = false;
  }



  void zCGamepadQuickBar::SetBaseTextureName( const zSTRING& textureName ) {
    BaseTextureName = textureName;
  }



  void zCGamepadQuickBar::SetItem( oCItem* item, uint& circleID, uint& cellID ) {
    if( item != Null ) {
      int instance = item->instanz;
      for( uint i = 0; i < Circles.GetNum(); i++ )
        for( uint j = 0; j < Circles[i].GetNum(); j++ )
          if( Circles[i][j].InstanceID == instance )
            Circles[i][j].InstanceID = Invalid;
    }

    auto& cell = GetCell( circleID, cellID );
    cell.SetItem( item );
  }



  bool zCGamepadQuickBar::SetItemInActiveCell( oCItem* item ) {
    uint circleID, cellID;
    GetSelectedCellID( circleID, cellID );
    if( circleID != Invalid && cellID != Invalid ) {
      SetItem( item, circleID, cellID );
      return true;
    }

    return false;
  }



  void zCGamepadQuickBar::PushItem( oCItem* item, const uint& circleID ) {
    if( Circles.GetNum() < circleID + 1 ) {
      uint needCirclesNum = circleID + 1 - Circles.GetNum();
      for( uint i = 0; i < needCirclesNum; i++ )
        Circles.Create();
    }

    Circles[circleID].Create().SetItem( item );
  }



  void zCGamepadQuickBar::UpdateCells() {
    uint circlesNum = Circles.GetNum();
    if( circlesNum == 0 )
      return;

    uint distancePerCircle = 4196 / circlesNum;
    for( uint i = 0; i < circlesNum; i++ ) {
      uint cellsNum = Circles[i].GetNum();
      if( !cellsNum )
        continue;

      float anglePerCell = RAD360 / (float)cellsNum;
      zVEC2 itemPosition( 0.0f, distancePerCircle * (i + 1) - distancePerCircle / 2 );
      itemPosition.Rotate( anglePerCell * 0.5f );

      for( uint j = 0; j < cellsNum; j++ ) {
        auto& cell = Circles[i][j];
        int itemRendererSize = distancePerCircle * 75 / 100; // (int)((float)distancePerCircle * 0.75f);
        cell.SetViewport( 4096 + itemPosition[VX] - itemRendererSize / 2,
                          4096 - itemPosition[VY] - itemRendererSize / 2,
                          itemRendererSize,
                          itemRendererSize );

        itemPosition.Rotate( anglePerCell );
        zSTRING textureName  = string::Combine( "%z_%u_%u.TGA",    BaseTextureName, i, j );
        zSTRING textureNameH = string::Combine( "%z_%u_%u_H.TGA", BaseTextureName, i, j );
        cell.Background->InsertBack( textureName );
        cell.Background_Highlight->InsertBack( textureNameH );
      }
    }
  }



  zTGamepadQuickBarCell& zCGamepadQuickBar::GetCell( const uint& circleID, const uint& cellID ) {
    return Circles[circleID][cellID];
  }



  void zCGamepadQuickBar::GetSelectedCellID( uint& circleID, uint& cellID ) {
    if( !IsOpened ) {
      circleID = LastCircleID;
      cellID   = LastCellID;
      return;
    }

    circleID = Invalid;
    cellID   = Invalid;
    uint circlesNum = Circles.GetNum();
    if( !circlesNum )
      return;
    
    static zTStickState stickLeft, stickRight;
    XInputDevice.GetStickStatesCircle( stickLeft, stickRight );

    // TO DO поменять ход от центрального круга
    int stickDistInline = STICK_MAX * 0.3;
    int stickDistPerCircle = STICK_MAX * 0.7 / (int&)circlesNum;
    int stickLength = stickRight.Length();
    if( stickLength <= DEADZONE_R / 3 )
      return;

    circleID     = stickRight.Length() / (stickDistInline + stickDistPerCircle);
    LastCircleID = circleID;
    auto& circle = Circles[circleID];
    if( circle.IsEmpty() )
      return;

    float anglePerCell = RAD360 / (float)circle.GetNum();
    zVEC2 vector       = zVEC2( (float)stickRight.X, (float)stickRight.Y );
    float angle         = vector.GetAngle();
    cellID             = angle / RAD360 * ((float)circle.GetNum());
    LastCellID         = cellID;
  }



  oCItem* zCGamepadQuickBar::GetSelectedItem() {
    uint circleID, cellID;
    GetSelectedCellID( circleID, cellID );
    if( circleID != Invalid && cellID != Invalid )
      return GetCell( circleID, cellID ).Item;
    
    return Null;
  }



  static zCOLOR SelectedCellColor = zCOLOR( 255, 110, 110 );

  void zCGamepadQuickBar::BlitCells() {
    static zCWorld* wld  = zfactory->CreateWorld();
    float highlightAlpha = sin(((float)Timer::GetTime()) * 0.01f) * 40.0f;

    uint circleID, cellID;
    GetSelectedCellID( circleID, cellID );

    for( uint i = 0; i < Circles.GetNum(); i++ ) {
      auto& circle = Circles[i];
      for( uint j = 0; j < circle.GetNum(); j++ ) {
        bool needHighlight = circleID == i && cellID == j;
        auto& cell         = circle[j];
        oCItem* item       = cell.Item;

        cell.Background_Highlight->SetTransparency( 215 + (int)highlightAlpha );
        cell.Foreground->ClrPrintwin();

        InsertItem( cell.Background );
        InsertItem( cell.ItemRenderer );
        InsertItem( cell.Foreground );
        if( needHighlight )
          InsertItem( cell.Background_Highlight );

        if( NeedToMarkEquipedItems ) {
          cell.Background->color = item && item->HasFlag( ITM_FLAG_ACTIVE ) ?
            SelectedCellColor : GFX_WHITE;
        }

        cell.Background->Blit();
        cell.ItemRenderer->Blit();
        if( needHighlight )
          cell.Background_Highlight->Blit();

        if( item ) {
          item->RenderItem( wld, cell.ItemRenderer, 0.0 );

          if( item->amount > 1 ) {
            cell.Foreground->PrintCXY( Z item->amount );
            cell.Foreground->Blit();
          }
        }

        RemoveItem( cell.Background );
        RemoveItem( cell.ItemRenderer );
        RemoveItem( cell.Foreground );
        if( needHighlight )
          RemoveItem( cell.Background_Highlight );
      }
    }
  }



  void zCGamepadQuickBar::Blit() {
    uint circleID, cellID;
    GetSelectedCellID( circleID, cellID );
    if( circleID != Invalid && cellID != Invalid ) {
      auto& cell = GetCell( circleID, cellID );
      if( cell.Item != Null ) {
        zSTRING& itemName   = cell.Item->description;
        int itemAmount      = cell.Item->amount;
        zSTRING description = string::Combine( "%z (%i)", itemName, itemAmount );
        int itemNameSizeX   = Parent->FontSize( description );
        int itemNamePosX    = vposx + (vsizex - itemNameSizeX) / 2;
        Parent->Print( itemNamePosX, vposy + vsizey, description );
        Parent->BlitText();
        Parent->ClrPrintwin();
      }
    }

    zCView::Blit();
    BlitCells();
  }






  void zCGamepadQuickBar::Show( zCView* parent ) {
    if( IsOpened )
      return;

    Parent = parent;
    OverlayMouseHooksCount++;

    Parent->InsertItem( this );
    SetEnableHandleEvent( True );
    SetHandleEventTop();
    int sy = 4196;
    int sx = Parent->anx( Parent->nay( sy ) );
    SetSize( sx, sy );

         if( Alignment == zEGamepadQuickBarAlignment_Left )   SetPos( sx / 2, sy / 2 );
    else if( Alignment == zEGamepadQuickBarAlignment_Center ) SetPos( 4196 - sx / 2, sy / 2 );
    else                                                      SetPos( 8192 - sx - sx / 2, sy / 2 );

    ShowCells();
    IsOpened = True;
  }



  void zCGamepadQuickBar::ShowAt( zCView* parent, const zEGamepadQuickBarAlignment& align ) {
    Alignment = align;
    Show( parent );
    Alignment = zEGamepadQuickBarAlignment_Right;
  }



  void zCGamepadQuickBar::ShowCells() {
    Array<zTGamepadQuickBarCell*> needToFind;

    for( uint i = 0; i < Circles.GetNum(); i++ ) {
      auto& circle = Circles[i];
      for( uint j = 0; j < circle.GetNum(); j++ ) {
        if( circle[j].InstanceID != Invalid ) {
          needToFind.Insert( &circle[j] );
          circle[j].Item = Null;
        }
      }
    }

    player->inventory2.UnpackAllItems();
    auto list = player->inventory2.contents->next;
    while( list ) {
      oCItem* item = list->data;
      for( uint i = 0; i < needToFind.GetNum(); i++ ) {
        if( needToFind[i]->InstanceID == item->instanz ) {
          needToFind[i]->Item = item;
          needToFind.RemoveAt( i-- );
        }
      }

      list = list->next;
    }

    // TO DO g1
  }



  void zCGamepadQuickBar::Close() {
    if( !IsOpened )
      return;

    OverlayMouseHooksCount--;

    owner->RemoveItem( this );
    SetEnableHandleEvent( False );
    CloseCells();
    IsOpened = False;
    Parent = Null;
  }



  void zCGamepadQuickBar::CloseCells() {
    /*for( uint i = 0; i < Circles.GetNum(); i++ ) {
      auto& circle = Circles[i];
      for( uint j = 0; j < circle.GetNum(); j++ )
        circle[j].Item = Null;
    }*/
  }



  int zCGamepadQuickBar::HandleEvent( int key ) {
    if( key == KEY_UP ) {

      return False;
    }

    if( key == KEY_DOWN ) {

      return False;
    }

    if( key == KEY_ESCAPE || key == MOUSE_BUTTONRIGHT ) {
      Close();
      return False;
    }

    if( key == KEY_RETURN || key == KEY_LCONTROL || key == MOUSE_BUTTONLEFT ) {
      Close();
      return False;
    }

    return False;
  }












  zCGamepadQuickBar_Weapons::zCGamepadQuickBar_Weapons() : zCGamepadQuickBar() {
    for( uint i = 0; i < 2; i++ )
      PushItem( Null, 0 );

    for( uint i = 0; i < 7; i++ )
      PushItem( Null, 1 );

    SetBaseTextureName( "SPELLBOOK" );
    NeedToMarkEquipedItems = false;
  }



  void zCGamepadQuickBar_Weapons::Show( zCView* parent ) {
    if( IsOpened )
      return;

    Parent = parent;
    for( uint i = 0; i < 7; i++ ) {
      auto& cell = GetCell( 1, i );

      oCSpell* spell = player->GetSpellBook()->GetSpellByKey( i + 1 );
      if( !spell ) {
        cell.SetItem( Null );
        continue;
      }

      oCItem* item = player->GetSpellBook()->GetSpellItem( spell );
      cell.SetItem( item );
    }

    GetCell( 0, 0 ).SetItem( player->GetEquippedMeleeWeapon() );
    GetCell( 0, 1 ).SetItem( player->GetEquippedRangedWeapon() );

    zCGamepadQuickBar::Show( parent );
    UpdateCells();
  }



  string zCGamepadQuickBar_Weapons::GetDescription() {
    switch( Union.GetSystemLanguage() ) {
      case Lang_Rus: return (W L"Магия и оружия").     WToA( ANSI_CODEPAGE_CYRILLIC );
      case Lang_Deu: return (W L"Magie und Waffen").   WToA( ANSI_COPEDAGE_NORTHORWESTERN_EUROPEAN );
      case Lang_Pol: return (W L"Magia i broń").       WToA( ANSI_COPEDAGE_CENTRALOREASTERN_EUROPEAN );
      default:       return (W L"Magic and weapons" ). WToA( ANSI_COPEDAGE_NORTHORWESTERN_EUROPEAN );
    }
  }



  zCGamepadQuickBar_Weapons* zCGamepadQuickBar_Weapons::GetInstance() {
    static zCGamepadQuickBar_Weapons* quickBar_weapons = new zCGamepadQuickBar_Weapons();
    return quickBar_weapons;
  }









  zCGamepadQuickBar_Items::zCGamepadQuickBar_Items() : zCGamepadQuickBar() {

    string QuickSlots_Option = "2, 7";
    Union.GetSysPackOption().Read( QuickSlots_Option, "zGamePad", "QuickBar_Counts", QuickSlots_Option );
    QuickSlots_Option.Regex_Replace( "\\s", "" );
    Array<string> QuickSlots_Tokens = QuickSlots_Option.Split( "," );

    for( uint i = 0; i < QuickSlots_Tokens.GetNum(); i++ ) {
      uint numInCircle = QuickSlots_Tokens[i].ToInt32();
      for( uint j = 0; j < numInCircle; j++ )
        PushItem( Null, i );
    }

    string QuickSlots_TextureBase = "QUICKSLOT";
    Union.GetSysPackOption().Read( QuickSlots_TextureBase, "zGamePad", "QuickBar_TextureBase", QuickSlots_TextureBase );
    SetBaseTextureName( QuickSlots_TextureBase );
    NeedToMarkEquipedItems = true;
  }



  void zCGamepadQuickBar_Items::ResetVisuals() {
    for( uint i = 0; i < Circles.GetNum(); i++ )
      for( uint j = 0; j < Circles[i].GetNum(); j++ )
        Circles[i][j].Item = Null;
  }



  void zCGamepadQuickBar_Items::ResetInstances() {
    for( uint i = 0; i < Circles.GetNum(); i++ )
      for( uint j = 0; j < Circles[i].GetNum(); j++ )
        Circles[i][j].InstanceID = Invalid;
  }



  Array<zTGamepadQuickBarCell*> zCGamepadQuickBar_Items::GetNotEmptyCells() {
    Array<zTGamepadQuickBarCell*> array;
    for( uint i = 0; i < Circles.GetNum(); i++ )
      for( uint j = 0; j < Circles[i].GetNum(); j++ )
        if( Circles[i][j].InstanceID != Invalid )
          array.Insert( &Circles[i][j] );

    return array;
  }



  void zCGamepadQuickBar_Items::UpdateVisuals() {
    ResetVisuals();
    auto notEmptyCells = GetNotEmptyCells();

    player->inventory2.UnpackAllItems();
    auto list = player->inventory2.contents->next;
    while( list ) {
      oCItem* item = list->data;

      for( uint i = 0; i < notEmptyCells.GetNum(); i++ ) {
        if( item->instanz == notEmptyCells[i]->InstanceID ) {
          notEmptyCells[i]->Item = item;
          notEmptyCells.RemoveAt( i-- );
        }
      }

      list = list->next;
    }
  }



  void zCGamepadQuickBar_Items::Archive( zCArchiver& ar ) {
    int notEmptyCell_Count = (int)GetNotEmptyCells().GetNum();
    ar.WriteInt( "NotEmpryCell_Count", notEmptyCell_Count );

    for( uint i = 0; i < Circles.GetNum(); i++ ) {
      for( uint j = 0; j < Circles[i].GetNum(); j++ ) {
        auto& cell = Circles[i][j];
        if( cell.InstanceID != Invalid ) {
          auto symbol = parser->GetSymbol( cell.InstanceID );
          if( !symbol )
            continue;

          int cellID[] = { i, j };
          ar.WriteRaw( "CellID", cellID, 8 );
          ar.WriteString( "InstanceName", symbol->name );
        }
      }
    }
  }
  

  
  void zCGamepadQuickBar_Items::Unarchive( zCArchiver& ar ) {
    ResetVisuals();
    ResetInstances();

    int notEmptyCell_Count = 0;
    ar.ReadInt( "NotEmpryCell_Count", notEmptyCell_Count );

    for( uint i = 0; i < (uint&)notEmptyCell_Count; i++ ) {
      int cellID[] = { 0, 0 };
      zSTRING symbolName;
      ar.ReadRaw( "CellID", cellID, 8 );
      ar.ReadString( "InstanceName", symbolName );
      auto symbolID = parser->GetIndex( symbolName );
      if( symbolID == Invalid )
        continue;

      if( cellID[0] < Circles.GetNum() ) {
        if( cellID[1] < Circles[cellID[0]].GetNum() ) {
          auto& cell = Circles[cellID[0]][cellID[1]];
          cell.InstanceID = symbolID;
        }
      }
    }
  }



  void zCGamepadQuickBar_Items::Show( zCView* parent ) {
    if( IsOpened )
      return;

    Parent = parent;
    UpdateVisuals();
    zCGamepadQuickBar::Show( parent );
    UpdateCells();
  }



  string zCGamepadQuickBar_Items::GetDescription() {
    switch( Union.GetSystemLanguage() ) {
      case Lang_Rus: return (W L"Предметы").    WToA( ANSI_CODEPAGE_CYRILLIC );
      case Lang_Deu: return (W L"Artikel").     WToA( ANSI_COPEDAGE_NORTHORWESTERN_EUROPEAN );
      case Lang_Pol: return (W L"Przedmiotów"). WToA( ANSI_COPEDAGE_CENTRALOREASTERN_EUROPEAN );
      default:       return (W L"Items" ).      WToA( ANSI_COPEDAGE_NORTHORWESTERN_EUROPEAN );
    }
  }



  void zCGamepadQuickBar_Items::ResetAll() {
    ResetVisuals();
    ResetInstances();
  }



  zCGamepadQuickBar_Items* zCGamepadQuickBar_Items::GetInstance() {
    static zCGamepadQuickBar_Items* quickBar_weapons = new zCGamepadQuickBar_Items();
    return quickBar_weapons;
  }
}
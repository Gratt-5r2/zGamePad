// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  zTGamepadQuickBarCell::zTGamepadQuickBarCell() {
    Item = Null;
    InstanceID = Invalid;
    Offset = 1.0f;
    Background = new zCView( 0, 0, 8192, 8192 );
    Background_Highlight = new zCView( 0, 0, 8192, 8192 );
    Background_Highlight->SetAlphaBlendFunc( zRND_ALPHA_FUNC_ADD );
    Background_Highlight->SetTransparency( 255 );
    ItemRenderer = new zCView( 0, 0, 8192, 8192 );
    ItemRenderer->SetFont( "FONT_OLD_20_WHITE.TGA" );
    Foreground = new zCView( 0, 0, 8192, 8192 );
    Foreground->SetFont( "FONT_OLD_10_WHITE.TGA" );
    Foreground->SetAlphaBlendFunc( zRND_ALPHA_FUNC_BLEND );
    Foreground->SetTransparency( 150 );
    Foreground->InsertBack( "SPELLBOOK_AMOUNT.TGA" );
  }


  void zTGamepadQuickBarCell::SetItem( oCItem* item ) {
    InstanceID = item != Null ? item->instanz : Invalid;
    Item = Null;
  }



  void zTGamepadQuickBarCell::SetViewport( const int& px, const int& py, const int& sx, const int& sy ) {
    ItemRenderer->SetPos( px, py );
    ItemRenderer->SetSize( sx, sy );
    Foreground->SetPos( px, py + sy / 3 );
    Foreground->SetSize( sx, sy );
  }





  zCGamepadQuickBar::zCGamepadQuickBar() : zCGamepadOverlay() {
    Alignment  = zEGamepadQuickBarAlignment_Right;
    LastRingID = Invalid;
    LastCellID = Invalid;
    SetFont( "FONT_OLD_20_WHITE.TGA" );
    NeedToMarkEquipedItems = false;
  }



  void zCGamepadQuickBar::SetBaseTextureName( const zSTRING& textureName ) {
    BaseTextureName = textureName;
  }



  void zCGamepadQuickBar::SetItem( oCItem* item, uint& ringID, uint& cellID ) {
    if( item != Null ) {
      int instance = item->instanz;
      for( uint i = 0; i < Rings.GetNum(); i++ )
        for( uint j = 0; j < Rings[i].GetNum(); j++ )
          if( Rings[i][j].InstanceID == instance )
            Rings[i][j].InstanceID = Invalid;
    }

    auto& cell = GetCell( ringID, cellID );
    cell.SetItem( item );
  }



  bool zCGamepadQuickBar::SetItemInActiveCell( oCItem* item ) {
    uint ringID, cellID;
    GetSelectedCellID( ringID, cellID );
    if( ringID != Invalid && cellID != Invalid ) {
      SetItem( item, ringID, cellID );
      return true;
    }

    return false;
  }



  zTGamepadQuickBarCell& zCGamepadQuickBar::PushItem( oCItem* item, const uint& ringID ) {
    if( Rings.GetNum() < ringID + 1 ) {
      uint needRingsNum = ringID + 1 - Rings.GetNum();
      for( uint i = 0; i < needRingsNum; i++ )
        Rings.Create();
    }

    auto& cell = Rings[ringID].Create();
    cell.SetItem( item );
    return cell;
  }



  static Array<float> QuickBar_ReadOffsets() {
    Array<float> offsets;
    string QuickSlots_Offset = Opt_QuickBar_Offsets;
    QuickSlots_Offset.Regex_Replace( "\\s", "" );
    Array<string> tokens = QuickSlots_Offset.Split( "," );
    for( uint i = 0; i < tokens.GetNum(); i++ )
      offsets += tokens[i].ToReal32();
    
    return offsets;
  }

  static Array<uint> QuickBar_ReadCounts() {
    Array<uint> counts;
    string QuickSlots_Option = Opt_QuickBar_Counts;
    QuickSlots_Option.Regex_Replace( "\\s", "" );
    Array<string> tokens = QuickSlots_Option.Split( "," );
    for( uint i = 0; i < tokens.GetNum(); i++ )
      counts += tokens[i].ToInt32();

    return counts;
  }



  void zCGamepadQuickBar::UpdateCells() {
    uint ringsNum = Rings.GetNum();
    if( ringsNum == 0 )
      return;

    uint distancePerRing = 4196 / ringsNum;
    for( uint i = 0; i < ringsNum; i++ ) {
      uint cellsNum = Rings[i].GetNum();
      if( !cellsNum )
        continue;

      float anglePerCell = RAD360 / (float)cellsNum;
      uint itemDistance  = distancePerRing * (i + 1) - distancePerRing / 2;
      zVEC2 itemPosition = zVEC2( 0.0f, (float)itemDistance );
      itemPosition.Rotate( anglePerCell * 0.5f );

      for( uint j = 0; j < cellsNum; j++ ) {
        auto& cell = Rings[i][j];
        int itemRendererSize = distancePerRing * 75 / 100;
        cell.SetViewport( 4096 + (int)(itemPosition[VX] * cell.Offset) - itemRendererSize / 2,
                          4096 - (int)(itemPosition[VY] * cell.Offset) - itemRendererSize / 2,
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



  zTGamepadQuickBarCell& zCGamepadQuickBar::GetCell( const uint& ringID, const uint& cellID ) {
    return Rings[ringID][cellID];
  }



  float SmoothIncrease( const float& value, const float& max, const uint& strength = 1 ) {
    static float limit = PI / 2.0f;
    float multiplier = limit / max;
    float result = value;

    for( uint i = 0; i < strength; i++ )
      result = (cos( result * multiplier + PI ) + 1.0f) * max;

    return result;
  }

  void zCGamepadQuickBar::GetSelectedCellID( uint& ringID, uint& cellID ) {
    static float stickCircleMax = 25000.0f;
    if( !IsOpened ) {
      ringID = LastRingID;
      cellID = LastCellID;
      return;
    }

    ringID = Invalid;
    cellID = Invalid;
    uint ringsNum = Rings.GetNum();
    if( !ringsNum )
      return;

    static const float JOY_EPS = 0.01f;
    static const float MOUSE_EPS = 0.0005f;

    // Define a stick offset in ring coordinates
    XInputDevice.GetStickStatesCircle( StickStateLeft, StickStateRight );
    zVEC2 stickVector = zVEC2( (float)StickStateRight.X, (float)StickStateRight.Y );
    if( stickVector.Length() < JOY_EPS ) {

      // Alternative select method
      // with using the mouse device
      float x, y, z;
      Input_GetMousePosReal( x, y, z );
#if ENGINE < Engine_G2
      x /= 10;
      y /= 10;
#endif
      if( abs( x + y ) > MOUSE_EPS ) {
        MouseState += zVEC2( x, -y ) * 500.0f;
        if( MouseState.Length() > stickCircleMax )
          MouseState.Normalize() *= stickCircleMax;
      }

      stickVector += MouseState;
    }
    else
      MouseState = 0.0f;

    // Accent to the more comfortable selection of
    // center rings. Range of collisions in center in
    // quicbar must be large then at the edge.
    float smoothedLength = SmoothIncrease( stickVector.Length(), (float)stickCircleMax, 3 );
    stickVector.Normalize() *= smoothedLength;

    // Radius of one ring of ring
    int stickDistPerRing = (int)stickCircleMax / (int&)ringsNum;
    if( stickVector.Length() <= JOY_EPS )
      return;

    // Current ring id by stick offset
    ringID = (int)stickVector.Length() / stickDistPerRing;
    if( ringID >= Rings.GetNum() )
      ringID = Rings.GetNum() - 1;

    LastRingID = ringID;
    auto& ring = Rings[ringID];
    if( ring.IsEmpty() )
      return;

    // Current cell by stick angle
    float anglePerCell = RAD360 / (float)ring.GetNum();
    float angle = stickVector.GetAngle();
    cellID = (uint)(angle / RAD360 * (float)ring.GetNum());
    LastCellID = cellID;
  }



  oCItem* zCGamepadQuickBar::GetSelectedItem() {
    uint ringID, cellID;
    GetSelectedCellID( ringID, cellID );
    if( ringID != Invalid && cellID != Invalid )
      return GetCell( ringID, cellID ).Item;
    
    return Null;
  }



  static zCOLOR SelectedCellColor = zCOLOR( 255, 110, 110 );

  void zCGamepadQuickBar::BlitCells() {
    static zCWorld* wld  = zfactory->CreateWorld();
    float highlightAlpha = sin(((float)Timer::GetTime()) * 0.01f) * 40.0f;

    uint ringID, cellID;
    GetSelectedCellID( ringID, cellID );

    for( uint i = 0; i < Rings.GetNum(); i++ ) {
      auto& ring = Rings[i];
      for( uint j = 0; j < ring.GetNum(); j++ ) {
        bool needHighlight = ringID == i && cellID == j;
        auto& cell         = ring[j];
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
    uint ringID, cellID;
    GetSelectedCellID( ringID, cellID );
    if( ringID != Invalid && cellID != Invalid ) {
      auto& cell = GetCell( ringID, cellID );
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

    for( uint i = 0; i < Rings.GetNum(); i++ ) {
      auto& ring = Rings[i];
      for( uint j = 0; j < ring.GetNum(); j++ ) {
        if( ring[j].InstanceID != Invalid ) {
          needToFind.Insert( &ring[j] );
          ring[j].Item = Null;
        }
      }
    }

#if ENGINE >= Engine_G2
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
#else
    player->inventory2.UnpackAllItems();
    for( uint i = 0; i < INV_MAX; i++ ) {
      auto list = player->inventory2.inventory[i].next;
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
    }
#endif
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
    /*for( uint i = 0; i < Rings.GetNum(); i++ ) {
      auto& ring = Rings[i];
      for( uint j = 0; j < ring.GetNum(); j++ )
        ring[j].Item = Null;
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
      case Lang_Rus: return (W L"Магия и оружие").     WToA( ANSI_CODEPAGE_CYRILLIC );
      case Lang_Ger: return (W L"Magie und Waffen").   WToA( ANSI_COPEDAGE_NORTHORWESTERN_EUROPEAN );
      case Lang_Pol: return (W L"Magia i bronie").     WToA( ANSI_COPEDAGE_CENTRALOREASTERN_EUROPEAN );
      default:       return (W L"Magic and weapons" ). WToA( ANSI_COPEDAGE_NORTHORWESTERN_EUROPEAN );
    }
  }



  zCGamepadQuickBar_Weapons* zCGamepadQuickBar_Weapons::GetInstance() {
    static zCGamepadQuickBar_Weapons* quickBar_weapons = new zCGamepadQuickBar_Weapons();
    return quickBar_weapons;
  }









  zCGamepadQuickBar_Items::zCGamepadQuickBar_Items() : zCGamepadQuickBar() {
    Array<uint> counts = QuickBar_ReadCounts();
    Array<float> offsets = QuickBar_ReadOffsets();

    for( uint i = 0; i < counts.GetNum(); i++ ) {
      uint numInRing = counts[i];
      float offset = i < offsets.GetNum() ? offsets[i] : 1.0f;
      for( uint j = 0; j < numInRing; j++ )
        PushItem( Null, i ).Offset = offset;
    }

    SetBaseTextureName( Opt_QuickBar_TextureBase );
    NeedToMarkEquipedItems = true;
  }



  void zCGamepadQuickBar_Items::ResetVisuals() {
    for( uint i = 0; i < Rings.GetNum(); i++ )
      for( uint j = 0; j < Rings[i].GetNum(); j++ )
        Rings[i][j].Item = Null;
  }



  void zCGamepadQuickBar_Items::ResetInstances() {
    for( uint i = 0; i < Rings.GetNum(); i++ )
      for( uint j = 0; j < Rings[i].GetNum(); j++ )
        Rings[i][j].InstanceID = Invalid;
  }



  Array<zTGamepadQuickBarCell*> zCGamepadQuickBar_Items::GetNotEmptyCells() {
    Array<zTGamepadQuickBarCell*> array;
    for( uint i = 0; i < Rings.GetNum(); i++ )
      for( uint j = 0; j < Rings[i].GetNum(); j++ )
        if( Rings[i][j].InstanceID != Invalid )
          array.Insert( &Rings[i][j] );

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

    for( uint i = 0; i < Rings.GetNum(); i++ ) {
      for( uint j = 0; j < Rings[i].GetNum(); j++ ) {
        auto& cell = Rings[i][j];
        if( cell.InstanceID != Invalid ) {
          auto symbol = parser->GetSymbol( cell.InstanceID );
          if( !symbol )
            continue;

          uint cellID[] = { i, j };
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
      uint cellID[] = { 0, 0 };
      zSTRING symbolName;
      ar.ReadRaw( "CellID", cellID, 8 );
      ar.ReadString( "InstanceName", symbolName );
      auto symbolID = parser->GetIndex( symbolName );
      if( symbolID == Invalid )
        continue;

      if( cellID[0] < Rings.GetNum() ) {
        if( cellID[1] < Rings[cellID[0]].GetNum() ) {
          auto& cell = Rings[cellID[0]][cellID[1]];
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
      case Lang_Ger: return (W L"Gegenstände"). WToA( ANSI_COPEDAGE_NORTHORWESTERN_EUROPEAN );
      case Lang_Pol: return (W L"Przedmioty").  WToA( ANSI_COPEDAGE_CENTRALOREASTERN_EUROPEAN );
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
// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  zLastSaveInfo::zLastSaveInfo() {
    LastSavedSlotNr = -1;
    LastSavedSlotNr_Ranged = -1;
    Mode = zLastSave_UseAllSlots;
    LoadFromFile();
  }


  void zLastSaveInfo::LoadFromFile() {
    string zGamePadQuickSave;
    zGamePadQuickSave.ReadFromFile( GetSaveFileName() );
    Array<string> info = zGamePadQuickSave.Split( "|" );
    if( info.GetNum() < 4 )
      return;

    int version = info[0].ToInt32();
    LastSavedSlotNr = info[1].ToInt32();
    LastSavedSlotNr_Ranged = info[2].ToInt32();
    Mode = (zLastSaveInfoMode)info[3].ToInt32();
  }


  void zLastSaveInfo::SaveToFile() {
    string zGamePadQuickSave = string::Combine( "%i|%i|%i|%i",
      0,
      LastSavedSlotNr,
      LastSavedSlotNr_Ranged,
      Mode );
    zGamePadQuickSave.WriteToFile( GetSaveFileName() );
  }


  int zLastSaveInfo::GetFirstAvailableSlotNr() {
    switch( Mode ) {
    case zLastSave_UseAllSlots:
      return SAVEGAME_SLOT_MIN;
    case zLastSave_UseFirstSlots:
      return SAVEGAME_SLOT_MIN;
    case zLastSave_UseLastSlots:
      return SAVEGAME_SLOT_MAX - (SlotsCountMaxPerMode - 1);
    }

    return SAVEGAME_SLOT_MIN;
  }


  int zLastSaveInfo::GetLastAvailableSlotNr() {
    switch( Mode ) {
    case zLastSave_UseAllSlots:
      return SAVEGAME_SLOT_MAX;
    case zLastSave_UseFirstSlots:
      return SAVEGAME_SLOT_MIN + (SlotsCountMaxPerMode - 1);
    case zLastSave_UseLastSlots:
      return SAVEGAME_SLOT_MAX;
    }

    return SAVEGAME_SLOT_MAX;
  }


  bool zLastSaveInfo::IsOnRange( const int& slotNr ) {
    return
      slotNr >= GetFirstAvailableSlotNr() &&
      slotNr <= GetLastAvailableSlotNr();
  }


  int zLastSaveInfo::GetLastSavedSlotNr() {
    if( LastSavedSlotNr == -1 )
      return SAVEGAME_SLOT_MIN;

    return LastSavedSlotNr;
  }


  int zLastSaveInfo::GetLastSavedSlotNr_Ranged() {
    if( LastSavedSlotNr_Ranged == -1 )
      return GetFirstAvailableSlotNr();

    if( !IsOnRange( LastSavedSlotNr_Ranged ) )
      return GetLastAvailableSlotNr();

    return LastSavedSlotNr_Ranged;
  }


  int zLastSaveInfo::GetNextSaveSlotNr() {
    if( LastSavedSlotNr_Ranged == -1 )
      return GetFirstAvailableSlotNr();

    int nextNr = GetLastSavedSlotNr_Ranged() + 1;
    if( nextNr > GetLastAvailableSlotNr() )
      nextNr = GetFirstAvailableSlotNr();

    return nextNr;
  }


  void zLastSaveInfo::UpdateSaveSlotNr( const int& slotNr ) {
    if( Mode == zLastSave_DontUse )
      return;

    if( slotNr < SAVEGAME_SLOT_QUICK || slotNr > SAVEGAME_SLOT_MAX )
      return;

    LastSavedSlotNr = slotNr;
    if( IsOnRange( slotNr ) )
      LastSavedSlotNr_Ranged = slotNr;

    SaveToFile();
  }


  zLastSaveInfoMode zLastSaveInfo::GetSaveMode() {
    return Mode;
  }


  void zLastSaveInfo::SetSaveMode( const zLastSaveInfoMode& mode ) {
    Mode = mode;
  }


  string zLastSaveInfo::GetSaveFileName() {
    static string name =
      Union.GetGameDirectory() +
      A zoptions->GetDirString( zTOptionPaths::DIR_SAVEGAMES ) +
      A "zGamePadQuickSave.inf";

    return name;
  }


  zLastSaveInfo& zLastSaveInfo::GetInstance() {
    static zLastSaveInfo instance;
    return instance;
  }




  HOOK Hook_oCMenuSavegame_ScreenInit PATCH( &oCMenuSavegame::ScreenInit, &oCMenuSavegame::ScreenInit_Union );

  void oCMenuSavegame::ScreenInit_Union() {
    THISCALL( Hook_oCMenuSavegame_ScreenInit )();
    if( zLastSaveInfo::GetInstance().GetSaveMode() == zLastSave_DontUse )
      return;

    if( mode == SAVE ) {
      SAVEGAME_SLOT_MAX = CalculateSaveSlotsCount();
      SelectSlotNr( zLastSaveInfo::GetInstance().GetNextSaveSlotNr() );
    }
    else
      SelectSlotNr( zLastSaveInfo::GetInstance().GetLastSavedSlotNr() );
  }


  int oCMenuSavegame::CalculateSaveSlotsCount() {
    int count = 0;
    for( int i = 0, sel = 0; i < m_listItems.GetNum(); i++ ) {
      auto item = m_listItems[i];
      if( item->GetType() == MENU_ITEM_INPUT )
        count++;
    }

    return count;
  }


  void oCMenuSavegame::SelectSlotNr( const int& nr ) {
    for( int i = 0, sel = 0; i < m_listItems.GetNum(); i++ ) {
      auto item = m_listItems[i];
      if( !item->GetIsSelectable() )
        continue;

      int slotNr = GetMenuItemSlotNr( m_listItems[i] );
      if( slotNr < SAVEGAME_SLOT_QUICK || slotNr > SAVEGAME_SLOT_MAX )
        continue;

      if( slotNr == nr ) {
        m_mainSel = slotNr;
        SetActiveItem( item );
        m_selSlot = GetMenuItemSlotNr( m_listItems[m_mainSel] );
        HandleSlotChange( m_selSlot );
        break;
      }
    }
  }
}
// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  bool ActiveVideo = false;



  bool IsFightMode() {
    return player ? player->fmode != 0 : 0;
  }



  bool IsFightModeRange() {
    return player ? (player->fmode == NPC_WEAPON_BOW || player->fmode == NPC_WEAPON_CBOW) : 0;
  }



  bool IsCanShoot() {
    return IsFightModeRange() && zKeyPressed( GetBinded( GAME_ACTION ) );
  }



  bool IsFightModeMelee() {
    return IsFightMode() && !IsFightModeRange();
  }



  bool IsNotFightMode() {
    return player ? player->fmode == 0 : 1;
  }



  bool IsSneak() {
    return player && player->human_ai && player->human_ai->walkmode == ANI_WALKMODE_SNEAK;
  }



  bool IsDive() {
    return player && player->human_ai && player->human_ai->walkmode == ANI_WALKMODE_DIVE;
  }



  bool HasFocusTarget() {
    return player && player->GetFocusVob();
  }



  bool HasLockedTarget() {
#if ENGINE >= Engine_G2
    return HasFocusTarget() && (IsFightMode() || oCNpc::s_bTargetLocked);
#else
    return True;
#endif
  }



  bool IsInChooseWeapon() {
    return zKeyPressed( GetBinded( GAME_WEAPON ) );
  }



  bool SpellBookIsOpen() {
    if( !player )
      return false;

    oCMag_Book* mb = player->GetSpellBook();
    return mb && mb->open;
  }



  bool IsInInventory() {
    return player && player->inventory2.IsOpen() && IsNotFightMode();
  }



  bool IsInInterface() {
    return (zCInputCallback::inputList.GetNextInList()->GetData() != ogame || SpellBookIsOpen() || IsInChooseWeapon()) && !IsInInventory() && IsNotFightMode();
  }



  bool IsNotInInterface() {
    return !IsInInterface() && !IsInInventory() && IsNotFightMode();
  }



  bool IsNotSpellInInterface() {
    return !IsInInterface() && !IsInInventory();
  }



  bool IsInTransform() {
    if( !player )
      return false;

    oCSpell* spell;
    for( int spellID = 47 /*SPL_TRFSHEEP*/; spellID <= 58 /*SPL_TRFDRAGONSNAPPER*/; spellID++ ) {
      spell = player->IsSpellActive( spellID );
      if( spell )
        return true;
    }

    return false;
  }



  bool IsNotInTransform() {
    return !IsInTransform();
  }



  bool IsCanDrink() {
    return (IsNotFightMode() && (oCZoneMusic::s_herostatus == oHERO_STATUS_STD || player->GetAttribute( NPC_ATR_HITPOINTS ) <= 5)) && !IsInInventory();
  }



  bool IsPlayVideo() {
    return ActiveVideo;
  }



  bool IsNotPlayVideo() {
    return !ActiveVideo
#if ENGINE < Engine_G2
      && IsNotInInterface()
#endif
      ;
  }


  // ------------------------------------------------

  bool Cond_FightMode() {
    return player && player->fmode != 0;
  }



  bool Cond_FightModeMelee() {
    return player && (player->fmode >= NPC_WEAPON_FIST && player->fmode <= NPC_WEAPON_2HS);
  }



  bool Cond_FightModeRange() {
    return player && (player->fmode == NPC_WEAPON_BOW || player->fmode == NPC_WEAPON_CBOW);
  }



  bool Cond_FightModeMagic() {
    return player && player->fmode == NPC_WEAPON_MAG;
  }



  bool Cond_CanShoot() {
    return IsFightModeRange() && zKeyPressed( GetBinded( GAME_ACTION ) );
  }



  bool Cond_CanSneaking() {
    return player && player->human_ai && player->human_ai->walkmode == ANI_WALKMODE_SNEAK;
  }



  bool Cond_Diving() {
    return player && player->human_ai && player->human_ai->walkmode == ANI_WALKMODE_DIVE;
  }



  bool Cond_HasFocusVob() {
    return player && player->GetFocusVob();
  }



  bool Cond_HasFocusNpc() {
    return player && player->GetFocusNpc();
  }



  bool Cond_TargetIsLocked() {
#if ENGINE >= Engine_G2
    return HasFocusTarget() && (IsFightMode() || oCNpc::s_bTargetLocked);
#else
    return True;
#endif
  }



  bool Cond_OnChooseWeapon() {
#if ENGINE >= Engine_G2
    return zKeyPressed( GetBinded( GAME_WEAPON ) );
#else
    return zKeyPressed( KEY_3 );
#endif
  }



  bool Cond_OnSpellBook() {
    oCMag_Book* magBook = player ? player->GetSpellBook() : Null;
    return magBook && magBook->open;
  }



  static bool PlayerIsTalking() {
    if( !player )
      return true; // for interfaces

    if( player->GetTalkingWith() ) {
      static int index = parser->GetIndex( "ZS_TALK" );
      if( player->state.IsInState( index ) || player->GetTalkingWith()->state.IsInState( index ) )
        return true;
    }

    return false;
  }



  bool Cond_TopCallbackIsDialog() {
    zCInputCallback* topCallback = zCInputCallback::inputList.GetNextInList()->GetData();
    zCViewDialog* dialog = dynamic_cast<zCViewDialog*>(topCallback);
    return dialog != Null && oCInformationManager::GetInformationManager().IsDone == 0;
  }



  bool Cond_TopCallbackIsTradeDialog() {
#if ENGINE <= Engine_G1A
    zCInputCallback* topCallback = zCInputCallback::inputList.GetNextInList()->GetData();
    oCViewDialogTrade* dialogTrade = dynamic_cast<oCViewDialogTrade*>(topCallback);
    if( dialogTrade && dialogTrade->DlgChoice && dialogTrade->DlgChoice->IsActive() )
      return true;

    return false;
#else
    return true;
#endif
  }



  bool Cond_TopCallbackIsMenu() {
    zCInputCallback* topCallback = zCInputCallback::inputList.GetNextInList()->GetData();
    zCMenu* menu = dynamic_cast<zCMenu*>(topCallback);
    if( menu )
      return true;

    zCMenuItem* item = dynamic_cast<zCMenuItem*>(topCallback);
    return item != Null;
  }



  bool Cond_InventoryIsOpen() {
    zCInputCallback* topCallback = zCInputCallback::inputList.GetNextInList()->GetData();
#if ENGINE <= Engine_G1A
    // �����-FIX: ������� ��������� ������� ������� ������ ������
    // �������������, ������� �� ���������� ��������� �� ���������.
    if( Cond_TopCallbackIsTradeDialog() )
      return true;
#endif
    return player && player->inventory2.IsOpen();
  }



  // �����-FIX: ������ ����� choice ������ ���������� �������� ���������,
  // ����������, �������� �� ������� � ������, ��� � ������ ��������
  // ����� ������ �������. �������� ��� ���.
  static void FixEmptyDialog() {
    zCInputCallback* topCallback = zCInputCallback::inputList.GetNextInList()->GetData();

    zCViewDialog* dialog = dynamic_cast<zCViewDialog*>(topCallback);
    if( dialog && dialog->IsDone )
      zCInputCallback::inputList.Remove( dialog );
  }






  bool Cond_InterfaceIsOpen() {
    //FixEmptyDialog();
    zCInputCallback* topCallback = zCInputCallback::inputList.GetNextInList()->GetData();


#if ENGINE <= Engine_G1A
    // �����-FIX: ������� ��������� ������� ������� ������ ������
    // �������������, ������� �� ���������� ��������� �� ���������.
    if( Cond_TopCallbackIsTradeDialog() )
      return true;
#endif
    bool isInDialogState   = oCInformationManager::GetInformationManager().IsDone == 0;
    bool isInterfaceActive = /*(topCallback != ogame && !Cond_TopCallbackIsDialog())*/ Cond_TopCallbackIsMenu() || Cond_TopCallbackIsDialog() || Cond_OnSpellBook() || Cond_OnChooseWeapon() || PlayerIsTalking();
    bool isOtherConditions = !Cond_InventoryIsOpen();

    return isInterfaceActive && isOtherConditions;
  }



  bool Cond_InTransformation() {
    oCSpell* spell;
    for( int spellID = 47 /*SPL_TRFSHEEP*/; spellID <= 58 /*SPL_TRFDRAGONSNAPPER*/; spellID++ ) {
      spell = player ? player->IsSpellActive( spellID ) : Null;
      if( spell )
        return true;
    }

    return false;
  }



  bool Cond_CanQuickPotionDrink() {
    if( !player )
      return false;

    return !IsFightMode() && !Cond_InventoryIsOpen() &&
      (oCZoneMusic::s_herostatus == oHERO_STATUS_STD || player->GetAttribute( NPC_ATR_HITPOINTS ) <= 5);
  }



  bool Cond_VideoIsOpen() {
    return ActiveVideo;
  }



  bool Cond_CanLockTarget() {
    return Cond_FightMode() || oCNpc::s_bTargetLocked != False;
  }
}
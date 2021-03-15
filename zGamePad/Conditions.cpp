// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  Map<LPCONDITION, bool>& Gamepad_GetStaticConditions() {
    static Map<LPCONDITION, bool> static_Conditions;
    return static_Conditions;
  }

  void Gamepad_SetStaticCondition( LPCONDITION condition ) {
    auto& pair = Gamepad_GetStaticConditions()[condition];
    if( pair.IsNull() )
      return Gamepad_GetStaticConditions().Insert( condition, condition() );
    pair.GetValue() = condition();
  }

  bool Gamepad_GetStaticCondition( LPCONDITION condition ) {
    return Gamepad_GetStaticConditions()[condition];
  }

  // Update conditions once per frame
  void Gamepad_UpdateStaticConditions() {
    Gamepad_SetStaticCondition( Cond_FightMode );
    Gamepad_SetStaticCondition( Cond_FightModeMelee );
    Gamepad_SetStaticCondition( Cond_FightModeRange );
    Gamepad_SetStaticCondition( Cond_FightModeMagic );
    Gamepad_SetStaticCondition( Cond_CanShoot );
    Gamepad_SetStaticCondition( Cond_CanSneaking );
    Gamepad_SetStaticCondition( Cond_Diving );
    Gamepad_SetStaticCondition( Cond_HasFocusVob );
    Gamepad_SetStaticCondition( Cond_HasFocusNpc );
    Gamepad_SetStaticCondition( Cond_TargetIsLocked );
    Gamepad_SetStaticCondition( Cond_OnChooseWeapon );
    Gamepad_SetStaticCondition( Cond_InventoryIsOpen );
    Gamepad_SetStaticCondition( Cond_InTransformation );
    Gamepad_SetStaticCondition( Cond_CanQuickPotionDrink );
    Gamepad_SetStaticCondition( Cond_VideoIsOpen );
    Gamepad_SetStaticCondition( Cond_CanLockTarget );
    Gamepad_SetStaticCondition( Cond_G1 );
    Gamepad_SetStaticCondition( Cond_G2 );
    Gamepad_SetStaticCondition( Cond_IsDialogTop );
    Gamepad_SetStaticCondition( Cond_IsDocumentTop );
    Gamepad_SetStaticCondition( Cond_IsOverlayTop );
    Gamepad_SetStaticCondition( Cond_IsMenuTop );
    Gamepad_SetStaticCondition( Cond_OnSpellBook );
    Gamepad_SetStaticCondition( Cond_IsPlayerTalking );
    Gamepad_SetStaticCondition( Cond_InterfaceIsOpen );
  }



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
    return Cond_FightModeRange() && zKeyPressed( GetBinded( GAME_ACTION ) );
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
    return player && player->GetFocusVob() && (Cond_FightMode() || oCNpc::s_bTargetLocked);
#else
    return True;
#endif
  }



  bool Cond_OnChooseWeapon() {
#if ENGINE >= Engine_G2
    return zKeyPressed( GetBinded( GAME_WEAPON ) );
#else
    return zKeyPressed( KEY_3 ); // TO DO
#endif
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



  bool Cond_InventoryIsOpen() {
#if ENGINE <= Engine_G1A
    if( Cond_TopCallbackIsTradeDialog() )
      return true;
#endif
    return player && player->inventory2.IsOpen();
  }



  bool Cond_InTransformation() {
    return player ? !player->IsHuman() : false;
  }



  bool Cond_CanQuickPotionDrink() {
    return false; // Use quick bar on newest versions of plugin !!!
  }



  bool ActiveVideo = false;

  bool Cond_VideoIsOpen() {
    return ActiveVideo;
  }



  bool Cond_CanLockTarget() {
    return Cond_FightMode() || oCNpc::s_bTargetLocked != False;
  }



  bool Cond_G1() {
    return Union.GetEngineVersion() <= Engine_G1A;
  }



  bool Cond_G2() {
    return Union.GetEngineVersion() >= Engine_G2;
  }



  bool Cond_IsDialogTop() {
    zCInputCallback* topCallback = zCInputCallback::inputList.GetNextInList()->GetData();
    zCViewDialog* dialog = dynamic_cast<zCViewDialog*>(topCallback);
    return dialog != Null && oCInformationManager::GetInformationManager().IsDone == 0;
  }



  bool Cond_IsDocumentTop() {
    zCInputCallback* topCallback = zCInputCallback::inputList.GetNextInList()->GetData();
    oCViewDocument* document = dynamic_cast<oCViewDocument*>(topCallback);
    if( document != Null )
      return true;

    oCDoc* doc = dynamic_cast<oCDoc*>(topCallback);
    if( doc != Null )
      return true;

    oCDocumentManager* docMan = dynamic_cast<oCDocumentManager*>(topCallback);
    return docMan != Null;
  }



  bool Cond_IsOverlayTop() {
    zCInputCallback* topCallback = zCInputCallback::inputList.GetNextInList()->GetData();
    zCGamepadOverlay* overlay = dynamic_cast<zCGamepadOverlay*>(topCallback);
    return overlay != Null;
  }



  bool Cond_IsMenuTop() {
    zCInputCallback* topCallback = zCInputCallback::inputList.GetNextInList()->GetData();
    zCMenu* menu = dynamic_cast<zCMenu*>(topCallback);
    if( menu )
      return true;

    zCMenuItem* item = dynamic_cast<zCMenuItem*>(topCallback);
    return item != Null;
  }



  bool Cond_OnSpellBook() {
    oCMag_Book* magBook = player ? player->GetSpellBook() : Null;
    return magBook && magBook->open;
  }



  bool Cond_IsPlayerTalking() {
    if( !player )
      return true; // for interfaces

    static int index = parser->GetIndex( "ZS_TALK" );
    if( player->GetTalkingWith() )
      if( player->state.IsInState( index ) || player->GetTalkingWith()->state.IsInState( index ) )
        return true;

    return false;
  }



  // For any interface items, except Inventory
  bool Cond_InterfaceIsOpen() {
    zCInputCallback* topCallback = zCInputCallback::inputList.GetNextInList()->GetData();
#if ENGINE <= Engine_G1A
    if( Cond_TopCallbackIsTradeDialog() )
      return true;
#endif

    bool mainConditions =
      Cond_IsDialogTop() ||
      Cond_IsDocumentTop() ||
      Cond_IsOverlayTop() ||
      Cond_IsMenuTop() ||
      Cond_OnSpellBook() ||
      Cond_IsPlayerTalking() ||
      Cond_OnChooseWeapon();

    bool otherConditions =
      !Cond_InventoryIsOpen();

    return mainConditions && otherConditions;
  }
}
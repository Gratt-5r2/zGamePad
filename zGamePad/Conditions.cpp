// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  bool ActiveVideo = false;



  bool IsPlayVideo() {
    return ActiveVideo;
  }



  bool IsNotPlayVideo() {
    return !ActiveVideo;
  }



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
}
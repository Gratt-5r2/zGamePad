// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  // Angle 0-360 rad
  float GetAngleBetweenNpcs( oCNpc* killer, oCNpc* enemy ) {
    zVEC3 vector3D = (enemy->GetPositionWorld() - killer->GetPositionWorld()).Normalize();
    zVEC2 vector2D = zVEC2( vector3D[VX], vector3D[VZ] );
    zVEC3 lookAt3D = killer->GetAtVectorWorld();
    zVEC2 lookAt2D = zVEC2( lookAt3D[VX], lookAt3D[VZ] );
    return vector2D.Rotate( -lookAt2D.GetAngle() ).GetAngle();
  }



  //
  bool_t CheckCollision( zCVob* vob1, zCVob* vob2 ) {
    if( !vob1 || !vob2 )
      return False;

    oCWorld* world = ogame->GetGameWorld();
    zVEC3    rayStart1 = vob1->GetPositionWorld();
    zVEC3    rayStart2 = ogame->GetCameraVob()->GetPositionWorld();
    zVEC3    rayEnd = vob2->GetPositionWorld();
    bool_t   hasCollision = world->TraceRayFirstHit( rayStart1, rayEnd - rayStart1, vob1, zTRACERAY_STAT_POLY | zTRACERAY_POLY_IGNORE_TRANSP ) ||
      world->TraceRayFirstHit( rayStart2, rayEnd - rayStart2, vob1, zTRACERAY_STAT_POLY | zTRACERAY_POLY_IGNORE_TRANSP );

    return hasCollision;
  }



  // Turn killer to enemy
  void SetHeadingToEnemy( oCNpc* killer, oCNpc* enemy ) {
    killer->RotateLocalY( GetAngleBetweenNpcs( killer, enemy ) * DEGREE );
  }


  inline void Npc_SetEnemy( oCNpc* attacker, oCNpc* victim ) {
    attacker->SetFocusVob( victim );
    attacker->SetEnemy( victim );
    attacker->anictrl->hitTarget = victim;
    SetHeadingToEnemy( attacker, victim );
  }



  // Find next LEFT nearest enemy
  bool GetNextLeftEnemy() {
    oCNpc* npc = player->GetNearestFightNpcLeft_Union();
    if( npc && !npc->IsDead() ) {
      Npc_SetEnemy( player, npc );
      return true;
    }

    return false;
  }



  // Find next RIGHT nearest enemy
  bool GetNextRightEnemy() {
    oCNpc* npc = player->GetNearestFightNpcRight_Union();
    if( npc && !npc->IsDead() ) {
      Npc_SetEnemy( player, npc );
      return true;
    }

    return false;
  }



  // Find next nearest enemy
  bool GetNextEnemy() {
    oCNpc* npc = player->GetNearestFightNpc_Union();
    if( npc && !npc->IsDead() ) {
      Npc_SetEnemy( player, npc );
      return true;
    }

    return false;
  }


  //
  static int CanSeeEnemy( oCNpc* npc1, oCNpc* npc2 ) {
    return
      npc1->CanSee( npc2, True ) ||
      npc2->CanSee( ogame->GetCameraVob(), True );
  }


  //
  Array<oCNpc*> oCNpc::GetNearestFightNpcList() {
    zCArray<zCVob*> vobList;
    Array<oCNpc*>   npcList;
    CreateVobList( vobList, oCNpcFocus::focus->GetMaxRange() * 1.5f );

    for( int i = 0; i < vobList.GetNum(); i++ ) {
      oCNpc* npc = vobList[i]->CastTo<oCNpc>();
      if( npc && !npc->IsDead() && !npc->IsUnconscious() )
        if( player->focus_vob != npc && npc != player && CanSeeEnemy( this, npc ) )
          npcList.Insert( npc );
    }

    return npcList;
  }


  //
  Array<oCNpc*> oCNpc::GetNearestVisibleFightNpcList() {
    Array<oCNpc*> npcs = GetNearestFightNpcList();
    Array<oCNpc*> npcsVisible;
    for( uint i = 0; i < npcs.GetNum(); i++ )
      if( !CheckCollision( player, npcs[i] ) )
        npcsVisible += npcs[i];
    
    return npcsVisible;
  }



  // Check focus npc
  void CheckDeadTarget() {
    oCNpc* targetNpc = player->GetFocusNpc();
    if( targetNpc && (targetNpc->IsDead() || targetNpc->IsUnconscious()) )
      if( oCNpc::s_bTargetLocked )
        if( !GetNextEnemy() )
          oCNpc::s_bTargetLocked = False;
  }



  void CheckTargetDistance() {
    static uint loseTimer = 0;
    if( !oCNpc::s_bTargetLocked ) {
      if( loseTimer )
        loseTimer = 0;

      return;
    }

    oCNpc* targetNpc = player->GetFocusNpc();

    if( targetNpc ) {
      float maxRange = oCNpcFocus::focus->GetMaxRange() * 2.0f;
      if( targetNpc->GetDistanceToVob( *player ) > maxRange ) {
        if( loseTimer == 0 )
          loseTimer = Timer::GetTime();

        else if( Timer::GetTime() - loseTimer >= 3000 )
          if( !GetNextEnemy() )
            oCNpc::s_bTargetLocked = False;
      }
      else if( loseTimer > 0 )
        loseTimer = 0;
    }
  }



  // 
  void CheckInvisibleTarget() {
    static uint loseTimer = 0;
    if( !oCNpc::s_bTargetLocked ) {
      if( loseTimer )
        loseTimer = 0;

      return;
    }

    oCNpc* targetNpc = player->GetFocusNpc();
    if( targetNpc ) {
      if( CheckCollision( player, targetNpc ) ) {
        if( loseTimer == 0 )
          loseTimer = Timer::GetTime();

        else if( Timer::GetTime() - loseTimer >= 3000 )
          if( !GetNextEnemy() )
            oCNpc::s_bTargetLocked = False;
      }
      else if( loseTimer > 0 )
        loseTimer = 0;
    }
  }



  // Locked marker
  void DrawTarget() {
    if( !player )
      return;

    oCNpc* targetNpc = player->GetFocusNpc();
    if( player->fmode == NPC_WEAPON_NONE || !targetNpc )
      oCNpc::s_bTargetLocked = False;

    static zCView* target = Null;
    static int size = 16;

    if( !target ) {
      target = new zCView();
      target->InsertBack( "Gp_Target" );
      size = GetScaleMultiplier() == 1 ? 128 : 64;
    }

    // Next target after dead current
    CheckDeadTarget();
    CheckTargetDistance();
    CheckInvisibleTarget();

    if( oCNpc::s_bTargetLocked ) {
      zVEC3 position = targetNpc->GetPositionWorld();

      int x, y;
      GetProjection( x, y, position );

      int psx = zPixelX( size );
      int psy = zPixelY( size );

      screen->InsertItem( target );
      target->SetSize( psx, psy );
      target->SetPos( x - psx / 2, y - psy / 2 );
    }
    else if( !oCNpc::s_bTargetLocked )
      screen->RemoveItem( target );
  }



  // 
  oCNpc* oCNpc::GetNearestFightNpcRight_Union() {
    Array<oCNpc*> npcList = GetNearestVisibleFightNpcList();

    oCNpc* nearestNpc  = Null;
    float nearestAngle = RAD360;
    for( uint i = 0; i < npcList.GetNum(); i++ ) {

      float angle = GetAngleBetweenNpcs( this, npcList[i] );
      if( angle < nearestAngle ) {
        nearestAngle = angle;
        nearestNpc   = npcList[i];
      }
    }

    return nearestNpc;
  }



  // 
  oCNpc* oCNpc::GetNearestFightNpcLeft_Union() {
    Array<oCNpc*> npcList = GetNearestVisibleFightNpcList();

    oCNpc* nearestNpc  = Null;
    float nearestAngle = RAD360;
    for( uint i = 0; i < npcList.GetNum(); i++ ) {

      float angle = RAD360 - GetAngleBetweenNpcs( this, npcList[i] );
      if( angle < nearestAngle ) {
        nearestAngle = angle;
        nearestNpc   = npcList[i];
      }
    }

    return nearestNpc;
  }



  // 
  oCNpc* oCNpc::GetNearestFightNpc_Union() {
    Array<oCNpc*> npcList = GetNearestVisibleFightNpcList();

    oCNpc* nearestNpc  = Null;
    float nearestAngle = RAD360;
    for( uint i = 0; i < npcList.GetNum(); i++ ) {

      float angle = RAD360 - GetAngleBetweenNpcs( this, npcList[i] );
      if( angle > RAD180 )
        angle = RAD360 - angle;

      if( angle < nearestAngle ) {
        nearestAngle = angle;
        nearestNpc   = npcList[i];
      }
    }

    return nearestNpc;
  }



#if ENGINE <= Engine_G1A
  int oCNpc::s_bTargetLocked = False;

  HOOK Hook_oCNpc_CollectFocusVob PATCH( &oCNpc::CollectFocusVob, &oCNpc::CollectFocusVob_Union );

  void oCNpc::CollectFocusVob_Union() {
    if( s_bTargetLocked ) {
      oCNpc* focusNpc = GetFocusNpc();
      if( !focusNpc || (!focusNpc->IsDead_Union() && !focusNpc->IsUnconscious()) )
        return;

      s_bTargetLocked = False;
    }

    THISCALL( Hook_oCNpc_CollectFocusVob )();
  }
#endif



  void oCNpc::TurnToEnemyInAttack() {
    if( !human_ai )
      return;

    bool isFightAni =
      human_ai->IsStateAniActive( human_ai->_t_hitl ) ||
      human_ai->IsStateAniActive( human_ai->_t_hitr ) ||
      human_ai->IsStateAniActive( human_ai->_t_hitf );

    if( isFightAni && focus_vob )
      Turn( focus_vob->GetPositionWorld() );
  }



  static void FocusNpcLoop() {
    DrawTarget();

    if( zKeyToggled( KEY_F14 ) )
      GetNextLeftEnemy();

    if( zKeyToggled( KEY_F15 ) )
      GetNextRightEnemy();

#if ENGINE <= Engine_G2A
    if( zKeyToggled( KEY_F13 ) )
      if( player->GetFocusNpc() || GetNextEnemy() )
        oCNpc::s_bTargetLocked = !oCNpc::s_bTargetLocked;

    player->TurnToEnemyInAttack();
#endif
  }
}
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



  // Turn killer to enemy
  void SetHeadingToEnemy( oCNpc* killer, oCNpc* enemy ) {
    killer->RotateLocalY( GetAngleBetweenNpcs( killer, enemy ) * DEGREE );
  }



  // Find next LEFT nearest enemy
  bool GetNextLeftEnemy() {
    oCNpc* npc = player->GetNearestFightNpcLeft_Union();
    if( npc && !npc->IsDead() ) {
      player->SetFocusVob( npc );
      player->SetEnemy( npc );
      SetHeadingToEnemy( player, npc );
      return true;
    }

    return false;
  }



  // Find next RIGHT nearest enemy
  bool GetNextRightEnemy() {
    oCNpc* npc = player->GetNearestFightNpcRight_Union();
    if( npc && !npc->IsDead() ) {
      player->SetFocusVob( npc );
      player->SetEnemy( npc );
      SetHeadingToEnemy( player, npc );
      return true;
    }

    return false;
  }



  // Find next nearest enemy
  bool GetNextEnemy() {
    oCNpc* npc = player->GetNearestFightNpc_Union();
    if( npc && !npc->IsDead() ) {
      player->SetFocusVob( npc );
      player->SetEnemy( npc );
      SetHeadingToEnemy( player, npc );
      return true;
    }

    return false;
  }



  // Check focus npc
  void CheckDeadTarget() {
    oCNpc* targetNpc = player->GetFocusNpc();
    if( targetNpc && (targetNpc->IsDead() || targetNpc->IsUnconscious()) )
      if( oCNpc::s_bTargetLocked )
        if( !GetNextEnemy() )
          oCNpc::s_bTargetLocked = False;
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

      bool scaleEnabled;
      Union.GetSysPackOption().Read( scaleEnabled, "INTERFACE", "Scale" );
      size = scaleEnabled ? 128 : 64;
    }

    // Next target after dead current
    CheckDeadTarget();

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
    zCArray<zCVob*> vobList;
    zCArray<oCNpc*> npcList;
    CreateVobList( vobList, (float)GetFightRange() * 2.0f );

    for( int i = 0; i < vobList.GetNum(); i++ ) {
      oCNpc* npc = vobList[i]->CastTo<oCNpc>();
      if( npc && !npc->IsDead() && !npc->IsUnconscious() )
        if( player->focus_vob != npc && npc != player )
          npcList.Insert( npc );
    }

    oCNpc* nearestNpc  = Null;
    float nearestAngle = RAD360;
    for( int i = 0; i < npcList.GetNum(); i++ ) {

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
    zCArray<zCVob*> vobList;
    zCArray<oCNpc*> npcList;
    CreateVobList( vobList, (float)GetFightRange() * 2.0f );

    for( int i = 0; i < vobList.GetNum(); i++ ) {
      oCNpc* npc = vobList[i]->CastTo<oCNpc>();
      if( npc && !npc->IsDead() && !npc->IsUnconscious() )
        if( player->focus_vob != npc && npc != player )
          npcList.Insert( npc );
    }

    oCNpc* nearestNpc  = Null;
    float nearestAngle = RAD360;
    for( int i = 0; i < npcList.GetNum(); i++ ) {

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
    zCArray<zCVob*> vobList;
    zCArray<oCNpc*> npcList;
    CreateVobList( vobList, (float)GetFightRange() * 2.0f );

    for( int i = 0; i < vobList.GetNum(); i++ ) {
      oCNpc* npc = vobList[i]->CastTo<oCNpc>();
      if( npc && !npc->IsDead() && !npc->IsUnconscious() )
        if( player->focus_vob != npc && npc != player )
          npcList.Insert( npc );
    }

    oCNpc* nearestNpc  = Null;
    float nearestAngle = RAD360;
    for( int i = 0; i < npcList.GetNum(); i++ ) {

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
}
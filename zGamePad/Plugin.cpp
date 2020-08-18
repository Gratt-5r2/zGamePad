// This file added in headers queue
// File: "Sources.h"

namespace GOTHIC_ENGINE {
#if ENGINE < Engine_G2
  int oCNpc::s_bTargetLocked = False;
#endif



  void SetHeadingToEnemy( oCNpc* killer, oCNpc* enemy ) {
    zVEC3 vector3D = (enemy->GetPositionWorld() - killer->GetPositionWorld()).Normalize();
    zVEC2 vector2D = zVEC2( vector3D[VX], vector3D[VZ] );
    zVEC3 lookAt3D = killer->GetAtVectorWorld();
    zVEC2 lookAt2D = zVEC2( lookAt3D[VX], lookAt3D[VZ] );

    float angle = lookAt2D.Rotate( -vector2D.GetAngle() ).GetAngle();
    killer->RotateLocalY( -angle * DEGREE );
  }

  void GetNextLeftEnemy() {
    oCNpc* npc = player->GetNearestFightNpcLeft();
    if( npc ) {
      player->SetFocusVob( npc );
      player->SetEnemy( npc );
      SetHeadingToEnemy( player, npc );
    }
  }

  void GetNextRightEnemy() {
    oCNpc* npc = player->GetNearestFightNpcRight();
    if( npc ) {
      player->SetFocusVob( npc );
      player->SetEnemy( npc );
      SetHeadingToEnemy( player, npc );
    }
  }





  void Game_Entry() {
  }

  void Game_Init() {
    // zoptions->WriteBool( "GAME", "usePotionKeys",      True, True );
    // zoptions->WriteBool( "GAME", "useGothic1Controls", True, True );
    // XInputDevice.InitDevice();
  }

  void Game_Exit() {
  }


#if 0
  HOOK Hook_oCNpc_CreateVobList AS( &oCNpc::CreateVobList, &oCNpc::CreateVobList_Union );

  void oCNpc::CreateVobList_Union( float max_dist ) {
    THISCALL( Hook_oCNpc_CreateVobList )( max_dist );

    if( this == player && fmode != NPC_WEAPON_NONE ) {
      for( uint i = 0; i < vobList.GetNum(); i++ ) {
        oCNpc* npc = vobList[i]->CastTo<oCNpc>();
        if( npc && npc->aiscriptvars[15] )
          vobList.RemoveIndex( i-- );
      }
    }
  }




  HOOK Hook_oCNpc_GetNearestValidVob AS( &oCNpc::GetNearestValidVob, &oCNpc::GetNearestValidVob_Union );

  void oCNpc::GetNearestValidVob_Union( float max_dist ) {
    if( this == player && fmode != NPC_WEAPON_NONE )
      return;

    THISCALL( Hook_oCNpc_GetNearestValidVob )(max_dist);
  }

  HOOK Hook_oCNpc_GetNearestVob AS( &oCNpc::GetNearestVob, &oCNpc::GetNearestVob_Union );

  void oCNpc::GetNearestVob_Union( float max_dist ) {
    if( this == player && fmode != NPC_WEAPON_NONE )
      return;

    THISCALL( Hook_oCNpc_GetNearestVob )(max_dist);
  }

  // HOOK Hook_oCNpc_CollectFocusVob AS( &oCNpc::CollectFocusVob, &oCNpc::CollectFocusVob_Union );

  void oCNpc::CollectFocusVob_Union( const int a_bForce ) {

  }
#endif

  void CheckDeadTarget() {
    static bool_t targetEnabled = False;
    targetEnabled = oCNpc::s_bTargetLocked;

    oCNpc* targetNpc = player->GetFocusNpc();
    if( targetNpc && targetNpc->IsDead() ) {
      GetNextRightEnemy();

      if( player->GetFocusNpc() )
        oCNpc::s_bTargetLocked = targetEnabled;
    }
#if 0
    static CTimer nextTarget;
    nextTarget.Attach();

    oCNpc* targetNpc = player->GetFocusNpc();
    if( targetNpc && targetNpc->IsDead() )
      cmd << "DEAD" << endl;



    if( oCNpc::s_bTargetLocked ) {
      if( targetNpc && targetNpc->IsDead() ) {
        if( nextTarget( 0, 2000 ) ) {

          player->CollectFocusVob( True );
          oCNpc::s_bTargetLocked = True;

          if( !player->GetFocusNpc() )
            oCNpc::s_bTargetLocked = False;
        }
      }
    }
#endif
  }

  void DrawTarget() {
    if( !player )
      return;

    if( player->fmode == NPC_WEAPON_NONE )
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

    // CheckDeadTarget();

    if( oCNpc::s_bTargetLocked ) {
      oCNpc* targetNpc = player->GetFocusNpc();
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



  void Game_Loop() {
    static bool InterfaceInitialized_GetMousePos = false;
    if( !InterfaceInitialized_GetMousePos ) {
      uint address = ZenDef( 0x004C8BD0, 0x004D8CF0, 0x004D3170, 0x004D5730 );
      Hook_GetMousePos.Attach( address, &zCInput_Win32::GetMousePos_Union );
      InterfaceInitialized_GetMousePos = true;
#if ENGINE >= Engine_G2
      oCGame::s_bUsePotionKeys = True;
#endif
    }

    DrawTarget();

    if( zKeyToggled( KEY_F14 ) )
      GetNextLeftEnemy();

    if( zKeyToggled( KEY_F15 ) )
      GetNextRightEnemy();

    // screen->PrintCXY( AHEX32( zCInputCallback::inputList[0] ) );
  }

  // Information about current saving or loading world
  TSaveLoadGameInfo& SaveLoadGameInfo = UnionCore::SaveLoadGameInfo;

  void Game_SaveBegin() {
  }

  void Game_SaveEnd() {
  }

  void LoadBegin() {
  }


  void LoadEnd() {
  }

  void Game_LoadBegin_NewGame() {
    LoadBegin();
  }

  void Game_LoadEnd_NewGame() {
    LoadEnd();
  }

  void Game_LoadBegin_SaveGame() {
    LoadBegin();
  }

  void Game_LoadEnd_SaveGame() {
    LoadEnd();
  }

  void Game_LoadBegin_ChangeLevel() {
    LoadBegin();
  }

  void Game_LoadEnd_ChangeLevel() {
    LoadEnd();
  }

  void Game_LoadBegin_Trigger() {
  }
  
  void Game_LoadEnd_Trigger() {
  }
  
  void Game_Pause() {
  }
  
  void Game_Unpause() {
  }
  
  void Game_DefineExternals() {
  }

  /*
  Functions call order on Game initialization:
    - Game_Entry           * Gothic entry point
    - Game_DefineExternals * Define external script functions
    - Game_Init            * After DAT files init
  
  Functions call order on Change level:
    - Game_LoadBegin_Trigger     * Entry in trigger
    - Game_LoadEnd_Trigger       *
    - Game_Loop                  * Frame call window
    - Game_LoadBegin_ChangeLevel * Load begin
    - Game_SaveBegin             * Save previous level information
    - Game_SaveEnd               *
    - Game_LoadEnd_ChangeLevel   *
  
  Functions call order on Save game:
    - Game_Pause     * Open menu
    - Game_Unpause   * Click on save
    - Game_Loop      * Frame call window
    - Game_SaveBegin * Save begin
    - Game_SaveEnd   *
  
  Functions call order on Load game:
    - Game_Pause              * Open menu
    - Game_Unpause            * Click on load
    - Game_LoadBegin_SaveGame * Load begin
    - Game_LoadEnd_SaveGame   *
  */

#define AppDefault True
  CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
    Enabled( AppDefault ) Game_Entry,
    Enabled( AppDefault ) Game_Init,
    Enabled( AppDefault ) Game_Exit,
    Enabled( AppDefault ) Game_Loop,
    Enabled( AppDefault ) Game_SaveBegin,
    Enabled( AppDefault ) Game_SaveEnd,
    Enabled( AppDefault ) Game_LoadBegin_NewGame,
    Enabled( AppDefault ) Game_LoadEnd_NewGame,
    Enabled( AppDefault ) Game_LoadBegin_SaveGame,
    Enabled( AppDefault ) Game_LoadEnd_SaveGame,
    Enabled( AppDefault ) Game_LoadBegin_ChangeLevel,
    Enabled( AppDefault ) Game_LoadEnd_ChangeLevel,
    Enabled( AppDefault ) Game_LoadBegin_Trigger,
    Enabled( AppDefault ) Game_LoadEnd_Trigger,
    Enabled( AppDefault ) Game_Pause,
    Enabled( AppDefault ) Game_Unpause,
    Enabled( AppDefault ) Game_DefineExternals
  );
}
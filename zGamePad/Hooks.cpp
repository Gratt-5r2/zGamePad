// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  HOOK Hook_oCAIHuman_WeaponChoose AS( &oCAIHuman::WeaponChoose, &oCAIHuman::WeaponChoose_Union );

  void oCAIHuman::WeaponChoose_Union() {
    if( Pressed( GAME_WEAPON ) ) {
      if( wmode_selected >= NPC_WEAPON_MAG ) {
        oCMag_Book* mb = npc->GetSpellBook();
        if( !mb->open )
          mb->Open( 1 );
      }
    }

    THISCALL( Hook_oCAIHuman_WeaponChoose )();
  }



  HOOK Hook_GetMousePos PATCH_IF( &zCInput_Win32::GetMousePos, &zCInput_Win32::GetMousePos_Union, false );

  void zCInput_Win32::GetMousePos_Union( float& x, float& y, float& z ) {
    static float stickSensitivity = GetStickSensitivity();
    static float rollbackSensitivity = 1.0f;
    if( zCGamepadQuickBar::OverlayMouseHooksCount > 0 ) {
      x = 0.0f;
      y = 0.0f;
      z = 0.0f;
      rollbackSensitivity = -1.0f;
      return;
    }

    THISCALL( Hook_GetMousePos )(x, y, z);
    x += ((float)XInputDevice.RightStick.X) / 3000.0f * stickSensitivity;
    y += ((float)XInputDevice.RightStick.Y) / -3000.0f * stickSensitivity;

    if( rollbackSensitivity < 1.0f ) {
      if( abs( x + y ) > 0.3f ) {
        rollbackSensitivity += 0.5f * ztimer->frameTimeFloat / 1000.0f;
        if( rollbackSensitivity > 1.0f )
          rollbackSensitivity = 1.0f;
      }
      else
        rollbackSensitivity = 1.0f;
    }

    float multiplier = max( rollbackSensitivity, 0.0f );
    x *= multiplier;
    y *= multiplier;
  }



  void Input_GetMousePosReal( float& x, float& y, float& z ) {
    ((zCInput_Win32*)zinput->*Hook_GetMousePos)(x, y, z);
  }



  // void zCCamera::PreRenderProcessing()
  void __fastcall zCCamera_PreRenderProcessing( zCCamera* _this, void* vt );

  HOOK Hook_zCCamera_PreRenderProcessing AS( &zCCamera::PreRenderProcessing, &zCCamera_PreRenderProcessing );

  void __fastcall zCCamera_PreRenderProcessing( zCCamera* _this, void* vt ) {
    if( _this->tremorScale >= 0.0001 )
      XInputDevice.StartVibration( 65535.0f * _this->tremorScale );

    Hook_zCCamera_PreRenderProcessing( _this, vt );
  }



  HOOK Hook_oCNpc_OnDamage PATCH( &oCNpc::OnDamage, &oCNpc::OnDamage_Union );

  void oCNpc::OnDamage_Union( oSDamageDescriptor& desc ) {
    THISCALL( Hook_oCNpc_OnDamage )(desc);

    if( this == player ) {
      int value = (int)(65535.0f / (float)attribute[NPC_ATR_HITPOINTSMAX] * desc.fDamageTotal);
      XInputDevice.StartVibration( max( 65535.0f, value ) );
    }

    if( desc.pNpcAttacker == player ) {
      int value = (int)(16384.0f / (float)attribute[NPC_ATR_HITPOINTSMAX] * desc.fDamageTotal);
      XInputDevice.StartVibration( max( 16384.0f, value ) );
    }
  }



  // void zCRnd_D3D::BeginFrame()
  void __fastcall zCRnd_D3D_BeginFrame( zCRnd_D3D* _this, void* vtable );

  HOOK Hook_zCRnd_D3D_BeginFrame PATCH( &zCRnd_D3D::BeginFrame, &zCRnd_D3D_BeginFrame );

  void __fastcall zCRnd_D3D_BeginFrame( zCRnd_D3D* _this, void* vtable ) {
    XInputDevice.UpdateGamePad();
    Hook_zCRnd_D3D_BeginFrame( _this, vtable );
  }



  // public: virtual int __thiscall zCBinkPlayer::PlayFrame(void)
  int __fastcall oCBinkPlayer_PlayFrame( zCBinkPlayer*, void* );

  HOOK Hook_oCBinkPlayer_PlayFrame PATCH( &zCBinkPlayer::PlayHandleEvents, &oCBinkPlayer_PlayFrame );

  int __fastcall oCBinkPlayer_PlayFrame( zCBinkPlayer* _this, void* vtable ) {
    ActiveVideo = true;
    XInputDevice.UpdateGamePad();
    ActiveVideo = false;
    return Hook_oCBinkPlayer_PlayFrame( _this, vtable );
  }



  // void CGameManager::GameInit()
  HOOK Hook_CGameManager_Init PATCH( &CGameManager::Init, &CGameManager::Init_Union );

  void CGameManager::Init_Union( HWND& hWnd ) {
    THISCALL( Hook_CGameManager_Init )(hWnd);
    XInputDevice.InitDevice();
  }



  // void __thiscall CGameManager::ApplySomeSettings(void)
  HOOK Hook_CGameManager_ApplySomeSettings AS( &CGameManager::ApplySomeSettings, &CGameManager::ApplySomeSettings_Union );

  void CGameManager::ApplySomeSettings_Union() {
    THISCALL( Hook_CGameManager_ApplySomeSettings) ();
    XInputDevice.UpdateControls();
  }





  int LogicalKeyPressed( uint16 lkey ) {
    zCArray<uint16> keys;
    zinput->GetBinding( lkey, keys );
    for( int i = 0; i < keys.GetNum(); i++ )
      if( zinput->KeyPressed( keys[i] ) )
        return True;

    return False;
  }

#if 0
  int oCAIHuman::Pressed_Union( int key ) {
    if( key == GAME_ACTION ) {
      if( !LogicalKeyPressed( GAME_UP ) )
        if( IsStateAniActive( _t_hitf ) || IsStateAniActive( _t_hitl ) || IsStateAniActive( _t_hitr ) )
          return True;
    }

    return False;

    /*if( key == GAME_ACTION )
      if( !Pressed( GAME_UP ) && !Pressed( GAME_LEFT ) && !Pressed( GAME_RIGHT ) && !Pressed( GAME_STRAFELEFT ) && !Pressed( GAME_STRAFERIGHT ) )
        if( IsStateAniActive( _t_hitf ) || IsStateAniActive( _t_hitl ) || IsStateAniActive( _t_hitr ) )
          return True;

    return False;*/
  }
#endif


  int oCAIHuman::IsOnFightAni() {
    return
      //IsStateAniActive( _t_hitf ) ||
      IsStateAniActive( _t_hitl ) ||
      IsStateAniActive( _t_hitr );
  }


#if 1
  HOOK Hook_zCInput_Win32_GetState PATCH( &zCInput_Win32::GetState, &zCInput::GetState_Union );

  inline bool IsMovementKeyPressed() {
    return
      LogicalKeyPressed( GAME_UP )   ||
      LogicalKeyPressed( GAME_DOWN ) ||
      LogicalKeyPressed( GAME_LEFT ) ||
      LogicalKeyPressed( GAME_RIGHT );
  }

  float zCInput::GetState_Union( unsigned short key ) {
    if( key == GAME_ACTION ) {
      if( !LogicalKeyPressed( GAME_DOWN ) )
        if( player->human_ai->IsOnFightAni() ) {
          return True;
      }
    }

    return THISCALL( Hook_zCInput_Win32_GetState )(key);

#if 0
    float OK = THISCALL( Hook_zCInput_Win32_GetState )(key);

    if( key == GAME_ACTION ) {
      if( player->human_ai->IsOnFightAni() )
        if( !IsMovementKeyPressed() && !LogicalKeyPressed( GAME_PARADE ) )
          return True;

      /*if( LogicalKeyPressed( GAME_DOWN ) && LogicalKeyPressed( GAME_ACTION ) )
        return True;

      if( player && player->human_ai )
        if( !IsMovementKeyPressed() && player->human_ai->IsOnFightAni() )
          return True;*/
    }

    return OK;
#endif

    //if( key == GAME_ACTION && 0 ) {
    //  if( player && player->human_ai ) {
    //    if( player->human_ai->Pressed_Union( key ) )
    //      return True;
    //  }
    //  //else
    //  //  return True;
    //}

    //return THISCALL( Hook_zCInput_Win32_GetState )(key);
  }
#endif



  HOOK Hook_oCNpcInventory_HandleEvent PATCH( &oCNpcInventory::HandleEvent, &oCNpcInventory::HandleEvent_Union );

  int oCNpcInventory::HandleEvent_Union( int key ) {
    auto itemsCircle = zCGamepadQuickBar_Items::GetInstance();
    static bool waitingSlotID = false;

    if( key == KEY_COMMA ) {
      itemsCircle->ShowAt( screen, zEGamepadQuickBarAlignment_Center );
      waitingSlotID = true;
      return True;
    }

    if( waitingSlotID ) {
      if( key == KEY_ESCAPE || key == MOUSE_BUTTONRIGHT )
        waitingSlotID = false;
      
      if( key == KEY_RETURN || key == KEY_LCONTROL || key == MOUSE_BUTTONLEFT ) {
        oCItem* item = GetSelectedItem();
        itemsCircle->SetItemInActiveCell( item );
        waitingSlotID = false;
      }

      if( key == KEY_DELETE )
        itemsCircle->SetItemInActiveCell( Null );

      return True;
    }

    return THISCALL( Hook_oCNpcInventory_HandleEvent )(key);
  }
}
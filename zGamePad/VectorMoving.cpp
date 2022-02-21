// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  static bool  BodyAngleUndef    = true;
  static bool  BodyAngleInversed = false;
  static float BodyAngle         = 0.0f;

  static void RotateNodeWorld( zCModel* model, float angle, string name, bool x = true ) {
    auto nodeInst = model->SearchNode( name.Upper() );
    if( !nodeInst )
      return;

    zMAT4 rotation = Alg_Rotation3D( zVEC3( 0.0f, 1.0f, 0.0f ), angle );
    nodeInst->trafo = rotation * nodeInst->trafo;
  }


  static void RotateNode( zCModel* model, float angle, string name ) {
    auto nodeInst = model->SearchNode( name.Upper() );
    if( !nodeInst )
      return;

    nodeInst->trafo.PostRotateX( angle );
  }

  static bool PlayerIsOnJump() {
    auto aniCtrl = player->anictrl;
    return player->bodyState == BS_JUMP;

    auto model = player->GetModel();
    if( model->isFlying )
      return true;

    return
      model->IsAniActive( aniCtrl->t_jump_2_stand ) ||
      model->IsAniActive( aniCtrl->_t_jump_2_runl ) ||
      model->IsAniActive( aniCtrl->_t_runl_2_jump );
  }

  static bool PlayerIsOnSneak() {
    return player->anictrl->walkmode == ANI_WALKMODE_SNEAK;
  }

  static bool PlayerIsOnWater() {
    return
      player->anictrl->walkmode == ANI_WALKMODE_SWIM  ||
      player->anictrl->walkmode == ANI_WALKMODE_WATER ||
      player->anictrl->walkmode == ANI_WALKMODE_DIVE;
  }

  static bool PlayerIsMonster() {
    return player->IsHuman() == False && player->IsSkeleton() == False && player->IsOrc();
  }

  static void TurnBody( zCModel* model, float angle ) {
    RotateNodeWorld( model, angle, "BIP01", false );
    if( PlayerIsOnJump() || PlayerIsOnSneak() || PlayerIsOnWater() || PlayerIsMonster() )
      return;

    RotateNode( model, angle * -0.245f * 0.8f, "BIP01 SPINE" );
    RotateNode( model, angle * -0.245f * 0.8f, "BIP01 SPINE1" );
    RotateNode( model, angle * -0.245f * 0.8f, "BIP01 SPINE2" );
    RotateNode( model, angle * -0.245f * 1.8f, "BIP01 HEAD" );
  }

  static bool TurnBody_InputAccess() {
    zTStickState left, right;
    XInputDevice.GetStickStatesSquare( left, right );
    return left.Length() >= DEADZONE_L;
  }

  static bool TurnBody_AnimationAccess( zCModel* model ) {
    if( !model )
      return false;

    BodyAngleInversed = false;

    // Standart set
    bool isRuning =
      model->IsAniActive( player->anictrl->_t_walk_2_walkl  ) ||
      model->IsAniActive( player->anictrl->_t_walkl_2_walk  ) ||
      model->IsAniActive( player->anictrl->_t_walkr_2_walk  ) ||
      model->IsAniActive( player->anictrl->_s_walkl         ) ||
      model->IsAniActive( player->anictrl->_s_jump          ) ||
      model->IsAniActive( player->anictrl->_t_stand_2_jump  ) ||
      model->IsAniActive( player->anictrl->t_jump_2_stand   ) ||
      model->IsAniActive( player->anictrl->_t_runl_2_jump   ) ||
      model->IsAniActive( player->anictrl->_t_runr_2_jump   ) ||
      model->IsAniActive( player->anictrl->s_swimf          ) ||
      model->IsAniActive( player->anictrl->t_swim_2_swimf   ) ||
      model->IsAniActive( player->anictrl->t_swimf_2_swim   );

    // Special animations
    if( !isRuning ) {
      zSTRING	wm = oCAniCtrl_Human::GetWeaponHitString( player->GetWeaponMode() );
      isRuning =
        model->IsAniActive( "T_" + wm + "WALKL_2_" + wm + "RUNL"  ) ||
        model->IsAniActive( "T_" + wm + "RUNL_2_"  + wm + "WALKL" ) ||
        model->IsAniActive( "T_" + wm + "WALKL_2_JUMP"            ) ||
        model->IsAniActive( "T_" + wm + "RUNL_2_JUMP"             ) ||
        model->IsAniActive( "T_" + wm + "RUNR_2_JUMP"             ) ||
        model->IsAniActive( "T_" + wm + "JUMP_2_"  + wm + "WALKL" ) ||
        model->IsAniActive( "T_" + wm + "JUMP_2_"  + wm + "RUNL"  ) ||
        model->IsAniActive( "T_" + wm + "RUNL_2_"  + wm + "RUN"   ) ||
        model->IsAniActive( "T_" + wm + "JUMPR_2_" + wm + "STAND" );
    }

    if( isRuning )
      return true;

    bool isBackwark =
      model->IsAniActive( player->anictrl->_t_walkbr_2_walk ) ||
      model->IsAniActive( player->anictrl->_t_walkbl_2_walk ) ||
      model->IsAniActive( player->anictrl->_t_walk_2_walkbl ) ||
      model->IsAniActive( player->anictrl->_s_walkbl        ) ||
      model->IsAniActive( player->anictrl->_s_walkbr        ) ||
      model->IsAniActive( player->anictrl->t_swim_2_swimb   ) ||
      model->IsAniActive( player->anictrl->t_swimb_2_swim   ) ||
      model->IsAniActive( player->anictrl->s_swimb          );

    if( !isBackwark ) {
      zSTRING	wm = oCAniCtrl_Human::GetWeaponHitString( player->GetWeaponMode() );
      isBackwark =
        model->IsAniActive( "T_" + wm + "RUN_2_"     + wm + "RUNBL"   ) ||
        model->IsAniActive( "T_" + wm + "RUN_2_"     + wm + "RUNBR"   ) ||
        model->IsAniActive( "T_" + wm + "RUNBL_2_"   + wm + "RUN"     ) ||
        model->IsAniActive( "T_" + wm + "RUNBR_2_"   + wm + "RUN"     ) ||
        model->IsAniActive( "T_" + wm + "WALK_2_"    + wm + "WALKBL"  ) ||
        model->IsAniActive( "T_" + wm + "WALK_2_"    + wm + "WALKBR"  ) ||
        model->IsAniActive( "T_" + wm + "WALKBL_2_"  + wm + "WALK"    ) ||
        model->IsAniActive( "T_" + wm + "WALKBR_2_"  + wm + "WALK"    ) ||
        model->IsAniActive( "S_" + wm + "WALKBL"                      ) ||
        model->IsAniActive( "S_" + wm + "WALKBR"                      );
    }

    zTStickState left, right;
    XInputDevice.GetStickStatesSquare( left, right );
    if( isBackwark && left.Y < 0 ) {
      BodyAngleInversed = true;
      return true;
    }

    return false;
  }



  static float Alg_SafeDiv( const float& a, const float& b ) {
    if( a == 0.0f || b == 0.0f )
      return 0.0f;

    return a / b;
  }



  static float Alg_AngleDeadZoneRAD( float angle, float deadAngle, const float& deadSize ) {
    float angleSign  = angle < 0.0f ? -1.0f : 1.0f;
    float deadRadius = deadSize * 0.5f;
    angle            = angle * angleSign;

    if( angle < deadAngle - deadRadius ) {
      float mult = Alg_SafeDiv( deadAngle, deadAngle - deadRadius );
      angle = angle * mult;
    }
    else if( angle > deadAngle + deadRadius ) {
      angle      = RAD360 - angle;
      deadAngle  = RAD360 - deadAngle;
      float mult = Alg_SafeDiv( deadAngle, deadAngle - deadRadius );
      angle      = RAD360 - angle * mult;
    }
    else
      angle = deadAngle;

    return angle * angleSign;
  }

  static float GetRotationBodyAngle( bool useStrafeDeadZone ) {
    if( Opt_MotionType == 0 )
      return 0.0f;

    zTStickState left, right;
    XInputDevice.GetStickStatesSquare( left, right );
    zVEC2 direction = zVEC2( left.X, left.Y ).Normalize();

    float angleRad = direction.GetAngle();
    if( angleRad > RAD180 )
      angleRad -= RAD360;

    angleRad = Opt_MotionType == 1 ?
      SmoothIncrease( angleRad, RAD90 ) :
      angleRad;

    if( useStrafeDeadZone )
      angleRad = Alg_AngleDeadZoneRAD( angleRad, RAD90, 60.0f * RAD );

    return angleRad;
  }

  static float Alg_SeekToValue( float src, float dst, float intensity ) {
    float diff = dst - src;
    float sub  = diff * intensity * ztimer->frameTimeFloat;

    if( abs( sub ) > abs( diff ) )
      src = dst;
    else
      src += sub;

    return src;
  }

  static float Alg_SeekToAngle( float src, float dst, float intensity, bool loopedCircle ) {
    if( loopedCircle ) {
      if( (dst - src) > RAD180 ) dst -= RAD360;
      if( (src - dst) > RAD180 ) dst += RAD360;
    }
    float value = Alg_SeekToValue( src, dst, intensity );
    if( loopedCircle ) {
      if( value > +RAD180 ) value -= RAD360;
      if( value < -RAD180 ) value += RAD360;
    }
    return value;
  }


  static float Alg_GetAngleDifference( float src, float dst ) {
    if( (dst - src) > RAD180 ) src += RAD360;
    if( (src - dst) > RAD180 ) dst += RAD360;
    return dst - src;
  }


  static float GetBackwardAngle( float angle ) {
    float inverse = angle < 0.0f ? -1.0f : 1.0f;
    return (RAD180 - abs( angle )) * -1.0f * inverse;
  }


  static float GetBodyAngle() {
    if( BodyAngleInversed )
      return GetBackwardAngle( BodyAngle );

    return BodyAngle;
  }

  static void UpdateBodyAngle( zCModel* model ) {
    static float seekIntensitive = 0.01f;

    bool lastInversed    = BodyAngleInversed;
    bool inputAccess     = TurnBody_InputAccess();
    bool animationAccess = TurnBody_AnimationAccess( model );
    bool isOnFightMode   = player->GetWeaponMode() >= NPC_WEAPON_FIST && player->GetWeaponMode() <= NPC_WEAPON_2HS;

    if( animationAccess ) {
      if( inputAccess ) {
        /*
         * Continue spine rotation
         */
        if( !BodyAngleUndef ) {
          float rotationAngle = GetRotationBodyAngle( isOnFightMode );

          // Instant transition from far 
          // angles in fight mode. For
          // example - strafe to strafe.
          if( isOnFightMode && abs( rotationAngle - BodyAngle ) > RAD135 )
            BodyAngle = rotationAngle;

          // Smoothed transition spine
          // angles to left stick direction
          else
            BodyAngle = Alg_SeekToAngle( BodyAngle, rotationAngle, seekIntensitive, BodyAngleInversed );
        }
        /*
         * First spine foration
         */
        else
          BodyAngle = GetRotationBodyAngle( false );
      }

      BodyAngleUndef = false;
    }
    else
      BodyAngleUndef = true;
    

    // Complete spine rotation smoothly
    if( BodyAngleUndef ) {
      BodyAngleInversed = lastInversed;
      BodyAngle = Alg_SeekToAngle( GetBodyAngle(), 0.0f, seekIntensitive, BodyAngleInversed );
      BodyAngleInversed = false;
    }
  }



  void TurnPlayerBody( zCModel* model ) {
    if( BodyAngle == 0 )
      return;

    model->vobTrans.RotateXZ( GetBodyAngle() );
  }

  void TurnPlayerBody2( zCModel* model ) {
    if( BodyAngle == 0 )
      return;

    TurnBody( model, GetBodyAngle() * DEGREE );
  }




  HOOK Hook_CMovementTracker_UpdatePlayerPos PATCH( &zCMovementTracker::UpdatePlayerPos, &zCMovementTracker::UpdatePlayerPos_Union );

  void zCMovementTracker::UpdatePlayerPos_Union( zVEC3 const& position ) {
    static float motionOffset = 0.0f;
    float movMult = sin( BodyAngle );
    motionOffset = Alg_SeekToValue( motionOffset, 150.0f * movMult, 0.0015f );
    zVEC3 rightVector = target->GetRightVectorWorld() * motionOffset;

    THISCALL( Hook_CMovementTracker_UpdatePlayerPos )(position + rightVector);
  }












































  HOOK Hook_zCModel_CalcTransBlending PATCH_IF( &zCModel::CalcTransBlending, &zCModel::CalcTransBlending_Union, 1 );

  static bool CanTurnThisModel = false;

  void zCModel::CalcTransBlending_Union() {
    THISCALL( Hook_zCModel_CalcTransBlending )();
    if( CanTurnThisModel ) {
      UpdateBodyAngle( this );
      TurnPlayerBody( this );
    }
  }



  HOOK Hook_zCVob_DoFrameActivity PATCH( &zCVob::DoFrameActivity, &zCVob::DoFrameActivity_Union );

  extern void SetHeadingToEnemy( oCNpc* killer, oCNpc* enemy );

  void zCVob::DoFrameActivity_Union() {
    CanTurnThisModel = this == player && !ogame->pause_screen;
    THISCALL( Hook_zCVob_DoFrameActivity )();
    if( CanTurnThisModel ) {
      bool strafe = XInputDevice.StrafeButtonIsPressed();
      if( oCNpc::s_bTargetLocked && player->GetWeaponMode() != NPC_WEAPON_NONE && strafe ) {
        oCNpc* focusNpc = player->GetFocusNpc();
        if( focusNpc && !focusNpc->IsDead() )
          player->TurnToEnemy();
      }

      TurnPlayerBody2( player->GetModel() );
    }

    CanTurnThisModel = false;
  }














  

  HOOK Hook_oCAniCtrl_Human_Forward AS( &oCAniCtrl_Human::_Forward, &oCAniCtrl_Human::_Forward_Union );

  void oCAniCtrl_Human::_Forward_Union() {
    int aniMode = GetActionMode();
    if( aniMode != ANI_ACTION_DIVE && aniMode != ANI_ACTION_SWIM )
      if( GetModel()->IsAniActive( _s_walkbl ) )
        return _Stand();

    return THISCALL( Hook_oCAniCtrl_Human_Forward )();
  }

  HOOK Hook_oCAniCtrl_Human_Backward AS( &oCAniCtrl_Human::_Backward, &oCAniCtrl_Human::_Backward_Union );

  void oCAniCtrl_Human::_Backward_Union() {
    int aniMode = GetActionMode();
    if( aniMode != ANI_ACTION_DIVE && aniMode != ANI_ACTION_SWIM )
      if( GetModel()->IsAniActive( _s_walkl ) )
        return _Stand();
    
    return THISCALL( Hook_oCAniCtrl_Human_Backward )();
  }






  template <class T>
  inline void zClamp( T& x, const T min, const T max ) {
    if( x < min ) x = min; else
      if( x > max ) x = max;
  }

  inline float zAbsApprox( const float a ) {
    int tmp = *(int*)&a;
    tmp &= 0x7FFFFFFF;
    return *(float*)&tmp;
  };

#define Alg_Rad2Deg(rad) (zREAL(zREAL(rad)*zREAL(180.0f)/zREAL(PI)))

  HOOK Hook_zCAIPlayer_DoAutoRoll PATCH( &zCAIPlayer::DoAutoRoll, &zCAIPlayer::DoAutoRoll_Union );

  void zCAIPlayer::DoAutoRoll_Union() {
    if( vob != player || oldState == zMV_STATE_DIVE )
      return THISCALL( Hook_zCAIPlayer_DoAutoRoll )();

    if( !autoRollEnabled || vob->physicsEnabled )
      return;

    static float autoRollAngle = 0.0f;

    zVEC3 right       = vob->trafoObjToWorld.GetRightVector();
    zVEC3 up          = vob->trafoObjToWorld.GetUpVector();
    float algleSeeked = 0.0f;

    if( autoRollPos != autoRollPosDest ) {
      float angle           = right.GetAngleXZ();
      static float oldAngle = angle;
      float rotation        = Alg_GetAngleDifference( angle, oldAngle ) * DEGREE;
      oldAngle              = angle;

      float autoRollMult = BodyAngle * 3.0f;
      zClamp( autoRollMult, -PI / 2, +PI / 2 );
      autoRollMult = 1.0f - abs( sin( autoRollMult ) );

      algleSeeked = Alg_SafeDiv( rotation * 100.0f, ztimer->frameTimeFloat ) * autoRollMult;
      zClamp( algleSeeked, -15.0f, 15.0f );
    }

    autoRollSpeed = 0.005f;
    autoRollAngle = Alg_SeekToValue( autoRollAngle, algleSeeked, autoRollSpeed );

    if( up[VY] < 0.9999f )
      vob->ResetXZRotationsWorld();

    if( (int)autoRollAngle != 0 )
      vob->RotateLocalZ( autoRollAngle );
  }



  HOOK Hook_zCAIPlayer_CheckEnoughSpaceMoveForward PATCH( &zCAIPlayer::CheckEnoughSpaceMoveForward, &zCAIPlayer::CheckEnoughSpaceMoveForward_Union );

  int zCAIPlayer::CheckEnoughSpaceMoveForward_Union( const int a ) {
    if( vob != player )
      return THISCALL( Hook_zCAIPlayer_CheckEnoughSpaceMoveForward )(a);

    float angle = GetRotationBodyAngle( false );
    zVEC3 vectorAt = vob->GetAtVectorWorld().RotateXZ( angle );
    return CheckEnoughSpaceMoveDir( vectorAt, a );
  }
}
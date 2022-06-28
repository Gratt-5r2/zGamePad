// Supported with union (c) 2020 Union team

// User API for oCNpc
// Add your methods here

void OnDamage_Union( oSDamageDescriptor& );
int FocusCheck_Union( const zCVob* vob, const int type, const int last_type, const zREAL low_azi, zREAL& azi );
int FocusCheckBBox_Union( const zCVob* vob, const int type, const int last_type, const zREAL low_azi, zREAL& azi );
//int IsDead_Union();
void ToggleFocusVob_Union( int left );
void CreateVobList_Union( float max_dist );

void GetNearestValidVob_Union( float max_dist );
void GetNearestVob_Union( float max_dist );
int CanDrawWeapon_Union();
// void CollectFocusVob_Union( const int a_bForce );

#if ENGINE < Engine_G2
static int s_bTargetLocked;
void CollectFocusVob_Union();
int IsDead_Union() { return attribute[NPC_ATR_HITPOINTS] <= 0; }
#endif

void TurnToEnemyInAttack();
Array<oCNpc*> GetNearestFightNpcList();
Array<oCNpc*> GetNearestVisibleFightNpcList();
oCNpc* oCNpc::GetNearestFightNpcRight_Union();
oCNpc* oCNpc::GetNearestFightNpcLeft_Union();
oCNpc* oCNpc::GetNearestFightNpc_Union();
int CanInterruptAttack();
int EV_DrawWeapon_Union( oCMsgWeapon* );

#if ENGINE == Engine_G2
int IsDead() { return attribute[NPC_ATR_HITPOINTS] <= 0; }
#endif
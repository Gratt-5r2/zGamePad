// Supported with union (c) 2020 Union team

// User API for oCNpc
// Add your methods here

void OnDamage_Union( oSDamageDescriptor& );
int FocusCheck_Union( const zCVob* vob, const int type, const int last_type, const zREAL low_azi, zREAL& azi );
int FocusCheckBBox_Union( const zCVob* vob, const int type, const int last_type, const zREAL low_azi, zREAL& azi );
int IsDead_Union();
void ToggleFocusVob_Union( int left );
void CreateVobList_Union( float max_dist );

void GetNearestValidVob_Union( float max_dist );
void GetNearestVob_Union( float max_dist );
void CollectFocusVob_Union( const int a_bForce );

#if ENGINE < Engine_G2
static int s_bTargetLocked;
#endif
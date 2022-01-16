// Supported with union (c) 2020 Union team

// User API for zCModel
// Add your methods here

void CalcTransBlending_Union();

inline int IsAniActive( int aniID ) {
	return IsAniActive( GetAniFromAniID( aniID ) );
};

inline int IsAniActive( const zSTRING& aniName ) {
	return IsAniActive( GetAniIDFromAniName( aniName ) );
};
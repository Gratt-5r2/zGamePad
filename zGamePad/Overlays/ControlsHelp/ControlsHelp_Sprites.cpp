// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  zCCombination_SpriteList::zTSprite::zTSprite( const string& xboxTex, const string& psTex ) {
    static int alpha =  GetHintsTransparency();
    XBOX = new zCView();
    PS = new zCView();
    XBOX->InsertBack( xboxTex );
    PS->InsertBack( psTex );
    XBOX->SetAlphaBlendFunc( zRND_ALPHA_FUNC_BLEND );
    PS->SetAlphaBlendFunc( zRND_ALPHA_FUNC_BLEND );
    XBOX->SetTransparency( alpha );
    PS->SetTransparency( alpha );
  }



  zCView* zCCombination_SpriteList::zTSprite::GetSprite( const int gamepadID ) {
    return gamepadID == 0 ? XBOX : PS;
  }



  zCCombination_SpriteList::zCCombination_SpriteList() {
    Initialize();
  }



  void zCCombination_SpriteList::Initialize() {
    Union.GetSysPackOption().Read( ControllerScheme, "zGamePad", "ControllerScheme", 0 );

    SpriteList.Insert( JOY_UP,           new zTSprite( "XboxOne_Dpad_Up",           "PS4_Dpad_Up"           ) );
    SpriteList.Insert( JOY_DOWN,         new zTSprite( "XboxOne_Dpad_Down",         "PS4_Dpad_Down"         ) );
    SpriteList.Insert( JOY_LEFT,         new zTSprite( "XboxOne_Dpad_Left",         "PS4_Dpad_Left"         ) );
    SpriteList.Insert( JOY_RIGHT,        new zTSprite( "XboxOne_Dpad_Right",        "PS4_Dpad_Right"        ) );
    SpriteList.Insert( JOY_MENU,         new zTSprite( "XboxOne_Menu",              "PS4_Options"           ) );
    SpriteList.Insert( JOY_VIEW,         new zTSprite( "XboxOne_Windows",           "PS4_Share"             ) );
    SpriteList.Insert( JOY_LSTICK,       new zTSprite( "XboxOne_Left_Stick_Click",  "PS4_Left_Stick_Click"  ) );
    SpriteList.Insert( JOY_RSTICK,       new zTSprite( "XboxOne_Right_Stick_Click", "PS4_Right_Stick_Click" ) );
    SpriteList.Insert( JOY_LB,           new zTSprite( "XboxOne_LB",                "PS4_L1"                ) );
    SpriteList.Insert( JOY_RB,           new zTSprite( "XboxOne_RB",                "PS4_R1"                ) );
    SpriteList.Insert( JOY_A,            new zTSprite( "XboxOne_A",                 "PS4_Cross"             ) );
    SpriteList.Insert( JOY_B,            new zTSprite( "XboxOne_B",                 "PS4_Circle"            ) );
    SpriteList.Insert( JOY_X,            new zTSprite( "XboxOne_X",                 "PS4_Square"            ) );
    SpriteList.Insert( JOY_Y,            new zTSprite( "XboxOne_Y",                 "PS4_Triangle"          ) );
    SpriteList.Insert( JOY_LSTICK_LOWUP, new zTSprite( "XboxOne_Left_Stick",        "PS4_Left_Stick"        ) );
    SpriteList.Insert( JOY_LSTICK_UP,    new zTSprite( "XboxOne_Left_Stick",        "PS4_Left_Stick"        ) );
    SpriteList.Insert( JOY_LSTICK_DOWN,  new zTSprite( "XboxOne_Left_Stick",        "PS4_Left_Stick"        ) );
    SpriteList.Insert( JOY_LSTICK_LEFT,  new zTSprite( "XboxOne_Left_Stick",        "PS4_Left_Stick"        ) );
    SpriteList.Insert( JOY_LSTICK_RIGHT, new zTSprite( "XboxOne_Left_Stick",        "PS4_Left_Stick"        ) );
    SpriteList.Insert( JOY_LT,           new zTSprite( "XboxOne_LT",                "PS4_L2"                ) );
    SpriteList.Insert( JOY_RT,           new zTSprite( "XboxOne_RT",                "PS4_R2"                ) );
    SpriteList.Insert( JOY_DPAD_FULL,    new zTSprite( "XboxOne_Dpad",              "PS4_Dpad"              ) );
    SpriteList.Insert( JOY_DPAD_UPDOWN,  new zTSprite( "XboxOne_Dpad_UpDown",       "PS4_Dpad_UpDown"       ) );
    SpriteList.Insert( JOY_LSTICK_FULL,  new zTSprite( "XboxOne_Left_Stick",        "PS4_Left_Stick"        ) );
    SpriteList.Insert( JOY_RSTICK_FULL,  new zTSprite( "XboxOne_Right_Stick",       "PS4_Right_Stick"       ) );
  }



  zCView* zCCombination_SpriteList::GetSprite( JOYKEY key ) {
    auto& pair = SpriteList[key];
    if( pair.IsNull() )
      return Null;

    return pair.GetValue()->GetSprite( ControllerScheme );
  }



  zCCombination_SpriteList& zCCombination_SpriteList::GetInstance() {
    static zCCombination_SpriteList* instance = new zCCombination_SpriteList();
    return *instance;
  }
}
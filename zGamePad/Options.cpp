// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  void ApplyGamepadOptions() {
    int gamepadEnabled           = zoptions->ReadBool(   "ZGAMEPAD", "Enabled",              True );
    int controllerID             = zoptions->ReadInt(    "ZGAMEPAD", "ControllerID",         0 );
    zSTRING controlsFile         = zoptions->ReadString( "ZGAMEPAD", "ControlsFile",         "Controls.Gamepad" );
    float stickSensitivity       = zoptions->ReadReal(   "ZGAMEPAD", "StickSensitivity",     0.5f );
    int hintsEnabled             = zoptions->ReadBool(   "ZGAMEPAD", "HintsEnabled",         True );
    int controllerScheme         = zoptions->ReadInt(    "ZGAMEPAD", "ControllerScheme",     0 );
    float hintsIconScale         = zoptions->ReadReal(   "ZGAMEPAD", "HintsIconScale",       0.5f );
    float hintsTransparency      = zoptions->ReadReal(   "ZGAMEPAD", "HintsTransparency",    0.7f );
    float motionType             = zoptions->ReadInt(    "ZGAMEPAD", "MotionType",           1 );
    zSTRING quickBar_Counts      = zoptions->ReadString( "ZGAMEPAD", "QuickBar_Counts",      "4, 9" );
    zSTRING quickBar_TextureBase = zoptions->ReadString( "ZGAMEPAD", "QuickBar_TextureBase", "QUICKSLOT" );
    zSTRING quickBar_Offsets     = zoptions->ReadString( "ZGAMEPAD", "QuickBar_Offsets",     "1.3, 1.0" );

    auto& spOptions = Union.GetSysPackOption();
    if( spOptions.IsExists( "MOUSE", "NoDirectXAxisMode" ) )
      spOptions.Read( Opt_NoDxMode, "MOUSE", "NoDirectXAxisMode" );

    Opt_GamepadEnabled       = gamepadEnabled != false;
    Opt_ControllerID         = controllerID;
    Opt_ControlsFile         = controlsFile;
    Opt_StickSensitivity     = stickSensitivity * 2.0f;
    Opt_HintsEnabled         = hintsEnabled != false;
    Opt_ControllerScheme     = controllerScheme;
    Opt_HintsIconScale       = hintsIconScale * 5.0f;
    Opt_HintsTransparency    = (int)(hintsTransparency * 255.0f);
    Opt_MotionType           = motionType;
    Opt_QuickBar_Counts      = quickBar_Counts;
    Opt_QuickBar_TextureBase = quickBar_TextureBase;
    Opt_QuickBar_Offsets     = quickBar_Offsets;

    switch( Union.GetSystemLanguage() ) {
      case Lang_Rus:
        Opt_UseItemError = "Необходимо быть на земле.";
        break;
      case Lang_Eng:
        Opt_UseItemError = "You should be on the ground.";
        break;
      case Lang_Ger:
        Opt_UseItemError = "Du solltest dich am Boden befinden.";
        break;
      case Lang_Pol:
        Opt_UseItemError = "Użycie przedmiotów możliwe tylko na gruncie.";
        break;
    }
  }
}
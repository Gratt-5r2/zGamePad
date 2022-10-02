// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  bool Opt_GamepadEnabled          = false;
  int Opt_ControllerID             = 0;
  zSTRING Opt_ControlsFile         = "Controls.Gamepad";
  float Opt_StickSensitivity       = 1.0;
  bool Opt_HintsEnabled            = true;
  int Opt_ControllerScheme         = 0;
  float Opt_HintsIconScale         = 2.0;
  int Opt_HintsTransparency        = 185;
  int Opt_MotionType               = 1;
  zSTRING Opt_QuickBar_Counts      = "4, 9";
  zSTRING Opt_QuickBar_TextureBase = "QUICKSLOT";
  zSTRING Opt_QuickBar_Offsets     = "1.3, 1.0";
  zSTRING Opt_UseItemError         = "You should be on the ground.";
  bool Opt_NoDxMode                = false;
  int Opt_QuickSaveMode            = 0;
  int Opt_Vibration                = 1;
  int Opt_InvertY                  = 0;

  void ApplyGamepadOptions();
}
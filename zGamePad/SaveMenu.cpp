// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  HOOK Ivk_zCMenuItem_Input AS( &zCMenuItem::Input, &zCMenuItem::Input_Union );

  string GetWorldTimeString() {
    oCWorldTimer* wldTimer = ogame->GetWorldTimer();
    int hour, min;
    wldTimer->GetTime( hour, min );
    int day = wldTimer->GetDay() + 1;
    return string::Combine( "Day: %i   Time: %i:%i", day, hour, min );
  }


  int zCMenuItem::Input_Union( int length, zSTRING& output ) {
    zCMenuItemInput* itemInput = dynamic_cast<zCMenuItemInput*>(this);
    if( itemInput ) {
      string oldValue = itemInput->GetValue();
      if( oldValue == "---" || oldValue.StartWith( "Day: " ) ) {
        string newValue = GetWorldTimeString();

        output = Z newValue;
        int confirm = THISCALL( Ivk_zCMenuItem_Input )(length, output);
        if( !confirm )
          output = Z oldValue;

        return confirm;
      }
    }

    return THISCALL( Ivk_zCMenuItem_Input )(length, output);
  }
}
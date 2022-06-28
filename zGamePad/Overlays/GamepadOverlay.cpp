// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  inline double min2( const double& a, const double& b ) {
    return min( a, b );
  }

  inline double max2( const double& a, const double& b ) {
    return max( a, b );
  }



  bool oCPortalRoomManager::IsNpcInRoom( oCNpc* npc ) {
    if( !npc )
      return false;

    const zSTRING* portalName = npc->GetSectorNameVobIsIn();
    if( !portalName )
      return false;

    int portalIndex = GetPortalRoomIndex( *portalName );
    if( portalIndex == Invalid )
      return false;

    oCPortalRoom* portalRoom = portals[portalIndex];
    return portalRoom != Null;
  }


  zCGamepadOverlay::zCGamepadOverlay() {
    Parent = Null;
    IsOpened = False;
    OverlayedColor = GFX_WHITE;
  }



  bool zCGamepadOverlay::Opened() {
    return IsOpened;
  }


  void zCGamepadOverlay::SetDaylightColor() {
    static zCView* statCol = 0;
    static zCView* dynCol = 0;
    if( statCol == 0 ) {
      statCol = new zCView();
      dynCol = new zCView();
      statCol->SetPos( 2000, 2000 );
      dynCol->SetPos( 2000, 2500 );
      statCol->SetSize( 300, 300 );
      dynCol->SetSize( 300, 300 );
      statCol->InsertBack( "white" );
      dynCol->InsertBack( "white" );
      screen->InsertItem( statCol );
      screen->InsertItem( dynCol );
    }

    statCol->SetColor( player->GetLightColorDyn() );
    dynCol->SetColor( player->GetLightColorStat() );


    zCOLOR colorDyn = player->GetLightColorDyn();
    zCOLOR colorStat = player->GetLightColorStat();
    zCOLOR color;
    color.r = ((int)colorDyn.r + (int)colorStat.r) / 2;
    color.g = ((int)colorDyn.g + (int)colorStat.g) / 2;
    color.b = ((int)colorDyn.b + (int)colorStat.b) / 2;

    // zCOLOR absoluteColor;
    // double intensity = ((double)color.r + (double)color.g + (double)color.b) / 3.0;
    // double multiplier = SafeDiv( 255.0, intensity );
    // absoluteColor.r = max2( (byte)min2((double)color.r * multiplier, 255.0), 105 );
    // absoluteColor.g = max2( (byte)min2((double)color.g * multiplier, 255.0), 105 );
    // absoluteColor.b = max2( (byte)min2((double)color.b * multiplier, 255.0), 105 );
    // absoluteColor.alpha = color.alpha;

    zCOLOR brightColor;
    int maxChannel = 0;
    if( color.r > maxChannel ) maxChannel = color.r;
    if( color.g > maxChannel ) maxChannel = color.g;
    if( color.b > maxChannel ) maxChannel = color.b;
    double multiplier = SafeDiv( 255.0, (double)maxChannel );
    brightColor.r = max2( (byte)((double)color.r * multiplier), 80 );
    brightColor.g = max2( (byte)((double)color.g * multiplier), 80 );
    brightColor.b = max2( (byte)((double)color.b * multiplier), 80 );
    brightColor.alpha = 255;

    ChangeColor( brightColor );
    return;

    oCWorld* world = ogame->GetGameWorld();
    if( !world )
      return;

    zCSkyControler_Outdoor* skyController = world->GetActiveSkyControler()->CastTo<zCSkyControler_Outdoor>();
    bool isInRoom = ogame->GetPortalRoomManager()->IsNpcInRoom( player );
    if( !skyController || isInRoom )
      ChangeColor( player->GetLightColorAtCenter() );
    else
      ChangeColor( skyController->GetDaylightColorFromIntensity( 255 ) );
  }


  void zCGamepadOverlay::Show( zCView* parent ) {
    SetDaylightColor();
  }



  void zCGamepadOverlay::ChangeColor( const zCOLOR& col ) {
    OverlayedColor = col;
  }



  void zCGamepadOverlay::InsertChild( zCGamepadOverlay* overlay ) {
    Childs += overlay;
  }



  void zCGamepadOverlay::RemoveChild( zCGamepadOverlay* overlay ) {
    if( Childs.HasEqual( overlay ) )
      Childs -= overlay;
  }



  string zCGamepadOverlay::GetDescription() {
    return "Unnamed overlay";
  }




  void OverlaysLoop() {
    if( !player || player->IsDead() || !player->IsHuman() )
      return;

    static zCGamepadQuickMenu* quickMenu       = zCGamepadQuickMenu::GetInstance();
    static zCGamepadControlsHelp* controlsHelp = zCGamepadControlsHelp::GetInstance();
    static zCGamepadQuickBar_Items* itemsRing  = zCGamepadQuickBar_Items::GetInstance();

    if( !Cond_ConsoleIsOpen() && zKeyToggled( KEY_3 ) )
      if( !player->inventory2.IsOpen() )
        quickMenu->Show( screen );

    // controlsHelp->Top();
  }
}
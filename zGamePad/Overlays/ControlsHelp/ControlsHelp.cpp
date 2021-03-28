// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  Array<zTCombination_Help*> zTCombination_Help::Helps;

  zTCombination_Help::zTCombination_Help() {
    Enabled = false;
    Helps += this;
  }



  zTCombination_Help::zTCombination_Help( const string& text, Array<JOYKEY> keys ) {
    Text = text;
    Keys = keys;
    Enabled = false;
    Helps += this;
  }



  zTCombination_Help* zTCombination_Help::Create( const string& text, Array<JOYKEY> keys ) {
    for( uint i = 0; i < Helps.GetNum(); i++ )
      if( Helps[i]->Text == text && Helps[i]->Keys == keys )
        return Helps[i];

    return new zTCombination_Help( text, keys );
  }



  zTCombination_Help::~zTCombination_Help() {
    Helps -= this;
  }



  zCGamepadControlsHelp::zCGamepadControlsHelp() {
    Show( screen );
    Background = new zCView();
    Background->SetFont( screen->GetFontName() );
    SetFont( screen->GetFontName() );
  }



  void zCGamepadControlsHelp::Show( zCView* parent ) {
    if( IsOpened )
      return;

    Parent = parent;
    Parent->InsertItem( this );
    SetSize( 8192, 8192 );
    IsOpened = true;
  }



  void zCGamepadControlsHelp::Close() {
    if( !IsOpened )
      return;

    Parent->RemoveItem( this );
    Parent = Null;
    IsOpened = false;
  }



  void zCGamepadControlsHelp::Blit() {
    if( !Opt_GamepadEnabled || !Opt_HintsEnabled || (!XInputDevice.IsConnected() && !DS4Device.IsConnected()) )
      return;

    int startPosX = 8192;
#if ENGINE < Engine_G2
    if( player && player->inventory2.IsOpen() ) {
      int sx, sy;
      player->inventory2.GetSize( sx, sy );
      startPosX = 8192 - (int)((float)sx * CalculateInterfaceScale()) - 100;
    }
#endif

    InsertItem( Background );

    auto spriteList = zCCombination_SpriteList::GetInstance();
    auto& helps     = zTCombination_Help::Helps;

    for( uint i = 0, e = 0; i < helps.GetNum(); i++ ) {
      auto& help = helps[i];
      if( !help->Enabled )
        continue;

      zSTRING text = help->Text;
      int fontX    = FontSize( text );
      int fontY    = (int)((float)FontY() * Opt_HintsIconScale);
      int spriteSY = fontY;
      int spriteSX = anx( nay( fontY ) );
      uint keysNum = help->Keys.GetNum();
      int backSX   = fontX;
      int backSY   = fontY;
      int backPX   = startPosX - fontX - spriteSX * (int&)keysNum;
      int backPY   = 7200 - fontY * e++;

      Background->ClrPrintwin();
      Background->SetSize( backSX, backSY );
      Background->fontColor.alpha = Opt_HintsTransparency;
      Background->PrintCXY( text );
      Background->SetPos( backPX, backPY );
      Background->Blit();

      for( uint k = 0; k < keysNum; k++ ) {
        zCView* sprite = spriteList.GetSprite( help->Keys[k] );
        if( sprite == Null )
          continue;

        InsertItem( sprite );
        sprite->SetPos( backPX + backSX + spriteSX * k, backPY );
        sprite->SetSize( spriteSX, spriteSY );
        sprite->Blit();
        sprite->SetTransparency( Opt_HintsTransparency );
        RemoveItem( sprite );
      }
    }

    RemoveItem( Background );
  }



  zCGamepadControlsHelp* zCGamepadControlsHelp::GetInstance() {
    static zCGamepadControlsHelp* instance = new zCGamepadControlsHelp();
    return instance;
  }
}
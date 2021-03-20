// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  class zCCombination_SpriteList {
    struct zTSprite {
      zCView* XBOX;
      zCView* PS;
      zCView* GetSprite( const int gamepadID );
      zTSprite( const string& xboxTex, const string& psTex );
    };

    Map<JOYKEY, zTSprite*> SpriteList;
    zCCombination_SpriteList();
    void Initialize();
  public:

    zCView* GetSprite( JOYKEY key );
    static zCCombination_SpriteList& GetInstance();
  };


  struct zTCombination_Help : public zCObject {
    static Array<zTCombination_Help*> Helps;
    string Text;
    Array<JOYKEY> Keys;
    bool Enabled;
    
    static zTCombination_Help* Create( const string& text, Array<JOYKEY> keys );
    ~zTCombination_Help();

  protected:
    zTCombination_Help();
    zTCombination_Help( const string& text, Array<JOYKEY> keys );
  };




	class zCGamepadControlsHelp : public zCGamepadOverlay {
		zCView* Background;
		zCGamepadControlsHelp();
	public:

		virtual void Show( zCView* parent );
		virtual void Close();
		virtual void Blit();
		static zCGamepadControlsHelp* GetInstance();
	};
}
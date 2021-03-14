// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class zTGamepadQuickMenu_Selector;
	class zCGamepadQuickMenu;
  typedef void( *zTGamepadQuickMenu_Event )( zTGamepadQuickMenu_Selector* );
	


	struct zTGamepadQuickMenu_Selector : public zCView {
		zCGamepadQuickMenu* Parent;
		string Description;

		zTGamepadQuickMenu_Selector( zCGamepadQuickMenu* parent );
		virtual void Blit();
		virtual void Show();
		virtual void Close();
	};



	class zCGamepadQuickMenu : public zCGamepadOverlay {
		Array<zTGamepadQuickMenu_Selector*> Selectors;
		zCView* LeftSide;
		zCGamepadOverlay* ActiveOverlay;
		uint SelectedMenu;

	protected:
		zCGamepadQuickMenu();
		void UpdateSelectors();
		virtual ~zCGamepadQuickMenu();
	public:

		virtual void Blit();
		virtual void Show( zCView* parent );
		virtual void Close();
		virtual void SelectNextMenu();
		virtual void SelectPrevMenu();
		virtual void SetSelectedMenu( const uint& index );
		virtual void InsertChild( zCGamepadOverlay* overlay );
		virtual void RemoveChild( zCGamepadOverlay* overlay );
		virtual int HandleEvent( int key );

		static zCGamepadQuickMenu* GetInstance();
	};
}
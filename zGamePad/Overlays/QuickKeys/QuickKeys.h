// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class zCGamepadQuickKeys : public zCGamepadOverlay {
	protected:
		
	public:
		
		virtual void Show( zCView* parent );
		virtual void Close();
		virtual int HandleEvent( int key );
		virtual string GetDescription();

		static zCGamepadQuickKeys* GetInstance();
	};
}
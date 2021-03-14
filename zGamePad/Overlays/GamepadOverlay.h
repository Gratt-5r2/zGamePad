// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class zCGamepadOverlay : public zCView {
	protected:
		zCView* Parent;
		Array<zCGamepadOverlay*> Childs;
		bool IsOpened;
	public:

		zCGamepadOverlay();
		bool Opened();
		virtual void Show( zCView* parent ) = 0;
		virtual void Close() = 0;
		virtual void InsertChild( zCGamepadOverlay* overlay );
		virtual void RemoveChild( zCGamepadOverlay* overlay );
		virtual string GetDescription();
	};
}
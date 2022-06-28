// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class zCGamepadOverlay : public zCView {
	protected:
		zCView* Parent;
		Array<zCGamepadOverlay*> Childs;
		bool IsOpened;
		zCOLOR OverlayedColor;
	public:

		zCGamepadOverlay();
		bool Opened();
		void SetDaylightColor();
		virtual void Show( zCView* parent );
		virtual void Close() = 0;
		virtual void InsertChild( zCGamepadOverlay* overlay );
		virtual void RemoveChild( zCGamepadOverlay* overlay );
		virtual void ChangeColor( const zCOLOR& color );
		virtual string GetDescription();
	};
}
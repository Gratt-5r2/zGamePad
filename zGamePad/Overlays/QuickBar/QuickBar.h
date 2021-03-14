// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	enum zEGamepadQuickBarAlignment {
		zEGamepadQuickBarAlignment_Left,
		zEGamepadQuickBarAlignment_Center,
		zEGamepadQuickBarAlignment_Right
	};


	struct zTGamepadQuickBarCell {
		oCItem* Item;
		int InstanceID;
		zCView* Background;
		zCView* Background_Highlight;
		zCView* ItemRenderer;
		zCView* Foreground;

		zTGamepadQuickBarCell();
		void SetItem( oCItem* item );
		void SetViewport( const int& px, const int& py, const int& sx, const int& sy );
	};


	class zCGamepadQuickBar : public zCGamepadOverlay {
	protected:
		Array<Array<zTGamepadQuickBarCell>> Circles;
		zSTRING BaseTextureName;
		void BlitCells();
		void ShowCells();
		void CloseCells();
		uint LastCircleID;
		uint LastCellID;
		bool NeedToMarkEquipedItems;
	public:
		zEGamepadQuickBarAlignment Alignment;

		zCGamepadQuickBar();
		void GetSelectedCellID( uint& circleID, uint& cellID );
		oCItem* GetSelectedItem();
		void SetBaseTextureName( const zSTRING& textureName );
		void SetItem( oCItem* item, uint& circleID, uint& cellID );
		bool SetItemInActiveCell( oCItem* item );
		void PushItem( oCItem* item, const uint& circleID );
		void UpdateCells();
		zTGamepadQuickBarCell& GetCell( const uint& circleID, const uint& cellID );
		virtual void Blit();
		virtual void Show( zCView* parent );
		virtual void ShowAt( zCView* parent, const zEGamepadQuickBarAlignment& align = zEGamepadQuickBarAlignment_Right );
		virtual void Close();
		virtual int HandleEvent( int key );

		static float HighlightAlpha;
		static uint OverlayMouseHooksCount;
	};

	float zCGamepadQuickBar::HighlightAlpha = 0.0f;
	uint zCGamepadQuickBar::OverlayMouseHooksCount = 0;



	class zCGamepadQuickBar_Weapons : public zCGamepadQuickBar {
		zCGamepadQuickBar_Weapons();
	public:

		virtual void Show( zCView* parent );
		virtual string GetDescription();
		static zCGamepadQuickBar_Weapons* GetInstance();
	};



	class zCGamepadQuickBar_Items : public zCGamepadQuickBar {
		zCGamepadQuickBar_Items();
		void ResetVisuals();
		void ResetInstances();
		void UpdateVisuals();
		Array<zTGamepadQuickBarCell*> GetNotEmptyCells();
	public:

		void Archive( zCArchiver& ar );
		void Unarchive( zCArchiver& ar );
		virtual void Show( zCView* parent );
		virtual string GetDescription();
		void ResetAll();
		static zCGamepadQuickBar_Items* GetInstance();
	};




}
META
{
  Parser = Menu;
  MergeMode = true;
};


// Overrided options menu for
// insertion of a new back button.
instance MENU_OPTIONS(C_MENU_DEF)
{
  MENU_OPTIONS_Old();
  items[100] = "MENUITEM_OPT_BACK_UNION";
};


// Prototype of a Union menu button.
instance MENUITEM_OPT_UNION(C_MENU_ITEM)
{
	Text[0]          = "Union";
	onselaction[0]   = SEL_ACTION_STARTMENU;
	onselaction_s[0] = "MENU_OPT_UNION";
	Flags            = Flags | IT_TXT_CENTER;
};


// Overrided back button in the options menu.
// Used for fast insertion of the Union menu button.
instance MENUITEM_OPT_BACK(C_MENU_ITEM)
{
  MENUITEM_OPT_BACK_OLD();
  MENUITEM_OPT_UNION();
  posy = posy - dimy / 2;
};


// New back menu based on the original back button.
instance MENUITEM_OPT_BACK_UNION(C_MENU_ITEM)
{
  MENUITEM_OPT_BACK_OLD();
  posy = posy + dimy / 2;
};







// Counter of Union subMenu items
var int MENU_OPT_UNION_PY;

// Base Union menu
instance MENU_OPT_UNION(C_MENU_DEF)
{
    MENU_OPT_UNION_PY = 0;
	backpic = MENU_BACK_PIC;
    Search_Items_Automatically("MENUITEM_UNION_AUTO_*");

	defaultoutgame = 0;
	defaultingame = 0;
	Flags = Flags | MENU_SHOW_INFO;
};


prototype C_MENU_ITEM_UNION(C_MENU_ITEM)
{
    C_MENU_ITEM_DEF();
	backpic = MENU_ITEM_BACK_PIC;
	dimx    = 8192;
	dimy    = 600;
	posx    = 0;
	posy    = MENU_OPT_UNION_PY;
	flags   = flags | IT_TXT_CENTER;
    
    MENU_OPT_UNION_PY += 300;
};


instance MENUITEM_UNION_AUTO_BACK(C_MENU_ITEM) // MENUITEM_OPT_UNION_BACK(C_MENU_ITEM)
{
	MENUITEM_OPT_BACK_OLD();
};



















META
{
  Parser    = Menu;
  MergeMode = true;
};


instance MENU_OPTIONS(C_MENU_DEF)
{
  MENU_OPTIONS_Old();
  items[100] = "MENUITEM_OPT_BACK_NEW";
};


instance MENUITEM_OPT_UNION(C_MENU_ITEM)
{
  onselaction[0]   = SEL_ACTION_STARTMENU;
  onselaction_s[0] = "MENU_OPT_UNION";
  Flags            = Flags | IT_TXT_CENTER;
  text[0]          = Str_GetLocalizedString(
    "Union & Плагины",
    "Union & Plugins",
    "Union & Plugins",
    "Union & Plugins"
  );
};


instance MENUITEM_OPT_BACK(C_MENU_ITEM)
{
  MENUITEM_OPT_BACK_OLD();
  MENUITEM_OPT_UNION();
  posy = posy - dimy / 2 + 100;
};


instance MENUITEM_OPT_BACK_NEW(C_MENU_ITEM)
{
  MENUITEM_OPT_BACK_OLD();
  posy = posy + dimy / 2 + 100;
};



var int MENU_OPT_UNION_PY;

instance MENU_OPT_UNION(C_MENU_DEF)
{
  Menu_SearchItems("MENUITEM_UNION_AUTO_*");
  MENU_OPT_UNION_PY = 1200;
  backpic           = MENU_BACK_PIC;
  items[0]          = "UNION_MENUITEM_TITLE";
  items[100]        = "UNION_MENUITEM_BACK";
  defaultoutgame    = 0;
  defaultingame     = 0;
  Flags             = Flags | MENU_SHOW_INFO;
};


prototype C_MENU_ITEM_UNION(C_MENU_ITEM)
{
  C_MENU_ITEM_DEF();
  backpic  = MENU_ITEM_BACK_PIC;
  dimx     = 8192;
  dimy     = 600;
  posx     = 0;
  posy     = MENU_OPT_UNION_PY;
  flags    = flags | IT_TXT_CENTER;
  fontname = MENU_FONT_SMALL;
    
  MENU_OPT_UNION_PY += 180;
};


instance UNION_MENUITEM_TITLE(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = 800;
  dimx    = 8100;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = Str_GetLocalizedString(
    "Список",
    "LIst",
    "LIsta",
    "LIste"
  );
};


instance UNION_MENUITEM_BACK(C_MENU_ITEM_DEF)
{
  posx           = 1000;
  posy           = 6892;
  dimx           = 6192;
  onselaction[0] = SEL_ACTION_BACK;
  flags          = flags | IT_TXT_CENTER;
  text[0]        = Str_GetLocalizedString(
    "НАЗАД",
    "BACK",
    "ZURÜCK",
    "WRÓĆ"
  );
};
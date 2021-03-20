META
{
  Parser = Menu;
  After = zUnionMenu.d;
};

instance MENUITEM_UNION_AUTO_ZGAMEPAD(C_MENU_ITEM_UNION)
{
	text[0]          = "zGamePad";
	text[1]          = "Configure gamepad";
	onselaction[0]   = SEL_ACTION_STARTMENU;
	onselaction_s[0] = "MENU_OPT_ZGAMEPAD";
};





instance MENU_OPT_ZGAMEPAD(C_MENU_DEF)
{
	backpic = MENU_BACK_PIC;
	items[0] = "MENUITEM_ZGAMEPAD_HEADLINE";
	items[1] = "MENUITEM_ZGAMEPAD_ENABLED";
	items[2] = "MENUITEM_ZGAMEPAD_ENABLED_CHOICE";
	items[3] = "MENUITEM_ZGAMEPAD_GAMEPAD_ID";
	items[4] = "MENUITEM_ZGAMEPAD_GAMEPAD_ID_CHOICE";
	items[5] = "MENUITEM_ZGAMEPAD_SENSITIVITY";
	items[6] = "MENUITEM_ZGAMEPAD_SENSITIVITY_SLIDER";
	items[7] = "MENUITEM_ZGAMEPAD_HINTS";
	items[8] = "MENUITEM_ZGAMEPAD_HINTS_CHOICE";
	items[9] = "MENUITEM_ZGAMEPAD_HINTS_SCHEME";
	items[10] = "MENUITEM_ZGAMEPAD_HINTS_SCHEME_CHOICE";
	items[11] = "MENUITEM_ZGAMEPAD_HINTS_SCALE";
	items[12] = "MENUITEM_ZGAMEPAD_HINTS_SCALE_SLIDER";
	items[13] = "MENUITEM_ZGAMEPAD_HINTS_TRANSPARENCY";
	items[14] = "MENUITEM_ZGAMEPAD_HINTS_TRANSPARENCY_SLIDER";
	items[16] = "MENUITEM_ZGAMEPAD_OPEN_LINK";
	items[17] = "MENUITEM_ZGAMEPAD_BACK";
	flags = flags | MENU_SHOW_INFO;
};


// Headline
instance MENUITEM_ZGAMEPAD_HEADLINE(C_MENU_ITEM_DEF)
{
	text[0] = "zGamePad settings";
	type = MENU_ITEM_TEXT;
	posx = 0;
	posy = MENU_TITLE_Y;
	dimx = 8100;
	flags = flags & ~IT_SELECTABLE;
	flags = flags | IT_TXT_CENTER;
};



instance MENUITEM_ZGAMEPAD_ENABLED(C_MENU_ITEM_DEF)
{
	backpic = MENU_ITEM_BACK_PIC;
	text[0] = "Enabled";
	text[1] = "Toggle on/off";
	posx = 1000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 0);
	dimx = 4000;
	dimy = 750;
	onselaction[0] = SEL_ACTION_UNDEF;
	flags = flags | IT_EFFECTS_NEXT;
};

instance MENUITEM_ZGAMEPAD_ENABLED_CHOICE(C_MENU_ITEM_DEF)
{
	backpic = MENU_CHOICE_BACK_PIC;
	type = MENU_ITEM_CHOICEBOX;
	text[0] = "no|yes";
	fontname = MENU_FONT_SMALL;
	posx = 5000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 0) + MENU_CHOICE_YPLUS;
	dimx = MENU_SLIDER_DX;
	dimy = MENU_CHOICE_DY;
	onchgsetoption = "Enabled";
	onchgsetoptionsection = "ZGAMEPAD";
	flags = flags & ~IT_SELECTABLE;
	flags = flags | IT_TXT_CENTER;
};



instance MENUITEM_ZGAMEPAD_GAMEPAD_ID(C_MENU_ITEM_DEF)
{
	backpic = MENU_ITEM_BACK_PIC;
	text[0] = "Gamepad ID";
	text[1] = "ID of your connected device";
	posx = 1000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 1);
	dimx = 4000;
	dimy = 750;
	onselaction[0] = SEL_ACTION_UNDEF;
	flags = flags | IT_EFFECTS_NEXT;
};

instance MENUITEM_ZGAMEPAD_GAMEPAD_ID_CHOICE(C_MENU_ITEM_DEF)
{
	backpic = MENU_CHOICE_BACK_PIC;
	type = MENU_ITEM_CHOICEBOX;
	text[0] = "1|2|3|4";
	fontname = MENU_FONT_SMALL;
	posx = 5000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 1) + MENU_CHOICE_YPLUS;
	dimx = MENU_SLIDER_DX;
	dimy = MENU_CHOICE_DY;
	onchgsetoption = "ControllerID";
	onchgsetoptionsection = "ZGAMEPAD";
	flags = flags & ~IT_SELECTABLE;
	flags = flags | IT_TXT_CENTER;
};



instance MENUITEM_ZGAMEPAD_SENSITIVITY(C_MENU_ITEM_DEF)
{
	backpic = MENU_ITEM_BACK_PIC;
	text[0] = "Stick sensitivity";
	text[1] = "";
	posx = 1000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 2);
	dimx = 4000;
	dimy = 750;
	onselaction[0] = SEL_ACTION_UNDEF;
	flags = flags | IT_EFFECTS_NEXT;
};

instance MENUITEM_ZGAMEPAD_SENSITIVITY_SLIDER(C_MENU_ITEM_DEF)
{
	backpic = MENU_SLIDER_BACK_PIC;
	type = MENU_ITEM_SLIDER;
	text[0] = "";
	fontname = MENU_FONT_SMALL;
	posx = 5000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 2) + MENU_SLIDER_YPLUS;
	dimx = 2000;
	dimy = MENU_SLIDER_DY;
	onchgsetoption = "StickSensitivity";
	onchgsetoptionsection = "ZGAMEPAD";
	userfloat[0] = 25;
	userstring[0] = MENU_SLIDER_POS_PIC;
	flags = flags & ~IT_SELECTABLE;
	flags = flags | IT_TXT_CENTER;
};



instance MENUITEM_ZGAMEPAD_HINTS(C_MENU_ITEM_DEF)
{
	backpic = MENU_ITEM_BACK_PIC;
	text[0] = "Hints";
	text[1] = "Hints about events by gamepad buttons";
	posx = 1000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 3);
	dimx = 4000;
	dimy = 750;
	onselaction[0] = SEL_ACTION_UNDEF;
	flags = flags | IT_EFFECTS_NEXT;
};

instance MENUITEM_ZGAMEPAD_HINTS_CHOICE(C_MENU_ITEM_DEF)
{
	backpic = MENU_CHOICE_BACK_PIC;
	type = MENU_ITEM_CHOICEBOX;
	text[0] = "off|on";
	fontname = MENU_FONT_SMALL;
	posx = 5000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 3) + MENU_CHOICE_YPLUS;
	dimx = 2000;
	dimy = MENU_CHOICE_DY;
	onchgsetoption = "HintsEnabled";
	onchgsetoptionsection = "ZGAMEPAD";
	flags = flags & ~IT_SELECTABLE;
	flags = flags | IT_TXT_CENTER;
};



instance MENUITEM_ZGAMEPAD_HINTS_SCHEME(C_MENU_ITEM_DEF)
{
	backpic = MENU_ITEM_BACK_PIC;
	text[0] = "Hints scheme";
	text[1] = "Toggle gamepad icons";
	posx = 1000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 4);
	dimx = 4000;
	dimy = 750;
	onselaction[0] = SEL_ACTION_UNDEF;
	flags = flags | IT_EFFECTS_NEXT;
};

instance MENUITEM_ZGAMEPAD_HINTS_SCHEME_CHOICE(C_MENU_ITEM_DEF)
{
	backpic = MENU_CHOICE_BACK_PIC;
	type = MENU_ITEM_CHOICEBOX;
	text[0] = "XBOX|PLAYSTATION";
	fontname = MENU_FONT_SMALL;
	posx = 5000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 4) + MENU_CHOICE_YPLUS;
	dimx = 2000;
	dimy = MENU_CHOICE_DY;
	onchgsetoption = "ControllerScheme";
	onchgsetoptionsection = "ZGAMEPAD";
	flags = flags & ~IT_SELECTABLE;
	flags = flags | IT_TXT_CENTER;
};



instance MENUITEM_ZGAMEPAD_HINTS_SCALE(C_MENU_ITEM_DEF)
{
	backpic = MENU_ITEM_BACK_PIC;
	text[0] = "Hint icons scale";
	text[1] = "";
	posx = 1000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 5);
	dimx = 4000;
	dimy = 750;
	onselaction[0] = SEL_ACTION_UNDEF;
	flags = flags | IT_EFFECTS_NEXT;
};

instance MENUITEM_ZGAMEPAD_HINTS_SCALE_SLIDER(C_MENU_ITEM_DEF)
{
	backpic = MENU_SLIDER_BACK_PIC;
	type = MENU_ITEM_SLIDER;
	text[0] = "";
	fontname = MENU_FONT_SMALL;
	posx = 5000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 5) + MENU_SLIDER_YPLUS;
	dimx = 2000;
	dimy = MENU_SLIDER_DY;
	onchgsetoption = "HintsIconScale";
	onchgsetoptionsection = "ZGAMEPAD";
	userfloat[0] = 25;
	userstring[0] = MENU_SLIDER_POS_PIC;
	flags = flags & ~IT_SELECTABLE;
	flags = flags | IT_TXT_CENTER;
};



instance MENUITEM_ZGAMEPAD_HINTS_TRANSPARENCY(C_MENU_ITEM_DEF)
{
	backpic = MENU_ITEM_BACK_PIC;
	text[0] = "Hints transparency";
	text[1] = "";
	posx = 1000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 6);
	dimx = 4000;
	dimy = 750;
	onselaction[0] = SEL_ACTION_UNDEF;
	flags = flags | IT_EFFECTS_NEXT;
};

instance MENUITEM_ZGAMEPAD_HINTS_TRANSPARENCY_SLIDER(C_MENU_ITEM_DEF)
{
	backpic = MENU_SLIDER_BACK_PIC;
	type = MENU_ITEM_SLIDER;
	text[0] = "";
	fontname = MENU_FONT_SMALL;
	posx = 5000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 6) + MENU_SLIDER_YPLUS;
	dimx = 2000;
	dimy = MENU_SLIDER_DY;
	onchgsetoption = "HintsTransparency";
	onchgsetoptionsection = "ZGAMEPAD";
	userfloat[0] = 25;
	userstring[0] = MENU_SLIDER_POS_PIC;
	flags = flags & ~IT_SELECTABLE;
	flags = flags | IT_TXT_CENTER;
};





instance MENUITEM_ZGAMEPAD_OPEN_LINK(C_MENU_ITEM_DEF)
{
	backpic = MENU_ITEM_BACK_PIC;
	text[0] = "Open project page";
	posx = 1000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 7);
	dimx = 6192;
	dimy = MENU_SOUND_DY;
	onselaction[0] = SEL_ACTION_UNDEF;
  oneventaction[1] = gamepad_open_project_link;
	flags = flags | IT_TXT_CENTER;
};

func int gamepad_open_project_link()
{
	Open_Link("https://worldofplayers.ru/threads/42005/");
	return 0;
};





instance MENUITEM_ZGAMEPAD_BACK(C_MENU_ITEM_DEF)
{
	backpic = MENU_ITEM_BACK_PIC;
	text[0] = "BACK";
	posx = 1000;
	posy = MENU_START_Y + (MENU_SOUND_DY * 9);
	dimx = 6192;
	dimy = MENU_SOUND_DY;
	onselaction[0] = SEL_ACTION_BACK;
	flags = flags | IT_TXT_CENTER;
};






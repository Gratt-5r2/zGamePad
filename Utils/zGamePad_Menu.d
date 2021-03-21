META
{
  Parser = Menu;
  After = zUnionMenu.d;
};

// Important !!!
// Supporting Unicode symbols.
// File encoding: UTF-8 (without BOM).

const int ZGAMEPAD_MENU_TEXT_X       = 450;
const int ZGAMEPAD_MENU_TEXT_DX      = 8000;
const int ZGAMEPAD_MENU_TEXT_DY      = 750;
const int ZGAMEPAD_MENU_SLIDER_X     = 5500;
const int ZGAMEPAD_MENU_SLIDER_DX    = 2000;
const int ZGAMEPAD_MENU_SLIDER_DY    = 600;
const int ZGAMEPAD_MENU_SLIDER_YPLUS = 0;
const int ZGAMEPAD_MENU_CHOICE_DX    = 2000;
const int ZGAMEPAD_MENU_CHOICE_DY    = 350;
const int ZGAMEPAD_MENU_CHOICE_YPLUS = 120;
const int ZGAMEPAD_MENU_TITLE_Y      = 450;
const int ZGAMEPAD_MENU_START_Y      = 1400;
const int ZGAMEPAD_MENU_BACK_Y       = 6500;
const int ZGAMEPAD_MENU_DY           = 550;
const int ZGAMEPAD_MENU_INFO_X       = 300;
const int ZGAMEPAD_MENU_INFO_Y       = 7780;

const string ZGAMEPAD_MENU_ITEM_BACK_PIC   = "";
const string ZGAMEPAD_MENU_BACK_PIC        = "MENU_INGAME.TGA";
const string ZGAMEPAD_MENU_CHOICE_BACK_PIC = "MENU_CHOICE_BACK.TGA";
const string ZGAMEPAD_MENU_SLIDER_POS_PIC  = "MENU_SLIDER_POS.TGA";
const string ZGAMEPAD_MENU_SLIDER_BACK_PIC = "MENU_SLIDER_BACK.TGA";


instance MENUITEM_UNION_AUTO_ZGAMEPAD(C_MENU_ITEM_UNION)
{
  text[0]          = "zGamePad";
  text[1]          = "Configure gamepad";
  onselaction[0]   = SEL_ACTION_STARTMENU;
  onselaction_s[0] = "ZGAMEPAD_MENU_OPT";
};


instance ZGAMEPAD_MENU_OPT(C_MENU_DEF)
{
  backpic   = ZGAMEPAD_MENU_BACK_PIC;
  items[0]  = "ZGAMEPAD_MENUITEM_HEADLINE";
  items[1]  = "ZGAMEPAD_MENUITEM_ENABLED";
  items[2]  = "ZGAMEPAD_MENUITEM_ENABLED_CHOICE";
  items[3]  = "ZGAMEPAD_MENUITEM_GAMEPAD_ID";
  items[4]  = "ZGAMEPAD_MENUITEM_GAMEPAD_ID_CHOICE";
  items[5]  = "ZGAMEPAD_MENUITEM_SENSITIVITY";
  items[6]  = "ZGAMEPAD_MENUITEM_SENSITIVITY_SLIDER";
  items[7]  = "ZGAMEPAD_MENUITEM_HINTS";
  items[8]  = "ZGAMEPAD_MENUITEM_HINTS_CHOICE";
  items[9]  = "ZGAMEPAD_MENUITEM_HINTS_SCHEME";
  items[10] = "ZGAMEPAD_MENUITEM_HINTS_SCHEME_CHOICE";
  items[11] = "ZGAMEPAD_MENUITEM_HINTS_SCALE";
  items[12] = "ZGAMEPAD_MENUITEM_HINTS_SCALE_SLIDER";
  items[13] = "ZGAMEPAD_MENUITEM_HINTS_TRANSPARENCY";
  items[14] = "ZGAMEPAD_MENUITEM_HINTS_TRANSPARENCY_SLIDER";
  items[16] = "ZGAMEPAD_MENUITEM_OPEN_LINK";
  items[17] = "UNION_MENUITEM_BACK";
  flags     = flags | MENU_SHOW_INFO;
};


instance C_ZGAMEPAD_MENU_ITEM_TEXT_BASE(C_MENU_ITEM_DEF)
{
  backpic        = ZGAMEPAD_MENU_ITEM_BACK_PIC;
  posx           = ZGAMEPAD_MENU_TEXT_X;
  posy           = ZGAMEPAD_MENU_START_Y;
  dimx           = ZGAMEPAD_MENU_TEXT_DX;
  dimy           = ZGAMEPAD_MENU_TEXT_DY;
  flags          = flags | IT_EFFECTS_NEXT;
  onselaction[0] = SEL_ACTION_UNDEF;
};


instance C_ZGAMEPAD_MENUITEM_CHOICE_BASE(C_MENU_ITEM_DEF)
{
  backpic  = ZGAMEPAD_MENU_CHOICE_BACK_PIC;
  type     = MENU_ITEM_CHOICEBOX;
  fontname = MENU_FONT_SMALL;
  posx     = ZGAMEPAD_MENU_SLIDER_X;
  posy     = ZGAMEPAD_MENU_START_Y + ZGAMEPAD_MENU_CHOICE_YPLUS;
  dimx     = ZGAMEPAD_MENU_SLIDER_DX;
  dimy     = ZGAMEPAD_MENU_CHOICE_DY;
  flags    = flags & ~IT_SELECTABLE;
  flags    = flags | IT_TXT_CENTER;
};


instance C_ZGAMEPAD_MENUITEM_SLIDER_BASE(C_MENU_ITEM_DEF)
{
  backpic       = ZGAMEPAD_MENU_SLIDER_BACK_PIC;
  type          = MENU_ITEM_SLIDER;
  fontname      = MENU_FONT_SMALL;
  posx          = ZGAMEPAD_MENU_SLIDER_X;
  posy          = ZGAMEPAD_MENU_START_Y + ZGAMEPAD_MENU_CHOICE_YPLUS;
  dimx          = ZGAMEPAD_MENU_SLIDER_DX;
  dimy          = ZGAMEPAD_MENU_SLIDER_DY;
  flags         = flags & ~IT_SELECTABLE;
  flags         = flags | IT_TXT_CENTER;
  userstring[0] = MENU_SLIDER_POS_PIC;
};


instance ZGAMEPAD_MENUITEM_HEADLINE(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = ZGAMEPAD_MENU_TITLE_Y;
  dimx    = 8100;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = Str_GetLocalizedString(
    "ZGAMEPAD НАСТРОЙКИ",
    "ZGAMEPAD SETTINGS",
    "ZGAMEPAD EINSTELLUNGEN",
    "USTAWIENIA ZGAMEPAD"
  );
};


instance ZGAMEPAD_MENUITEM_ENABLED(C_MENU_ITEM)
{
  C_ZGAMEPAD_MENU_ITEM_TEXT_BASE();
  posy += ZGAMEPAD_MENU_DY * 0;
  
  text[0] = Str_GetLocalizedString(
    "Включен",
    "Enabled",
    "Aktiviert",
    "Włączony"
  );
};


instance ZGAMEPAD_MENUITEM_ENABLED_CHOICE(C_MENU_ITEM_DEF)
{
  C_ZGAMEPAD_MENUITEM_CHOICE_BASE();
  posy += ZGAMEPAD_MENU_DY * 0;
  
  onchgsetoption        = "Enabled";
  onchgsetoptionsection = "ZGAMEPAD";
  text[0]               = Str_GetLocalizedString(
    "нет|да",
    "no|yes",
    "Ja|Nein",
    "nie|tak"
  );
};


instance ZGAMEPAD_MENUITEM_GAMEPAD_ID(C_MENU_ITEM_DEF)
{
  C_ZGAMEPAD_MENU_ITEM_TEXT_BASE();
  posy += ZGAMEPAD_MENU_DY * 1;
  
  text[0] = Str_GetLocalizedString(
    "Номер контроллера",
    "Controller ID",
    "Controller ID",
    "ID Kontrolera"
  );
};


instance ZGAMEPAD_MENUITEM_GAMEPAD_ID_CHOICE(C_MENU_ITEM_DEF)
{
  C_ZGAMEPAD_MENUITEM_CHOICE_BASE();
  posy += ZGAMEPAD_MENU_DY * 1;
  
  onchgsetoption        = "ControllerID";
  onchgsetoptionsection = "ZGAMEPAD";
  text[0]               = "1|2|3|4";
};


instance ZGAMEPAD_MENUITEM_SENSITIVITY(C_MENU_ITEM_DEF)
{
  C_ZGAMEPAD_MENU_ITEM_TEXT_BASE();
  posy += ZGAMEPAD_MENU_DY * 2;
  
  text[0] = Str_GetLocalizedString(
    "Чувствительность",
    "Sensitivity",
    "Empfindlichkeit",
    "Czułość"
  );
};


instance ZGAMEPAD_MENUITEM_SENSITIVITY_SLIDER(C_MENU_ITEM_DEF)
{
  C_ZGAMEPAD_MENUITEM_SLIDER_BASE();
  posy += ZGAMEPAD_MENU_DY * 2;

  onchgsetoption        = "StickSensitivity";
  onchgsetoptionsection = "ZGAMEPAD";
  userfloat[0]          = 25;
};


instance ZGAMEPAD_MENUITEM_HINTS(C_MENU_ITEM_DEF)
{
  C_ZGAMEPAD_MENU_ITEM_TEXT_BASE();
  posy += ZGAMEPAD_MENU_DY * 3;

  text[0] = Str_GetLocalizedString(
    "Подсказки",
    "Hints",
    "Hinweise",
    "Podpowiedzi"
  );
};


instance ZGAMEPAD_MENUITEM_HINTS_CHOICE(C_MENU_ITEM_DEF)
{
  C_ZGAMEPAD_MENUITEM_CHOICE_BASE();
  posy += ZGAMEPAD_MENU_DY * 3;
  
  onchgsetoption        = "HintsEnabled";
  onchgsetoptionsection = "ZGAMEPAD";
  text[0]               = Str_GetLocalizedString(
    "выкл|вкл",
    "off|on",
    "Aus|Ein",
    "wyłączone|włączone"
  );
};


instance ZGAMEPAD_MENUITEM_HINTS_SCHEME(C_MENU_ITEM_DEF)
{
  C_ZGAMEPAD_MENU_ITEM_TEXT_BASE();
  posy += ZGAMEPAD_MENU_DY * 4;

  backpic = ZGAMEPAD_MENU_ITEM_BACK_PIC;
  text[0] = Str_GetLocalizedString(
    "Вид подсказок",
    "Hint icons",
    "Hinweisicons",
    "Ikony podpowiedzi"
  );
};


instance ZGAMEPAD_MENUITEM_HINTS_SCHEME_CHOICE(C_MENU_ITEM_DEF)
{
  C_ZGAMEPAD_MENUITEM_CHOICE_BASE();
  posy += ZGAMEPAD_MENU_DY * 4;
  
  onchgsetoption        = "ControllerScheme";
  onchgsetoptionsection = "ZGAMEPAD";
  text[0]               = "XBOX|PLAYSTATION";
};


instance ZGAMEPAD_MENUITEM_HINTS_SCALE(C_MENU_ITEM_DEF)
{
  C_ZGAMEPAD_MENU_ITEM_TEXT_BASE();
  posy += ZGAMEPAD_MENU_DY * 5;

  text[0] = Str_GetLocalizedString(
    "Размер подсказок",
    "Hint scale",
    "Hinweisscalierung",
    "Rozmiar podpowiedzi"
  );
};


instance ZGAMEPAD_MENUITEM_HINTS_SCALE_SLIDER(C_MENU_ITEM_DEF)
{
  C_ZGAMEPAD_MENUITEM_SLIDER_BASE();
  posy += ZGAMEPAD_MENU_DY * 5;

  onchgsetoption        = "HintsIconScale";
  onchgsetoptionsection = "ZGAMEPAD";
  userfloat[0]          = 25;
};


instance ZGAMEPAD_MENUITEM_HINTS_TRANSPARENCY(C_MENU_ITEM_DEF)
{
  C_ZGAMEPAD_MENU_ITEM_TEXT_BASE();
  posy += ZGAMEPAD_MENU_DY * 6;
  
  text[0] = Str_GetLocalizedString(
    "Прозрачность подсказок",
    "Hints transparency",
    "Hinweisdurchsichtigkeit",
    "Przeźroczystość podpowiedzi"
  );
};


instance ZGAMEPAD_MENUITEM_HINTS_TRANSPARENCY_SLIDER(C_MENU_ITEM_DEF)
{
  C_ZGAMEPAD_MENUITEM_SLIDER_BASE();
  posy += ZGAMEPAD_MENU_DY * 6;

  onchgsetoption        = "HintsTransparency";
  onchgsetoptionsection = "ZGAMEPAD";
  userfloat[0]          = 25;
};


instance ZGAMEPAD_MENUITEM_OPEN_LINK(C_MENU_ITEM_DEF)
{
  C_ZGAMEPAD_MENU_ITEM_TEXT_BASE();
  posy += ZGAMEPAD_MENU_DY * 8;

  posx             = 64;
  onselaction[0]   = SEL_ACTION_UNDEF;
  oneventaction[1] = gamepad_open_project_link;
  flags            = flags | IT_TXT_CENTER;
  text[0]          = Str_GetLocalizedString(
    "Открыть страницу проекта",
    "Open project page",
    "Projektseite öffnen",
    "Otwórz stronę projektu"
  );
};


func int gamepad_open_project_link()
{
  Open_Link("https://worldofplayers.ru/threads/42005/");
  return 0;
};













const string ZGAMEPAD_MENU_SHADOW = "SPELLBOOK_AMOUNT.TGA";


instance ZGAMEPAD_MENU_DISCONNECTED(C_MENU_DEF)
{
  backpic  = ZGAMEPAD_MENU_SHADOW;
  items[0] = "ZGAMEPAD_MENUITEM_DISCONNECTED_HEADLINE";
  items[1] = "ZGAMEPAD_MENUITEM_DISCONNECTED_MESSAGE_01";
  items[2] = "UNION_MENUITEM_DISCONNECTED_BACK";
  flags    = flags | MENU_SHOW_INFO;
};


instance ZGAMEPAD_MENUITEM_DISCONNECTED_HEADLINE(C_MENU_ITEM_DEF)
{
  backpic  = ZGAMEPAD_MENU_SHADOW;
  type     = MENU_ITEM_TEXT;
  posx     = 0;
  posy     = 3000;
  dimx     = 8100;
  flags    = flags & ~IT_SELECTABLE;
  flags    = flags | IT_TXT_CENTER;
  text[0]  = Str_GetLocalizedString(
    "Контроллер был отключен!",
    "Controller has been disconnected!",
    "Controller wurde getrennt!",
    "Kontroler został odłączony!"
  );
};


instance ZGAMEPAD_MENUITEM_DISCONNECTED_MESSAGE_01(C_MENU_ITEM_DEF)
{
  backpic  = ZGAMEPAD_MENU_SHADOW;
  type     = MENU_ITEM_TEXT;
  posx     = 0;
  posy     = 3700;
  dimx     = 8100;
  flags    = flags & ~IT_SELECTABLE;
  flags    = flags | IT_TXT_CENTER;
  fontname = MENU_FONT_SMALL;
  text[0]  = Str_GetLocalizedString(
    "Подключите контроллер и нажмите Продолжить.",
    "Connect the controller and click Continue.",
    "Schließen Sie den Controller an und klicken Sie auf Weiter.",
    "Podłącz kontroler i wciśnij Kontynuuj."
  );
};

instance UNION_MENUITEM_DISCONNECTED_BACK(C_MENU_ITEM)
{
  backpic  = ZGAMEPAD_MENU_SHADOW;
  UNION_MENUITEM_BACK();
  posy    = 5000;
  text[0] = Str_GetLocalizedString(
    "Продолжить",
    "Continue",
    "ZURÜCK",
    "WRÓĆ"
  );
};
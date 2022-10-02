META
{
  Parser = Menu;
  After = zUnionMenu.d;
};

namespace zGamePad {
  const int i_MenuTextX                  = 450;
  const int i_MenuTextWidth              = 8000;
  const int i_MenuTextHeight             = 750;
  const int i_MenuSliderX                = 5500;
  const int i_MenuSliderWidth            = 2000;
  const int i_MenuSliderHeight           = 600;
  const int i_MenuSliderDimY             = 80;
  const int i_MenuChoiceWidth            = 2000;
  const int i_MenuChoiceHeight           = 350;
  const int i_MenuChoiceDimY             = 120;
  const int i_MenuItemTitleStartY        = 450;
  const int i_MenuItemStartY             = 1400;
  const int i_MenuStepY                  = 450;
  const string s_MenuItemBackground      = "";
  const string s_MenuBackground          = "MENU_INGAME.TGA";
  const string s_MenuChoiceBackground    = "MENU_choice_BACK.TGA";
  const string s_MenuSliderPosBackground = "MENU_slider_POS.TGA";
  const string s_MenuSliderBackground    = "MENU_slider_BACK.TGA";


  instance :MenuItem_Union_Auto_zGamePad(C_MENU_ITEM_UNION_DEF) {
    text[0]          = "zGamePad";
    text[1]          = "Configure gamepad";
    onselaction[0]   = SEL_ACTION_STARTMENU;
    onselaction_s[0] = "zGamePad:Menu_zGamePad";
  };


  prototype IMenuBase(C_MENU) {
    C_MENU_DEF();
    backpic  = s_MenuBackground;
    items[0] = "";
    flags    = flags | MENU_SHOW_INFO;
  };


  prototype IMenuItemTextBase(C_MENU_ITEM) {
    C_MENU_ITEM_DEF();
    backpic        = s_MenuItemBackground;
    posx           = i_MenuTextX;
    posy           = i_MenuItemStartY;
    dimx           = i_MenuTextWidth;
    dimy           = i_MenuTextHeight;
    flags          = flags | IT_EFFECTS_NEXT;
    onselaction[0] = SEL_ACTION_UNDEF;
  };


  prototype IMenuItemChoiceBase(C_MENU_ITEM) {
    C_MENU_ITEM_DEF();
    backpic  = s_MenuChoiceBackground;
    type     = MENU_ITEM_choiceBOX;
    fontname = MENU_FONT_SMALL;
    posx     = i_MenuSliderX;
    posy     = i_MenuItemStartY + i_MenuChoiceDimY;
    dimx     = i_MenuSliderWidth;
    dimy     = i_MenuChoiceHeight;
    flags    = flags & ~IT_SELECTABLE;
    flags    = flags | IT_TXT_CENTER;
  };


  prototype IMenuItemSliderBase(C_MENU_ITEM)
  {
    C_MENU_ITEM_DEF();
    backpic       = s_MenuSliderBackground;
    type          = MENU_ITEM_slider;
    fontname      = MENU_FONT_SMALL;
    posx          = i_MenuSliderX;
    posy          = i_MenuItemStartY + i_MenuSliderDimY;
    dimx          = i_MenuSliderWidth;
    dimy          = i_MenuSliderHeight;
    flags         = flags & ~IT_SELECTABLE;
    flags         = flags | IT_TXT_CENTER;
    userstring[0] = s_MenuSliderPosBackground;
  };


  instance Menu_zGamePad(IMenuBase) {
    Menu_SearchItems( "zGamePad:MenuItem_*" );
  };


  instance MenuItem_Headline(C_MENU_ITEM_DEF) {
    type    = MENU_ITEM_TEXT;
    posx    = 0;
    posy    = i_MenuItemTitleStartY;
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


  instance MenuItem_Enabled(IMenuItemTextBase) {
    posy   += i_MenuStepY * 0;
    text[0] = Str_GetLocalizedString(
      "Включен",
      "Enabled",
      "Aktiviert",
      "Włączony"
    );
  };


  instance MenuItem_EnabledChoice(IMenuItemChoiceBase) {
    posy                 += i_MenuStepY * 0;
    onchgsetoption        = "Enabled";
    onchgsetoptionsection = "zGamePad";
    text[0]               = Str_GetLocalizedString(
      "нет|да",
      "no|yes",
      "Nein|Ja",
      "nie|tak"
    );
  };


  instance MenuItem_GamePadId(IMenuItemTextBase) {
    posy   += i_MenuStepY * 1;
    text[0] = Str_GetLocalizedString(
      "Номер контроллера",
      "Controller ID",
      "Controller ID",
      "ID Kontrolera"
    );
  };


  instance MenuItem_GamePadIdChoice(IMenuItemChoiceBase) {
    posy                 += i_MenuStepY * 1;
    onchgsetoption        = "ControllerID";
    onchgsetoptionsection = "zGamePad";
    text[0]               = "1|2|3|4";
  };


  instance MenuItem_Sensitivity(IMenuItemTextBase) {
    posy   += i_MenuStepY * 2;
    text[0] = Str_GetLocalizedString(
      "Чувствительность",
      "Sensitivity",
      "Empfindlichkeit",
      "Czułość"
    );
  };


  instance MenuItem_SensitivitySlider(IMenuItemSliderBase) {
    posy                 += i_MenuStepY * 2 - i_MenuSliderDimY;
    onchgsetoption        = "StickSensitivity";
    onchgsetoptionsection = "zGamePad";
    userfloat[0]          = 25;
  };


  instance MenuItem_Hints(IMenuItemTextBase) {
    posy   += i_MenuStepY * 3;
    text[0] = Str_GetLocalizedString(
      "Подсказки",
      "Hints",
      "Hinweise",
      "Porady"
    );
  };


  instance MenuItem_HintsChoice(IMenuItemChoiceBase) {
    posy                 += i_MenuStepY * 3;
    onchgsetoption        = "HintsEnabled";
    onchgsetoptionsection = "zGamePad";
    text[0]               = Str_GetLocalizedString(
      "Выкл|Вкл",
      "Off|On",
      "Aus|An",
      "Wył.|Wł."
    );
  };


  instance MenuItem_HintsScheme(IMenuItemTextBase) {
    posy   += i_MenuStepY * 4;
    backpic = s_MenuItemBackground;
    text[0] = Str_GetLocalizedString(
      "Вид подсказок",
      "Hint icons",
      "Hinweisicons",
      "Ikony porad"
    );
  };


  instance MenuItem_HintsSchemeChoice(IMenuItemChoiceBase) {
    posy                 += i_MenuStepY * 4;
    onchgsetoption        = "ControllerScheme";
    onchgsetoptionsection = "zGamePad";
    text[0]               = "XBOX|PLAYSTATION";
  };


  instance MenuItem_HintsScale(IMenuItemTextBase) {
    posy   += i_MenuStepY * 5;
    text[0] = Str_GetLocalizedString(
      "Размер подсказок",
      "Hint scale",
      "Hinweisscalierung",
      "Rozmiar porad"
    );
  };


  instance MenuItem_HintsScaleSlider(IMenuItemSliderBase) {
    posy                 += i_MenuStepY * 5 - i_MenuSliderDimY;
    onchgsetoption        = "HintsIconScale";
    onchgsetoptionsection = "zGamePad";
    userfloat[0]          = 25;
  };


  instance MenuItem_HintsTransparency(IMenuItemTextBase) {
    posy   += i_MenuStepY * 6;
    text[0] = Str_GetLocalizedString(
      "Прозрачность подсказок",
      "Hints transparency",
      "Hinweisdurchsichtigkeit",
      "Przeźroczystość porad"
    );
  };


  instance MenuItem_HintsTransparencySlider(IMenuItemSliderBase) {
    posy                 += i_MenuStepY * 6 - i_MenuSliderDimY;
    onchgsetoption        = "HintsTransparency";
    onchgsetoptionsection = "zGamePad";
    userfloat[0]          = 25;
  };


  instance MenuItem_QuicksaveMode(IMenuItemTextBase) {
    posy   += i_MenuStepY * 7;
    backpic = s_MenuItemBackground;
    text[0] = Str_GetLocalizedString(
      "Чередование сохранений",
      "Rotate saveslots",
      "Rotate saveslots",
      "Naprz. zapis."
    );
  };


  instance MenuItem_QuicksaveModeChoice(IMenuItemChoiceBase) {
    posy                 += i_MenuStepY * 7;
    onchgsetoption        = "QuickSaveMode";
    onchgsetoptionsection = "zGamePad";
    
    test(G2 || G2A) {
      text[0] = Str_GetLocalizedString(
        "Выкл|Все слоты|1-5|15-20",
        "Off|All slots|1-5|15-20",
        "Aus|Alle Slots|1-5|15-20",
        "Wyłączone|Wszystkie|1-5|15-20"
      );
    };
    test(G1 || G1A) {
      text[0] = Str_GetLocalizedString(
        "Выкл|Все слоты|Слоты 1-5|Слоты 10-15",
        "Off|All slots|Slots 1-5|Slots 10-15",
        "Aus|Alle Slots|Slots 1-5|Slots 10-15",
        "Wył.|Wszystkie|Sloty 1-5|Sloty 10-15"
      );
    };
  };


  instance MenuItem_Vibro(IMenuItemTextBase) {
    posy   += i_MenuStepY * 8;
    text[0] = Str_GetLocalizedString(
      "Вибрация",
      "Vibration",
      "Vibration",
      "Wibracja"
    );
  };


  instance MenuItem_VibroChoice(IMenuItemChoiceBase) {
    posy                 += i_MenuStepY * 8;
    onchgsetoption        = "Vibration";
    onchgsetoptionsection = "zGamePad";
    text[0]               = Str_GetLocalizedString(
      "Выкл|Вкл",
      "Off|On",
      "Aus|An",
      "Wył.|Wł."
    );
  };


  instance MenuItem_InvertY(IMenuItemTextBase) {
    posy   += i_MenuStepY * 9;
    text[0] = Str_GetLocalizedString(
      "Инвертировать Y",
      "Invert Y",
      "Invertieren Y",
      "Inwersja Y"
    );
  };


  instance MenuItem_InvertYChoice(IMenuItemChoiceBase) {
    posy                 += i_MenuStepY * 9;
    onchgsetoption        = "InvertY";
    onchgsetoptionsection = "zGamePad";
    text[0]               = Str_GetLocalizedString(
      "нет|да",
      "no|yes",
      "Nein|Ja",
      "nie|tak"
    );
  };


  instance MenuItem_OpenLink(IMenuItemTextBase) {
    posy            += i_MenuStepY * 10;
    posx             = 64;
    onselaction[0]   = SEL_ACTION_UNDEF;
    oneventaction[1] = Proc_OpenLink;
    flags            = flags | IT_TXT_CENTER;
    text[0]          = Str_GetLocalizedString(
      "Открыть страницу проекта",
      "Open project page",
      "Projektseite öffnen",
      "Otwórz stronę projektu"
    );
  };


  func int Proc_OpenLink() {
    test(Steam) {
      test(G2A) Open_Link("https://steamcommunity.com/sharedfiles/filedetails/?id=2793815202");
      test(G1)  Open_Link("https://steamcommunity.com/sharedfiles/filedetails/?id=2793930041");
    }
    else
      Open_Link("https://worldofplayers.ru/threads/42005/");
    return 0;
  };


  instance MenuItem_Back(C_MENU_ITEM) {
    Union_MenuItem_Back();
  };
};



namespace zGamePad_Disconnected {
  const string s_MenuBackground = "SPELLBOOK_AMOUNT.TGA";


  instance Menu_Disconnected(C_MENU_DEF) {
    backpic  = s_MenuBackground;
    items[0] = "zGamePad_Disconnected:MenuItem_DisconnectedHeadline";
    items[1] = "zGamePad_Disconnected:MenuItem_DisconnectedMessage";
    items[2] = "zGamePad_Disconnected:MenuItem_DisconnectedBack";
    flags    = flags | MENU_SHOW_INFO;
  };


  instance MenuItem_DisconnectedHeadline(C_MENU_ITEM_DEF) {
    backpic  = s_MenuBackground;
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


  instance MenuItem_DisconnectedMessage(C_MENU_ITEM_DEF) {
    backpic  = s_MenuBackground;
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


  instance MenuItem_DisconnectedBack(C_MENU_ITEM) {
    backpic  = s_MenuBackground;
    Union_MenuItem_Back();
    posy    = 5000;
    text[0] = Str_GetLocalizedString(
      "Продолжить",
      "Continue",
      "ZURÜCK",
      "WRÓĆ"
    );
  };
};
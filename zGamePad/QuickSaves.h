// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
#if ENGINE == Engine_G1
  const int& SAVEGAME_SLOT_QUICK = *(int*)0x007D1070;
  const int& SAVEGAME_SLOT_MIN = *(int*)0x007D1220;
  int SAVEGAME_SLOT_MAX = *(int*)0x007D1224;
#elif ENGINE == Engine_G1A
  const int& SAVEGAME_SLOT_QUICK = *(int*)0x008141E0;
  const int& SAVEGAME_SLOT_MIN = *(int*)0x008141E4;
  int SAVEGAME_SLOT_MAX = *(int*)0x008141E8;
#elif ENGINE == Engine_G2
  const int& SAVEGAME_SLOT_QUICK = *(int*)0x008212C8;
  const int& SAVEGAME_SLOT_MIN = *(int*)0x008212CC;
  int SAVEGAME_SLOT_MAX = *(int*)0x008212D0;
#elif ENGINE == Engine_G2A
  const int& SAVEGAME_SLOT_QUICK = *(int*)0x0082F2C8;
  const int& SAVEGAME_SLOT_MIN = *(int*)0x0082F2CC;
  int SAVEGAME_SLOT_MAX = *(int*)0x0082F2D0;
#endif


  const int SlotsCountMaxPerMode = 5;


  enum zLastSaveInfoMode {
    zLastSave_DontUse,
    zLastSave_UseAllSlots,
    zLastSave_UseFirstSlots,
    zLastSave_UseLastSlots
  };


  class zLastSaveInfo {
    int LastSavedSlotNr;
    int LastSavedSlotNr_Ranged;
    zLastSaveInfoMode Mode;

    zLastSaveInfo();
    void LoadFromFile();
    void SaveToFile();
  public:
    int GetFirstAvailableSlotNr();
    int GetLastAvailableSlotNr();
    bool IsOnRange( const int& slotNr );
    int GetLastSavedSlotNr();
    int GetLastSavedSlotNr_Ranged();
    int GetNextSaveSlotNr();
    void UpdateSaveSlotNr( const int& slotNr );
    zLastSaveInfoMode GetSaveMode();
    void SetSaveMode( const zLastSaveInfoMode& mode );
    static string GetSaveFileName();
    static zLastSaveInfo& GetInstance();
  };
}
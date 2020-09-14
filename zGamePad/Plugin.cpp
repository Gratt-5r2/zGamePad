// This file added in headers queue
// File: "Sources.h"

namespace GOTHIC_ENGINE {
  void Game_Entry() {
  }

  void Game_Init() {
  }

  void Game_Exit() {
  }

  inline zSTRING GetInfoName() {
    switch( oCInformationManager::GetInformationManager().Mode ) {
      case oCInformationManager::INFO_MGR_MODE_IMPORTANT: return "INFO_MGR_MODE_IMPORTANT";
      case oCInformationManager::INFO_MGR_MODE_INFO:      return "INFO_MGR_MODE_INFO";
      case oCInformationManager::INFO_MGR_MODE_CHOICE:    return "INFO_MGR_MODE_CHOICE";
      case oCInformationManager::INFO_MGR_MODE_TRADE:     return "INFO_MGR_MODE_TRADE";
    }
   
    return "...";
  }

  void Game_Loop() {
    static bool InterfaceInitialized_GetMousePos = false;
    if( !InterfaceInitialized_GetMousePos ) {
      uint address = ZenDef( 0x004C8BD0, 0x004D8CF0, 0x004D3170, 0x004D5730 );
      Hook_GetMousePos.Attach( address, &zCInput_Win32::GetMousePos_Union );
      InterfaceInitialized_GetMousePos = true;
#if ENGINE >= Engine_G2
      // oCGame::s_bUsePotionKeys = True;
#endif
    }

    DrawTarget();

    if( zKeyToggled( KEY_F14 ) )
      GetNextLeftEnemy();

    if( zKeyToggled( KEY_F15 ) )
      GetNextRightEnemy();

#if ENGINE <= Engine_G1A
    if( zKeyToggled( KEY_F13 ) && player->GetFocusNpc() )
      oCNpc::s_bTargetLocked = !oCNpc::s_bTargetLocked;
#endif
  }



  // Information about current saving or loading world
  TSaveLoadGameInfo& SaveLoadGameInfo = UnionCore::SaveLoadGameInfo;

  void Game_SaveBegin() {
  }

  void Game_SaveEnd() {
  }

  void LoadBegin() {
  }


  void LoadEnd() {
  }

  void Game_LoadBegin_NewGame() {
    LoadBegin();
  }

  void Game_LoadEnd_NewGame() {
    LoadEnd();
  }

  void Game_LoadBegin_SaveGame() {
    LoadBegin();
  }

  void Game_LoadEnd_SaveGame() {
    LoadEnd();
  }

  void Game_LoadBegin_ChangeLevel() {
    LoadBegin();
  }

  void Game_LoadEnd_ChangeLevel() {
    LoadEnd();
  }

  void Game_LoadBegin_Trigger() {
  }
  
  void Game_LoadEnd_Trigger() {
  }
  
  void Game_Pause() {
  }
  
  void Game_Unpause() {
  }
  
  void Game_DefineExternals() {
  }

  /*
  Functions call order on Game initialization:
    - Game_Entry           * Gothic entry point
    - Game_DefineExternals * Define external script functions
    - Game_Init            * After DAT files init
  
  Functions call order on Change level:
    - Game_LoadBegin_Trigger     * Entry in trigger
    - Game_LoadEnd_Trigger       *
    - Game_Loop                  * Frame call window
    - Game_LoadBegin_ChangeLevel * Load begin
    - Game_SaveBegin             * Save previous level information
    - Game_SaveEnd               *
    - Game_LoadEnd_ChangeLevel   *
  
  Functions call order on Save game:
    - Game_Pause     * Open menu
    - Game_Unpause   * Click on save
    - Game_Loop      * Frame call window
    - Game_SaveBegin * Save begin
    - Game_SaveEnd   *
  
  Functions call order on Load game:
    - Game_Pause              * Open menu
    - Game_Unpause            * Click on load
    - Game_LoadBegin_SaveGame * Load begin
    - Game_LoadEnd_SaveGame   *
  */

#define AppDefault True
  CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
    Enabled( AppDefault ) Game_Entry,
    Enabled( AppDefault ) Game_Init,
    Enabled( AppDefault ) Game_Exit,
    Enabled( AppDefault ) Game_Loop,
    Enabled( AppDefault ) Game_SaveBegin,
    Enabled( AppDefault ) Game_SaveEnd,
    Enabled( AppDefault ) Game_LoadBegin_NewGame,
    Enabled( AppDefault ) Game_LoadEnd_NewGame,
    Enabled( AppDefault ) Game_LoadBegin_SaveGame,
    Enabled( AppDefault ) Game_LoadEnd_SaveGame,
    Enabled( AppDefault ) Game_LoadBegin_ChangeLevel,
    Enabled( AppDefault ) Game_LoadEnd_ChangeLevel,
    Enabled( AppDefault ) Game_LoadBegin_Trigger,
    Enabled( AppDefault ) Game_LoadEnd_Trigger,
    Enabled( AppDefault ) Game_Pause,
    Enabled( AppDefault ) Game_Unpause,
    Enabled( AppDefault ) Game_DefineExternals
  );
}
// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  void zCXInputDevice::InitCombinations() {
    if( ParseControlFile() )
      return;

    // 
    cmd << "No controls file . . ." << endl;
  }
}
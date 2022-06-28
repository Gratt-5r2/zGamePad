// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  static bool InitializeControlsFileArrays( Array<string>& controls, Array<string>& overlays ) {
    if( !CHECK_THIS_ENGINE )
      return false;

    char** physicalFiles    = Null;
    char** virsualFiles     = Null;
    long physicalFilesCount = vdf_filelist_physical( physicalFiles );
    long virtualFilesCount  = vdf_filelist_virtual( virsualFiles );

    string fileNameTmp;
    if( physicalFilesCount ) {
      for( long i = 0; i < physicalFilesCount; i++ ) {
        fileNameTmp = physicalFiles[i];
             if( fileNameTmp.EndWith( ".GAMEPAD.OVERLAY" ) ) overlays |= fileNameTmp;
        else if( fileNameTmp.EndWith( ".GAMEPAD" ) )         controls |= fileNameTmp;
        delete[] physicalFiles[i];
      }
      delete[] physicalFiles;
    }

    if( virtualFilesCount ) {
      for( long i = 0; i < virtualFilesCount; i++ ) {
        fileNameTmp = virsualFiles[i];
             if( fileNameTmp.EndWith( ".GAMEPAD.OVERLAY" ) ) overlays |= fileNameTmp;
        else if( fileNameTmp.EndWith( ".GAMEPAD" ) )         controls |= fileNameTmp;
        delete[] virsualFiles[i];
      }
      delete[] virsualFiles;
    }

    controls.QuickSort();
    overlays.QuickSort();
    return physicalFilesCount > 0 && virtualFilesCount > 0;
  }


  static Array<string> Controls, Overlays;
  static bool ControlFilesFound = InitializeControlsFileArrays( Controls, Overlays );


  void zCXInputDevice::InitCombinations() {
    if( !ControlFilesFound ) {
      cmd << "No controls files . . ." << endl;
      return;
    }

    if( Opt_ControlsFile.IsEmpty() )
      Opt_ControlsFile = "Controls.Gamepad";

    if( !ParseControlFile( Opt_ControlsFile ) ) {
      cmd << "No controls files . . ." << endl;
      return;
    }

    for( uint i = 0; i < Overlays.GetNum(); i++ )
      ParseControlFile( Overlays[i] );
  }
}
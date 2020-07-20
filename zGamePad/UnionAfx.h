// Supported with union (c) 2018 Union team

#ifndef __UNION_AFX_H__
#define __UNION_AFX_H__

// 1.0f+ disabled as default
#define UNION_NO_WARNINGS
#include ".build_settings.inl"



extern "C" {
  #include <Windows.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <math.h>
  #include <Psapi.h>
  #include <time.h>
}



#pragma comment (lib, "Psapi.lib")
#pragma comment (lib, "Shw32.lib")
#pragma comment (lib, "Vdfs32g.lib")



#define SHWAPI __declspec (dllimport)
#define ASTAPI __declspec (dllimport)
#define VDFAPI __declspec (dllimport)



// memory operations
#include "Memory\Shw32.h"



// base classes of Union: arrays, strings, options and etc
#include "Common\Common.h"
using namespace Common;



// functional classes of union: hooks, applications, plugins . . .
#include "Core\Core.h"
using namespace UnionCore;



// operations with virtual file system
#include "Vdfs\Vdfs.h"
using namespace Vdfs32;



// systempack patch classes
#include "SystemPack\Union.Patch\CPatchFile.h"
using namespace SystemPack;



// gothic engine classes for g1, g1a, g2 and g2a
#include "ZenGin\zGothicAPI.h"



#endif // __UNION_AFX_H__
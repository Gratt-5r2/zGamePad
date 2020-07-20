// Supported with union (c) 2018 Union team

#ifndef __ZGOTHIC_API_H__
#define __ZGOTHIC_API_H__

#pragma warning(disable:4244)
// gothic api for Gothic I v1.08k_mod
#include "Gothic_I_Classic/API/zEngine.h"
#ifdef __G1
#include "Gothic_I_Classic/VFTable_G1.h"
#endif


// gothic api for Gothic Sequel v1.12f (bin)
#include "Gothic_I_Addon/API/zEngine.h"
#ifdef __G1A
#include "Gothic_I_Addon/VFTable_G1A.h"
#endif


// gothic api for Gothic II classic v1.30
#include "Gothic_II_Classic/API/zEngine.h"
#ifdef __G2
#include "Gothic_II_Classic/VFTable_G2.h"
#endif


// gothic api for Gothic II NOTR v2.6 (fix)
#include "Gothic_II_Addon/API/zEngine.h"
#ifdef __G2A
#include "Gothic_II_Addon/VFTable_G2A.h"
#endif
#pragma warning(default:4244)


#endif // __ZGOTHIC_API_H__
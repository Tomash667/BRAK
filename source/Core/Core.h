#pragma once

//-----------------------------------------------------------------------------
// typedefs
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int uint;
typedef __int64 int64;
typedef unsigned __int64 uint64;
typedef const char* cstring;
#ifdef X64
typedef __int64 IntPointer;
typedef unsigned __int64 UIntPointer;
#else
typedef int IntPointer;
typedef unsigned int UIntPointer;
#endif

//-----------------------------------------------------------------------------
#include "CoreMath.h"
#include "Text.h"

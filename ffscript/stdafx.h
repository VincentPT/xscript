// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers



// TODO: reference additional headers your program requires here
#define INT2STRING(x) #x
#define INT2STRING_(x) INT2STRING(x)
#define S__LINE__ INT2STRING_(__LINE__)

#define LOG_MESSAGE(logType , str)  logType "[" __FUNCTION__ "][" S__LINE__ "]" + (str)

#define LOG_I(str)
#define LOG_E(str)
#define LOG_D(str)

#if defined(_M_AMD64) || defined(_M_X64) || defined(_WIN64)
	#define POINTER2STRING(p) std::to_string((unsigned long long) (p))
#else
	#define POINTER2STRING(p) std::to_string((unsigned int) (p))
#endif

/******************************************************************
* File:        CachedDelegate.cpp
* Description: implement abstract class of function object that invoke
*              a target function. This function object design to store
*              all necessary arguments of its target function when
*              it is created. The caller just to call 'call' only
*              to invoke the target function.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "CachedDelegate.h"

CachedDelegate::CachedDelegate()
{
}


CachedDelegate::~CachedDelegate()
{
}

void* CachedDelegate::getReturnValAsVoidPointer() const {
	return *((void**)_ret);
}

__int64 CachedDelegate::getReturnValAsInt64() const {
	return *(__int64*)_ret;
}

__int32 CachedDelegate::getReturnValAsInt32() const {
	return *(__int32*)_ret;
}

__int16 CachedDelegate::getReturnValAsInt16() const {
	return *(__int16*)_ret;
}

__int8 CachedDelegate::getReturnValAsInt8() const {
	return *(__int8*)_ret;
}

float CachedDelegate::getReturnValAsFloat() const {
	return *(float*)_ret;
}

double CachedDelegate::getReturnValAsDouble() const {
	return *(double*)_ret;
}

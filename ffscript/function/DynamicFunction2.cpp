/******************************************************************
* File:        DynamicFunction2.cpp
* Description: implement abstract class of function object that invoke
*              a target function. This function object design to not
*              store any argument of its target function when
*              it is created. The caller need to pass arguments
*              through an array of void* to invoke the target function.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "DynamicFunction2.h"
#include <cstdarg>

DFunction2::DFunction2() : 
	mFx(nullptr)
{
}


DFunction2::~DFunction2()
{
}
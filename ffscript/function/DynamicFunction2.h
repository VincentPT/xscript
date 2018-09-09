/******************************************************************
* File:        DynamicFunction2.h
* Description: declare abstract class of function object that invoke
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

#pragma once

//the library won't offer bind function, but it reduce calling cost
#define USE_EXTERNAL_PARAMS_ONLY

class DFunction2
{
protected:
	void* mFx;
#ifndef USE_EXTERNAL_PARAMS_ONLY
	int mFixedParamCount = 0;
#endif
public:
	DFunction2();
	virtual ~DFunction2();
	virtual void call(void* pReturnVal, void* param[]) = 0;
	virtual DFunction2* clone() = 0;
};


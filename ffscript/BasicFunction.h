/******************************************************************
* File:        BasicFunction.h
* Description: declare some basic expression function unit factory
*              classes.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ScriptCompiler.h"
#include "FunctionRegisterHelper.h"
#include "FunctionFactory.h"

namespace ffscript {
	///////////////////////////////////////////////
	void importBasicfunction(FunctionRegisterHelper& functionLib);
	void importCoreFunctions(FunctionRegisterHelper& functionLib);
}
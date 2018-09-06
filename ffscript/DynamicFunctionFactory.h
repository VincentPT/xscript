/******************************************************************
* File:        DynamicFunctionFactory.h
* Description: declare DynamicFunctionFactory class.  A factory class
*              that create dynamic parameter function unit from DFunction2
*              function object.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"
#include "FunctionFactory.h"

class DFunction2;

namespace ffscript {
	class ScriptCompiler;
	class DynamicFunctionFactory :
		public FunctionFactory
	{
		DFunction2Ref _nativeFunction;
	public:
		DynamicFunctionFactory(const std::string& returnType, DFunction2* nativeFunction, ScriptCompiler*);
		virtual ~DynamicFunctionFactory();
		Function* createFunction(const std::string& name, int id);
	};
}

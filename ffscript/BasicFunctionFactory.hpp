/******************************************************************
* File:        BasicFunctionFactory.hpp
* Description: define BasicFunctionFactory template class. A factory class
*              that create fixed parameter function unit from DFunction2
*              function object.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "expressionunit.h"
#include "function/CdeclFunction2.hpp"
#include "FunctionFactory.h"
#include "ffscript.h"

namespace ffscript {
	template <int paramSize>
	class BasicFunctionFactory : public FunctionFactory
	{
		UNIT_TYPE _functionType;
		int _priority;
		const char* _returnType;
		DFunction2Ref _nativeFunction;

	public:
		BasicFunctionFactory(UNIT_TYPE functionType, int priority, const char* returnType, DFunction2* nativeFunction, ScriptCompiler* scriptCompiler) :
			FunctionFactory(nullptr, scriptCompiler),
			_functionType(functionType),
			_priority(priority),
			_returnType(returnType),
			_nativeFunction(nativeFunction)
		{
			this->setReturnType(ScriptType::parseType(scriptCompiler, _returnType));
		}

		virtual ~BasicFunctionFactory() {
		}

		Function* createFunction(const std::string& name, int id) {
			NativeFunction* function = new FixParamFunction<paramSize>(name, _functionType, _priority, _returnType);
			function->setNative(_nativeFunction);
			return function;
		}
	};
}
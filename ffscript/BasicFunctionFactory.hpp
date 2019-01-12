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
#include "function/FunctionDelegate3.hpp"
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

	class FunctionFactoryCdecl : public FunctionFactory
	{
		typedef Function* (*FCreator) (const std::string&, int);

		FCreator _fCreator;

	public:
		FunctionFactoryCdecl(FCreator creator, const ScriptType& returnType);
		virtual ~FunctionFactoryCdecl();
		Function* createFunction(const std::string& name, int id);
	};

	class UserFunctionFactory : public FunctionFactory
	{
		int _paramSize;
	public:
		UserFunctionFactory(ScriptCompiler* scriptCompiler, const std::string& returnType, int paramSize);
		virtual ~UserFunctionFactory();
		Function* createFunction(const std::string& name, int id);
		virtual DFunction2Ref createNative() = 0;
	};


	class DefaultUserFunctionFactory : public UserFunctionFactory
	{
		DFunction2Ref _nativeFunction;
	public:
		DefaultUserFunctionFactory(DFunction2Ref nativeFunction, ScriptCompiler* scriptCompiler, const std::string& returnType, int paramSize);
		virtual ~DefaultUserFunctionFactory();
		DFunction2Ref createNative();
	};
}
#pragma once
#include "ffscript.h"
#include "FunctionFactory.h"

class DFunction2;

namespace ffscript {
	class ScriptCompiler;
	class DynamicFunctionFactory :
		public FunctionFactory
	{
		const char* _returnType;
		DFunction2Ref _nativeFunction;
	public:
		DynamicFunctionFactory(const char* returnType, DFunction2* nativeFunction, ScriptCompiler*);
		virtual ~DynamicFunctionFactory();
		Function* createFunction(const std::string& name, int id);
	};
}

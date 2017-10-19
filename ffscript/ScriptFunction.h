#pragma once
#include "expressionunit.h"
#include "FunctionFactory.h"
#include <vector>

namespace ffscript {

	class ScriptCompiler;

	class ScriptFunction : public Function
	{
	protected:
		std::vector<ExecutableUnitRef> params;
		int _registeredParamCount;
	public:
		ScriptFunction(const std::string& name, const ScriptType& returnType, int registeredParamCount);
		virtual ~ScriptFunction();

		virtual int pushParam(ExecutableUnitRef pExeUnit);
		virtual ExecutableUnitRef popParam();
		virtual const ExecutableUnitRef& getChild(int index) const;
		virtual ExecutableUnitRef& getChild(int index);
		virtual int getChildCount();
	};

	class ScriptFunctionFactory : public FunctionFactory {
	public:
		ScriptFunctionFactory(const char* functionName, ScriptCompiler* scriptCompiler);
		virtual ~ScriptFunctionFactory();
		Function* createFunction(const std::string& name, int id);
	};
}

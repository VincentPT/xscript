#pragma once
#include "expressionunit.h"
#include "FunctionFactory.h"

namespace ffscript {
	class ScriptCompiler;

	class RefFunction :
		public Function
	{
		ExecutableUnitRef _value;
	public:
		RefFunction();
		virtual ~RefFunction();
		void setValueOfVariable(ExecutableUnitRef pExeUnit);
		ExecutableUnitRef& getValueOfVariable();
	public:
		virtual int pushParam(ExecutableUnitRef pExeUnit);
		virtual ExecutableUnitRef popParam();
		virtual const ExecutableUnitRef& getChild(int index) const;
		virtual ExecutableUnitRef& getChild(int index);
		virtual int getChildCount();
		virtual void* Execute();
	};

	class MakeRefFactory : public FunctionFactory {
	public:
		MakeRefFactory(ScriptCompiler* scriptCompiler);
		virtual ~MakeRefFactory();
		virtual Function* createFunction(const std::string& name, int id);
		virtual void addParamType(const ScriptType& argType);
	};

	class DeRefFactory : public FunctionFactory {
	public:
		DeRefFactory(ScriptCompiler* scriptCompiler);
		virtual ~DeRefFactory();
		virtual Function* createFunction(const std::string& name, int id);
		virtual void addParamType(const ScriptType& argType);
	};
}
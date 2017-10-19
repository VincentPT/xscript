#pragma once
#include "expressionunit.h"
#include "FunctionFactory.h"
#include "ScriptType.h"

namespace ffscript {
	class ScriptCompiler;

	//class RefFunction :
	//	public Function
	//{
	//	ExecutableUnitRef _value;
	//public:
	//	RefFunction();
	//	virtual ~RefFunction();
	//	void setValueOfVariable(ExecutableUnitRef pExeUnit);
	//	ExecutableUnitRef& getValueOfVariable();
	//public:
	//	virtual int pushParam(ExecutableUnitRef pExeUnit);
	//	virtual ExecutableUnitRef popParam();
	//	virtual const ExecutableUnitRef& getChild(int index) const;
	//	virtual ExecutableUnitRef& getChild(int index);
	//	virtual int getChildCount();
	//	virtual void* Execute();
	//};

	class ConditionalOperatorFactory : public FunctionFactory {
	public:
		ConditionalOperatorFactory(ScriptCompiler* scriptCompiler);
		virtual ~ConditionalOperatorFactory();
		virtual Function* createFunction(const std::string& name, int id);
		virtual void addParamType(const ScriptType& paramType);
	};
}
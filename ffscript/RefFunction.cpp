#include "stdafx.h"
#include "RefFunction.h"
#include "ScriptCompiler.h"

namespace ffscript {
	RefFunction::RefFunction() : Function(MAKING_REF_FUNC, EXP_UNIT_ID_MAKE_REF, FUNCTION_PRIORITY_UNARY_PREFIX, "TBD")
	{
	}

	RefFunction::~RefFunction()
	{
	}

	void RefFunction::setValueOfVariable(ExecutableUnitRef pExeUnit) {
		_value = pExeUnit;
	}

	ExecutableUnitRef& RefFunction::getValueOfVariable() {
		return _value;
	}

	int RefFunction::pushParam(ExecutableUnitRef pExeUnit) {
		//if (ISOPERAND(pExeUnit)) {
			if (_value.get() == nullptr) {
				_value = pExeUnit;
				if(!_returnType.isSemiRefType())
					this->setReturnType(pExeUnit->getReturnType().makeRef());
				return 0;
			}
		//}
		return -1;
	}

	ExecutableUnitRef RefFunction::popParam() {
		return _value;
	}

	const ExecutableUnitRef& RefFunction::getChild(int index) const {
		if (index == 0) {
			return _value;
		}
		static ExecutableUnitRef dummy(nullptr);
		return dummy;
	}

	ExecutableUnitRef& RefFunction::getChild(int index) {
		if (index == 0) {
			return _value;
		}
		static ExecutableUnitRef dummy(nullptr);
		return dummy;
	}

	int RefFunction::getChildCount() {
		if (_value) {
			return 1;
		}
		return 0;
	}

	void* RefFunction::Execute() {
		return nullptr;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	MakeRefFactory::MakeRefFactory(ScriptCompiler* scriptCompiler) : FunctionFactory(MAKING_REF_FUNC, scriptCompiler) {}
	MakeRefFactory::~MakeRefFactory() {}

	Function* MakeRefFactory::createFunction(const std::string& name, int id) {
		return new RefFunction();
	}

	void MakeRefFactory::addParamType(const ScriptType& argType) {
		FunctionFactory::addParamType(argType);
		this->setReturnType(argType.makeRef());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	DeRefFactory::DeRefFactory(ScriptCompiler* scriptCompiler) : FunctionFactory(DEREF_OPERATOR, scriptCompiler) {}
	DeRefFactory::~DeRefFactory() {}

	Function* DeRefFactory::createFunction(const std::string& name, int id) {
		return new FixParamFunction<1>(DEREF_OPERATOR, EXP_UNIT_ID_DEREF, FUNCTION_PRIORITY_UNARY_PREFIX, getReturnType());
	}

	void DeRefFactory::addParamType(const ScriptType& argType) {
		FunctionFactory::addParamType(argType);
		this->setReturnType(argType.deRef());
	}
}
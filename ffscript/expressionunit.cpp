/******************************************************************
* File:        expressionunit.cpp
* Description: implement expression unit classes. Each class is a
*              expression unit kind and play a role in compiling
*              progress.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "../Logger/remotelogger.h"
#include "ScriptScope.h"
#include "expressionunit.h"

#define INVALID_CHAR_INDEX 0xFFFF

namespace ffscript {
	ExpUnit::ExpUnit() : _indexInExpression(-1), _sourceCharIndex(INVALID_CHAR_INDEX) {
		LOG_D("Create expression unit " + POINTER2STRING(this));
	}
	ExpUnit::~ExpUnit() {
		LOG_D("Delete expression unit " + POINTER2STRING(this));
	}

	void ExpUnit::setIndex(int idx) {
		_indexInExpression = (short)idx;
	}

	int ExpUnit::getIndex() const {
		return (int)_indexInExpression;
	}

	void ExpUnit::setSourceCharIndex(int idx) {
		if (idx < 0) {
			_sourceCharIndex = INVALID_CHAR_INDEX;
		}
		else {
			_sourceCharIndex = (unsigned short)idx;
		}
	}

	int ExpUnit::getSourceCharIndex() const {
		if (_sourceCharIndex == INVALID_CHAR_INDEX) return -1;
		return (int)_sourceCharIndex;
	}

	////////////////////////////////////////////////////////
	OpenCurlyBracket::OpenCurlyBracket() {}
	OpenCurlyBracket::~OpenCurlyBracket() {}
	const std::string& OpenCurlyBracket::toString() const {
		return OpenCurlyBracket::getFuncName();
	}
	UNIT_TYPE OpenCurlyBracket::getType() const {
		return EXP_UNIT_ID_OPEN_CURLY_BRACKET;
	}
	const std::string& OpenCurlyBracket::getFuncName() {
		static std::string name = "{";
		return name;
	}

	////////////////////////////////////////////////////////
	ClosedCurlyBracket::ClosedCurlyBracket() {}
	ClosedCurlyBracket::~ClosedCurlyBracket() {}
	const std::string& ClosedCurlyBracket::toString() const {
		return ClosedCurlyBracket::getFuncName();
	}
	UNIT_TYPE ClosedCurlyBracket::getType() const {
		return EXP_UNIT_ID_CLOSED_CURLY_BRACKET;
	}
	const std::string& ClosedCurlyBracket::getFuncName() {
		static std::string name = "}";
		return name;
	}

	////////////////////////////////////////////////////////
	OpenRoundBracket::OpenRoundBracket() {}
	OpenRoundBracket::~OpenRoundBracket() {}
	const std::string& OpenRoundBracket::toString() const {
		return OpenRoundBracket::getFuncName();
	}
	UNIT_TYPE OpenRoundBracket::getType() const {
		return EXP_UNIT_ID_OPEN_ROUND_BRACKET;
	}
	const std::string& OpenRoundBracket::getFuncName() {
		static std::string name = "(";
		return name;
	}

	////////////////////////////////////////////////////////
	ClosedRoundBracket::ClosedRoundBracket() {}
	ClosedRoundBracket::~ClosedRoundBracket() {}
	const std::string& ClosedRoundBracket::toString() const {
		return ClosedRoundBracket::getFuncName();
	}
	UNIT_TYPE ClosedRoundBracket::getType() const {
		return EXP_UNIT_ID_CLOSED_ROUND_BRACKET;
	}
	const std::string& ClosedRoundBracket::getFuncName() {
		static std::string name = ")";
		return name;
	}

	////////////////////////////////////////////////////////
	ParamSeperator::ParamSeperator() {}
	ParamSeperator::~ParamSeperator() {}
	const std::string& ParamSeperator::toString() const {
		return ParamSeperator::getFuncName();
	}
	UNIT_TYPE ParamSeperator::getType() const {
		return EXP_UNIT_ID_FUNC_SEPERATOR;
	}
	const std::string& ParamSeperator::getFuncName() {
		static std::string name = ",";
		return name;
	}
	////////////////////////////////////////////////////////
	OpenSquareBracket::OpenSquareBracket() {}
	OpenSquareBracket::~OpenSquareBracket() {}
	const std::string& OpenSquareBracket::toString() const {
		return OpenSquareBracket::getFuncName();
	}
	UNIT_TYPE OpenSquareBracket::getType() const {
		return EXP_UNIT_ID_OPEN_SQUARE_BRACKET;
	}
	const std::string& OpenSquareBracket::getFuncName() {
		static std::string name = "[";
		return name;
	}
	////////////////////////////////////////////////////////
	ClosedSquareBracket::ClosedSquareBracket() {}
	ClosedSquareBracket::~ClosedSquareBracket() {}
	const std::string& ClosedSquareBracket::toString() const {
		return ClosedSquareBracket::getFuncName();
	}
	UNIT_TYPE ClosedSquareBracket::getType() const {
		return EXP_UNIT_ID_CLOSED_SQUARE_BRACKET;
	}
	const std::string& ClosedSquareBracket::getFuncName() {
		static std::string name = "]";
		return name;
	}
	////////////////////////////////////////////////////////
	ExecutableUnit::ExecutableUnit(const std::string& returnType) : ExecutableUnit(returnType, DATA_TYPE_UNKNOWN) {
	}

	ExecutableUnit::ExecutableUnit(const std::string& returnType, const int returnTypeInt) : _returnType(returnTypeInt, returnType) {
	}

	ExecutableUnit::ExecutableUnit(const ScriptType& returnType) : _returnType(returnType) {
	}

	ExecutableUnit::~ExecutableUnit() {}

	const ScriptType& ExecutableUnit::getReturnType()const {
		return _returnType;
	}

	ScriptType& ExecutableUnit::getReturnType() {
		return _returnType;
	}

	void ExecutableUnit::setReturnType(const ScriptType& type) {
		_returnType = type;
	}
	///////////////////////////////////////////////////////
	ConstOperandBase::ConstOperandBase(const std::string& valueType) : ExecutableUnit(valueType){}
	ConstOperandBase::ConstOperandBase(const ScriptType& valueType) : ExecutableUnit(valueType) {}
	ConstOperandBase::~ConstOperandBase() {}
	const std::string& ConstOperandBase::toString() const { return _valueInString; }

	UNIT_TYPE ConstOperandBase::getType() const {
		return EXP_UNIT_ID_CONST;
	}

	///////////////////////////////////////////////////////
	IncompletedUserFunctionUnit::IncompletedUserFunctionUnit(const std::string& functionName) : ConstOperandBase("string") {
		_valueInString = functionName;
	}

	IncompletedUserFunctionUnit::~IncompletedUserFunctionUnit() {}
	
	UNIT_TYPE IncompletedUserFunctionUnit::getType() const {
		return EXP_UNIT_ID_INCOMPFUNC;
	}

	int IncompletedUserFunctionUnit::getDataSize() const {
		return 0;
	}

	void* IncompletedUserFunctionUnit::Execute() {
		return nullptr;
	}

	////////////////////////////////////////////////////////
	CXOperand::CXOperand(ScriptScope* scope, Variable* variable) : CXOperand(scope, variable, AUTO_VARIABLE_TYPE)
	{}

	CXOperand::CXOperand(ScriptScope* scope, Variable* variable, const std::string& valueType) :
		_variable(variable),
		_currentScope(scope),
		ExecutableUnit(valueType)
	{}

	CXOperand::CXOperand(ScriptScope* scope, Variable* variable, const ScriptType& valueType) :
		_variable(variable),
		_currentScope(scope),
		ExecutableUnit(valueType)
	{}

	CXOperand::~CXOperand() {}
	const std::string& CXOperand::toString()const {
		return _variable->getName();
	}
	void* CXOperand::Execute() {
		return nullptr; //_currentScope->getAbsoluteAddress(_variable->getOffset());
	}
	UNIT_TYPE CXOperand::getType()const {
		return EXP_UNIT_ID_XOPERAND;
	}

	ScriptScope* CXOperand::getScope() const {
		return _currentScope;
	}

	void CXOperand::setVariable(Variable* pVariable) {
		_variable = pVariable;
	}

	Variable* CXOperand::getVariable() const {
		return _variable;
	}

	ExecutableUnit* CXOperand::clone() {
		ExecutableUnit* newInstance = nullptr;
		if (_variable) {
			newInstance = new CXOperand(getScope(), _variable->clone());
		}
		else {
			newInstance = new CXOperand(getScope(), nullptr);
		}
		newInstance->setSourceCharIndex(getSourceCharIndex());
		return newInstance;
	}
	////////////////////////////////////////////////////////
	Function::Function(const std::string& name, unsigned int functionType, int iPriority, const std::string& returnType) :
		_name(name),
		_functionType(functionType),
		_priority(iPriority),
		ExecutableUnit(returnType),
		_paramSize(0)
	{}

	Function::Function(const std::string& name, unsigned int functionType, int iPriority, const ScriptType& returnType) :
		_name(name),
		_functionType(functionType),
		_priority(iPriority),
		ExecutableUnit(returnType),
		_paramSize(0)
	{}
	Function::~Function() {
	}

	int Function::getPriority() {
		return _priority;
	}

	const std::string& Function::toString()const {
		return _name;
	}

	const std::string& Function::getName()const {
		return _name;
	}

	bool Function::IsOrdered(Function* pFunc) {
		int priority1 = this->getPriority();
		int priority2 = pFunc->getPriority();
		if (priority1 < priority2)
		{
			return true;
		}
		if (priority1 == priority2 && (FUNCTION_ODER_RIGHT_2_LEFT & priority1))
		{
			return true;
		}
		return false;
	}

	void* Function::Execute() {
		return storedData;
	}

	UNIT_TYPE Function::getType()const {
		return _functionType;
	}		
	
	int Function::getId()const {
		return _functionId;
	}
	void Function::setId(int functionId) {
		_functionId = functionId;
	}

	int Function::getParamsDataSize() const {
		return _paramSize;
	}

	void Function::setParamsDataSize (int paramSize) {
		_paramSize = paramSize;
	}

	////////////////////////////////////////////////////////
	NativeFunction::NativeFunction(const std::string& name, unsigned int functionType, int iPriority, const std::string& returnType) : Function(name, functionType, iPriority, returnType)
	{}
	NativeFunction::NativeFunction(const std::string& name, unsigned int functionType, int iPriority, const ScriptType& returnType) : Function(name, functionType, iPriority, returnType)
	{}
	NativeFunction::~NativeFunction() {
	}

	const DFunction2Ref& NativeFunction::getNative()const {
		return _nativeFunction;
	}
	void NativeFunction::setNative(const DFunction2Ref& nativeFunction) {
		_nativeFunction = nativeFunction;
	}

	////////////////////////////////////////////////////////
	DynamicParamFunction::DynamicParamFunction(const std::string& name, unsigned int functionType, int iPriority, const std::string& returnType) : _maxParam(-1),
		NativeFunction(name, functionType, iPriority, returnType){
	}

	DynamicParamFunction::DynamicParamFunction(const std::string& name, unsigned int functionType, int iPriority, const ScriptType& returnType) : _maxParam(-1),
		NativeFunction(name, functionType, iPriority, returnType) {
	}

	DynamicParamFunction::DynamicParamFunction(const std::string& name, unsigned int functionType, int iPriority, int maxParam) : _maxParam(maxParam),
		NativeFunction(name, functionType, iPriority, "TBD") {
	}

	DynamicParamFunction::~DynamicParamFunction() {}

	int DynamicParamFunction::pushParam(ExecutableUnitRef pExeUnit) {
		if (_maxParam != -1 && (int)_params.size() >= _maxParam) return -1;
		_params.push_back(pExeUnit);

		if (_maxParam <= -1)
			return 1;

		return _maxParam - (int) _params.size();
	}
	ExecutableUnitRef DynamicParamFunction::popParam() {
		if (_params.size() == 0) {
			return nullptr;
		}
		ExecutableUnitRef top = _params.back();
		//if (top) {
			_params.pop_back();
		//}
		return top;
	}

	
	std::list<ExecutableUnitRef>& DynamicParamFunction::getParams() {
		return _params;
	}

	const std::list<ExecutableUnitRef>& DynamicParamFunction::getParams() const {
		return _params;
	}

	const ExecutableUnitRef& DynamicParamFunction::getChild(int index) const {
		auto param = _params.begin();
		auto end = _params.end();
		for (int i = 0; param != end; i++, param++) {
			if (i == index) {
				return *param;
			}
		}

		return _params.back();
	}

	int DynamicParamFunction::pushParamFront(ExecutableUnitRef pExeUnit) {
		if (_maxParam != -1 && (int)_params.size() >= _maxParam) return -1;
		_params.push_front(pExeUnit);

		if (_maxParam <= -1)
			return 1;

		return _maxParam - (int)_params.size();
	}

	ExecutableUnitRef DynamicParamFunction::popParamFront() {
		if (_params.size() == 0) {
			return nullptr;
		}
		ExecutableUnitRef top = _params.front();
		if (top) {
			_params.pop_front();
		}
		return top;
	}

	ExecutableUnitRef& DynamicParamFunction::getChild(int index) {
		auto param = _params.begin();
		auto end = _params.end();
		for (int i = 0; param != end; i++, param++) {
			if (i == index) {
				return *param;
			}
		}

		return _params.back();
	}
	int DynamicParamFunction::getChildCount() {
		return (int)_params.size();
	}

	ExecutableUnitRef& DynamicParamFunction::getFirstParam() {
		_currentPos = _params.begin();
		return *_currentPos;
	}

	ExecutableUnitRef& DynamicParamFunction::getNextParam() {
		return *_currentPos++;
	}

	int DynamicParamFunction::getMaxParam() const {
		return _maxParam;
	}

	void DynamicParamFunction::setMaxParam(int maxParam) {
		_maxParam = maxParam;
	}

	////////////////////////////////////////////////////////
	ParamUnitCollection::ParamUnitCollection() : DynamicParamFunction("_dummy_param_collection", EXP_UNIT_ID_COLLECTION, FUNCTION_PRIORITY_LOWEST, "void") {}
	ParamUnitCollection::~ParamUnitCollection() {}
}
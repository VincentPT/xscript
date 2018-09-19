/******************************************************************
* File:        ScriptCompiler.cpp
* Description: implement ScriptCompiler class. A class play a role in
*              script compiling progress.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "ScriptCompiler.h"
#include "FactoryTree.h"
#include "FunctionFactory.h"
#include "Internal.h"
#include "RefFunction.h"
#include "DefaultCommands.h"
#include <algorithm>
#include "ScriptType.h"
#include "Utils.h"
#include "Utility.hpp"
#include "FwdCompositeConstrutorUnit.h"
#include "CompositeConstrutorUnit.h"
#include "FunctionRegisterHelper.h"
#include <stdarg.h>

#define TYPE_CONVERSION_MAKE_KEY(source, target)  (((uint64_t)(source) << 32) | target)

namespace ffscript {
	std::string key_while("while");
	std::string key_for("for");
	std::string key_if("if");
	std::string key_else("else");
	std::string key_elseIf("else if");
	std::string key_return("return");
	std::string key_break("break");
	std::string key_continue("continue");
	std::string key_pointer(POINTER_SIGN);
	std::string key_array(ARRAY_SIGN);

	ScriptCompiler::ScriptCompiler() : _program(nullptr), _logger(nullptr)
	{
		_functionLibRef = (FuncLibraryRef)(new FuncLibrary());
		_typeManagerRef = (TypeManagerRef)(new TypeManager());

		_keywordMap.insert(std::make_pair(key_if, KEYWORD_IF));
		_keywordMap.insert(std::make_pair(key_else, KEYWORD_ELSE));
		_keywordMap.insert(std::make_pair(key_while, KEYWORD_WHILE));
		_keywordMap.insert(std::make_pair(key_for, KEYWORD_FOR));
		_keywordMap.insert(std::make_pair(key_return, KEYWORD_RETURN));
		_keywordMap.insert(std::make_pair(key_break, KEYWORD_BREAK));
		_keywordMap.insert(std::make_pair(key_continue, KEYWORD_CONTINUE));

		//pre-defined operators for compile only
		static OperatorEntry preCompileOperators[] = {
			{ "+", nullptr, EXP_UNIT_ID_OPERATOR_ADD, FUNCTION_PRIORITY_ADDITIVE, 2 },
			{ "-", nullptr, EXP_UNIT_ID_OPERATOR_SUB, FUNCTION_PRIORITY_ADDITIVE, 2 },
			{ "neg", "-", EXP_UNIT_ID_OPERATOR_NEG, FUNCTION_PRIORITY_UNARY_PREFIX, 1 },
			{ "*", nullptr, EXP_UNIT_ID_OPERATOR_MUL, FUNCTION_PRIORITY_MULTIPLICATIVE, 2 },
			{ "/", nullptr, EXP_UNIT_ID_OPERATOR_DEV, FUNCTION_PRIORITY_MULTIPLICATIVE, 2 },
			{ "%", nullptr, EXP_UNIT_ID_OPERATOR_MOD, FUNCTION_PRIORITY_MODULAR, 2 },
			{ "=", nullptr, EXP_UNIT_ID_OPERATOR_ASSIGNMENT, FUNCTION_PRIORITY_ASSIGNMENT, 2 },
			{ "&", nullptr, EXP_UNIT_ID_BITWISE_AND, FUNCTION_PRIORITY_BITWISE_AND, 2 },
			{ MAKING_SEMI_REF_FUNC, "&", EXP_UNIT_ID_SEMI_REF, FUNCTION_PRIORITY_UNARY_PREFIX, 1 },
			{ "|", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_BITWISE_OR, 2 },
			{ "^", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_BITWISE_XOR, 2 },
			{ ">>", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_SHIFT, 2 },
			{ "<<", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_SHIFT, 2 },
			{ "==", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_EQUALITY, 2 },
			{ "!=", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_EQUALITY, 2 },
			{ ">", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_RELATIONAL, 2 },
			{ "<", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_RELATIONAL, 2 },
			{ ">=", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_RELATIONAL, 2 },
			{ "<=", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_RELATIONAL, 2 },
			{ "&&", nullptr, EXP_UNIT_ID_OPERATOR_LOGIC_AND, FUNCTION_PRIORITY_LOGICAL_AND, 2 },
			{ "||", nullptr, EXP_UNIT_ID_OPERATOR_LOGIC_OR, FUNCTION_PRIORITY_LOGICAL_OR, 2 },
			{ "!", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_UNARY_PREFIX, 1 },
			{ "~", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_UNARY_PREFIX, 1 },
			{ "post_fix_increase", nullptr, EXP_UNIT_ID_OPERATOR_POSTFIX_INC, FUNCTION_PRIORITY_POSTFIX, 1 },
			{ "++", nullptr, EXP_UNIT_ID_OPERATOR_PREFIX_INC, FUNCTION_PRIORITY_PRETFIX, 1 },
			{ "post_fix_decrease", nullptr, EXP_UNIT_ID_OPERATOR_POSTFIX_DEC, FUNCTION_PRIORITY_POSTFIX, 1 },
			{ "--", nullptr, EXP_UNIT_ID_OPERATOR_PREFIX_DEC, FUNCTION_PRIORITY_PRETFIX, 1 },

			{ "+=", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_ASSIGNMENT, 2 },
			{ "-=", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_ASSIGNMENT, 2 },
			{ "*=", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_ASSIGNMENT, 2 },
			{ "/=", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_ASSIGNMENT, 2 },
			{ "%=", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_ASSIGNMENT, 2 },
			{ "&=", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_ASSIGNMENT, 2 },
			{ "|=", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_ASSIGNMENT, 2 },
			{ "^=", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_ASSIGNMENT, 2 },
			{ ">>=", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_ASSIGNMENT, 2 },
			{ "<<=", nullptr, EXP_UNIT_ID_USER_OPER, FUNCTION_PRIORITY_ASSIGNMENT, 2 },

			{ "?", CONDITIONAL_FUNCTION, EXP_UNIT_ID_FUNC_CONDITIONAL, FUNCTION_PRIORITY_CONDITIONAL, 3 },
			{ ":", nullptr, EXP_UNIT_ID_FUNC_CHOICE, FUNCTION_PRIORITY_CHOICE, 1 },

			{ MAKING_REF_FUNC,			nullptr, EXP_UNIT_ID_MAKE_REF,					FUNCTION_PRIORITY_UNARY_PREFIX,	1 },
			{ DEFAULT_COPY_OPERATOR,	nullptr, EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR,	FUNCTION_PRIORITY_ASSIGNMENT,		2 },
		};
		int n = sizeof(preCompileOperators) / sizeof(preCompileOperators[0]);
		OperatorEntry* first = &preCompileOperators[0];
		OperatorEntry* end = preCompileOperators + n;
		while (first < end)
		{
			_preCompileOperators.insert(std::make_pair(first->name, first));
			first++;
		}

		////register make reference function for all types
		//MakeRefFactory* makeRef = new MakeRefFactory();
		//_factoriesStorage.push_back(FunctionFactoryRef(makeRef));

		//int functionId = (int)_functionFactories.size();
		//makeRef->setFunctionId(functionId);
		//_functionFactories.push_back(makeRef);

		//list<OverLoadingItem> overloadingTemp;
		//list<OverLoadingItem>* pOverloadingFuncs;
		//auto it = _functionNameFactoryMap.insert(std::make_pair(makeRef->getName(), overloadingTemp));
		//pOverloadingFuncs = &(it.first->second);

		//OverLoadingItem factoryItem;
		//factoryItem.functionId = functionId;
		//factoryItem.paramCount = 1;

		//pOverloadingFuncs->push_back(factoryItem);

		_messageBuffer.resize(256);
	}

	ScriptCompiler::~ScriptCompiler()
	{
	}

	void ScriptCompiler::pushScope(ScriptScope* scope) {
		_scopeStack.push(scope);
	}

	ScriptScope* ScriptCompiler::popScope() {
		if (_scopeStack.size() == 0) return nullptr;
		ScriptScope* scope = _scopeStack.top();
		_scopeStack.pop();
		return scope;
	}

	ScriptScope* ScriptCompiler::currentScope() const {
		if (_scopeStack.size() == 0) return nullptr;
		return _scopeStack.top();
	}

	int ScriptCompiler::registType(const std::string& type, int mask) {
		int iRet = _typeManagerRef->registType(type, mask);
		if (IS_UNKNOWN_TYPE(iRet)) {
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("cannot register type '%s'", type.c_str()));
		}

		return iRet;
	}

	std::string ScriptCompiler::getType(int typeId) const {
		return _typeManagerRef->getType(typeId);
	}

	bool ScriptCompiler::isRefSign(const std::string& token) const {
		return token == key_pointer;
	}

	int ScriptCompiler::getMakingRefFunction() {
		return this->findFunction(MAKING_REF_FUNC, "void");
	}

	int ScriptCompiler::registConditionalFunction(FunctionFactory* factory) {
		return registFunction(CONDITIONAL_FUNCTION, "", factory);
	}

	int ScriptCompiler::getConditionalFunction() {
		return this->findFunction(CONDITIONAL_FUNCTION, "");
	}

	void ScriptCompiler::setRuntimeInfoConstructor(int functionId) {
		_functionInfoConstructorId = functionId;
	}
	int ScriptCompiler::getRuntimeInfoConstructor() const {
		return _functionInfoConstructorId;
	}

	void ScriptCompiler::setRuntimeInfoDestructor(int functionId) {
		_functionInfoDestructorId = functionId;
	}
	int ScriptCompiler::getRuntimeInfoDestructor() const {
		return _functionInfoDestructorId;
	}

	int ScriptCompiler::registDefaultCopyFunction(FunctionFactory* factory) {
		return registFunction(DEFAULT_COPY_OPERATOR, "ref void, void", factory);
	}

	void ScriptCompiler::setRuntimeInfoCopyConstructor(int functionId) {
		_functionInfoCopyConstructorId = functionId;
	}
	int ScriptCompiler::getRuntimeInfoCopyConstructor() const {
		return _functionInfoCopyConstructorId;
	}

	int ScriptCompiler::getDefaultCopyFunction() {		
		return this->findFunction(DEFAULT_COPY_OPERATOR, "ref void, void");
	}

	int ScriptCompiler::getTypeSize(int typeId) const {
		return _typeManagerRef->getTypeSize(typeId);
	}

	int ScriptCompiler::getTypeSizeInStack(int typeId) const {
		return _typeManagerRef->getTypeSizeInStack(typeId);
	}

	int ScriptCompiler::getTypeSize(const ScriptType& sType) const {
		return getTypeSize(sType.iType());
	}

	void ScriptCompiler::setTypeSize(int typeId, int size) {
		if (size > MAX_DATA_SIZE || size < 0) {
			throw std::runtime_error("array size reach maximum of data size 64kb");
		}
		_typeManagerRef->setTypeSize(typeId, size);
	}

	int ScriptCompiler::getType(const std::string& type) const {
		return _typeManagerRef->getType(type);
	}

	int ScriptCompiler::registStruct(StructClass* pStruct) {
		int ret = _typeManagerRef->registStruct(pStruct);
		if (IS_UNKNOWN_TYPE(ret)) {
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("register struct %s failed", pStruct->getName().c_str()));
		}

		return ret;
	}

	const StructClass* ScriptCompiler::getStruct(int type) {
		return _typeManagerRef->getStruct(type);
	}

	ExpUnit* ScriptCompiler::createExpUnitFromName(const std::string& name) {
		auto res = _functionLibRef->findOverloadingFuncRoot(name);
		if (res == nullptr || res->size() == 0) {
			auto functionId = _functionLibRef->findDynamicFunctionOnly(name);
			if (functionId < 0) {
				return nullptr;
			}
			//with dynamic function, we can dertermine return type of function base on function name only.
			//because dynamic function does not allow overloading
			FunctionFactory* factory = getFunctionFactory(functionId);
			/*int iReturnType = factory->getReturnType();
			string sReturnType = this->getType(iReturnType);*/

			return factory->createFunction(name, functionId);// new DynamicParamFunction(name, EXP_UNIT_ID_DYNAMIC_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, sReturnType);
		}
		const char* ss = name.c_str();
		if (name.length() == 1 && (*ss == '*' || *ss == '/' || *ss == '%')) {
			//here is some pre-defined operators, we already know max param count for them is two
			//other pre-defined operators is specified in pre-defined operators table.
			return new DynamicParamFunction(name, EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, 2);
		}		
		//almost function, we cannot know return type if we base only on function name, because of overloading.
		//so, we need leave it as TBD and we will determine it when the code is linked
		return new DynamicParamFunction(name, EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "TBD");
	}

	Function* ScriptCompiler::createFunctionFromId(int functionId) {
		FunctionFactory* factory = _functionFactories[functionId];
		auto pFunc = factory->build(factory->getName());
		if (pFunc->getReturnType().isUnkownType()) {
			pFunc->getReturnType().updateType(this);
		}
		return pFunc;
	}

	const list<OverLoadingItem>* ScriptCompiler::findOverloadingFuncRoot(const std::string& name) {
		return _functionLibRef->findOverloadingFuncRoot(name);
	}

	int ScriptCompiler::findFunction(const std::string& name, const std::string& sargs) {
		return _functionLibRef->findFunction(this, name, sargs);
	}

	int ScriptCompiler::findDynamicFunctionOnly(const std::string& name) {
		return _functionLibRef->findDynamicFunctionOnly(name);
	}

	int ScriptCompiler::findFunction(const std::string& name, const std::vector<ScriptType>& paramTypes) {
		return _functionLibRef->findFunction(this, name, paramTypes);
	}

	int ScriptCompiler::registFunction(const std::string& name, const std::vector<ScriptType>& paramTypes, FunctionFactory* factory) {
		int functionId = (int)_functionFactories.size();
		if (_functionLibRef->mapFunction(name, paramTypes, functionId) == false) {
			auto functionSign = buildFunctionSign(name, paramTypes);
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("function signature '%s' is already registered", functionSign.c_str()));
			return -1;
		}
		for (auto type : paramTypes) {
			factory->addParamType(type);
		}

		factory->setFunctionId(functionId);
		factory->setName(name.c_str());
		_functionFactories.push_back(factory);
		return functionId;
	}

	bool parseArgumentTypes(ScriptCompiler* scriptCompiler, const std::string& sargs, vector<ScriptType>& args) {
		std::wstring wsargs(sargs.begin(), sargs.end());
		auto begin = wsargs.c_str();
		auto end = begin + wsargs.size();
		for (auto c = begin; c < end; c++) {
			ScriptType scriptType;

			c = scriptCompiler->readType(c, end, scriptType);
			if (scriptType.isUnkownType()) {
				return false;
			}
			args.push_back(scriptType);
			c = trimLeft(c, end);
		}
		return true;
	}

	int ScriptCompiler::registFunction(const std::string& name, const std::string& sagrs, FunctionFactory* factory) {
		factory->setCompiler(this);
		
		vector<ScriptType> args;
		if (!parseArgumentTypes(this, sagrs, args)) {
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("invalid arguments of function: %s(%s)", name.c_str(), sagrs.c_str()));
			return -1;
		}
		
		return registFunction(name, args, factory);
	}

	void ScriptCompiler::unregisterFunction(int functionId) {
		FunctionFactory* functionFactory = _functionFactories[functionId];
		if (functionFactory == nullptr) {
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("cannot find function id: %d", functionId));
			return;
		}
		_functionFactories[functionId] = nullptr;
		_functionLibRef->unmapFunction(functionFactory->getName(), functionId);

		for (auto it = _constructorMap.begin(); it != _constructorMap.end();) {
			if (it->second == functionId) {
				auto tit = it;
				it++;
				_constructorMap.erase(tit);				
				continue;
			}
			it++;
		}

		for (auto it = _destructorMap.begin(); it != _destructorMap.end();) {
			if (it->second == functionId) {
				auto tit = it;
				it++;
				_destructorMap.erase(tit);				
				continue;
			}
			it++;
		}
	}

	int ScriptCompiler::registDynamicFunction(const std::string& name, FunctionFactory* factory) {
		int functionId = (int)_functionFactories.size();
		if (_functionLibRef->mapDynamicFunction(name, functionId) == false) {
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("cannot register function: %s(...)", name.c_str()));
			return -1;
		}

		factory->setFunctionId(functionId);
		factory->setName(name.c_str());
		_functionFactories.push_back(factory);

		return functionId;
	}

	bool ScriptCompiler::registerTypeConversionAccurative(int sourceType, int targetType, int accurative){
		auto key = TYPE_CONVERSION_MAKE_KEY(sourceType, targetType);
		auto it = _typeConversionMap.insert(std::make_pair(key, accurative));
		return it.second;
	}

	int ScriptCompiler::findConversionAccurative(int sourceType, int targetType) {
		auto key = TYPE_CONVERSION_MAKE_KEY(sourceType, targetType);
		auto it = _typeConversionMap.find(key);
		if (it != _typeConversionMap.end()) {
			return it->second;
		}
		LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("cannot find accuration of conversion: %s -> %s", getType(sourceType).c_str(), getType(targetType).c_str()));
		return -1;
	}

	FunctionFactory* ScriptCompiler::getFunctionFactory(int functionId) const {
		if (functionId < 0 || functionId >= (int)_functionFactories.size()) {
			return nullptr;
		}
		return _functionFactories[functionId];
	}

	void ScriptCompiler::setErrorText(const std::string& errorMsg) {
		_lastError = errorMsg;
	}

	const std::string& ScriptCompiler::getLastError() const {
		return _lastError;
	}

	void ScriptCompiler::setLogger(CompilationLogger* logger) {
		_logger = logger;
	}

	CompilationLogger* ScriptCompiler::getLogger() const {
		return _logger;
	}

	EKeyword ScriptCompiler::findKeyword(const std::string& keyword) const {
		auto it = _keywordMap.find(keyword);
		if (it != _keywordMap.end()) {
			return it->second;
		}
		return KEYWORD_UNKNOWN;
	}

	const OperatorEntry* ScriptCompiler::findPredefinedOperator(const std::string& keyword) const {
		auto it = _preCompileOperators.find(keyword);
		if (it != _preCompileOperators.end()) {
			return it->second;
		}
		return nullptr;
	}

	bool ScriptCompiler::registConstructor(int type, int functionId) {
		auto functionFactory = getFunctionFactory(functionId);
		if (functionFactory == nullptr) {
			this->setErrorText("operator is not found");
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("operator id is not found: %d", functionId));
			return false;
		}
		if (functionFactory->getParamCount() < 1) {
			this->setErrorText("operator is specified to an incompatible function");
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("function '%s()' is not applicable for a constructor", functionFactory->getName()));
			return false;
		}

		ScriptType& paramTpe = functionFactory->getParamType(0);
		if (paramTpe.refLevel() != 1) {
			this->setErrorText("operator is specified to an incompatible function");
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("function '%s' is not applicable for a constructor, the first param must be a pointer", functionFactory->getFullFuntionName().c_str()));
			return false;
		}

		if (functionFactory->getParamCount() == 1) {
			return registDefaultConstructor(type, functionId);
		}
		if (functionFactory->getParamCount() == 2) {
			return registBinaryConstructor(type, functionId);
		}

		ConstructorIDListRef dummy;
		auto it = _constructorsMap.insert( make_pair(type, dummy) );
		if (it.second) {
			it.first->second = make_shared<ConstructorIDList>();
			it.first->second->push_back(functionId);

			return true;
		}

		// check if there is same constructor prototype is registered
		auto desireOverloadingItem = _functionLibRef->findFunctionInfo(functionId);
		auto desireParamTypes = desireOverloadingItem->paramTypes;
		auto& constructorListRef = it.first->second;
		for (auto it = constructorListRef->begin(); it != constructorListRef->end(); it++) {
			auto pRegisteredItem = _functionLibRef->findFunctionInfo(*it);
			auto& registeredParamTypes = pRegisteredItem->paramTypes;

			if (desireParamTypes.size() == registeredParamTypes.size()) {
				auto ptjt = registeredParamTypes.begin();
				for (auto ptit = desireParamTypes.begin(); ptit != desireParamTypes.end(); ptit++, ptjt++) {
					if (ptit != ptjt) {
						break;
					}
				}
				if (ptjt == registeredParamTypes.end()) {
					return false;
				}
			}
		}

		constructorListRef->push_back(functionId);
		return true;
	}

	bool ScriptCompiler::registDefaultConstructor(int type, int functionId) {
		auto it = _constructorMap.insert(std::make_pair(type, functionId));
		if (it.second == false) {
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("the default constructor for type '%s' is already defined", getType(type).c_str()));
			return false;
		}

		return true;
	}

	bool ScriptCompiler::registBinaryConstructor(int type, int functionId) {

		// check data type of argument #2
		auto factory = getFunctionFactory(functionId);
		auto& secondArgType = factory->getParamType(1);
		//if (secondArgType.origin() == secondArgType.iType() && secondArgType.iType() == type) {
		//	// data type of second argument must be ref type if it same type of constructor object
		//	LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("function '%s' is not applicable for a constructor, the first param must be a pointer", functionFactory->getFullFuntionName().c_str()));
		//	return false;
		//}

		auto functionFactory = getFunctionFactory(functionId);
		ScriptType& paramType2 = functionFactory->getParamType(1);

		auto key = type;

		BinaryFunctionParamMapRef emptyId;
		BinaryFunctionParamMap* registFunctions;

		auto it = _copyConstructorMap.insert(std::make_pair(key, emptyId));
		if (it.second == false) {
			auto& newList = it.first->second;
			registFunctions = newList.get();
		}
		else
		{
			auto& newMap = it.first->second;
			newMap.reset(new BinaryFunctionParamMap());

			registFunctions = newMap.get();
		}

		string subKey = paramType2.sType();
		registFunctions->operator[](subKey) = functionId;

		return true;
	}

	int ScriptCompiler::getDefaultConstructor(int type) {
		auto it = _constructorMap.find(type);
		if (it != _constructorMap.end()) {
			return it->second;
		}

		return -1;
	}

	bool ScriptCompiler::registDestructor(int type, int functionId) {
		auto functionFactory = getFunctionFactory(functionId);
		if (functionFactory == nullptr) {
			this->setErrorText("function is not found");
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, L"function is not found");
			return false;
		}
		if (functionFactory->getParamCount() < 1) {
			this->setErrorText("operator is specified to an incompatible function");
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("destructor for type '%s' is specified to an incompatible function '%s'", getType(type).c_str(), functionFactory->getFullFuntionName().c_str()));
			return false;
		}

		ScriptType& paramTpe = functionFactory->getParamType(0);
		if (paramTpe.refLevel() != 1) {
			this->setErrorText("operator is specified to an incompatible function");
			return false;
		}

		//overwrite if the constructor is already exist
		auto it = _destructorMap.insert(std::make_pair(type, functionId));
		if (it.second == false) {
			it.first->second = functionId;
		}

		return true;
	}

	int ScriptCompiler::getDestructor(int type) {
		auto it = _destructorMap.find(type);
		if (it != _destructorMap.end()) {
			return it->second;
		}

		return -1;
	}

	int ScriptCompiler::getBinaryConstructor(int rootType, const ScriptType& paramType) {
		auto key = rootType;
		const ScriptType& param2Type = paramType;
		string subKey = param2Type.sType();

		auto it = _copyConstructorMap.find(key);
		if (it == _copyConstructorMap.end()) {
			return -1;
		}

		BinaryFunctionParamMap* registFunctions = it->second.get();
		auto it2 = registFunctions->find(subKey);

		//find exactly match constructor
		if (it2 != registFunctions->end()) {			
			return it2->second;
		}

		return -1;
	}

	std::shared_ptr<list<CandidateInfo>> ScriptCompiler::getBinaryConstructor(const ScriptType& type, const ExecutableUnitRef& paramUnit, bool findExtracly) {
		auto key = type;
		const ScriptType& param2Type = paramUnit->getReturnType();
		string subKey = param2Type.sType();

		std::shared_ptr<list<CandidateInfo>> candidates;

		auto it = _copyConstructorMap.find(key.iType());
		if (it == _copyConstructorMap.end()) {
			return candidates;
		}

		BinaryFunctionParamMap* registFunctions = it->second.get();
		auto it2 = registFunctions->find(subKey);

		//find exactly match constructor
		if (it2 != registFunctions->end()) {
			auto overLoadingItem = _functionLibRef->findFunctionInfo(it2->second);
			CandidateInfo constructorCandidate;

			ParamCastingInfo emptyCasingInfo;
			emptyCasingInfo.accurative = 0;
			emptyCasingInfo.castingFunction = nullptr;

			constructorCandidate.item = overLoadingItem;
			constructorCandidate.paramCasting.push_back(emptyCasingInfo);
			constructorCandidate.totalAccurative = 0;

			candidates = make_shared<list<CandidateInfo>>();
			candidates->push_back(constructorCandidate);

			return candidates;
		}
		candidates = make_shared<list<CandidateInfo>>();

		ScriptType refVoidType(_typeManagerRef->getBasicTypes().TYPE_VOID | DATA_TYPE_POINTER_MASK, "ref void");
		for (auto it = registFunctions->begin(); it != registFunctions->end(); it++) {
			auto overLoadingItem = _functionLibRef->findFunctionInfo(it->second);
			auto& expectedType = *(overLoadingItem->paramTypes[1].get());

			ParamCastingInfo castingInfo;
			if (!findMatchingComposite(expectedType, paramUnit, castingInfo)) {
				if (!findMatching(refVoidType, expectedType, param2Type, castingInfo, true)) {
					continue;
				}
				else if (castingInfo.castingFunction) {
					castingInfo.castingFunction->setSourceCharIndex(paramUnit->getSourceCharIndex());
				}
			}

			CandidateInfo constructorCandidate;
			constructorCandidate.item = overLoadingItem;
			constructorCandidate.totalAccurative = castingInfo.accurative;
			constructorCandidate.paramCasting.push_back(castingInfo);

			candidates->push_back(constructorCandidate);
		}

		return candidates;
	}

	std::shared_ptr<list<CandidateInfo>> ScriptCompiler::getConstructor(const ScriptType& type, const ExecutableUnitRef& unit) {
		if (unit->getType() != EXP_UNIT_ID_DYNAMIC_FUNC) {
			return getBinaryConstructor(type, unit);
		}

		auto collector = dynamic_cast<DynamicParamFunction*>(unit.get());
		if (!collector) {
			// throw exception here
		}
		auto& params = collector->getParams();
		list<OverLoadingItem*> overloadingItems;
		
		auto it = _constructorsMap.find(type.iType());
		if (it != _constructorsMap.end()) {
			auto constructorList = it->second.get();
			if (constructorList == nullptr || constructorList->size() == 0) return nullptr;

			for (auto it = constructorList->begin(); it != constructorList->end(); it++) {
				auto pItem = _functionLibRef->findFunctionInfo(*it);
				if (pItem) {
					overloadingItems.push_back((OverLoadingItem*)pItem);
				}
			}
		}

		std::shared_ptr<list<CandidateInfo>> candidates = selectMultiCandidates(overloadingItems, params);
		return candidates;
	}

	void ScriptCompiler::getConstructors(int iType, list<OverLoadingItem*>& overloadingItems) const {
		auto it = _constructorsMap.find(iType);
		if (it != _constructorsMap.end()) {
			auto constructorList = it->second.get();
			if (constructorList == nullptr || constructorList->size() == 0) return;

			for (auto it = constructorList->begin(); it != constructorList->end(); it++) {
				auto pItem = _functionLibRef->findFunctionInfo(*it);
				if (pItem) {
					overloadingItems.push_back((OverLoadingItem*)pItem);
				}
			}
		}
		
		auto jt = _constructorMap.find(iType);
		if (jt != _constructorMap.end()) {
			auto pItem = _functionLibRef->findFunctionInfo(jt->second);
			if (pItem) {
				overloadingItems.push_back((OverLoadingItem*)pItem);
			}
		}

		auto kt = _copyConstructorMap.find(iType);
		if (kt != _copyConstructorMap.end()) {
			auto& copyConstrucrorOverloadings = kt->second;
			for (auto lt = copyConstrucrorOverloadings->begin(); lt != copyConstrucrorOverloadings->end(); lt++) {
				auto pItem = _functionLibRef->findFunctionInfo(lt->second);
				if (pItem) {
					overloadingItems.push_back((OverLoadingItem*)pItem);
				}				
			}

		}
	}

	bool inline canMakeSemiRef(const ScriptType& argType, const ScriptType& paramType) {
		return argType.isSemiRefType() && !paramType.isSemiRefType()/* && argType.refLevel() == paramType.refLevel()*/ && argType.origin() == paramType.origin();
	}

	bool ScriptCompiler::findMatchingLevel1(const ScriptType& refVoidType, const ScriptType& argumentType, const ScriptType& paramType, ParamCastingInfo& paramInfo) {
		//same type => perfect match
		if (argumentType == paramType) {
			paramInfo.accurative = 0; //best match
			paramInfo.castingFunction = nullptr; //no need to casting
			return true;
		}
		if (argumentType.origin() == paramType.origin() && paramType.isSemiRefType() &&
			argumentType.refLevel() == 1 && paramType.refLevel() == 0) {
			paramInfo.accurative = 0; //best match
			paramInfo.castingFunction = nullptr; //no need to casting
			return true;
		}

		if (argumentType == refVoidType && paramType.isRefType()) {
			paramInfo.accurative = 2; //change a specific ref type to general ref type(Ex in c++: int* -> void*
			paramInfo.castingFunction = nullptr; //no need to casting
			return true;
		}

		//not same origin type
		if (argumentType.origin() != paramType.origin()) {
			if (argumentType.isRefType() && (paramType.iType() & DATA_TYPE_ARRAY_MASK)) {
				auto arrayInfo = (StaticArrayInfo*)getTypeInfo(paramType.origin());
				ScriptType elmType(arrayInfo->elmType, "");
				if (arrayInfo && elmType.origin() == argumentType.origin() && argumentType.refLevel() - (int)arrayInfo->refLevel == 1) {
					if (paramType.isSemiRefType()) {
						paramInfo.accurative = 2; //change a specific ref type of static array to its element's ref type(Ex: array<int>& -> ref int)
						paramInfo.castingFunction = nullptr; //no need to casting
					}
					else {
						int functionId = getMakingRefFunction();
						if (functionId == -1) {
							setErrorText("Internal error: ref operator is not defined");
							return false;
						}
						paramInfo.castingFunction = FunctionRef(createFunctionFromId(functionId));
						paramInfo.castingFunction->setReturnType(argumentType);
						paramInfo.accurative = 2;
					}
				}
				return true;
			}
			return false;
		}
		if (paramType.refLevel() != argumentType.refLevel()) {
			// not allow ref level is different
			return false;
		}
		// now two types have the same origin, check if they are not same ref level
		//if (paramType.refLevel() - argumentType.refLevel() >= 1) {
		//	//param type is deeper level of ref than argument type
		//	return false;
		//}

		//if (argumentType.refLevel() - paramType.refLevel() == 1) {
		//	//argument type is ref type but param type is not
		//	//we can make reference to the param type
		//	//so this case is acceptable
		//	int functionId = getMakingRefFunction();
		//	if (functionId == -1) {
		//		setErrorText("Internal error: ref operator is not defined");
		//		return false;
		//	}
		//	paramInfo.castingFunction = FunctionRef(createFunctionFromId(functionId));
		//	paramInfo.castingFunction->setReturnType(argumentType);
		//	paramInfo.accurative = 2;

		//	return true;
		//}
		if (canMakeSemiRef(argumentType, paramType)) {
			int functionId = getMakingRefFunction();
			if (functionId == -1) {
				setErrorText("Internal error: ref operator is not defined");
				return false;
			}
			paramInfo.castingFunction = FunctionRef(createFunctionFromId(functionId));
			paramInfo.castingFunction->setReturnType(argumentType);
			paramInfo.accurative = 2;

			return true;
		}
		if (canMakeSemiRef(paramType, argumentType)) {

			auto semiRefLevel = paramType.semiRefLevel();

			DFunction2* derefCommand;
			if (semiRefLevel > 1)
			{
				derefCommand = new DeRefCommand2(getTypeSize(argumentType));
			}
			else {
				derefCommand = new DeRefCommand(getTypeSize(argumentType));
			}

			auto nativeFunction = new FixParamFunction<1>(DEREF_OPERATOR, EXP_UNIT_ID_DEREF, FUNCTION_PRIORITY_UNARY_PREFIX, argumentType);
			nativeFunction->setNative((DFunction2Ref)(derefCommand));

			paramInfo.castingFunction = FunctionRef(nativeFunction);
			paramInfo.accurative = 2;

			return true;
		}

		return false;
	}

	Function* ScriptCompiler::findCastingFunction(const ScriptType& sourceType, const ScriptType& targetType) {
		int functionId = findFunction(targetType.sType(), { sourceType });
		if (functionId < 0) {
			return nullptr;
		}

		auto theFunction = createFunctionFromId(functionId);
		if (theFunction) {
			auto& rt = theFunction->getReturnType();
			if (targetType.iType() != rt.iType()) {
				delete theFunction;
				theFunction = nullptr;
			}
		}
		return theFunction;
	}

	bool ScriptCompiler::findMatchingLevel2(const ScriptType& argumentType, const ScriptType& paramType, ParamCastingInfo& paramInfo) {
		//same type => perfect match
		if (argumentType == paramType) {
			paramInfo.accurative = 0; //best match
			paramInfo.castingFunction = nullptr; //no need to casting
			return true;
		}
		
		auto theFunction = findCastingFunction(paramType, argumentType);
		if (!theFunction) {
			if (!argumentType.isSemiRefType() && paramType.isSemiRefType()) {
				auto paramTypeOrigin = paramType.deSemiRef();
				theFunction = findCastingFunction(paramTypeOrigin, argumentType);
				if (!theFunction) {
					return false;
				}

				DFunction2Ref derefCommand = make_shared<DeRefCommand>(getTypeSize(argumentType));
				auto derefFunction = make_shared<FixParamFunction<1>>(DEREF_OPERATOR, EXP_UNIT_ID_DEREF, FUNCTION_PRIORITY_UNARY_PREFIX, argumentType);
				derefFunction->setNative(derefCommand);
				theFunction->pushParam(derefFunction);

				paramInfo.accurative = findConversionAccurative(paramTypeOrigin.iType(), argumentType.iType()) + 2;
				paramInfo.castingFunction = FunctionRef(theFunction);

				return true;
			}

			// the following commented code should not be used
			// its may lead to a invalid memory accessment in a function which modify its parameters.
#if 0
			else if (argumentType.isSemiRefType() && !paramType.isSemiRefType()) {
				auto argumentTypeOrigin = argumentType.deSemiRef();
				theFunction = findCastingFunction(paramType, argumentTypeOrigin);
				if (!theFunction) {
					return false;
				}

				int functionId = getMakingRefFunction();
				if (functionId == -1) {
					return false;
				}
				auto makeRef = createFunctionFromId(functionId);
				makeRef->pushParam(FunctionRef(theFunction));
				makeRef->setReturnType(argumentType);
				
				paramInfo.accurative = findConversionAccurative(paramType.iType(), argumentTypeOrigin.iType()) + 2;
				paramInfo.castingFunction = FunctionRef(makeRef);
				return true;
			}
			else if (argumentType.isSemiRefType() && paramType.isSemiRefType()) {
				auto argumentTypeOrigin = argumentType.deSemiRef();
				auto paramTypeOrigin = paramType.deSemiRef();
				theFunction = findCastingFunction(paramTypeOrigin, argumentTypeOrigin);
				if (!theFunction) {
					return false;
				}

				int functionId = getMakingRefFunction();
				if (functionId == -1) {
					return false;
				}
				// step to casting
				// 1. deref unit
				// 2. casting type of [1] to origin type of argument
				// 3. make unit of [2] to be ma reference

				// 1. deref unit
				DFunction2Ref derefCommand = make_shared<DeRefCommand>(getTypeSize(argumentTypeOrigin));
				auto derefFunction = make_shared<FixParamFunction<1>>(DEREF_OPERATOR, EXP_UNIT_ID_DEREF, FUNCTION_PRIORITY_UNARY_PREFIX, argumentTypeOrigin);
				derefFunction->setNative(derefCommand);
				
				// 2. casting type of [1] to origin type of argument
				theFunction->pushParam(derefFunction);
				
				// 3. make unit of [2] to be ma reference
				auto makeRef = createFunctionFromId(functionId);
				makeRef->pushParam(FunctionRef(theFunction));

				makeRef->setReturnType(argumentType);

				paramInfo.accurative = findConversionAccurative(paramType.iType(), argumentTypeOrigin.iType()) + 2;
				paramInfo.castingFunction = FunctionRef(makeRef);
				return true;
			}
#endif
			return false;
		}

		paramInfo.accurative = findConversionAccurative(paramType.iType(), argumentType.iType());
		paramInfo.castingFunction = FunctionRef(theFunction);

		return true;
	}

	bool ScriptCompiler::findMatchingConstructor(const ExecutableUnitRef& objectParam, const ExecutableUnitRef& secondParam, ParamCastingInfo& paramInfo) {
		auto& argumentType = objectParam->getReturnType();
		// check for composite type
		if (secondParam->getType() == EXP_UNIT_ID_DYNAMIC_FUNC) {
			// get constructors for the type
			list<OverLoadingItem*> constructors;
			getConstructors(argumentType.iType(), constructors);

			if (constructors.size() == 0) return false;

			auto collector = dynamic_cast<DynamicParamFunction*>(secondParam.get() );
			if (!collector) {
				// throw exception here
				throw std::runtime_error("internal error, unit is not a DynamicParamFunction");
			}
			
			list<ExecutableUnitRef> wholeConstructorParams;

			// build a making ref function unit
			int makeRefId = getMakingRefFunction();
			if (makeRefId < 0) {
				// throw exception here
				throw std::runtime_error("internal error, definition of ref operator is not found");
			}

			// create making ref function unit with empty parameter and initialized return type
			// this need for selecting candidate
			auto makeRefFunc = createFunctionFromId(makeRefId);
			if (!makeRefFunc) {
				// throw exception here
				throw std::runtime_error("internal error, definition of ref operator is not found");
			}

			makeRefFunc->pushParam(objectParam);
			FunctionRef makeRefFuncRef(makeRefFunc);
			// keep origin source char in new expression unit
			makeRefFunc->setSourceCharIndex(objectParam->getSourceCharIndex());

			// first parameter of constructor is always make ref function
			// that make the object need to be constructed to a reference then pass
			// the the constructor
			wholeConstructorParams.push_back(makeRefFuncRef);

			// parameters for constructor of the type
			auto& paramsForType = collector->getParams();
			wholeConstructorParams.insert(wholeConstructorParams.end(), paramsForType.begin(), paramsForType.end());

			// find a constructor that coressponding with parameters in
			auto candidates = selectMultiCandidates(constructors, wholeConstructorParams);
			auto pCandidate = selectSingleCandidate(candidates);

			if (!pCandidate) return false;
			auto& argumentTypes = pCandidate->item->paramTypes;
			auto& castingList = pCandidate->paramCasting;
			
			auto constructorUnit = createFunctionFromId(pCandidate->item->functionId);
			if (!constructorUnit) {
				// throw exception here
				throw std::runtime_error("internal error, constructor is not defined");
			}

			// keep origin source char in new expression unit
			constructorUnit->setSourceCharIndex(objectParam->getSourceCharIndex());

			// push the first parameter unit to constructor unit
			// the first parameter is the making ref function
			constructorUnit->pushParam(makeRefFuncRef);

			FunctionRef constructorUnitRef(constructorUnit);
			FwdCompositeConstrutorUnit* compositeUnit = new FwdCompositeConstrutorUnit(constructorUnitRef, argumentTypes, castingList);
			compositeUnit->setReturnType(argumentType);

			// keep origin source char in new expression unit
			compositeUnit->setSourceCharIndex(objectParam->getSourceCharIndex());

			// set it excludedded from destructor to prevent the data of variable is destroyed
			compositeUnit->setMask(compositeUnit->getMask() | UMASK_EXCLUDEFROMDESTRUCTOR);

			// set casting info to allow the engine choose the suitable candidate in case there are many
			// candidate can be applied to a given parameter
			paramInfo.castingFunction.reset(compositeUnit);
			auto& totalAccuracy = paramInfo.accurative;
			totalAccuracy = 0;
			for (auto it = castingList.begin(); it != castingList.end(); it++) {
				totalAccuracy += it->accurative;
			}

			constructorUnit->setMask(compositeUnit->getMask() | UMASK_CONSTRUCT_FACTOR);
			auto constructBuildInfo = currentScope()->applyDefaultConstructor(argumentType, constructorUnit);
			constructBuildInfo->operatorIndex = INDEPENDENT_CONSTRUCTOR;

			return true;
		}

		auto constructorFunc = applyConstructor(objectParam, secondParam, &paramInfo.accurative);
		if (constructorFunc) {

			FwdConstrutorUnit* fwdCtorUnit = new FwdConstrutorUnit(FunctionRef(constructorFunc));
			fwdCtorUnit->setReturnType(argumentType);

			// keep origin source char in new expression unit
			fwdCtorUnit->setSourceCharIndex(secondParam->getSourceCharIndex());

			// set it excludedded from destructor to prevent the data of variable is destroyed
			fwdCtorUnit->setMask(fwdCtorUnit->getMask() | UMASK_EXCLUDEFROMDESTRUCTOR);

			paramInfo.castingFunction.reset(fwdCtorUnit);
			auto constructBuildInfo = currentScope()->applyDefaultConstructor(argumentType, constructorFunc);
			constructBuildInfo->operatorIndex = INDEPENDENT_CONSTRUCTOR;
			return true;
		}

		return false;
	}

	int ScriptCompiler::findMatching(const ScriptType& refVoidType, const ScriptType& argumentType, const ScriptType& paramType, ParamCastingInfo& paramInfo, bool tryFindingLevel2) {
		if (findMatchingLevel1(refVoidType, argumentType, paramType, paramInfo)) {
			return 1;
		}
		if (tryFindingLevel2 && findMatchingLevel2(argumentType, paramType, paramInfo)) {
			return 2;
		}

		return 0;
	}

	std::shared_ptr<list<CandidateInfo>> ScriptCompiler::selectMultiCandidates(const list<OverLoadingItem*>& overloadingItems, const std::list<ExecutableUnitRef>& params) {		
		int n = (int)params.size();
		ParamCastingInfo paramInfoTemp;
		paramInfoTemp.accurative = 0;
		paramInfoTemp.castingFunction = nullptr;

		auto overloadingCandidates = make_shared<list<CandidateInfo>>();

		// filter overloading functions by number of parameters
		for (auto it = overloadingItems.begin(); it != overloadingItems.end(); ++it) {
			if ((*it)->paramTypes.size() == (size_t)n) {
				CandidateInfo candidateInfo;
				overloadingCandidates->push_back(candidateInfo);
				CandidateInfo& candidateInfoRef = overloadingCandidates->back();
				candidateInfoRef.item = (*it);
				candidateInfoRef.paramCasting.resize(n, paramInfoTemp);
			}
		}
		int overloadingSize = (int)overloadingCandidates->size();

		ScriptType refVoidType(_typeManagerRef->getBasicTypes().TYPE_VOID | DATA_TYPE_POINTER_MASK, "ref void");
		// filter candidates by the ability to convert each paramter type to its argument type
		int i = 0;
		for (auto pit = params.begin(); pit != params.end(); pit++, i++) {
			auto& paramType = (*pit)->getReturnType();
			auto it = overloadingCandidates->begin();
			decltype(it) itTemp;

			while (it != overloadingCandidates->end()) {
				auto& argumentType = *(it->item->paramTypes[i]);
				auto& paramCasting = it->paramCasting.at(i);
				if (findMatchingComposite(argumentType, *pit, paramCasting)) {
					it++;
				}
				else if (findMatching(refVoidType, argumentType, paramType, paramCasting, true)) {
					// keep origin source char in new expression unit
					if (paramCasting.castingFunction) {
						paramCasting.castingFunction->setSourceCharIndex((*pit)->getSourceCharIndex());
					}
					++it;
				}
				else {
					itTemp = it;
					++it;
					overloadingCandidates->erase(itTemp);
				}
			}
		}

		// calculate totalAccurative to convert a given params set to a candidate arguments set
		for (auto it = overloadingCandidates->begin(); it != overloadingCandidates->end(); it++) {			
			it->totalAccurative = 0;
			for (i = 0; i < n; i++) {
				it->totalAccurative += it->paramCasting.at(i).accurative;
			}
		}

		return overloadingCandidates;
	}

	CandidateInfo* ScriptCompiler::selectSingleCandidate(const std::shared_ptr<list<CandidateInfo>>& candidates) {
		if (!candidates || candidates->size() == 0) return nullptr;

		auto it = std::min_element(candidates->begin(), candidates->end(), [](const CandidateInfo& c1, const CandidateInfo& c2) {
			return c1.totalAccurative < c2.totalAccurative;
		});

		return & *it;
	}

	ExecutableUnitRef ScriptCompiler::findBoolOperatorForType(int type, const ExecutableUnitRef& paramUnit, int* pAccurative) {
		string boolOperatorName = getType(type);
		ExecutableUnitRef boolOperatorFunc;

		auto boolOverLoadingFuncs = findOverloadingFuncRoot(boolOperatorName);
		if (!boolOverLoadingFuncs) {
			return boolOperatorFunc;
		}
		list<OverLoadingItem*> filtedItems;
		for (auto it = boolOverLoadingFuncs->begin(); it != boolOverLoadingFuncs->end(); it++) {
			if (it->paramTypes.size() == 1) {
				auto& item = *it;
				filtedItems.push_back((OverLoadingItem*)&item);
			}
		}
		list<ExecutableUnitRef> params = { paramUnit };
		auto candidates = selectMultiCandidates(filtedItems, params);
		auto candidate = selectSingleCandidate(candidates);

		ExecutableUnitRef param;

		if (candidate) {
			auto& functionInfo = candidate->item;
			auto& castingInfo = candidate->paramCasting.front();

			if (pAccurative) *pAccurative = castingInfo.accurative;

			if (castingInfo.castingFunction) {
				param = paramUnit;
				applyCasting(param, castingInfo.castingFunction);
				param->setMask(param->getMask() | UMASK_CASTINGUNITNOTINEXPRESSION);

				// keep origin source char in new expression unit
				param->setSourceCharIndex(paramUnit->getSourceCharIndex());
			}
			else {
				param = paramUnit;
			}
			auto newFunction = createFunctionFromId(functionInfo->functionId);
			newFunction->pushParam(param);

			// keep origin source char in new expression unit
			newFunction->setSourceCharIndex(param->getSourceCharIndex());

			boolOperatorFunc.reset(newFunction);
		}

		return boolOperatorFunc;
	}

	FunctionRef ScriptCompiler::applyParamToCandidate(const CandidateInfo& candidate, std::list<ExecutableUnitRef>& params) {
		FunctionRef functionRef;

		if (params.size() != candidate.paramCasting.size()) {
			return functionRef;
		}

		int functionId = candidate.item->functionId;
		auto pFunc = createFunctionFromId(functionId);
		if (pFunc == nullptr) {
			return functionRef;
		}

		auto& paramCasting = candidate.paramCasting;
		auto& argumentTypes = candidate.item->paramTypes;
		auto pit = params.begin();
		auto argit = argumentTypes.begin();
		for (auto it = paramCasting.begin(); it != paramCasting.end(); it++, pit++, argit++) {
			auto& castingF = it->castingFunction;
			auto& paramRef = *pit;
			if (castingF) {
				applyCastingAndOptimize(paramRef, castingF);
				functionRef->pushParam(paramRef);
				paramRef->setReturnType(*(argit->get()));
			}
			else {
				functionRef->pushParam(paramRef);
			}
		}


		return functionRef;
	}

	Function* ScriptCompiler::applyConstructor(const ExecutableUnitRef& variableUnit, const ExecutableUnitRef& argUnit, int* pAccurative) {
		//find registered copy contructor for current types

		auto param1 = variableUnit;
		auto param2 = argUnit;

		auto& param1Type = param1->getReturnType();
		auto& param2Type = param2->getReturnType();

		auto constructorCandidates = getConstructor(param1Type, param2);
		auto pCandidate = ScriptCompiler::selectSingleCandidate(constructorCandidates);

		//copy constructor found
		if (pCandidate) {
			auto& functionInfo = pCandidate->item;
			auto& castingInfo = pCandidate->paramCasting.front();

			//convert param 1 to ref
			if (param1Type.refLevel() == param2Type.refLevel()) {
				convertToRef(param1);
			}

			//param 2 is not availble for using imediately and need to be cast before use.
			if (castingInfo.castingFunction) {
				applyCasting(param2, castingInfo.castingFunction);
				param2->setMask(param2->getMask() | UMASK_CASTINGUNITNOTINEXPRESSION);

				if (pAccurative) {
					*pAccurative = castingInfo.accurative;
				}
			}
			auto newFunction = createFunctionFromId(functionInfo->functionId);
			newFunction->pushParam(param1);
			newFunction->pushParam(param2);
			newFunction->setMask(newFunction->getMask() | UMASK_CONSTRUCTOR);

			// keep origin source char in new expression unit
			newFunction->setSourceCharIndex(variableUnit->getSourceCharIndex());

			//auto xOperand = dynamic_cast<CXOperand*>(variableUnit.get());
			//auto pVariable = xOperand->getVariable();
			//auto scope = currentScope();
			//if (scope) {
			//	scope->applyDefaultConstructor(xOperand->getReturnType(), newFunction);
			//	if (pVariable) {
			//		scope->checkVariableToRunDestructor(pVariable);
			//	}
			//	scope->generateNextConstructId();
			//}

			return newFunction;
		}

		return nullptr;
	}

	bool ScriptCompiler::hasConstructor(int iType) const {
		auto it = _constructorsMap.find(iType);
		if (it != _constructorsMap.end()) {
			return true;
		}

		auto jt = _constructorMap.find(iType);
		if (jt != _constructorMap.end()) {
			return true;
		}

		auto kt = _copyConstructorMap.find(iType);
		if (kt != _copyConstructorMap.end()) {
			auto& copyConstrucrorOverloadings = kt->second;
			return copyConstrucrorOverloadings->size() != 0;
		}

		return false;
	}

	bool ScriptCompiler::convertToRef(ExecutableUnitRef& param) {
		if (param->getType() == EXP_UNIT_ID_DEREF) {
			//make ref meet deref then both is removed from the expression
			auto& paramOfDeref = ((Function*)param.get())->getChild(0);
			param = paramOfDeref;
			return true;
		}
		int refFunctionId = getMakingRefFunction();
		if (refFunctionId < 0) {
			setErrorText("Internal error: making ref function is missing");
			return false;
		}

		Function* refFunction = (Function*)createFunctionFromId(refFunctionId);
		refFunction->pushParam(param);
		refFunction->setMask(refFunction->getMask() | UMASK_CASTINGUNITNOTINEXPRESSION);

		// keep origin source char in new expression unit
		refFunction->setSourceCharIndex(param->getSourceCharIndex());

		param.reset((ExecutableUnit*)(refFunction));

		return true;
	}

	void ScriptCompiler::applyCasting(ExecutableUnitRef& unit, const FunctionRef& castingUnit) {
		if (castingUnit->getChildCount() == 1 && EXP_UNIT_ID_CREATE_OBJECT_COMPOSITE != castingUnit->getType()) {
			// for some cases, the casting unit is alerady fill param
			// these cases is multiple level casting and the last
			// casting component unit is the unit that integrate directly
			// to the origin unit
			Function* existingCastingComponent = castingUnit.get();
			do
			{
				existingCastingComponent = dynamic_cast<Function*>(existingCastingComponent->getChild(0).get());
				if (existingCastingComponent == nullptr) {
					throw std::exception("casting unit is invalid");
				}
				if (existingCastingComponent->getChildCount() == 0) {
					existingCastingComponent->pushParam(unit);
					break;
				}
			} while (true);
		}
		else
		{
			castingUnit->pushParam(unit);
		}
		unit = castingUnit;
	}

	///
	/// check ability of assigning a dynamic array to a struct and apply a new function t o
	/// allow assign each member of struct to a parameter unit of second operand unit
	///
	FunctionRef ScriptCompiler::applyConstructorForCompisiteType(const ExecutableUnitRef& firstParamUnit, const ExecutableUnitRef& secondOperand, bool& hasNoError) {
		ScriptType typeVoid(getTypeManager()->getBasicTypes().TYPE_VOID, "void");
		FunctionRef assignmentCompositeUnit;

		list<pair<Variable*, ExecutableUnitRef>> assigments;
		int accurative = 0;
		hasNoError = breakCompositeConstructor(firstParamUnit, secondOperand, assigments, accurative);

		if (hasNoError && assigments.size()) {
			auto& firstUnit = assigments.front().second;
			if (assigments.size() == 1 && firstUnit->getType() == EXP_UNIT_ID_CREATE_OBJECT_COMPOSITE) {
				assignmentCompositeUnit = dynamic_pointer_cast<Function>(firstUnit);
			}
			else {
				assignmentCompositeUnit = make_shared<CompositeConstrutorUnit>(assigments);
				// keep origin source char in new expression unit
				assignmentCompositeUnit->setSourceCharIndex(firstUnit->getSourceCharIndex());
			}
			assignmentCompositeUnit->setReturnType(typeVoid);
		}

		return assignmentCompositeUnit;
	}

	FunctionRef ScriptCompiler::applyAssigmentForCompisiteType(const ExecutableUnitRef& firstParamUnit, const ExecutableUnitRef& secondOperand, bool& hasNoError) {
		ScriptType typeVoid(getTypeManager()->getBasicTypes().TYPE_VOID, "void");
		FunctionRef assignmentCompositeUnit;

		list<pair<Variable*, ExecutableUnitRef>> assigments;
		int accurative = 0;
		hasNoError = breakCompositeConstructor(firstParamUnit, secondOperand, assigments, accurative);

		if (hasNoError && assigments.size()) {
			auto& firstUnit = assigments.front().second;
			if (assigments.size() == 1 && firstUnit->getType() == EXP_UNIT_ID_CREATE_OBJECT_COMPOSITE) {
				assignmentCompositeUnit = dynamic_pointer_cast<Function>(firstUnit);
			}
			else {
				assignmentCompositeUnit = make_shared<CompositeConstrutorUnit>(assigments);
				// keep origin source char in new expression unit
				assignmentCompositeUnit->setSourceCharIndex(firstUnit->getSourceCharIndex());
			}
			assignmentCompositeUnit->setReturnType(typeVoid);
		}

		return assignmentCompositeUnit;
	}

	bool ScriptCompiler::findMatchingComposite(const ScriptType& argumentType, const ExecutableUnitRef& unit, ParamCastingInfo& paramInfo) {
		auto scope = currentScope();
		// only work if current code is in a scope
		if (!scope) {
			return false;
		}
		auto& paramType = unit->getReturnType();
		
		// register a temporary variable
		auto pVariable = new Variable("_temporary_variable");
		pVariable->setDataType(argumentType);
		pVariable->setOffset(-1);

		CXOperand* pCXOperand = new CXOperand(scope, pVariable, pVariable->getDataType());
		ExecutableUnitRef variableUnitRef(pCXOperand);

		// keep origin source char in new expression unit
		pCXOperand->setSourceCharIndex(unit->getSourceCharIndex());

		if  ((argumentType.origin() != paramType.origin() || (argumentType.refLevel() == 0 && paramType.refLevel() > 0)) &&
			findMatchingConstructor(variableUnitRef, unit, paramInfo)) {
			auto& constructorUnit = paramInfo.castingFunction;
			scope->applyTemporaryVariableFor(constructorUnit.get(), pVariable);

			// remove ExcludeFromDestructor flag from the mask
			unsigned int newMask = ~UMASK_EXCLUDEFROMDESTRUCTOR;
			constructorUnit->setMask( constructorUnit->getMask() & newMask );

			return true;
		}

		if (unit->getType() == EXP_UNIT_ID_DYNAMIC_FUNC) {
			auto assignmentCompositeUnit = make_shared<CompositeConstrutorUnit>();
			// keep origin source char in new expression unit
			assignmentCompositeUnit->setSourceCharIndex(unit->getSourceCharIndex());

			ScriptType typeVoid(getTypeManager()->getBasicTypes().TYPE_VOID, "void");
			list<pair<Variable*, ExecutableUnitRef>> assigments;

			auto secondParamUnitRef = dynamic_pointer_cast<DynamicParamFunction>(unit);
			paramInfo.accurative = 0;
			bool hasNoError = breakCompositeConstructor(variableUnitRef, secondParamUnitRef, assigments, paramInfo.accurative);

			if (hasNoError && assigments.size()) {
				assignmentCompositeUnit->setAssigments(assigments);
				assignmentCompositeUnit->setReturnType(typeVoid);

				paramInfo.castingFunction = assignmentCompositeUnit;

				scope->applyTemporaryVariableFor(assignmentCompositeUnit.get(), pVariable);
				return true;
			}
		}
		delete pVariable;
		return false;
	}

	inline void checkUnitForExcludingDestructor(ScriptCompiler* scriptCompiler, const ExecutableUnitRef& unit) {
		if (ISFUNCTION(unit)) {
			unit->setMask(unit->getMask() | UMASK_EXCLUDEFROMDESTRUCTOR);
		}
	}

	bool ScriptCompiler::breakCompositeConstructor(const ExecutableUnitRef& variableUnit, const ExecutableUnitRef& secondOperand, list<pair<Variable*, ExecutableUnitRef>>& assigments, int& accurative) {
		const ScriptType& param1Type = variableUnit->getReturnType();

		auto pCXOperand = dynamic_cast<CXOperand*>(variableUnit.get());
		if (!pCXOperand) {
			// cannot applied assigment for non variable unit
			return false;
		}
		Variable* pVariable = pCXOperand->getVariable();

		// first try to find matching constructor for object need to initialized
		list<OverLoadingItem*> overloadingConstructors;
		getConstructors(param1Type.iType(), overloadingConstructors);
		if (overloadingConstructors.size()) {
			ParamCastingInfo castingInfo;
			if (!findMatchingConstructor(variableUnit, secondOperand, castingInfo)) {
				// cannot find a matching overloading constructor for given param unit(secondOperand)
				// exit the function because for type has constructors, we only allow construct it
				// by one of its constructor not by other operators.
				return false;
			}

			auto tranformFunction = castingInfo.castingFunction;
			tranformFunction->pushParam(secondOperand);
			assigments.emplace_back(make_pair(pVariable, ExecutableUnitRef(tranformFunction)));
			accurative += castingInfo.accurative;
			return true;
		}

		// second, try to initialize composite object
		// composite object can only initialize by a dynamic array
		if (secondOperand->getType() != EXP_UNIT_ID_DYNAMIC_FUNC) {
			return false;
		}

		auto paramCollector = dynamic_cast<DynamicParamFunction*>(secondOperand.get());

		constexpr int lackOfInitialValueAccurative = 10;
		ScriptType refVoidType(getTypeManager()->getBasicTypes().TYPE_VOID | DATA_TYPE_POINTER_MASK, "ref void");

		auto scope = currentScope();
		auto applyTranformTypeUnit = [scope, this, &assigments, &accurative, &refVoidType](ExecutableUnitRef& memberVariableUnit, ExecutableUnitRef& paramUnit) -> bool {
			auto pCXMemberOperand = dynamic_cast<CXOperand*>(memberVariableUnit.get());
			if (!pCXMemberOperand) {
				// cannot applied assigment for non variable unit
				return false;
			}
			Variable* memberVariable = pCXMemberOperand->getVariable();
			auto& argumentType = memberVariable->getDataType();
			auto& paramType = paramUnit->getReturnType();

			if (!breakCompositeConstructor(memberVariableUnit, paramUnit, assigments, accurative)) {
				// cannot find matching constructor, and cannot find matching composite assigment
				// then try to another matching type for given types
				ExecutableUnitRef tranformUnitRef;

				int memberAccurative;
				// ...then first check copy constructor for data type of member of struct
				auto copyConstructor = applyConstructor(memberVariableUnit, paramUnit, &memberAccurative);				
				// has copy constructor for corressponding argument's types?...
				if (copyConstructor) {
					tranformUnitRef.reset(copyConstructor);
				}
				else if (hasConstructor(argumentType.iType())) {
					// has constructor but cannot find the matching one for given param unit
				}
				else {
					ParamCastingInfo paramCastingInfo;
					bool res = false;
					if (argumentType.origin() == paramType.origin()) {
						if ( (!paramType.isSemiRefType() || argumentType.isSemiRefType()) &&
							findMatchingLevel1(refVoidType, argumentType, paramType, paramCastingInfo)) {
							res = true;
						}
					}
					else if (findMatchingLevel2(argumentType, paramType, paramCastingInfo)) {
						res = true;
					}
					if (res) {
						if (paramCastingInfo.castingFunction) {
							tranformUnitRef = paramUnit;
							// keep origin source char in new expression unit
							paramCastingInfo.castingFunction->setSourceCharIndex(paramUnit->getSourceCharIndex());

							applyCastingAndOptimize(tranformUnitRef, paramCastingInfo.castingFunction);
							tranformUnitRef->setReturnType(argumentType);
						}
						else {
							checkUnitForExcludingDestructor(this, paramUnit);
							tranformUnitRef = paramUnit;
						}
						memberAccurative = paramCastingInfo.accurative;
					}
				}
				// check if there is no available unit can convert from data type of parameter unit to
				// data type of member in struct
				if (!tranformUnitRef) {
					setErrorText("cannot convert from type '" + paramType.sType() + "' to type '" + argumentType.sType() + "'");
					return false;
				}
				accurative += memberAccurative;
				assigments.emplace_back(make_pair(memberVariable, tranformUnitRef));
			}

			return true;
		};

		// check if data type of variable is a struct
		auto pStruct = getStruct(param1Type.iType());
		if (pStruct) {
			MemberInfo memberInfo;
			string memberName;
			bool res = pStruct->getMemberFirst(&memberName, &memberInfo);
			auto& params = paramCollector->getParams();
			auto iter = params.begin();

			// break assigment for each member of the struct
			while (res && iter != params.end()) {
				auto& paramUnit = *iter;
				auto& argumentType = memberInfo.type;
				auto& paramType = paramUnit->getReturnType();

				// create a variable that hold member offset in struct...
				auto memberVariable = scope->registMemberVariable(pVariable, pVariable->getName() + "." + memberName);
				memberVariable->setDataType(argumentType);
				memberVariable->setOffset(memberInfo.offset);

				CXOperand* pCXMemberOperand = new CXOperand(scope, memberVariable, memberVariable->getDataType());
				ExecutableUnitRef memberVariableRef(pCXMemberOperand);

				// keep origin source char in new expression unit
				pCXMemberOperand->setSourceCharIndex(paramUnit->getSourceCharIndex());

				if (!applyTranformTypeUnit(memberVariableRef, paramUnit)) {
					return false;
				}

				res = pStruct->getMemberNext(&memberName, &memberInfo);
				iter++;
			}

			while (res) {
				auto& argumentType = memberInfo.type;

				// create a variable that hold member offset in struct...
				auto memberVariable = scope->registMemberVariable(pVariable, pVariable->getName() + "." + memberName);
				memberVariable->setDataType(argumentType);
				memberVariable->setOffset(memberInfo.offset);

				// run default constructor for the left member if it has
				auto defaultConstructorId = getDefaultConstructor(argumentType.iType());
				if (defaultConstructorId >= 0) {
					auto defaultConstructor = scope->generateDefaultAutoOperator(defaultConstructorId, memberVariable);
					// keep origin source char in new expression unit
					defaultConstructor->setSourceCharIndex(variableUnit->getSourceCharIndex());

					assigments.emplace_back(make_pair(memberVariable, defaultConstructor));
				}
				else {
					// element has constructor but don't have default constructor
					// so, the engine don;t know how to initialize the left elements
					// which have not initialized yet
					if (hasConstructor(argumentType.iType())) {
						return false;
					}
				}

				res = pStruct->getMemberNext(&memberName, &memberInfo);
				iter++;

				accurative += lackOfInitialValueAccurative;
			}

			return true;
		}
		// check if data type of variable is a static array
		if (param1Type.iType() & DATA_TYPE_ARRAY_MASK) {
			auto arrayInfo = (StaticArrayInfo*)getTypeInfo(param1Type.origin());
			if (arrayInfo == nullptr) {
				return false;
			}
			auto argumentType = ScriptType::buildScriptType(this, arrayInfo->elmType, arrayInfo->refLevel);

			int elmIdx = 0;
			int elmOffset = 0;
			auto& elmCount = arrayInfo->elmCount;

			auto& params = paramCollector->getParams();
			for (auto it = params.begin(); it != params.end() && elmIdx < elmCount; it++, elmIdx++) {
				auto& paramUnit = *it;
				auto& paramType = paramUnit->getReturnType();

				// create a variable that hold member offset in array...
				auto memberVariable = scope->registMemberVariable(pVariable, pVariable->getName() + "[" + std::to_string(elmIdx) + "]");
				memberVariable->setDataType(argumentType);
				memberVariable->setOffset(elmOffset);

				CXOperand* pCXMemberOperand = new CXOperand(scope, memberVariable, memberVariable->getDataType());
				ExecutableUnitRef memberVariableRef(pCXMemberOperand);

				// keep origin source char in new expression unit
				pCXMemberOperand->setSourceCharIndex(paramUnit->getSourceCharIndex());

				if (!applyTranformTypeUnit(memberVariableRef, paramUnit)) {
					return false;
				}

				elmOffset += arrayInfo->elmSize;
			}

			auto defaultConstructorId = getDefaultConstructor(argumentType.iType());

			// run default constructor for the left elements if it has
			if (defaultConstructorId >= 0) {
				for (; elmIdx < elmCount; elmIdx++) {
					// create a variable that hold member offset in array...
					auto memberVariable = scope->registMemberVariable(pVariable, pVariable->getName() + "[" + std::to_string(elmIdx) + "]");
					memberVariable->setDataType(argumentType);
					memberVariable->setOffset(elmOffset);

					auto defaultConstructor = scope->generateDefaultAutoOperator(defaultConstructorId, memberVariable);
					// keep origin source char in new expression unit
					defaultConstructor->setSourceCharIndex(variableUnit->getSourceCharIndex());

					assigments.emplace_back(make_pair(memberVariable, defaultConstructor));
					elmOffset += arrayInfo->elmSize;
				}
			}
			else if (elmIdx < elmCount) {
				// element has constructor but don't have default constructor
				// so, the engine don;t know how to initialize the left elements
				// which have not initialized yet
				if (hasConstructor(argumentType.iType())) {
					return false;
				}
			}
			if (elmIdx < elmCount) {
				accurative += (elmCount - elmIdx) * lackOfInitialValueAccurative;
			}

			return true;
		}

		return false;
	}

	bool ScriptCompiler::breakCompositeAssigment(const ExecutableUnitRef& variableUnit, const ExecutableUnitRef& secondOperand, list<pair<Variable*, ExecutableUnitRef>>& assigments, int& accurative) {
		const ScriptType& param1Type = variableUnit->getReturnType();

		auto pCXOperand = dynamic_cast<CXOperand*>(variableUnit.get());
		if (!pCXOperand) {
			// cannot applied assigment for non variable unit
			return false;
		}
		Variable* pVariable = pCXOperand->getVariable();
		const ScriptType& param2Type = secondOperand->getReturnType();

		auto assigmentOverloadings = findOverloadingFuncRoot("=");
		if (assigmentOverloadings && assigmentOverloadings->size()) {
			auto param1Ref = param1Type.isSemiRefType() ? param1Type : param1Type.makeSemiRef();

			list<OverLoadingItem*> filterdItems;
			for (auto it = assigmentOverloadings->begin(); it != assigmentOverloadings->end(); it++) {
				auto& firstParamType = *(it->paramTypes[0]);
				if (firstParamType == param1Ref && it->paramTypes.size() == 2) {
					filterdItems.push_back((OverLoadingItem*)&*it);
				}
			}

			auto variableRefUnit = variableUnit;
			if (!param1Type.isSemiRefType()) {
				if (!convertToRef(variableRefUnit)) {
					throw exception("making ref failed");
				}
				variableRefUnit->setReturnType(param1Ref);
			}

			list<ExecutableUnitRef> params = { variableRefUnit , secondOperand};
			auto candidates = selectMultiCandidates(filterdItems, params);
			auto pCandidate = selectSingleCandidate(candidates);

			if (!pCandidate) {
				return false;
			}
		}

		// second, try to initialize composite object
		// composite object can only initialize by a dynamic array
		if (secondOperand->getType() != EXP_UNIT_ID_DYNAMIC_FUNC) {
			return false;
		}

		auto paramCollector = dynamic_cast<DynamicParamFunction*>(secondOperand.get());

		constexpr int lackOfInitialValueAccurative = 10;
		ScriptType refVoidType(getTypeManager()->getBasicTypes().TYPE_VOID | DATA_TYPE_POINTER_MASK, "ref void");

		auto scope = currentScope();
		auto applyTranformTypeUnit = [scope, this, &assigments, &accurative, &refVoidType](ExecutableUnitRef& memberVariableUnit, ExecutableUnitRef& paramUnit) -> bool {
			auto pCXMemberOperand = dynamic_cast<CXOperand*>(memberVariableUnit.get());
			if (!pCXMemberOperand) {
				// cannot applied assigment for non variable unit
				return false;
			}
			Variable* memberVariable = pCXMemberOperand->getVariable();
			auto& argumentType = memberVariable->getDataType();
			auto& paramType = paramUnit->getReturnType();

			if (!breakCompositeAssigment(memberVariableUnit, paramUnit, assigments, accurative)) {
				// cannot find matching constructor, and cannot find matching composite assigment
				// then try to another matching type for given types
				ExecutableUnitRef tranformUnitRef;

				int memberAccurative;
				// ...then first check copy constructor for data type of member of struct
				auto copyConstructor = applyConstructor(memberVariableUnit, paramUnit, &memberAccurative);
				// has copy constructor for corressponding argument's types?...
				if (copyConstructor) {
					tranformUnitRef.reset(copyConstructor);
				}
				else if (hasConstructor(argumentType.iType())) {
					// has constructor but cannot find the matching one for given param unit
				}
				else {
					ParamCastingInfo paramCastingInfo;
					bool res = false;
					if (argumentType.origin() == paramType.origin()) {
						if ((!paramType.isSemiRefType() || argumentType.isSemiRefType()) &&
							findMatchingLevel1(argumentType, paramType, refVoidType, paramCastingInfo)) {
							res = true;
						}
					}
					else if (findMatchingLevel2(argumentType, paramType, paramCastingInfo)) {
						res = true;
					}
					if (res) {
						if (!(paramCastingInfo.castingFunction)) {
							checkUnitForExcludingDestructor(this, paramUnit);
							tranformUnitRef = paramUnit;
						}
						else {
							tranformUnitRef = paramCastingInfo.castingFunction;
							// keep origin source char in new expression unit
							tranformUnitRef->setSourceCharIndex(paramUnit->getSourceCharIndex());
						}
						memberAccurative = paramCastingInfo.accurative;
					}
				}
				// check if there is no available unit can convert from data type of parameter unit to
				// data type of member in struct
				if (!tranformUnitRef) {
					setErrorText("cannot convert from type '" + paramType.sType() + "' to type '" + argumentType.sType() + "'");
					return false;
				}
				accurative += memberAccurative;
				assigments.emplace_back(make_pair(memberVariable, tranformUnitRef));
			}

			return true;
		};

		// check if data type of variable is a struct
		auto pStruct = getStruct(param1Type.iType());
		if (pStruct) {
			MemberInfo memberInfo;
			string memberName;
			bool res = pStruct->getMemberFirst(&memberName, &memberInfo);
			auto& params = paramCollector->getParams();
			auto iter = params.begin();

			// break assigment for each member of the struct
			while (res && iter != params.end()) {
				auto& paramUnit = *iter;
				auto& argumentType = memberInfo.type;
				auto& paramType = paramUnit->getReturnType();

				// create a variable that hold member offset in struct...
				auto memberVariable = scope->registMemberVariable(pVariable, pVariable->getName() + "." + memberName);
				memberVariable->setDataType(argumentType);
				memberVariable->setOffset(memberInfo.offset);

				CXOperand* pCXMemberOperand = new CXOperand(scope, memberVariable, memberVariable->getDataType());
				ExecutableUnitRef memberVariableRef(pCXMemberOperand);

				// keep origin source char in new expression unit
				pCXMemberOperand->setSourceCharIndex(paramUnit->getSourceCharIndex());

				if (!applyTranformTypeUnit(memberVariableRef, paramUnit)) {
					return false;
				}

				res = pStruct->getMemberNext(&memberName, &memberInfo);
				iter++;
			}

			while (res) {
				auto& argumentType = memberInfo.type;

				// create a variable that hold member offset in struct...
				auto memberVariable = scope->registMemberVariable(pVariable, pVariable->getName() + "." + memberName);
				memberVariable->setDataType(argumentType);
				memberVariable->setOffset(memberInfo.offset);

				// run default constructor for the left member if it has
				auto defaultConstructorId = getDefaultConstructor(argumentType.iType());
				if (defaultConstructorId >= 0) {
					auto defaultConstructor = scope->generateDefaultAutoOperator(defaultConstructorId, memberVariable);
					// keep origin source char in new expression unit
					defaultConstructor->setSourceCharIndex(variableUnit->getSourceCharIndex());

					assigments.emplace_back(make_pair(memberVariable, defaultConstructor));
				}
				else {
					// element has constructor but don't have default constructor
					// so, the engine don;t know how to initialize the left elements
					// which have not initialized yet
					if (hasConstructor(argumentType.iType())) {
						return false;
					}
				}

				res = pStruct->getMemberNext(&memberName, &memberInfo);
				iter++;

				accurative += lackOfInitialValueAccurative;
			}

			return true;
		}
		// check if data type of variable is a static array
		if (param1Type.iType() & DATA_TYPE_ARRAY_MASK) {
			auto arrayInfo = (StaticArrayInfo*)getTypeInfo(param1Type.origin());
			if (arrayInfo == nullptr) {
				return false;
			}
			auto argumentType = ScriptType::buildScriptType(this, arrayInfo->elmType, arrayInfo->refLevel);

			int elmIdx = 0;
			int elmOffset = 0;
			auto& elmCount = arrayInfo->elmCount;

			auto& params = paramCollector->getParams();
			for (auto it = params.begin(); it != params.end() && elmIdx < elmCount; it++, elmIdx++) {
				auto& paramUnit = *it;
				auto& paramType = paramUnit->getReturnType();

				// create a variable that hold member offset in array...
				auto memberVariable = scope->registMemberVariable(pVariable, pVariable->getName() + "[" + std::to_string(elmIdx) + "]");
				memberVariable->setDataType(argumentType);
				memberVariable->setOffset(elmOffset);

				CXOperand* pCXMemberOperand = new CXOperand(scope, memberVariable, memberVariable->getDataType());
				ExecutableUnitRef memberVariableRef(pCXMemberOperand);

				// keep origin source char in new expression unit
				pCXMemberOperand->setSourceCharIndex(paramUnit->getSourceCharIndex());

				if (!applyTranformTypeUnit(memberVariableRef, paramUnit)) {
					return false;
				}

				elmOffset += arrayInfo->elmSize;
			}

			auto defaultConstructorId = getDefaultConstructor(argumentType.iType());

			// run default constructor for the left elements if it has
			if (defaultConstructorId >= 0) {
				for (; elmIdx < elmCount; elmIdx++) {
					// create a variable that hold member offset in array...
					auto memberVariable = scope->registMemberVariable(pVariable, pVariable->getName() + "[" + std::to_string(elmIdx) + "]");
					memberVariable->setDataType(argumentType);
					memberVariable->setOffset(elmOffset);

					auto defaultConstructor = scope->generateDefaultAutoOperator(defaultConstructorId, memberVariable);
					// keep origin source char in new expression unit
					defaultConstructor->setSourceCharIndex(variableUnit->getSourceCharIndex());

					assigments.emplace_back(make_pair(memberVariable, defaultConstructor));
					elmOffset += arrayInfo->elmSize;
				}
			}
			else if (elmIdx < elmCount) {
				// element has constructor but don't have default constructor
				// so, the engine don;t know how to initialize the left elements
				// which have not initialized yet
				if (hasConstructor(argumentType.iType())) {
					return false;
				}
			}
			if (elmIdx < elmCount) {
				accurative += (elmCount - elmIdx) * lackOfInitialValueAccurative;
			}

			return true;
		}

		return false;
	}

	template <class Container>
	void simpleFilter(ScriptCompiler* scriptCompiler, const Container& parameterUnits, list<CandidateInfo>& overloadingCandidates, bool forToSearchMatchingLevel2) {
		ScriptType refVoidType(scriptCompiler->getTypeManager()->getBasicTypes().TYPE_VOID | DATA_TYPE_POINTER_MASK, "ref void");

		auto pit = parameterUnits.begin();
		int n = (int)parameterUnits.size();
		int overloadingSize = (int)overloadingCandidates.size();
		for (int i = 0; i < n ; i++, pit++) {
			auto& param = *pit;
			auto& paramType = param->getReturnType();
			auto it = overloadingCandidates.begin();
			decltype(it) itTemp;

			while (it != overloadingCandidates.end()) {
				auto& argumentType = *(it->item->paramTypes[i]);
				auto& paramCasting = it->paramCasting.at(i);
				if (scriptCompiler->findMatchingComposite(argumentType, param, paramCasting)) {
					++it;
				}
				else if (scriptCompiler->findMatching(refVoidType, argumentType, paramType, paramCasting, forToSearchMatchingLevel2 ? true : overloadingSize == 1)) {
					// keep origin source char in new expression unit
					if (paramCasting.castingFunction) {
						paramCasting.castingFunction->setSourceCharIndex(param->getSourceCharIndex());
					}
					++it;
				}
				else {
					itTemp = it;
					++it;
					overloadingCandidates.erase(itTemp);
				}
			}
		}
	}

	CandidateCollectionRef ScriptCompiler::filterCandidate(
		const string& functionName, int functionType,
		const list<OverLoadingItem>* overloadingFuncs,
		const std::vector<CandidateCollectionRef>& candidatesForParams, EExpressionResult& eResult) {

		int n = (int)candidatesForParams.size();
		ParamCastingInfo paramInfoTemp;
		paramInfoTemp.accurative = 0;
		paramInfoTemp.castingFunction = nullptr;

		list<CandidateInfo> overloadingCandidatesOrigin;

		//filter overloading functions by number of parameter
		for (auto it = overloadingFuncs->begin(); it != overloadingFuncs->end(); ++it) {
			if ((*it).paramTypes.size() == (size_t)n) {
				const OverLoadingItem& item = *it;

				CandidateInfo candidateInfo;
				overloadingCandidatesOrigin.push_back(candidateInfo);
				CandidateInfo& candidateInfoRef = overloadingCandidatesOrigin.back();
				candidateInfoRef.item = &item;
				candidateInfoRef.paramCasting.resize(n, paramInfoTemp);
			}
		}

		std::list<std::vector<ExecutableUnitRef>> paramPaths;
		listPaths<ExecutableUnitRef, CandidateCollection, ExecutableUnitRef>(candidatesForParams, paramPaths);

		std::map<int, CandidatePathInfo> candidateMap;
		for (auto pit = paramPaths.begin(); pit != paramPaths.end(); pit++) {
			auto& path = *pit;
			//prevent following expression to be executed by a native assigment function
			// int b = 1;
			// ref int a = b;
			// int& a = b;
			//but it will be processed by default assignment operator
			if (functionType == EXP_UNIT_ID_OPERATOR_ASSIGNMENT) {
				auto& param1 = path[0];
				auto& param2 = path[1];
				auto& param1Type = param1->getReturnType();
				auto& param2Type = param2->getReturnType();
				if ((param1->getType() == EXP_UNIT_ID_XOPERAND && param2Type.origin() == param1Type.origin() && param1Type.refLevel() - param2Type.refLevel() == 1) ||
					((param1->getMask() & UMASK_DECLAREINEXPRESSION) && param1Type.isSemiRefType() && param1->getType() == EXP_UNIT_ID_XOPERAND && param2->getType() == EXP_UNIT_ID_XOPERAND)
					) {
					//continue
					//following assigment is not allow
					//ref int b;
					//int a;
					//b = a;
					continue;
				}
			}

			list<CandidateInfo> overloadingCandidates = overloadingCandidatesOrigin;
			simpleFilter(this, path, overloadingCandidates, false);
			if (overloadingCandidates.size() == 0) {
				// when the code reach here, it means no operator found if we don't try to search matching level 2
				if (functionType == EXP_UNIT_ID_OPERATOR_ASSIGNMENT) {
					auto& param1 = path[0];
					auto& param2 = path[1];
					auto& param1Type = param1->getReturnType();
					auto& param2Type = param2->getReturnType();
					if (param1Type.iType() == param2Type.iType()) {
						// now, it means no assiment operator defined for the given type
						// for this case we don't try to search the candicate with matching level 2
						// because we use default assigment operator
						continue;
					}
				}
				overloadingCandidates = overloadingCandidatesOrigin;
				simpleFilter(this, path, overloadingCandidates, true);
			}

			//copy candidate to map but no duplicate candidate(function) id
			for (auto cit = overloadingCandidates.begin(); cit != overloadingCandidates.end(); cit++) {
				CandidatePathInfo candidate;
				candidate.candidate = *cit;
				candidate.paramPath = &path;
				auto it = candidateMap.insert(std::make_pair(candidate.candidate.item->functionId, candidate));
				CandidatePathInfo& insertedCandidate = it.first->second;
				//if the canidate id is exits, compare accurative to choose the best one
				if (it.second == false) {
					//calcuate total accurative for each
					int acc1 = insertedCandidate.candidate.totalAccurative;
					int acc2 = 0;
					for (int j = 0; j < n; j++) {
						acc2 += candidate.candidate.paramCasting.at(j).accurative;
					}
					//the better found, so we repelace it
					if (acc1 > acc2) {
						candidate.candidate.totalAccurative = acc2;
						it.first->second = candidate;
					}
				}
				else {
					int acc1 = 0;
					for (int j = 0; j < n; j++) {
						acc1 += insertedCandidate.candidate.paramCasting.at(j).accurative;
					}
					insertedCandidate.candidate.totalAccurative = acc1;
				}
			}
		}

		//now we have all candidate, we remove duplicate by function id
		//but if two function return the same type, it is an ambious call.
		//so, now we check ambious call
		for (auto it = candidateMap.begin(); it != candidateMap.end(); it++) {
			FunctionFactory* fp1 = getFunctionFactory(it->first);
			auto jt = it;
			for (++jt; jt != candidateMap.end(); jt++) {

				FunctionFactory* fp2 = getFunctionFactory(jt->first);
				if (fp1->getReturnType() == fp2->getReturnType()) {
					eResult = E_TYPE_AMBIOUS_CALL;
					setErrorText("ambious call for function " + functionName);
					return nullptr;
				}
			}
		}
		vector<std::pair<const int, CandidatePathInfo>*> candidateElems(candidateMap.size());
		auto elmIt = candidateElems.begin();
		for (auto it = candidateMap.begin(); it != candidateMap.end(); it++) {
			*elmIt++ = &(*it);
		}
		std::sort(candidateElems.begin(), candidateElems.end(),
			[](std::pair<const int, CandidatePathInfo>* elm1, std::pair<const int, CandidatePathInfo>* elm2) {
			return elm1->second.candidate.totalAccurative < elm2->second.candidate.totalAccurative;
		});

		//everything is ok now, it's time to build candidates		
		CandidateCollectionRef functionCandidates = std::make_shared<CandidateCollection>();
		for (auto it = candidateElems.begin(); it != candidateElems.end(); it++) {
			FunctionFactory* fp = getFunctionFactory((*it)->first);
			Function* f = fp->build(fp->getName());
			CandidatePathInfo& candidate = (*it)->second;

			std::vector<ExecutableUnitRef>& paramPath = *(candidate.paramPath);
			ParamCastingList& paramCastingList = candidate.candidate.paramCasting;

			auto& argTypes = candidate.candidate.item->paramTypes;

			for (int i = 0; i < n; i++) {
				FunctionRef& castingF = paramCastingList[i].castingFunction;
				ExecutableUnitRef& param = paramPath[i];
				if (castingF == nullptr) {
					f->pushParam(param);
				}
				else {
					auto paramTemp = param;
					applyCastingAndOptimize(paramTemp, castingF);
					paramTemp->setReturnType(*argTypes[i]);

					f->pushParam(paramTemp);
				}
			}

			functionCandidates->push_back(ExecutableUnitRef(f));
		}

		return functionCandidates;
	}

	bool ScriptCompiler::registFunctionOperator(int type, int functionId) {
		auto functionFactory = getFunctionFactory(functionId);
		if (functionFactory == nullptr) {
			this->setErrorText("operator is not found");
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, L"operator is not found");
			return false;
		}
		bool isDynamicFunction = findDynamicFunctionOnly(functionFactory->getName()) == functionId;
		if (isDynamicFunction == false) {
			if (functionFactory->getParamCount() < 1) {
				this->setErrorText("operator is specified to an incompatible function");
				LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("non parameter function '%s' is not applicable for 'operator()'", functionFactory->getName()));
				return false;
			}

			ScriptType& paramTpe1 = functionFactory->getParamType(0);
			if (paramTpe1.refLevel() > 1 || paramTpe1.origin() != type) {
				auto sType = getType(type);
				this->setErrorText("operator is specified to an incompatible function");
				LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("operator() for type %s is specified to an incompatible function '%s'", sType.c_str(), functionFactory->getFullFuntionName().c_str()));
				LOG_COMPILE_MESSAGE(_logger, MESSAGE_INFO, L"expected type of the first parameter of operator() must be same origin type and not a pointer level 2 or greater");
				return false;
			}
		}

		_functionCallMap[type] = functionId;
		return true;
	}

	int ScriptCompiler::getFunctionOperator(int type) {
		auto it = _functionCallMap.find(type);
		if (it == _functionCallMap.end()) {
			return - 1;
		}

		return it->second;
	}

	bool ScriptCompiler::registTypeInfo(int type, MemoryBlockRef typeInfoRef) {
		return _typeManagerRef->registTypeInfo(type, typeInfoRef);
	}

	void* ScriptCompiler::getTypeInfo(int type) {
		return _typeManagerRef->getTypeInfo(type);
	}

	Program* ScriptCompiler::bindProgram(Program* program) {
		Program* lastProgram = _program;
		_program = program;
		return lastProgram;
	}

	Program* ScriptCompiler::getProgram() const {
		return _program;
	}

	void ScriptCompiler::beginUserLib() {
		_functionLibRef->beginUserLib();
		_typeManagerRef->beginUserTypes();

		_systemLibMarkEnd = (LibraryMarkInfoRef)(new LibraryMarkInfo);
		_systemLibMarkEnd->functionIdx = (int)_functionFactories.size();		
	}

	void ScriptCompiler::clearUserLib() {
		_functionLibRef->clearUserLib();
		_typeManagerRef->clearUserTypes();

		if (_systemLibMarkEnd) {

			for (int funcId = _systemLibMarkEnd->functionIdx; funcId < (int)_functionFactories.size(); funcId++) {
				unregisterFunction(funcId);
			}

			_functionFactories.resize(_systemLibMarkEnd->functionIdx);
			_systemLibMarkEnd.reset();
		}
	}

	void ScriptCompiler::takeOwnership(FunctionFactory* factory) {
		_factoriesStorage.push_back((FunctionFactoryRef)(factory) );
	}

	const FuncLibraryRef& ScriptCompiler::getFunctionLib() const {
		return _functionLibRef;
	}

	const TypeManagerRef& ScriptCompiler::getTypeManager() const {
		return _typeManagerRef;
	}

	TemplateRef ScriptCompiler::registTemplate(const std::string& name, const vector<std::string>& args) {
		string keyStr = name + "_" + std::to_string( (int) args.size() );
		TemplateRef dummy;
		auto it = _templates.insert(std::make_pair( keyStr, dummy) );
		if (it.second) {
			auto& templateRef = it.first->second;
			templateRef.reset( new Template(name) );

			for (auto it = args.begin(); it != args.end(); it++) {
				templateRef->addArg(*it);
			}

			return templateRef;
		}

		return nullptr;
	}

	TemplateRef ScriptCompiler::findTemplate(const std::string& name, int argCount) {
		string keyStr = name + "_" + std::to_string(argCount);
		TemplateRef dummy;
		auto it = _templates.find(keyStr);
		if (it == _templates.end()) {
			return nullptr;
		}

		return it->second;
	}

	void ScriptCompiler::setConstantMap(const string& constantName, const DFunctionRef& createConstantObjFunc) {		
		auto it = _constantMap.insert( std::make_pair(constantName, createConstantObjFunc) );
		//overwrite if constant name is existed
		if (it.second == false) {
			it.first->second = createConstantObjFunc;
		}
	}

	DFunctionRef ScriptCompiler::findConstantMap(const string& constantName) const {
		auto it = _constantMap.find(constantName);
		if (it != _constantMap.end()) {
			return it->second;
		}

		return nullptr;
	}

	int ScriptCompiler::buildFunctionType(const OverLoadingItem& item) {
		auto functionFactory = getFunctionFactory(item.functionId);

		string stype(FUNCTOR_SIGN "<");
		stype.append(functionFactory->getReturnType().sType());
		if (item.mask & ITEM_MASK_DYNAMIC_FUNCTION) {
			stype.append("(...)");
		}
		else
		{
			stype.append(1, '(');
			auto& paramTypes = item.paramTypes;
			for (auto it = paramTypes.begin(); it != paramTypes.end();) {
				stype.append((*it)->sType());
				it++;
				if (it != paramTypes.end()) {
					stype.append(", ");
				}
			}
			stype.append(1, ')');
		}
		stype.append(1, '>');
		
		return registFunctionType(stype);
	}

	const wchar_t* ScriptCompiler::parseFunctionType(const wchar_t* text, const wchar_t* end, ScriptType& returnType, std::list<ScriptType>& argTypes, bool& isDynamicFunction) {
		auto c = trimLeft(text, end);
		if (*c != '<') return nullptr;
		c = readType(c + 1, end, returnType);
		if (c == nullptr) return nullptr;

		c = trimLeft(c, end);
		if (c >= end || *c != '(') {
			return nullptr;
		}
		c++;		
		isDynamicFunction = false;
		while (c < end)
		{
			c = trimLeft(c, end);
			if (*c == ')') break;
			if (*c == ',') c++;
			
			argTypes.push_back(ScriptType());
			auto& argType = argTypes.back();
			auto d = readType(c, end, argType);
			if (d == nullptr) {
				//check if the argument type is dynamic format
				if (argTypes.size() != 0) return nullptr;
				if (end - c < 3) return nullptr;
				if (*c++ != '.') return nullptr;
				if (*c++ != '.') return nullptr;
				if(*c++ != '.') return nullptr;
				isDynamicFunction = true;

				d = c;
			}
			c = d;
			c = trimLeft(c, end);
			if (*c == ')') {
				break;
			}
			//ignore argument name then read to end
			c = lastCharInToken(c, end);
		}
		c++;
		c = trimLeft(c, end);
		if (*c != '>') return nullptr;
		c++;
		return c;
	}

	int ScriptCompiler::registFunctionType(const std::string& functionType) {
		int iType = _typeManagerRef->registType(functionType, DATA_TYPE_FUNCTION_MASK);
		if (iType == DATA_TYPE_INVALID) {
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_INFO, formatMessage("cannot register function type '%s'", functionType.c_str()));
			return iType;
		}
		else if (IS_UNKNOWN_TYPE(iType)) {
			iType = getType(functionType);
		}
		else {
			setTypeSize(iType, sizeof(RuntimeFunctionInfo));

			// register default constructor/destructor
			registConstructor(iType, getRuntimeInfoConstructor());
			registDestructor(iType, getRuntimeInfoDestructor());

			// register copy constructor
			string copyArgs("ref ");
			copyArgs.append(getType(iType));
			copyArgs.append(", ");
			copyArgs.append(getType(iType));
			copyArgs.append("&");

			FunctionRegisterHelper fb(this);
			int f = fb.registFunction(SYSTEM_FUNCTION_COPY_CONSTRUCTOR, copyArgs, new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", createFunctionCdecl<void, RuntimeFunctionInfo*, RuntimeFunctionInfo*>(runtimeFunctionInfoCopyConstructor), this), true);
			if (!registBinaryConstructor(iType, f)) {
				//throw exception("Cannot register copy constructor for function object");
				LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("Cannot register copy constructor for function object '%s'", functionType.c_str()));
				return -1;
			}

			copyArgs = "ref ";
			copyArgs.append(getType(iType));
			copyArgs.append(", " SYSTEM_NULL_TYPE);

			f = fb.registFunction("_initize_by_null", copyArgs, new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void",
				createFunctionCdecl<void, RuntimeFunctionInfo*, void*>(runtimeFunctionInfoConstructByNull), this), true);
			if (!registConstructor(iType, f)) {
				//throw exception("Cannot register initliaze constructor to null for function object");
				LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage("Cannot register initliaze constructor to null for function object '%s'", functionType.c_str()));
				return -1;
			}
		}

		return iType;
	}

	int ScriptCompiler::registArrayType(const std::wstring& arrayType) {
		auto c = arrayType.c_str();
		auto end = c + arrayType.size();
	
		constexpr int array_sign_size = sizeof(ARRAY_SIGN) - 1;
		c = trimLeft(c, end);
		if (wcsncmp(c, WIDEN(ARRAY_SIGN), array_sign_size)) {
			setErrorText("Invalid array type");
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage(L"Invalid array type '%s'", arrayType.c_str()));
			return DATA_TYPE_UNKNOWN;
		}
		c += array_sign_size;
		ScriptType elmType;
		vector<int> dimensions;
		if (parseArrayType(c, end, elmType, dimensions) == nullptr) {
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage(L"Invalid array type '%s': %s", arrayType.c_str(), convertToWstring(getLastError()).c_str()));
			return DATA_TYPE_UNKNOWN;
		}

		int iType = registArrayType(elmType, dimensions);
		if (IS_UNKNOWN_TYPE(iType)) {
			LOG_COMPILE_MESSAGE(_logger, MESSAGE_WARNING, formatMessage(L"Invalid array type '%s': %s", arrayType.c_str(), convertToWstring(getLastError()).c_str()));
		}
		return iType;
	}

	int ScriptCompiler::registArrayType(const ScriptType& elmType, const std::vector<int>& dimensions) {
		if (dimensions.size() == 0) {
			setErrorText("ilegal array dimension");
			return DATA_TYPE_UNKNOWN;
		}

		int iElmType = elmType.iType();
		if (elmType.isUnkownType()) {
			setErrorText("unknown data type " + elmType.sType());
			return DATA_TYPE_UNKNOWN;
		}

		int iType;
		int dim;
		auto refLevel = (char)elmType.refLevel();		

		int i = (int)dimensions.size();

		for (i--; i >= 0; i--) {
			// build array type in string
			std::string arrayType(ARRAY_SIGN "<");
			arrayType.append(elmType.sType());
			for (auto j = i; j < (int)dimensions.size(); j++) {
				arrayType.push_back(',');
				arrayType.append(std::to_string(dimensions[j]));
			}
			arrayType.push_back('>');

			// register array type
			iType = registType(arrayType, DATA_TYPE_ARRAY_MASK);
			// check if the type is already registered...
			if (iType == DATA_TYPE_UNKNOWN) {
				// ...then just get the type id
				iType = getType(arrayType);
			}
			else {
				// ...register other stuffs for the type

				// compute array size
				int arraySizeInBytes = dimensions[i] * getTypeSize(iElmType);

				// register type size
				try {
					setTypeSize(iType, arraySizeInBytes);
				}
				catch(std::exception& e) {
					setErrorText(e.what());
					return DATA_TYPE_UNKNOWN;
				}
				dim = (int)(dimensions.size() - i);
				int arrayInfoSize = sizeof(StaticArrayInfo);
				auto arrayTypeInfoBlock = std::make_shared<BufferBlock>(arrayInfoSize);

				auto pArrayInfo = (StaticArrayInfo*)arrayTypeInfoBlock->getDataRef();
				pArrayInfo->dim = (unsigned char)dim;
				pArrayInfo->elmSize = getTypeSize(iElmType);
				pArrayInfo->elmType = iElmType;
				pArrayInfo->elmCount = dimensions[i];
				pArrayInfo->refLevel = refLevel;
				pArrayInfo->reserved2 = 0;


				registTypeInfo(iType, arrayTypeInfoBlock);
			}

			refLevel = 0;
			iElmType = iType;
		}

		return iType;
	}

	bool ScriptCompiler::parseFunctionType(int type, ScriptType& returnType, std::list<ScriptType>& argTypes, bool& isDynamicFunction) {
		std::string stype = getType(type);
		std::wstring wstype(stype.begin(), stype.end());
		const wchar_t* text = wstype.data();
		const wchar_t* end =  text + wstype.size();
		auto c = trimLeft(text, end);
		size_t idx1 = stype.find(FUNCTOR_SIGN, c -  text);
		if (idx1 != 0) {
			return false;
		}
		idx1 += sizeof(FUNCTOR_SIGN) - 1;

		c = parseFunctionType(c + idx1, end, returnType, argTypes, isDynamicFunction);

		return c != nullptr;
	}

	const wchar_t* ScriptCompiler::parseArrayType(const wchar_t* text, const wchar_t* end, ScriptType& elmType, std::vector<int>& dimensions) {
		auto c = trimLeft(text, end);
		if (*c != '<') {
			setErrorText("error array type");
			return nullptr;
		}
		c = readType(c + 1, end, elmType);
		if (c == nullptr) {
			setErrorText("error array type");
			return nullptr;
		}
		decltype(c) e;
		std::string token;
		int size;

		while (c < end)
		{
			c = trimLeft(c, end);
			if (*c == '>') break;
			if (*c != ',') {
				setErrorText("error type");
				return nullptr;
			}
			c++;
			c = trimLeft(c, end);
			e = lastCharInToken(c, end);
			if (c == e) return nullptr;

			size = 0;
			// check and convert the string to a integer
			for (; c < e; c++ ) {
				if (!isdigit(*c)) {
					if(c == e -1 && *c == '-') {
						setErrorText("array size must be a positive number");
					}
					else if (c == e - 1 && *c == '+') {
						break;
					}
					else {
						setErrorText("array size must be a constant integer");
					}
					return nullptr;
				}
				size = size * 10 + (int) (*c - '0');
			}
			////token 1 is expected as a integer
			//token = convertToAscii(d, e - d);
			//size = atoi(token.c_str());
			//if (size <= 0) {
			//	setErrorText("array size must be positive");
			//	return nullptr;
			//}
			dimensions.push_back(size);
		}
		if (*c != '>') {
			setErrorText("error array type");
			return nullptr;
		}
		c++;
		return c;
	}

	const wchar_t* ScriptCompiler::readType(const wchar_t* text, const wchar_t* end, ScriptType& stype) {
		const wchar_t* d = trimLeft(text, end);
		const wchar_t* e = d;
		const wchar_t* c;
		const wchar_t* endOfRef = nullptr;
		std::string token1;
		std::string subOfRef;
		int iType = DATA_TYPE_UNKNOWN;
		int mask = 0;

		ScopeAutoFunction updateScriptType([&token1, &subOfRef, &iType, &stype]() {
			stype.setType(iType);
			string typeStr = subOfRef;
			typeStr.append(token1);
			stype.setTypeStr(typeStr);
		});

		do {
			if (!checkBeginToken(*e)) return nullptr;

			c = lastCharInToken(e, end);
			if(c == e) return nullptr;
			//token 1 is expected as a data type
			token1 = convertToAscii(e, c - e);
			if (!isRefSign(token1)) {
				if (endOfRef) {
					subOfRef = convertToAscii(d, endOfRef - d);
					//add one space
					subOfRef.append(1, ' ');
				}
				break;
			}
			else {
				endOfRef = c;
				mask = DATA_TYPE_POINTER_MASK;
			}
			e = trimLeft(c, end);
		} while (e < end);

		if (e >= end) return c;

		if (token1 == FUNCTOR_SIGN) {
			ScriptType returnType;
			std::list<ScriptType> argTypes;
			bool isDynamicFunction;
			e += token1.length();
			c = parseFunctionType(e, end, returnType, argTypes, isDynamicFunction);
			if (c == nullptr) return nullptr;
			
			token1.append(1, '<');
			token1.append(returnType.sType());
			if (isDynamicFunction) {
				token1.append("(...)");
			}
			else {
				auto prev_end = argTypes.end();
				if (argTypes.begin() != prev_end) {
					prev_end--;
					token1.append(1, '(');
					for (auto it = argTypes.begin();; it++) {
						token1.append(it->sType());
						if (it == prev_end) {
							break;
						}
						token1.append(1, ',');
					}
					token1.append(1, ')');
				}
				else {
					token1.append("()");
				}
			}

			token1.append(1, '>');
			if( IS_UNKNOWN_TYPE(iType = registFunctionType(token1))) {
				return nullptr;
			}
		}
		else if (token1 == ARRAY_SIGN) {
			ScriptType elmType;
			std::vector<int> dimensions;
			
			e += token1.length();
			c = parseArrayType(e, end, elmType, dimensions);
			if (c == nullptr) return nullptr;
			iType = registArrayType(elmType, dimensions);
			if (iType == DATA_TYPE_UNKNOWN) {
				return nullptr;
			}

			token1 = getType(iType);
		}
		else {
			iType = getType(token1);
		}

		if (c < end) {
			auto d = trimLeft(c, end);
			if (*d == '&') {
				token1.push_back('&');
				c = d + 1;

				mask |= DATA_TYPE_REF_MASK;
			}
		}

		iType |= mask;

		return c;
	}

	const wchar_t* ScriptCompiler::formatMessage(const wchar_t* format, ...) {
		va_list args;
		va_start(args, format);
		_vsnwprintf(_messageBuffer.data(), _messageBuffer.size(), format, args);
		va_end(args);

		return _messageBuffer.data();
	}

	const wchar_t* ScriptCompiler::formatMessage(const char* format, ...) {
		std::vector<char> buffer(_messageBuffer.size());

		va_list args;
		va_start(args, format);
		_vsnprintf(buffer.data(), buffer.size(), format, args);
		va_end(args);

		_messageBuffer.assign(buffer.begin(), buffer.end());
		return _messageBuffer.data();
	}
}
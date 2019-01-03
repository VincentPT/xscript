/******************************************************************
* File:        FunctionRegisterHelper.cpp
* Description: implement FunctionRegisterHelper class. A class used
*              to help users register the function in a script
*              program.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "FunctionRegisterHelper.h"
#include "ScriptCompiler.h"
#include "FunctionFactory.h"
#include "function/DynamicFunction2.h"
#include "BasicFunctionFactory.hpp"

namespace ffscript {
	FunctionRegisterHelper::FunctionRegisterHelper(ScriptCompiler* scriptCompiler) : _scriptCompiler(scriptCompiler){}

	FunctionRegisterHelper::~FunctionRegisterHelper(){}

	void FunctionRegisterHelper::addFactory(FunctionFactory* factory, bool autoDelete ) {
		if (autoDelete) {
			_scriptCompiler->takeOwnership(factory);
		}
	}

	int FunctionRegisterHelper::registFunction(const std::string& name, const std::string& functionParams,
		FunctionFactory* factory, bool autoDelete) {
		int functionId = _scriptCompiler->registFunction(name, functionParams, factory);
		addFactory(factory, autoDelete);
		return functionId;
	}

	int FunctionRegisterHelper::registDynamicFunction(const std::string & name, FunctionFactory * factory, bool autoDelete)
	{
		int functionId = _scriptCompiler->registDynamicFunction(name, factory);
		addFactory(factory, autoDelete);
		return functionId;
	}

	int FunctionRegisterHelper::registPredefinedOperators(const std::string& name, const std::string& functionParams, const std::string& returnType, DFunction2* nativeFunction) {
		auto operatorEntry = _scriptCompiler->findPredefinedOperator(name);
		if (operatorEntry) {
			FunctionFactory* factory = nullptr;
			if (operatorEntry->maxParam == 1) {
				factory = new BasicFunctionFactory<1>(operatorEntry->operatorType, operatorEntry->priority, returnType.c_str(), nativeFunction, _scriptCompiler);
			}
			else {
				factory = new BasicFunctionFactory<2>(operatorEntry->operatorType, operatorEntry->priority, returnType.c_str(), nativeFunction, _scriptCompiler);
			}
			if (operatorEntry->nameInExpression) {
				return registFunction(operatorEntry->nameInExpression, functionParams, factory);
			}
			return registFunction(name, functionParams, factory);
		}
		return -1;
	}

	int FunctionRegisterHelper::registFunctions(FactoryItem* factories, int n) {
		
		FactoryItem* pItem = factories;
		FactoryItem* pItemMax = pItem + n;

		n = 0;
		while (pItem < pItemMax)
		{
			if (_scriptCompiler->registFunction(pItem->functionName, pItem->functionParams, pItem->factory) >= 0)
				n++;
			addFactory(pItem->factory, pItem->autoDelete);
			pItem++;
		}
		return n;
	}

	int FunctionRegisterHelper::registPredefinedOperators(PredefinedOperator* operators, int n) {
		PredefinedOperator* pItem = operators;
		PredefinedOperator* pItemMax = pItem + n;

		n = 0;
		while (pItem < pItemMax)
		{
			if (registPredefinedOperators(pItem->name, pItem->functionParams, pItem->returnType, pItem->nativeFunction) >= 0)
				n++;
			pItem++;
		}
		return n;
	}

	ScriptCompiler* FunctionRegisterHelper::getSriptCompiler() const {
		return _scriptCompiler;
	}

	int FunctionRegisterHelper::registerUserType(const std::string& type, unsigned int size) {
		int typeId = _scriptCompiler->registType(type);
		if (IS_UNKNOWN_TYPE(typeId)) {
			return typeId;
		}

		_scriptCompiler->setTypeSize(typeId, (int)size);
		return typeId;
	}

	int FunctionRegisterHelper::registerTypeAutoOperator(int typeId, const std::string& functionParams, FunctionFactory* factory, bool autoDelete, bool constructor) {
		if ((typeId & DATA_TYPE_ORIGIN_MASK) != typeId) {
			// type is not a raw type
			return -1;
		}
		std::string functionNamePrefix(constructor ?  "_ctor_" : "_dtor_");
		auto typeName = _scriptCompiler->getType(typeId);
		functionNamePrefix.append(typeName);
		int functionId = registFunction(functionNamePrefix, functionParams, factory, autoDelete);
		if (functionId < 0) {
			functionNamePrefix.append(1, '_');
			for (int i = 0; functionId < 0; i++) {
				functionId = registFunction(functionNamePrefix + std::to_string(i), functionParams, factory, autoDelete);
			}
		}

		bool res = constructor ? _scriptCompiler->registConstructor(typeId, functionId) :
			_scriptCompiler->registDestructor(typeId, functionId);

		if (res == false) {
			_scriptCompiler->unregisterFunction(functionId);
			functionId = -1;
		}
		return functionId;
	}

	int FunctionRegisterHelper::registerConstructor(int typeId, const std::string& functionParams, FunctionFactory* factory, bool autoDelete) {
		return registerTypeAutoOperator(typeId, functionParams, factory, autoDelete, true);
	}

	int FunctionRegisterHelper::registerDestructor(int typeId, FunctionFactory* factory, bool autoDelete) {
		ScriptType scriptType(typeId, _scriptCompiler->getType(typeId));
		scriptType = scriptType.makeRef();
		return registerTypeAutoOperator(typeId, scriptType.sType(), factory, autoDelete, false);
	}
}

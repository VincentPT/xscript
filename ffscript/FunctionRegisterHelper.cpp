/******************************************************************
* File:        FunctionRegisterHelper.cpp
* Description: implement FunctionRegisterHelper class. A class used
*              to help users register the function in a script
*              program.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "FunctionRegisterHelper.h"
#include "ScriptCompiler.h"
#include "FunctionFactory.h"
#include "function\DynamicFunction2.h"
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
}

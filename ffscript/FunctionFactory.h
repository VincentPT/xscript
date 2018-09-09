/******************************************************************
* File:        FunctionFactory.h
* Description: declare FunctionFactory class. A class used to create
*              a expression unit instance, not any expression unit
*              but function unit. A function unit is a expression
*              unit, it can either be a operator(such as '+') or a
*              function(such as 'sin').
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include <vector>
#include <string>
#include "ScriptType.h"

namespace ffscript {
	class Function;
	class ScriptCompiler;

	class FunctionFactory {
		std::vector<ScriptType> _registeredParamTypes;
		ScriptType _returnType;
		int _functionId;
		std::string _functionName;
		int _paramSize;
		ScriptCompiler* _scriptCompiler;
	public:
		FunctionFactory(const char* functionName, ScriptCompiler* scriptCompiler);
		FunctionFactory();
		virtual ~FunctionFactory();
		virtual Function* createFunction(const std::string& name, int id) = 0;
		virtual Function* build(const std::string& name);

		virtual void addParamType(const ScriptType& paramType);
		virtual ScriptType& getParamType(int i);
		int getParamCount() const;
		const char* getName() const;
		void setName(const char* functionName);
		void setFunctionId(int functionId);
		int getFunctionId() const;
		void setReturnType(const ScriptType& returnType);
		const ScriptType& getReturnType() const;
		int getParamsDataSize() const;
		ScriptCompiler* getCompiler() const;
		void setCompiler(ScriptCompiler* pCompiler);
		std::string getFullFuntionName() const;
	};
}
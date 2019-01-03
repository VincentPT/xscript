/******************************************************************
* File:        Executor.cpp
* Description: implement Executor class. A class contain commands(
*              script code) and data need for the code.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "function/CdeclFunction.hpp"
#include "function/DynamicFunction2.h"
#include "Executor.h"
#include "ScriptScope.h"
#include "Variable.h"
#include "Expression.h"
#include "Context.h"
#include "GlobalScope.h"
#include "ScriptCompiler.h"
#include "BasicType.h"
#include "RefFunction.h"
#include "InstructionCommand.h"

namespace ffscript {

	Executor::Executor(){}
	Executor::~Executor(){}	

	void Executor::runCode() {

		auto it = _commandList.begin();
		auto end = _commandList.end();

		Context* currentContext = Context::getCurrent();

		for (auto it = _commandList.begin(); it != end; ++it) {
			(*it)->execute();
		}
	}

	CommandList* Executor::getCode() {
		return &_commandList;
	}

	void Executor::addCommand(InstructionCommand* commandEntry) {
		_commandList.push_back(commandEntry);
		_commandContainer.push_back(CommandRef(commandEntry));
	}
}
/******************************************************************
* File:        ScriptTask.cpp
* Description: implement ScriptRunner class. A class is desgined to
*              execute a script function through function
*              id. Unlike ScriptRunner, this class will create its
*              own context and running the commands on its context.
*              So, we can use it to run the same code in seperated
*              contexts at the same time. This is main object of
*              this class, running the code in multi-thread.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "ScriptTask.h"
#include "Context.h"
#include "Program.h"
#include "InstructionCommand.h"

namespace ffscript {
	ScriptTask::ScriptTask(Program* program) : _program(program), _scriptContext(nullptr), _allocatedSize(0),
		_scriptRunner(nullptr), _lastCallFunctionId(-1)
	{
	}

	ScriptTask::~ScriptTask(){
		if (_scriptContext) {
			delete _scriptContext;
		}
		if (_scriptRunner) {
			delete _scriptRunner;
		}
	}

	void ScriptTask::runFunction(int stackSize, int functionId, const ScriptParamBuffer& paramBuffer) {
		runFunction(stackSize, functionId, &paramBuffer);
	}

	void ScriptTask::runFunction(int stackSize, int functionId, const ScriptParamBuffer* paramBuffer) {
		if (_scriptRunner == nullptr || _lastCallFunctionId != functionId) {
			if (_scriptRunner) delete _scriptRunner;
			_scriptRunner = new ScriptRunner(_program, functionId);
			_lastCallFunctionId = functionId;
		}

		if (_scriptContext == nullptr) {
			_scriptContext = new Context(stackSize);
		}
		else if (_scriptContext->getMemCapacity() < stackSize) {
			delete _scriptContext;
			_scriptContext = new Context(stackSize);
			_allocatedSize = 0;
		}
		else
		{
			_scriptContext->scopeUnallocate(_allocatedSize, 0);
		}

		Context::makeCurrent(_scriptContext);
		_scriptRunner->runFunction(paramBuffer);
	}

	void ScriptTask::runFunction(int functionId, const ScriptParamBuffer& paramBuffer) {
		runFunction(1024 * 1024, functionId, &paramBuffer);
	}

	void ScriptTask::runFunction(int functionId, const ScriptParamBuffer* paramBuffer) {
		runFunction(1024 * 1024, functionId, paramBuffer);
	}

	void* ScriptTask::getTaskResult() {
		Context::makeCurrent(_scriptContext);
		return _scriptRunner->getTaskResult();
	}
}
#include "stdafx.h"
#include "ScriptTask.h"
#include "Context.h"
#include "Program.h"
#include "InstructionCommand.h"

namespace ffscript {
	ScriptTask::ScriptTask(Program* program) : ScriptRunner(program), _scriptContext(nullptr), _allocatedSize(0)
	{
	}

	ScriptTask::~ScriptTask(){
		if (_scriptContext) {
			delete _scriptContext;
		}
	}

	void ScriptTask::runFunction(int stackSize, int functionId, const ScriptParamBuffer& paramBuffer) {
		runFunction(stackSize, functionId, &paramBuffer);
	}

	void ScriptTask::runFunction(int stackSize, int functionId, const ScriptParamBuffer* paramBuffer) {
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
		ScriptRunner::runFunction(functionId, paramBuffer);
	}

	void ScriptTask::runFunction(int functionId, const ScriptParamBuffer& paramBuffer) {
		runFunction(1024 * 1024, functionId, &paramBuffer);
	}

	void ScriptTask::runFunction(int functionId, const ScriptParamBuffer* paramBuffer) {
		runFunction(1024 * 1024, functionId, paramBuffer);
	}

	void* ScriptTask::getTaskResult() {
		Context::makeCurrent(_scriptContext);
		return ScriptRunner::getTaskResult();
	}
}
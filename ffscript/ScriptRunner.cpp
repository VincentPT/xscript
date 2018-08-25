#include "stdafx.h"
#include "ScriptRunner.h"
#include "Context.h"
#include "Program.h"
#include "InstructionCommand.h"

namespace ffscript {
	static const int s_returnOffset = SCRIPT_FUNCTION_RETURN_STORAGE_OFFSET;

	ScriptRunner::ScriptRunner(Program* program) : _program(program),
		_functionInfo(nullptr), _functionCode(nullptr), _functionId(-1)
	{
	}

	ScriptRunner::~ScriptRunner(){
	}

	void ScriptRunner::runFunction(int functionId, const ScriptParamBuffer* paramBuffer) {
		auto context = Context::getCurrent();

		Program* program = _program;

		if (_functionId != functionId) {
			_functionId = functionId;

			_functionInfo = program->getFunctionInfo(functionId);
			_functionCode = program->getFunctionPlainCode(functionId);
		}

		if (_functionInfo == nullptr) {
			return;
		}
		if (_functionCode == nullptr) {
			return;
		}

		int paramOffset = s_returnOffset + _functionInfo->returnStorageSize;
		if (paramBuffer != nullptr && _functionInfo->paramDataSize > 0) {
			context->write(paramOffset, paramBuffer->getBuffer(), _functionInfo->paramDataSize);
		}

		context->setCurrentCommand(program->getEndCommand() - 1);
		context->setEndCommand(program->getEndCommand());
		auto allocatedSize = _functionInfo->returnStorageSize + _functionInfo->paramDataSize;
		context->scopeAllocate(allocatedSize, 0);

#if USE_DIRECT_COPY_FOR_RETURN
#if USE_FUNCTION_TREE
		CallScriptFuntion3 callScriptCommand;
#else
		CallScriptFuntion2 callScriptCommand;
#endif
		
		callScriptCommand.setCommandData(s_returnOffset, paramOffset, _functionInfo->paramDataSize);		
#else
		CallScriptFuntion callScriptCommand;
		callScriptCommand.setCommandData(_resultSize, paramOffset, functionInfo->paramDataSize);
#endif
		
		callScriptCommand.setTargetCommand(_functionCode->first);
		callScriptCommand.execute();

#if !USE_FUNCTION_TREE
		_scriptContext->run();
#endif
	}

	void* ScriptRunner::getTaskResult() {
		auto context = Context::getCurrent();
		if (context) {
#if USE_DIRECT_COPY_FOR_RETURN
			return context->getAbsoluteAddress(s_returnOffset);
#else
			int functionResultOffset = context->getCurrentOffset() + context->getCurrentScopeSize();
			int targetOffset = context->getCurrentOffset() + s_returnOffset;

			void* sourceAddress = context->getAbsoluteAddress(functionResultOffset);

			context->write(targetOffset, sourceAddress, _resultSize);

			return context->getAbsoluteAddress(s_returnOffset);
#endif
		}
		return nullptr;
	}
}
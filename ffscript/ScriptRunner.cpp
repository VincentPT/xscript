#include "stdafx.h"
#include "ScriptRunner.h"
#include "Context.h"
#include "Program.h"
#include "InstructionCommand.h"

namespace ffscript {
	static const int s_returnOffset = SCRIPT_FUNCTION_RETURN_STORAGE_OFFSET;

	ScriptRunner::ScriptRunner(Program* program, int functionId) : _program(program), _functionInfo(nullptr)
	{
		_functionInfo = program->getFunctionInfo(functionId);
		auto functionCode = program->getFunctionPlainCode(functionId);

		int paramOffset = s_returnOffset + _functionInfo->returnStorageSize;

#if USE_DIRECT_COPY_FOR_RETURN
#if USE_FUNCTION_TREE
		CallScriptFuntion3* callScriptCommand = new CallScriptFuntion3();
#else
		CallScriptFuntion2* callScriptCommand = new CallScriptFuntion2();
#endif

		callScriptCommand->setCommandData(s_returnOffset, paramOffset, _functionInfo->paramDataSize);
#else
		CallScriptFuntion* callScriptCommand = new CallScriptFuntion();
		callScriptCommand.setCommandData(_resultSize, paramOffset, functionInfo->paramDataSize);
#endif
		callScriptCommand->setTargetCommand(functionCode->first);
		_scriptInvoker = callScriptCommand;
	}

	ScriptRunner::~ScriptRunner(){
	}

	void ScriptRunner::runFunction(const ScriptParamBuffer* paramBuffer) {
		auto context = Context::getCurrent();

		Program* program = _program;

		int paramOffset = s_returnOffset + _functionInfo->returnStorageSize;
		if (paramBuffer != nullptr && _functionInfo->paramDataSize > 0) {
			context->write(paramOffset, paramBuffer->getBuffer(), _functionInfo->paramDataSize);
		}

		context->setCurrentCommand(program->getEndCommand() - 1);
		context->setEndCommand(program->getEndCommand());
		auto allocatedSize = _functionInfo->returnStorageSize + _functionInfo->paramDataSize;
		context->scopeAllocate(allocatedSize, 0);

		_scriptInvoker->execute();

#if !USE_FUNCTION_TREE
		_scriptContext->run();
#endif
		context->scopeUnallocate(allocatedSize, 0);
	}

	void* ScriptRunner::getTaskResult() {
		auto context = Context::getCurrent();
		if (_functionInfo->returnStorageSize > 0 && context) {
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
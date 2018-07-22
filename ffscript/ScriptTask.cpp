#include "stdafx.h"
#include "ScriptTask.h"
#include "Context.h"
#include "Program.h"
#include "InstructionCommand.h"

namespace ffscript {

	static const int s_returnOffset = SCRIPT_FUNCTION_RETURN_STORAGE_OFFSET;

	ScriptTask::ScriptTask(Program* program) : _program(program), _scriptContext(nullptr), _allocatedSize(0),
		_functionInfo(nullptr), _functionCode(nullptr), _functionId(-1)
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

		_resultSize = _functionInfo->returnStorageSize;
		int paramOffset = s_returnOffset + _functionInfo->returnStorageSize;
		if (paramBuffer != nullptr && _functionInfo->paramDataSize > 0) {
			_scriptContext->write(paramOffset, paramBuffer->getBuffer(), _functionInfo->paramDataSize);
		}

		_scriptContext->setCurrentCommand(program->getEndCommand() - 1);
		_scriptContext->setEndCommand(program->getEndCommand());
		_allocatedSize = _resultSize + _functionInfo->paramDataSize;
		_scriptContext->scopeAllocate(_allocatedSize, 0);

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

	void ScriptTask::runFunction(int functionId, const ScriptParamBuffer& paramBuffer) {
		runFunction(1024 * 1024, functionId, &paramBuffer);
	}

	void ScriptTask::runFunction(int functionId, const ScriptParamBuffer* paramBuffer) {
		runFunction(1024 * 1024, functionId, paramBuffer);
	}

	void* ScriptTask::getTaskResult() {
		if (_scriptContext) {
#if USE_DIRECT_COPY_FOR_RETURN
			return _scriptContext->getAbsoluteAddress(s_returnOffset);
#else
			Context* context = _scriptContext;
			int functionResultOffset = context->getCurrentOffset() + context->getCurrentScopeSize();
			int targetOffset = context->getCurrentOffset() + s_returnOffset;

			void* sourceAddress = context->getAbsoluteAddress(functionResultOffset);

			context->write(targetOffset, sourceAddress, _resultSize);

			return context->getAbsoluteAddress(s_returnOffset);
#endif
		}
		return nullptr;
	}

	/*void ScriptTask::runFunction2(int functionId, const SimpleVariantArray* params) {
		FunctionInfo* functionInfo = _program->getFunctionInfo(functionId);
		if (functionInfo == nullptr) {
			return;
		}

		int paramSize = functionInfo->paramDataSize;
		char* paramData = (char*) malloc(paramSize);
		char* c = paramData;
		const SimpleVariant* pVariant = &(params->elems[0]);
		
		for (int i = 0; i < params->size; i++) {
			memcpy_s(c, paramSize, pVariant->pData, pVariant->size);
			pVariant++;
			paramSize -= pVariant->size;
			c += pVariant->size;
		}

		runFunction(functionId, paramData);
		free(paramData);
	}

	void ScriptTask::runFunction2(int stackSize, int functionId, const SimpleVariantArray* params) {

	}*/
}
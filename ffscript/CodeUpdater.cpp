/******************************************************************
* File:        CodeUpdater.cpp
* Description: implement CodeUpdater class. A class used to store
*              update tasks. Behavior of update task are various,
*              created in compiling time, and run in extraction code
*              time.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "CodeUpdater.h"
#include "InstructionCommand.h"
#include "function/CachedDelegate.h"
#include "Program.h"
#include "ScriptScope.h"
#include "GlobalScope.h"

namespace ffscript {

	CodeUpdater::CodeUpdater(ScriptScope* ownerScope) : _ownerScope(ownerScope) {
	}

	CodeUpdater::~CodeUpdater() {
	}

	void CodeUpdater::addUpdateLaterTask(const DelegateRef& task) {
		_updateLaterList.push_back(task);
	}

	void CodeUpdater::clear() {
		_updateLaterList.clear();
		_commandExecutorMap.clear();
	}

	void CodeUpdater::clearTask() {
		_updateLaterList.clear();
	}

	void CodeUpdater::runUpdate() {
		for (const DelegateRef& task : _updateLaterList) {
			task->call();
		}
	}

	void CodeUpdater::setUpdateInfo(CommandUnitBuilder* commandUnit, Executor* executor) {
		_commandExecutorMap.insert(std::make_pair(commandUnit, executor));
	}

	void CodeUpdater::saveUpdateInfo(CommandUnitBuilder* commandUnit, Executor* executor) {
		auto it = _commandExecutorMap.find(commandUnit);
		if (it != _commandExecutorMap.end()) {
			it->second = executor;
		}
	}

	Executor* CodeUpdater::findUpdateInfo(CommandUnitBuilder* commandUnit) const{
		auto it = _commandExecutorMap.find(commandUnit);
		if (it != _commandExecutorMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	CodeUpdater* CodeUpdater::getInstance(const ScriptScope* scope) {
		if (scope == nullptr) return nullptr;
		return ((GlobalScope*)scope->getRoot())->getCodeUpdater();
	}

	void CodeUpdater::updateScriptFunction(Program* program, CallScriptFuntion* command, int functionId) {
		CodeSegmentEntry* pFunctionCode = program->getFunctionPlainCode(functionId);
		command->setTargetCommand(pFunctionCode->first - 1);
	}

	void CodeUpdater::updateScriptFunction(Program* program, CallScriptFuntion2* command, int functionId) {
		CodeSegmentEntry* pFunctionCode = program->getFunctionPlainCode(functionId);
#if USE_FUNCTION_TREE
		command->setTargetCommand(pFunctionCode->first);
#else
		command->setTargetCommand(pFunctionCode->first - 1);
#endif		
	}

	void CodeUpdater::updateScriptFunctionObject(Program* program, RuntimeFunctionInfo* runtimeInfo, int functionId) {
		CodeSegmentEntry* pFunctionCode = program->getFunctionPlainCode(functionId);
		runtimeInfo->address = pFunctionCode->first;
	}

	void CodeUpdater::updateLamdaScriptFunctionObject(Program* program, CallCreateLambda* createLambdaInst, int functionId) {
		CodeSegmentEntry* pFunctionCode = program->getFunctionPlainCode(functionId);
		auto functionInfo = program->getFunctionInfo(functionId);
		int capturedDataOffset = SCRIPT_FUNCTION_RETURN_STORAGE_OFFSET + functionInfo->returnStorageSize + functionInfo->paramDataSize;
		createLambdaInst->setLambdaAddress(pFunctionCode->first, capturedDataOffset);
	}
}
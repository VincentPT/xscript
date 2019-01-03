/******************************************************************
* File:        Program.cpp
* Description: implement Program class. A class that manage the
*              compiled code(command) of a program.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "Program.h"
#include "Executor.h"
#include <Context.h>
#include "Expression.h"
#include "InstructionCommand.h"

namespace ffscript {
	Program::Program() : _commandCounter(0), _programCode(nullptr)
		//_moveOffset()
	{
		//_assitantFuncLib = (FuncLibraryRef)( new FuncLibrary() );
	}

	Program::~Program()
	{
		if (_programCode) {
			free(_programCode);
		}
	}

	void Program::addExecutor(const ExecutorRef& executor) {
		if (executor->getCode() != nullptr) {
			_commandContainer.push_back(executor);
			_commandCounter += (int)executor->getCode()->size();
		}
	}

	void Program::convertToPlainCode() {
		if (_commandCounter == 0) return;

		_expCmdMap.clear();
		if (_programCode) {
			free(_programCode);
		}
		_programCode = (CommandPointer)malloc(sizeof(InstructionCommand*)* _commandCounter);
		CommandPointer pCommand = _programCode;
		auto end1 = _commandContainer.end();
		for (auto it1 = _commandContainer.begin(); it1 != end1; ++it1) {
			auto end2 = (*it1)->getCode()->end();
	
			for (auto it2 = (*it1)->getCode()->begin(); it2 != end2; ++it2) {
				*pCommand = *it2;		
				pCommand++;
			}
			if ((*it1)->getCode()->size()) {
				CodeSegmentEntry plainCode(pCommand - (*it1)->getCode()->size(), pCommand - 1);
				_expCmdMap.insert(std::make_pair(it1->get(), plainCode));
			}
		}
	}

	CommandPointer Program::getFirstCommand() const {
		return _programCode;
	}

	CommandPointer Program::getEndCommand() const {
		return _programCode + _commandCounter;
	}

	CodeSegmentEntry* Program::getCode(Executor* pExcutor) {
		auto it = _expCmdMap.find(pExcutor);
		if (it != _expCmdMap.end()) {
			return &it->second;
		}
		return nullptr;
	}

	CodeSegmentEntry* Program::getFunctionPlainCode(int functionId) {
		auto it = _functionMap.find(functionId);
		if (it == _functionMap.end()) {
			return nullptr;
		}
		return &it->second;
	}

	void Program::setFunctionPlainCode(int functionId, const CodeSegmentEntry& functionCode) {
		_functionMap.insert( std::make_pair(functionId, functionCode));
	}

	FunctionInfo* Program::getFunctionInfo(int functionId) {
		auto it = _functionInfoMap.find(functionId);
		if (it == _functionInfoMap.end()) {
			return nullptr;
		}
		return &it->second;
	}

	//const OverLoadingItem* Program::findFunctionInfo2(int functionId) {
	//	return _assitantFuncLib->findFunctionInfo(functionId);
	//}

	void Program::setFunctionInfo(int functionId, const FunctionInfo& functionInfo) {
		_functionInfoMap.insert(std::make_pair(functionId, functionInfo));
	}

	//int Program::findFunction(const std::string& name, const std::vector<int>& paramTypes) {
	//	return _assitantFuncLib->findFunction(name, paramTypes);
	//}

	//int Program::mapFunction(const std::string& name, const std::vector<ScriptType>& paramTypes, int functionId) {
	//	return _assitantFuncLib->mapFunction(name, paramTypes, functionId);
	//}

	//int Program::mapDynamicFunction(const std::string& name, int functionId) {
	//	return _assitantFuncLib->mapDynamicFunction(name, functionId);
	//}
}
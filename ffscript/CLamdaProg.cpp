/******************************************************************
* File:        CLamdaProg.cpp
* Description: implement CLamdaProg class. A class used to store
*              compiled code and other stuffs that allow user
*              running the code with a global context.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "CLamdaProg.h"
#include "StaticContext.h"

namespace ffscript {
	CLamdaProg::CLamdaProg(Program* program) : _program(program),
		_globalDataSize(0),
		_globalCodeSize(0),
		_globalConstructorCount(0)
	{
	}

	CLamdaProg::~CLamdaProg()
	{
		if (_program) {
			delete _program;
		}
	}

	Program* CLamdaProg::getProgram() const {
		return _program;
	}

	void CLamdaProg::setContext(const std::shared_ptr<StaticContext>& contextRef) {
		_context = contextRef;
	}

	const std::shared_ptr<StaticContext>& CLamdaProg::getGlobalContext() const {
		return _context;
	}

	void CLamdaProg::runGlobalCode() {
		_context->pushContext(_globalConstructorCount);
		_context->scopeAllocate(_globalDataSize, _globalCodeSize);

		_context->run();
	}

	void CLamdaProg::cleanupGlobalMemory() {
		_context->runDestructorCommands();

		_context->scopeUnallocate(_globalDataSize, _globalCodeSize);
		_context->popContext();
	}

	bool CLamdaProg::StringCmp::operator()(const char* s1, const char* s2) const {
		return strcmp(s1, s2) < 0;
	}

	void CLamdaProg::addVariable(const std::shared_ptr<Variable>& variable) {
		auto& variableName = variable->getName();
		if (variableName != "") {
			_declaredVariableMap[variableName.c_str()] = variable.get();
		}
		_varibles.push_back(variable);
	}

	const std::list<std::shared_ptr<Variable>>& CLamdaProg::getVariables() const {
		return _varibles;
	}

	Variable* CLamdaProg::findDeclaredVariable(const char* variableName) const {
		auto it = _declaredVariableMap.find(variableName);
		if (it != _declaredVariableMap.end()) {
			return it->second;
		}

		return nullptr;
	}
}
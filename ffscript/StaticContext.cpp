/******************************************************************
* File:        StaticContext.cpp
* Description: implement StaticContext class. A type of context, but
*              the memory of its is global shared memory and can be
*              accessed any where, any time in the program.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "StaticContext.h"
#include "function\DynamicFunction.h"
#include "InstructionCommand.h"

namespace ffscript {
	StaticContext::StaticContext(unsigned char* threadData, int bufferSize) : Context(threadData, bufferSize) {}

	StaticContext::StaticContext(int bufferSize) : Context(bufferSize) {}

	StaticContext::~StaticContext()
	{
	}

	void StaticContext::addCommand(CommandPointer command) {
		_globalCommands.push_back(command);
		//Logger::WriteMessage(__FUNCTION__);
	}

	void StaticContext::addDestructorCommand(CommandPointer command) {
		_destructorCommands.push_back(command);
	}

	void StaticContext::runCommands(const std::list<CommandPointer>& commands) {
		auto currentContext = Context::getCurrent();
		Context::makeCurrent(this);

		for (auto it = commands.begin(); it != commands.end(); ++it) {
			(*(*it))->execute();
#ifndef THROW_EXCEPTION_ON_ERROR
			if (isError()) {
				//Logger::WriteMessage(__FUNCTION__);
				break;
			}
#endif
		}

		Context::makeCurrent(currentContext);
	}

	void StaticContext::run() {
		runCommands(_globalCommands);
	}

	void StaticContext::runDestructorCommands() {
		runCommands(_destructorCommands);
	}
}
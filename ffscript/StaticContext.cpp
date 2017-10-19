#include "stdafx.h"
#include "StaticContext.h"
#include "function\DynamicFunction.h"
#include "InstructionCommand.h"

//#include "CppUnitTest.h"
//using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ffscript {
	StaticContext::StaticContext(unsigned char* threadData, int bufferSize) : Context(threadData, bufferSize) {}

	StaticContext::StaticContext(int bufferSize) : Context(bufferSize) {}

	StaticContext::~StaticContext()
	{
	}

	void StaticContext::addCommand(CommandPointer command) {
		_globalComands.push_back(command);
		//Logger::WriteMessage(__FUNCTION__);
	}

	void StaticContext::run() {
		for (auto it = _globalComands.begin(); it != _globalComands.end(); ++it) {
			(*(*it))->execute();
#ifndef THROW_EXCEPTION_ON_ERROR
			if (isError()) {
				//Logger::WriteMessage(__FUNCTION__);
				break;
			}
#endif
		}
	}
}
/******************************************************************
* File:        ScriptTask.h
* Description: declare ScriptRunner class. A class is desgined to
*              execute a script function through function
*              id. Unlike ScriptRunner, this class will create its
*              own context and running the commands on its context.
*              So, we can use it to run the same code in seperated
*              contexts at the same time. This is main object of
*              this class, running the code in multi-thread.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"
#include "ScriptParamBuffer.hpp"
#include "ScriptRunner.h"

namespace ffscript {

	class Context;
	class Program;
	struct FunctionInfo;

	class ScriptTask
	{
		Context* _scriptContext;
		int _allocatedSize;
		ScriptRunner* _scriptRunner;
		Program* _program;

		int _lastCallFunctionId;
	public:
		ScriptTask(Program* program);
		virtual ~ScriptTask();

		void runFunction(int functionId, const ScriptParamBuffer* paramBuffer);
		void runFunction(int stackSize, int functionId, const ScriptParamBuffer* paramBuffer);
		void runFunction(int functionId, const ScriptParamBuffer& paramBuffer);
		void runFunction(int stackSize, int functionId, const ScriptParamBuffer& paramBuffer);
		/*void runFunction2(int functionId, const SimpleVariantArray* params);
		void runFunction2(int stackSize, int functionId, const SimpleVariantArray* params);*/
		void* getTaskResult();
	};
}

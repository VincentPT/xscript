/******************************************************************
* File:        ScriptRunner.h
* Description: declare ScriptRunner class. A class is desgined to
*              bind and execute a script function through function
*              id. User must set the current context before execute
*              the function.
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

namespace ffscript {
	class Program;
	struct FunctionInfo;
	class CallFuntion;

	class ScriptRunner
	{
	protected:
		Program* _program;
		FunctionInfo* _functionInfo;
		CallFuntion* _scriptInvoker;
	public:
		ScriptRunner(Program* program, int functionId);
		virtual ~ScriptRunner();

		virtual void runFunction(const ScriptParamBuffer* paramBuffer);
		virtual void* getTaskResult();
	};
}

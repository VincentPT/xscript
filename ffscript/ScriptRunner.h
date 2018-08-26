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

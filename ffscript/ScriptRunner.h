#pragma once
#include "ffscript.h"
#include "ScriptParamBuffer.hpp"

namespace ffscript {
	class Program;
	struct FunctionInfo;

	class ScriptRunner
	{
	protected:
		Program* _program;
		// cached data
		int _functionId;
		FunctionInfo* _functionInfo;
		CodeSegmentEntry* _functionCode;
	public:
		ScriptRunner(Program* program);
		virtual ~ScriptRunner();

		virtual void runFunction(int functionId, const ScriptParamBuffer* paramBuffer);
		virtual void* getTaskResult();
	};
}

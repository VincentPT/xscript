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

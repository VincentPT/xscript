#pragma once
#include "ffscript.h"
#include "ScriptParamBuffer.hpp"

namespace ffscript {

	class Context;
	class Program;

	class ScriptTask
	{
		Context* _scriptContext;
		int _resultSize;
		int _allocatedSize;
		Program* _program;
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

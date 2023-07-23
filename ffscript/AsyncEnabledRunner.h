/******************************************************************
* File:        AsyncEnabledRunner.cpp
* Description: declaration of AsyncEnabledRunner class. A class is
*              desgined to run program code with a entry function
*              and support await-like code
* Author:      Vincent Pham
*
* Copyright (c) 2023 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"
#include "ScriptParamBuffer.hpp"
#include "ScriptTask.h"
#include "ResumeEvent.h"
#include <list>
#include <mutex>
#include <condition_variable>

namespace ffscript {
	class Program;
	struct FunctionInfo;
	class CallFuntion;

	class AsyncEnabledRunner : public ScriptTask
	{
		std::mutex _m;
		std::condition_variable _cv;
		bool _wakeUp = false;
		bool _stop = false;
		std::list<ResumeEvent*> _availabeEvents;
	public:
		AsyncEnabledRunner(Program* program);
		virtual ~AsyncEnabledRunner();

		virtual void run(int entryFunction, const ScriptParamBuffer& paramBuffer);
		virtual void pushEvent(ResumeEvent*);
		void wakeMeUp();
		static AsyncEnabledRunner* current();
	};
}

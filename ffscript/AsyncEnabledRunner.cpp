/******************************************************************
* File:        AsyncEnabledRunner.cpp
* Description: implement AsyncEnabledRunner class. A class is
*              desgined to run program code with a entry function
*              and support await-like code
* Author:      Vincent Pham
*
* Copyright (c) 2023 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "AsyncEnabledRunner.h"
#include "Context.h"
#include "Program.h"
#include "InstructionCommand.h"
#include "ResumeEvent.h"

namespace ffscript {
	
#if _WIN32 || _WIN64
	__declspec(thread) AsyncEnabledRunner* _threadAsyncRunner = nullptr;
// Check GCC
#elif __GNUC__
	__thread Context* _threadAsyncRunner = nullptr;
#endif

	AsyncEnabledRunner::AsyncEnabledRunner(Program* program) : ScriptTask(program)
	{
		_threadAsyncRunner = this;
	}

	AsyncEnabledRunner::~AsyncEnabledRunner(){
	}

	void AsyncEnabledRunner::run(int entryFunction, const ScriptParamBuffer& paramBuffer) {
		// run main entry function
		this->runFunction(entryFunction, paramBuffer);

		// execute pending code if any
		while(_availabeEvents.size() && _stop == false) {
			auto resumeEvent = _availabeEvents.front();
			_availabeEvents.pop_front();
			auto runStatus = resumeEvent->run();

			if(runStatus == ResumeEvent::Done) {				
				delete resumeEvent;
			}
			else {
				// the pending task is not completed then we need to check run other pending task
				// in case there is no pending task left...
				if(_availabeEvents.size() == 0) {
					// ...then we sleep the current thread to save CPU cost until
					// a wakeup signal was fired
					// TODO, wait until signal was fired
					std::unique_lock<std::mutex> lk(_m);
					// block the calling until the task completed
					_cv.wait(lk, [this] {return _wakeUp | _stop; });
					// reset
					_wakeUp = false;
				}
				_availabeEvents.push_back(resumeEvent);
			}
		}
	}

	void AsyncEnabledRunner::pushEvent(ResumeEvent* pEvent) {
		_availabeEvents.push_back(pEvent);
	}

	AsyncEnabledRunner* AsyncEnabledRunner::current() {
		return _threadAsyncRunner;
	}

	void AsyncEnabledRunner::wakeMeUp() {
		std::lock_guard<std::mutex> lock(_m);
		_wakeUp = true;
		_cv.notify_one();
	}
}
/******************************************************************
* File:        ResumeEvent.cpp
* Description: implement ResumeEvent class. A class contains 
*              saved excution context that allow the runner can
*              continue execute the pending instruction commands
* Author:      Vincent Pham
*
* Copyright (c) 2023 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"
#include "Context.h"
#include <functional>

typedef std::function<void()> PostProcessing;

namespace ffscript {
	class ResumeEvent
	{
	public:
		enum AwaitStatus {
			Awaiting,
			Done,
		};
	private:
		Context* _context;
		PostProcessing _posProcessingFn;
		AwaitStatus _awaitStatus = Awaiting;	
	public:
		ResumeEvent(Context* context);
		virtual ~ResumeEvent();
		virtual AwaitStatus run();
		void setPostProcessing(PostProcessing&& fn);
		void setStatus(AwaitStatus status);
	};
}

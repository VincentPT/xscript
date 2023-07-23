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

#include "ResumeEvent.h"

namespace ffscript {
	ResumeEvent::ResumeEvent(Context* context) : _context(context)
	{
	}

	ResumeEvent::~ResumeEvent(){
		if(_posProcessingFn) {
			_posProcessingFn();
		}
	}

	ResumeEvent::AwaitStatus ResumeEvent::run() {
		if(_awaitStatus == Done) {
			_context->resumeFromPending();			
		}

		return _awaitStatus;
	}

	void ResumeEvent::setPostProcessing(PostProcessing&& fn) {
		_posProcessingFn = fn;
	}

	void ResumeEvent::setStatus(AwaitStatus status) {
		_awaitStatus = status;
	}
}
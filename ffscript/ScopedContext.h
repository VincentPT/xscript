/******************************************************************
* File:        ScopedContext.h
* Description: declare ScopedRunningContext class. This class is
*              guarantee that the current context is backup at
*              the beginning of its life time and restore at the
*              end of its life time.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "Context.h"

namespace ffscript {
	class ScopedRunningContext
	{
		Context* _pOldContext;
	public:
		ScopedRunningContext(Context* context);
		~ScopedRunningContext();
	};
}
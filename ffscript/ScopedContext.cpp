/******************************************************************
* File:        ScopedContext.cpp
* Description: implement ScopedRunningContext class. This class is
*              guarantee that the current context is backup at
*              the beginning of its life time and restore at the
*              end of its life time.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "ScopedContext.h"

namespace ffscript {
	ScopedRunningContext::ScopedRunningContext(Context* pContext)
	{
		_pOldContext = Context::getCurrent();
		Context::makeCurrent(pContext);
	}

	ScopedRunningContext::~ScopedRunningContext()
	{
		Context::makeCurrent(_pOldContext);
	}
}
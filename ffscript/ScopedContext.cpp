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
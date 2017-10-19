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
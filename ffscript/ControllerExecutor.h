#pragma once
#include "Executor.h"
namespace ffscript {
	class ControllerExecutor :
		public Executor
	{
	public:
		ControllerExecutor();
		virtual ~ControllerExecutor();
	};
}

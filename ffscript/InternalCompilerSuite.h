#pragma once
#include "CompilerSuite.h"

namespace ffscript {
	class InternalCompilerSuite :
		public CompilerSuite
	{
	public:
		InternalCompilerSuite();
		virtual ~InternalCompilerSuite();
		virtual void initialize(int globalMemSize);
	};
}
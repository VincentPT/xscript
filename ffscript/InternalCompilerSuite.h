/******************************************************************
* File:        InternalCompilerSuite.h
* Description: declare InternalCompilerSuite class. A compiler class
*              but used to compile constant expression only.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

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
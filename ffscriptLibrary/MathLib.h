/******************************************************************
* File:        MathLib.h
* Description: declare an interface to import math functions into the
*              script compiler.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"

namespace ffscript {
	class ScriptCompiler;
	void includeMathToCompiler(ScriptCompiler* scriptCompiler);
}

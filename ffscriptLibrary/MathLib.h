/******************************************************************
* File:        MathLib.h
* Description: declare an interface to import math functions into the
*              script compiler.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
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

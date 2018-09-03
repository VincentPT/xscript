/******************************************************************
* File:        ScopedCompilingScope.cpp
* Description: implement ScopedCompilingScope class. This class is
*              guarantee that the current scope is backup at
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
#include "ScopedCompilingScope.h"
#include "ScriptCompiler.h"

namespace ffscript {
	ScopedCompilingScope::ScopedCompilingScope(ScriptCompiler* compiler, ScriptScope* scope) : _compiler(compiler)
	{
		_compiler->pushScope(scope);
	}

	ScopedCompilingScope::~ScopedCompilingScope()
	{
		_compiler->popScope();
	}
}
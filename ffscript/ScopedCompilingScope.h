/******************************************************************
* File:        ScopedCompilingScope.h
* Description: declare ScopedCompilingScope class. This class is
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

#pragma once
namespace ffscript {

	class ScriptCompiler;
	class ScriptScope;

	class ScopedCompilingScope
	{
		ScriptCompiler* _compiler;
	public:
		ScopedCompilingScope(ScriptCompiler* compiler, ScriptScope* scope);
		~ScopedCompilingScope();
	};

}
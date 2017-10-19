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
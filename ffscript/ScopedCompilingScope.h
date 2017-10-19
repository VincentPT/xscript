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
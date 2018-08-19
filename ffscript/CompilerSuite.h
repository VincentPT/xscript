#pragma once
#include "ScriptCompiler.h"
#include "Program.h"
#include "GlobalScope.h"
#include "StaticContext.h"
#include "BasicFunctionFactory.hpp"
#include "BasicFunction.h"
#include "Context.h"
#include "StaticContext.h"
#include "ContextScope.h"
#include "ScopedCompilingScope.h"
#include "FunctionRegisterHelper.h"
#include "BasicType.h"
#include "ExpUnitExecutor.h"
#include "Preprocessor.h"

namespace ffscript {
	class CompilerSuite
	{
	protected:
		ScriptCompilerRef _pCompiler;
		GlobalScopeRef _globalScopeRef;
		PreprocessorRef _preprocessor;
	public:
		CompilerSuite();
		virtual void initialize(int globalMemSize);
		virtual ~CompilerSuite();

		Program* compileProgram(const wchar_t* codeStart, const wchar_t* codeEnd);
		ExpUnitExecutor* compileExpression(const wchar_t* expression);
		const GlobalScopeRef& getGlobalScope() const;
		const TypeManagerRef& getTypeManager() const;
		ScriptCompilerRef& getCompiler();
		void setPreprocessor(const PreprocessorRef& preprocessor);
		const PreprocessorRef getPreprocessor() const;
		void getLastCompliedPosition(int& line, int& column);
	};
}

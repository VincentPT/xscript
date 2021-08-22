/******************************************************************
* File:        GlobalScope.h
* Description: declare GlobalScope class. A class used to parse
*              the script of a program. It store all global variables
*              and command builders of the whole program.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "Variable.h"
#include "ScriptScope.h"
#include "StaticContext.h"
#include <list>
#include <vector>

namespace ffscript {

	class Executor;
	class CodeUpdater;
	class CLamdaProg;

	class GlobalScope : public ScriptScope
	{
		unique_ptr<StaticContext> _staticContextRef;
		std::list<int> _registeredFuntions;
		CodeUpdater* _updateLaterMan;
		bool _refContext;
		const WCHAR* _errorCompiledChar;
		const WCHAR* _beginCompileChar;
	public:
		GlobalScope(StaticContext* staticContext, ScriptCompiler* scriptCompiler);
		GlobalScope(int globalMemSize, ScriptCompiler* scriptCompiler);
		virtual ~GlobalScope();
		void* getGlobalAddress(int offset);
		void runGlobalCode();
		void cleanupGlobalMemory();
		virtual int correctAndOptimize(Program* program);
		const WCHAR* getErrorCompiledChar() const;
		const WCHAR* getBeginCompileChar() const;
		void setErrorCompilerChar(const WCHAR* c, bool force = false);
		void setErrorCompilerCharIndex(int idx);
		void setBeginCompileChar(const WCHAR* c);
		void convertSourceCharIndexToGlobal(const WCHAR* source, std::list<ExpUnitRef>& units);
		CLamdaProg* detachScriptProgram(Program* program);
	public:
		const wchar_t* parse(const wchar_t* text, const wchar_t* end);
		const wchar_t* parseAnonymous(const wchar_t* text, const wchar_t* end, const std::list<ExecutableUnitRef>& captureList, int& functionId);
		virtual bool extractCode(Program* program);		
		virtual int registScriptFunction(const std::string& name, const ScriptType& returnType, const std::vector<ScriptType>& paramTypes);
		CodeUpdater* getCodeUpdater() const;
		StaticContext* getContext() const;
	protected:
		const wchar_t* detectKeyword(const wchar_t* text, const wchar_t* end);
		const wchar_t* parseStruct(const wchar_t* text, const wchar_t* end);
	};
	typedef shared_ptr<GlobalScope> GlobalScopeRef;
}
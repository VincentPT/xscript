#pragma once
#include "Variable.h"
#include "ScriptScope.h"
#include "StaticContext.h"
#include <list>
#include <vector>

namespace ffscript {

	class Executor;
	class CodeUpdater;

	class GlobalScope : public ScriptScope
	{
		unique_ptr<StaticContext> _staticContextRef;
		std::list<int> _registeredFuntions;
		CodeUpdater* _updateLaterMan;
		bool _refContext;
	public:
		GlobalScope(StaticContext* staticContext, ScriptCompiler* scriptCompiler);
		GlobalScope(int globalMemSize, ScriptCompiler* scriptCompiler);
		virtual ~GlobalScope();
		void* getGlobalAddress(int offset);
		void runGlobalCode();
		virtual int correctAndOptimize(Program* program);
	public:
		const wchar_t* parse(const wchar_t* text, const wchar_t* end);
		const wchar_t* parseAnonymous(const wchar_t* text, const wchar_t* end, const std::list<ExecutableUnitRef>& captureList, int& functionId);
		virtual bool extractCode(Program* program);		
		virtual int registScriptFunction(const std::string& name, const ScriptType& returnType, const std::vector<ScriptType>& paramTypes);
		CodeUpdater* getCodeUpdater() const;
	protected:
		const wchar_t* detectKeyword(const wchar_t* text, const wchar_t* end);
		const wchar_t* parseStruct(const wchar_t* text, const wchar_t* end);
	};
	typedef shared_ptr<GlobalScope> GlobalScopeRef;
}
/******************************************************************
* File:        ContextScope.h
* Description: declare ContextScope class. A class use to compile
*              codes in a specific scope other than global scope.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ScriptScope.h"
#include "Program.h"
#include <string>

namespace ffscript {
	class FunctionScope;
	class Executor;
	class LoopScope;
	class ContextScope;

	enum class ContextScopeParseEvent {
		BeforeParseBody,
		AfterParseBody
	};

	typedef std::function<void(ContextScope*, ContextScopeParseEvent)> ParseEventHandler;

	class ContextScope :
		public ScriptScope
	{
		FunctionScope* _functionScope;
		LoopScope* _loopScope;
		CodeSegmentEntry _codeSegment;
		CommandPointer _beginExitScopeCommand;
		ExecutorRef _beginExecutor;
		ExecutorRef _endExecutor;
		CommandUnitBuilder* _beginExitScopeUnit;
		std::string _name;
		ParseEventHandler _parseContextBodyEventHandler;

	public:
		ContextScope(ScriptScope* parent, FunctionScope* functionScope);
		virtual ~ContextScope();

		void setLoopScope(LoopScope* loopScope);
		LoopScope* getLoopScope() const;
		FunctionScope* getFunctionScope() const;
		void setName(const std::string& name);

		//parser functions
	public:
		virtual const wchar_t* parse(const wchar_t* text, const wchar_t* end);
		virtual bool extractCode(Program* program);
		virtual bool updateCodeForControllerCommands(Program* program);
		const wchar_t* parseCondition(const wchar_t* text, const wchar_t* end);
		const wchar_t* parseIf(const wchar_t* text, const wchar_t* end);
		const wchar_t* parseWhile(const wchar_t* text, const wchar_t* end);
		const CodeSegmentEntry* getCode() const;
		CommandPointer getBeginExitScopeCommand() const;
		virtual void buildExitScopeCodeCommands(CommandList& commandList) const;
		void setCodeBegin( CommandPointer startCode);
		void setCodeEnd(CommandPointer endCode);
		virtual int correctAndOptimize(Program* program);

		void setParseBodyEventHandler(const ParseEventHandler&);
	protected:
		//Executor* getExcutorBegin() const;
		//Executor* getExcutorEnd() const;
		void applyExitScopeCommand();
		Function* checkAndGenerateDestructor(ScriptCompiler* scriptCompiler, const ScriptType& type);
		int checkAndGenerateDestructors(ScriptCompiler* scriptCompiler, ExecutableUnit* exeUnit, std::list<FunctionRef>& destructors);
		/*bool tryApplyConstructorForDeclarationExpression(Variable* pVariable, std::list<ExpUnitRef>& unitList, const ScriptType* expectedReturnType, EExpressionResult& eResult);*/
	};
}
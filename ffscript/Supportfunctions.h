#pragma once
#include <string>
#include "CommandUnitBuilder.h"
#include "ScriptScope.h"

class DFunction;

namespace ffscript {

	class FunctionScope;
	class ContextScope;
	class LoopScope;
	class ScriptScope;
	class Executor;
	class EnterContextScope;
	class ExitContextScope;
	class JumpIf;
	class CopyDataToRef;
	class PushParamOffset;
	class BreakCommand;
	class ContinueCommand;
	class Variable;
	class OperatorObject;
	class ExitScriptFuntionAtReturn;
	class TriggerCommand;
	class BeforeConstructorCall;
	class Jump;

	class CommandBuilder :
		public CommandUnitBuilder
	{
		UNIT_TYPE _type;
		std::string _name;
	public:
		CommandBuilder();
		CommandBuilder(UNIT_TYPE type, const std::string& name);
		virtual ~CommandBuilder();

		virtual const std::string& toString()const;
		virtual UNIT_TYPE getType()const;

		void setName(const std::string& name);
		void setType(UNIT_TYPE type);

		virtual Executor* buildNativeCommand() = 0;
	};

	class EnterScopeBuilder : public CommandBuilder {
		ContextScope* _contextScope;
	public:
		EnterScopeBuilder(ContextScope* contextScope);
		~EnterScopeBuilder();

		ContextScope* getScope() const;

		virtual Executor* buildNativeCommand();
		void fillParams(EnterContextScope* command) const;
	};

	class ExitScopeBuilder : public CommandBuilder {
		ContextScope* _contextScope;
		bool _restoreCall;
	public:
		ExitScopeBuilder(ContextScope* contextScope);
		~ExitScopeBuilder();
		void setRestoreCallFlag(bool blRestoreCall);

		ContextScope* getScope() const;

		virtual Executor* buildNativeCommand();
		static void fillParams(ContextScope* scope, ExitContextScope* command);

		static ExitContextScope* buildExitScopeCommand(ContextScope* scope);
	};

	class ExitFunctionBuilder : public CommandBuilder {
	public:
		ExitFunctionBuilder();
		~ExitFunctionBuilder();

		Executor* buildNativeCommand();
	};

	class ReturnCommandBuilder : public CommandBuilder {
		FunctionScope* _functionScope;
		ContextScope* _ownerScope;
		CommandUnitBuilder* _returnDataUnit;
	public:
		ReturnCommandBuilder(ContextScope* ownerScope, FunctionScope* functionScope);
		~ReturnCommandBuilder();

		void setReturnExpression(CommandUnitBuilder* returnDataUnit);

		Executor* buildNativeCommand();
		void fillParams(PushParamOffset* command) const;
	};

	class ReturnCommandBuilder2 : public CommandBuilder {
		FunctionScope* _functionScope;
		ContextScope* _ownerScope;
		CommandUnitBuilder* _returnDataUnit;
		int _indexPreventDestructorRun;
	public:
		ReturnCommandBuilder2(ContextScope* ownerScope, FunctionScope* functionScope);
		~ReturnCommandBuilder2();

		void setReturnExpression(CommandUnitBuilder* returnDataUnit);
		void setDestructorIndex(int indexPreventDestructorRun);

		Executor* buildNativeCommand();
		void fillParams(CopyDataToRef* command) const;
	};

	class BreakCommandBuilder : public CommandBuilder {
	protected:
		LoopScope* _loopScope;
		ContextScope* _ownerScope;
	public:
		BreakCommandBuilder(ContextScope* ownerScope, LoopScope* loopScope);
		virtual ~BreakCommandBuilder();

		LoopScope* getLoopScope() const;
		void setLoopScope(LoopScope* loopScope);

		virtual Executor* buildNativeCommand();
		void fillParams(BreakCommand* command) const;
	};

	class ContinueCommandBuilder : public BreakCommandBuilder {
	public:
		ContinueCommandBuilder(ContextScope* ownerScope, LoopScope* loopScope);
		~ContinueCommandBuilder();

		virtual Executor* buildNativeCommand();
		void fillParams(ContinueCommand* command) const;
	};

	class JumpToSubScopeCommandBuilder : public CommandBuilder {
		ContextScope* _subScope;
	public:
		JumpToSubScopeCommandBuilder(ContextScope* subScope);
		~JumpToSubScopeCommandBuilder();

		Executor* buildNativeCommand();
		void fillParams(Jump* command) const;
	};

	class IfCommandBuilder : public CommandBuilder {
		ContextScope* _ifScope;
		ContextScope* _elseScope;
		CommandUnitBuilder* _conditionUnit;
	public:
		IfCommandBuilder(ContextScope* ifScope);
		~IfCommandBuilder();

		ContextScope* getIfScope() const;
		void setIfScope(ContextScope* ifScope);

		ContextScope* getElseScope() const;
		void setElseScope(ContextScope* elseScope);

		void setConditionExpression(CommandUnitBuilder* conditionUnit);
		
		Executor* buildNativeCommand();		
		void fillParams(InstructionCommand* command) const;
	};

	class LoopCommandBuilder : public CommandBuilder {
		LoopScope* _loopScope;
	public:
		LoopCommandBuilder(LoopScope* loopScope);
		~LoopCommandBuilder();

		LoopScope* getLoopScope() const;
		void setLoopScope(LoopScope* loopScope);		

		Executor* buildNativeCommand();
		void fillParams(JumpIf* command) const;
	};

	/*class DefaultConstructorCommandBuilder : public CommandBuilder {
		Variable* _pVariable;
		OperatorObject* _pOperatorObject;
	public:
		DefaultConstructorCommandBuilder(OperatorObject* pOperatorObject, Variable* pVariable);
		~DefaultConstructorCommandBuilder();

		Executor* buildNativeCommand();
		void fillParams() const;
	};*/	
}
#pragma once
#include "InstructionCommand.h"
#include <function\DynamicFunction.h>

namespace ffscript {
	class FunctionCommand : public TargetedCommand {
	protected:
		TargetedCommand* _command;
	public:
		FunctionCommand();
		virtual ~FunctionCommand();
		virtual int pushCommandParam(TargetedCommand* command) = 0;
		virtual TargetedCommand* popCommandParam() = 0;
		void buildCommandText();
		virtual void setCommand(TargetedCommand* command);
		TargetedCommand* getCommand() const;
	};

	////////////////////////////////////////////////////
	class FunctionCommand0P : public FunctionCommand {	
	public:
		FunctionCommand0P();
		virtual ~FunctionCommand0P();
		virtual int pushCommandParam(TargetedCommand* command);
		virtual TargetedCommand* popCommandParam();
		virtual void execute();
	};

	////////////////////////////////////////////////////
	class FunctionCommand1P : public FunctionCommand {
	protected:
		TargetedCommand* _commandParam;
	public:
		FunctionCommand1P();
		virtual ~FunctionCommand1P();
		virtual int pushCommandParam(TargetedCommand* command);
		virtual TargetedCommand* popCommandParam();
		virtual void execute();
	};

	////////////////////////////////////////////////////
	class FunctionCommand2P : public FunctionCommand {
	protected:
		TargetedCommand* _commandParam1;
		TargetedCommand* _commandParam2;
	public:
		FunctionCommand2P();
		virtual ~FunctionCommand2P();
		virtual int pushCommandParam(TargetedCommand* command);
		virtual TargetedCommand* popCommandParam();
		virtual void execute();
	};

	////////////////////////////////////////////////////
	class FunctionCommandNP : public FunctionCommand {
	protected:
		TargetedCommand** _commandParams;
		int _nMaxParam;
		int _nParam;
	public:
		FunctionCommandNP(int n);
		virtual ~FunctionCommandNP();
		virtual int pushCommandParam(TargetedCommand* command);
		virtual TargetedCommand* popCommandParam();
		virtual void execute();
	};

	////////////////////////////////////////////////////
	class OptimizedLogicCommand : public FunctionCommand2P {
	protected:
		int _firstParamOffset;
		int _secondParamOffset;
		int _resultOffset;

		OptimizedLogicCommand();

	public:
		void setCommand(TargetedCommand* command);
	};

	////////////////////////////////////////////////////
	class LogicAndCommand : public OptimizedLogicCommand {
	public:
		LogicAndCommand();
		virtual void execute();
	};

	////////////////////////////////////////////////////
	class LogicOrCommand : public OptimizedLogicCommand {
	public:
		LogicOrCommand();
		virtual void execute();
	};
	
	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(ConditionalCommand, TargetedCommand);
	protected:
		TargetedCommand* _conditionUnit;
		TargetedCommand* _ifUnit;
		TargetedCommand* _elseUnit;
public:
	void setCommandData(TargetedCommand* conditionUnit, TargetedCommand* ifUnit, TargetedCommand* elseUnit);
	END_INSTRUCTION_COMMAND_DECLARE(ConditionalCommand);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(TriggerCommand, TargetedCommand);
protected:
	TargetedCommand* _mainCommand;
	std::shared_ptr<DFunction> _afterExecuteFunc;
	std::shared_ptr<DFunction> _beforeExecuteFunc;
public:
	void setCommand(TargetedCommand* mainCommand);
	void setBeforeTrigger(const std::shared_ptr<DFunction>& beforeExecuted);
	void setAfterTrigger(const std::shared_ptr<DFunction>& afterExecuted);
	END_INSTRUCTION_COMMAND_DECLARE(ConditionalCommand);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(ConditionTriggerCommand, TriggerCommand);	
public:
	END_INSTRUCTION_COMMAND_DECLARE(ConditionalCommand);
}

/******************************************************************
* File:        CommandTree.h
* Description: declare command unit classes. Instances of these
*              classes are compiled code of the script. They are
*              executable. They have main command and sub commands.
*              The sub commands are function parameters where the
*              main command is processor unit of the function.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

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
		void buildCommandText(std::list<std::string>& strCommands);
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
		void buildCommandText(std::list<std::string>& strCommands);
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
		void buildCommandText(std::list<std::string>& strCommands);
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
		int getParamCap() const;
		void buildCommandText(std::list<std::string>& strCommands);
	};

	////////////////////////////////////////////////////
	class OptimizedLogicCommand : public TargetedCommand {
	protected:
		TargetedCommand* _commandParam1;
		TargetedCommand* _commandParam2;
		OptimizedLogicCommand();
	public:
		virtual int pushCommandParam(TargetedCommand* command);
		virtual TargetedCommand* popCommandParam();
	};

	////////////////////////////////////////////////////
	class LogicAndCommand : public OptimizedLogicCommand {
	public:
		LogicAndCommand();
		virtual void execute();
		void buildCommandText(std::list<std::string>& strCommands);
	};

	////////////////////////////////////////////////////
	class LogicOrCommand : public OptimizedLogicCommand {
	public:
		LogicOrCommand();
		virtual void execute();
		void buildCommandText(std::list<std::string>& strCommands);
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
	std::string _beforeExecuteCommandName;
	std::string _afterExecuteCommandName;
public:
	void setCommand(TargetedCommand* mainCommand);
	void setBeforeTrigger(const std::shared_ptr<DFunction>& beforeExecuted, const std::string& commandName);
	void setAfterTrigger(const std::shared_ptr<DFunction>& afterExecuted, const std::string& commandName);
	END_INSTRUCTION_COMMAND_DECLARE(ConditionalCommand);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(ConditionTriggerCommand, TriggerCommand);	
public:
	END_INSTRUCTION_COMMAND_DECLARE(ConditionalCommand);
}

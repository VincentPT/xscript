#include "stdafx.h"
#include "CommandTree.h"
#include "Context.h"
#include "function\DynamicFunction2.h"

#include <iomanip>
#include <sstream>

namespace ffscript {

	/////////////////////////////////////////////////////////////////////////////////////
	FunctionCommand::FunctionCommand(): _command(nullptr){}
	FunctionCommand::~FunctionCommand() {
		if (_command) {
			delete _command;
		}
	}

	void FunctionCommand::setCommand(TargetedCommand* command) {
		_command = command;
	}

	TargetedCommand* FunctionCommand::getCommand() const {
		return _command;
	}

	void FunctionCommand::buildCommandText() {
		_command->buildCommandText();
		setCommandText(_command->toString());
	}

	/////////////////////////////////////////////////////////////////////////////////////
	FunctionCommand0P::FunctionCommand0P() {}
	FunctionCommand0P::~FunctionCommand0P(){}

	void FunctionCommand0P::execute() {		
		_command->execute();
	}

	int FunctionCommand0P::pushCommandParam(TargetedCommand* command) {
		return -1;
	}

	TargetedCommand* FunctionCommand0P::popCommandParam() {
		return nullptr;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	FunctionCommand1P::FunctionCommand1P() : _commandParam(nullptr) {}
	FunctionCommand1P::~FunctionCommand1P() {
		if (_commandParam) {
			delete _commandParam;
		}
	}

	void FunctionCommand1P::execute() {
		_commandParam->execute();
		_command->execute();
	}

	int FunctionCommand1P::pushCommandParam(TargetedCommand* command) {
		if (_commandParam) { return -1; }
		_commandParam = command;
		return 0;
	}

	TargetedCommand* FunctionCommand1P::popCommandParam() {
		TargetedCommand* backupCommand = _commandParam;
		_commandParam = nullptr;

		return backupCommand;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	FunctionCommand2P::FunctionCommand2P() : _commandParam1(nullptr), _commandParam2(nullptr) {}
	FunctionCommand2P::~FunctionCommand2P() {
		if (_commandParam1) {
			delete _commandParam1;
		}
		if (_commandParam2) {
 			delete _commandParam2;
		}
	}

	void FunctionCommand2P::execute() {
		_commandParam1->execute();
		_commandParam2->execute();
		_command->execute();
	}

	int FunctionCommand2P::pushCommandParam(TargetedCommand* command) {
		if (_commandParam2) { return -1; }
		if (_commandParam1) {
			_commandParam2 = command;
			return 1;
		}		
		_commandParam1 = command;
		return 0;
	}

	TargetedCommand* FunctionCommand2P::popCommandParam() {
		TargetedCommand* backupCommand;
		if (_commandParam2) {
			backupCommand = _commandParam2;
			_commandParam2 = nullptr;
		}
		else {
			backupCommand = _commandParam1;
			_commandParam1 = nullptr;
		}
		return backupCommand;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	FunctionCommandNP::FunctionCommandNP(int n) : _nParam(0) {
		_nMaxParam = n;
		_commandParams = (TargetedCommand**) malloc( n * sizeof(TargetedCommand*));
	}

	FunctionCommandNP::~FunctionCommandNP() {
		if (_commandParams) {
			TargetedCommand** command = _commandParams;
			TargetedCommand** end = command + _nParam;

			while (command < end) {
				delete (*command);
				command++;
			}
			free(_commandParams);
		}
	}

	void FunctionCommandNP::execute() {
		TargetedCommand** command = _commandParams;
		TargetedCommand** end = command + _nParam;

		while (command < end) {
			(*command)->execute();
			command++;
		}

		_command->execute();
	}

	int FunctionCommandNP::pushCommandParam(TargetedCommand* command) {
		if (_nParam == _nMaxParam) { return -1; }
		_commandParams[_nParam++] = command;
		return _nMaxParam - _nParam;
	}

	TargetedCommand* FunctionCommandNP::popCommandParam() {
		if (_nParam == 0) return nullptr;
		
		return _commandParams[--_nParam];
	}

	int FunctionCommandNP::getParamCap() const {
		return _nMaxParam;
	}
	/////////////////////////////////////////////////////////////////////////////////////
	OptimizedLogicCommand::OptimizedLogicCommand() : _commandParam1(nullptr), _commandParam2(nullptr) {
		setTargetSize(sizeof(bool));
	}

	int OptimizedLogicCommand::pushCommandParam(TargetedCommand* command) {
		if (_commandParam2) { return -1; }
		if (_commandParam1) {
			_commandParam2 = command;
			return 1;
		}
		_commandParam1 = command;
		return 0;
	}

	TargetedCommand* OptimizedLogicCommand::popCommandParam() {
		TargetedCommand* backupCommand;
		if (_commandParam2) {
			backupCommand = _commandParam2;
			_commandParam2 = nullptr;
		}
		else {
			backupCommand = _commandParam1;
			_commandParam1 = nullptr;
		}
		return backupCommand;
	}
	
	/////////////////////////////////////////////////////////////////////////////////////
	LogicAndCommand::LogicAndCommand() {}

	void LogicAndCommand::execute() {
		_commandParam1->execute();

		Context* context = Context::getCurrent();
		int paramOffset = _commandParam1->getTargetOffset() + context->getCurrentOffset();
		int returnOffset = getTargetOffset() + context->getCurrentOffset();
		bool* paramValueRef = (bool*)context->getAbsoluteAddress(paramOffset);
		bool* resultValueRef = (bool*)context->getAbsoluteAddress(returnOffset);

		if (*paramValueRef == false) {
			*resultValueRef = false;
		}
		else {
			_commandParam2->execute();
			paramOffset = _commandParam2->getTargetOffset() + context->getCurrentOffset();
			paramValueRef = (bool*)context->getAbsoluteAddress(paramOffset);
			*resultValueRef = *paramValueRef;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	LogicOrCommand::LogicOrCommand() {}

	void LogicOrCommand::execute() {
		_commandParam1->execute();

		Context* context = Context::getCurrent();
		int paramOffset = _commandParam1->getTargetOffset() + context->getCurrentOffset();
		int returnOffset = getTargetOffset() + context->getCurrentOffset();
		bool* paramValueRef = (bool*)context->getAbsoluteAddress(paramOffset);
		bool* resultValueRef = (bool*)context->getAbsoluteAddress(returnOffset);

		if (*paramValueRef == true) {
			*resultValueRef = true;
		}
		else {
			_commandParam2->execute();
			paramOffset = _commandParam2->getTargetOffset() + context->getCurrentOffset();
			paramValueRef = (bool*)context->getAbsoluteAddress(paramOffset);
			*resultValueRef = *paramValueRef;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	ConditionalCommand::ConditionalCommand() :
		_conditionUnit(nullptr), _ifUnit(nullptr), _elseUnit(nullptr) {}

	ConditionalCommand::~ConditionalCommand() {
		if (_conditionUnit) delete _conditionUnit;
		if (_ifUnit) delete _ifUnit;
		if (_elseUnit) delete _elseUnit;
	}
	
	void ConditionalCommand::buildCommandText() {
		std::stringstream ss;
		_conditionUnit->buildCommandText();
		_ifUnit->buildCommandText();
		_elseUnit->buildCommandText();

		ss << _conditionUnit->toString() << " ? " << _ifUnit->toString() << ":" << _elseUnit->toString();
		setCommandText(ss.str());
	}

	void ConditionalCommand::setCommandData(TargetedCommand* conditionUnit, TargetedCommand* ifUnit, TargetedCommand* elseUnit) {
		_conditionUnit = conditionUnit;
		_ifUnit = ifUnit;
		_elseUnit = elseUnit;
	}

	void ConditionalCommand::execute() {
		//fist execute condition
		_conditionUnit->execute();

		//read condition result and evaluate
		Context* context = Context::getCurrent();
		int conditionOffset = _conditionUnit->getTargetOffset() + context->getCurrentOffset();
		bool* conditionValue = (bool*)context->getAbsoluteAddress(conditionOffset);

		if (*conditionValue) {
			//execute if clause
			_ifUnit->execute();
		}
		else {
			//execute else clause
			_elseUnit->execute();
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	TriggerCommand::TriggerCommand() : _mainCommand(nullptr) {}

	TriggerCommand::~TriggerCommand() {
		if (_mainCommand) {
			delete _mainCommand;
		}
	}

	void TriggerCommand::buildCommandText() {		
		if (_mainCommand) {
			_mainCommand->buildCommandText();
			setCommandText(_mainCommand->toString());
		}		
	}

	void TriggerCommand::setCommand(TargetedCommand* mainCommand) {
		_mainCommand = mainCommand;
	}

	void TriggerCommand::setBeforeTrigger(const std::shared_ptr<DFunction>& beforeExecute) {
		_beforeExecuteFunc = beforeExecute;
	}

	void TriggerCommand::setAfterTrigger(const std::shared_ptr<DFunction>& afterExecute) {
		_afterExecuteFunc = afterExecute;
	}

	void TriggerCommand::execute() {
		if (_beforeExecuteFunc) {
			_beforeExecuteFunc->call();
		}

		_mainCommand->execute();

		if (_afterExecuteFunc) {
 			_afterExecuteFunc->call();
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	ConditionTriggerCommand::ConditionTriggerCommand() {}

	ConditionTriggerCommand::~ConditionTriggerCommand() {
	}

	void ConditionTriggerCommand::buildCommandText() {
		TriggerCommand::buildCommandText();
	}

	void ConditionTriggerCommand::execute() {
		char condition = 1;
		if (_beforeExecuteFunc) {
			_beforeExecuteFunc->call();
			condition = _beforeExecuteFunc->getReturnValAsInt8();
		}

		if (condition) {
			_mainCommand->execute();

			if (_afterExecuteFunc) {
				_afterExecuteFunc->call();
			}
		}
	}
}
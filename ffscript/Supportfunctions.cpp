#include "stdafx.h"
#include "Supportfunctions.h"
#include "ControllerExecutor.h"
#include "FunctionScope.h"
#include "LoopScope.h"
#include "function\CdeclFunction.hpp"
#include "function\MemberFunction.hpp"
#include "ScriptCompiler.h"
#include "FunctionFactory.h"
#include "CodeUpdater.h"
#include "ExpUnitExecutor.h"
#include "InstructionCommand.h"
#include "DefaultCommands.h"
#include "CommandTree.h"

namespace ffscript {
	CommandBuilder::CommandBuilder() : CommandBuilder(0, ""){
	}

	CommandBuilder::CommandBuilder(UNIT_TYPE type, const std::string& name) : _type(type), _name(name){
	}

	CommandBuilder::~CommandBuilder(){
	}

	const std::string& CommandBuilder::toString()const {
		return _name;

	}
	UNIT_TYPE CommandBuilder::getType()const {
		return _type;
	}

	void CommandBuilder::setName(const std::string& name) {
		_name = name;
	}

	void CommandBuilder::setType(UNIT_TYPE type) {
		_type = type;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	EnterScopeBuilder::EnterScopeBuilder(ContextScope* contextScope) : _contextScope(contextScope) {
	}

	EnterScopeBuilder::~EnterScopeBuilder() {}

	ContextScope* EnterScopeBuilder::getScope() const {
		return _contextScope;
	}

	Executor* EnterScopeBuilder::buildNativeCommand() {
		if (getScope() == nullptr) return nullptr;

		ControllerExecutor* pExcutor = new ControllerExecutor();
		
		auto enterScopeNative = new EnterContextScope();
		pExcutor->addCommand(enterScopeNative);

		CodeUpdater* updateLaterMan = CodeUpdater::getInstance(this->getScope());
		auto updateEnterCommand = new MFunction<void, EnterScopeBuilder, EnterContextScope*>(this, &EnterScopeBuilder::fillParams);
		updateEnterCommand->pushParam(enterScopeNative);
		updateLaterMan->addUpdateLaterTask(updateEnterCommand);

		return pExcutor;
	}

	void EnterScopeBuilder::fillParams(EnterContextScope* command) const {
		//get current relative offset of the scope
		//when this function is called, the scope is full filled with variales and codes
		//so it become scope size
		command->setScopeInfo(_contextScope->getDataSize() , _contextScope->getScopeSize() - _contextScope->getDataSize(), _contextScope->getConstructorCommandCount());
		command->storeAutoRunCommand(*_contextScope->getConstructorList());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	ExitScopeBuilder::ExitScopeBuilder(ContextScope* contextScope) : _contextScope(contextScope), _restoreCall(true) {
	}

	ExitScopeBuilder::~ExitScopeBuilder() {}

	void ExitScopeBuilder::setRestoreCallFlag(bool blRestoreCall) {
		_restoreCall = blRestoreCall;
	}

	ContextScope* ExitScopeBuilder::getScope() const {
		return _contextScope;
	}

	Executor* ExitScopeBuilder::buildNativeCommand() {
		if (getScope() == nullptr) return nullptr;

		ControllerExecutor* pExcutor = new ControllerExecutor();
		auto exitContextScope = buildExitScopeCommand(_contextScope);
		exitContextScope->setRestoreCallFlag(_restoreCall);
		pExcutor->addCommand(exitContextScope);

		return pExcutor;
	}

	ExitContextScope* ExitScopeBuilder::buildExitScopeCommand(ContextScope* scope) {
		ExitContextScope* exitScopeCommand = new ExitContextScope();

		CodeUpdater* updateLaterMan = CodeUpdater::getInstance(scope);
		auto updateExitScopeCommand = new CdeclFunction<void, ContextScope*, DFunction*>(ExitScopeBuilder::fillParams);
		updateExitScopeCommand->pushParam(scope);
		updateExitScopeCommand->pushParam(exitScopeCommand);
		updateLaterMan->addUpdateLaterTask(updateExitScopeCommand);

		return exitScopeCommand;
	}

	void ExitScopeBuilder::fillParams(ContextScope* scope, ExitContextScope* command) {
		//get current relative offset of the scope
		//when this function is called, the scope is full filled with variales and codes
		//so it become scope size
		int dataSize = scope->getDataSize();
		command->setScopeInfo(dataSize, scope->getScopeSize() - dataSize);
		command->storeAutoRunCommand(*scope->getConstructorList());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	ExitFunctionBuilder::ExitFunctionBuilder() {}
	ExitFunctionBuilder::~ExitFunctionBuilder() {}
	Executor* ExitFunctionBuilder::buildNativeCommand() {

		ControllerExecutor* pExcutor = new ControllerExecutor();
		auto exitFunctionCommand = new ExitFunctionAtTheEnd();
		pExcutor->addCommand(exitFunctionCommand);

		return pExcutor;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ReturnCommandBuilder_FillParam(ContextScope* scope, ContextScope* functionScope , ExitScriptFuntionAtReturn* exitCommand) {
		ContextScope* currScope = scope;
		auto& exitFunctionCommands = exitCommand->getCommands();
		while (true) {
			currScope->buildExitScopeCodeCommands(exitFunctionCommands);
			if (currScope == functionScope) {
				break;
			}
			currScope = (ContextScope*)currScope->getParent();
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ReturnCommandBuilder::ReturnCommandBuilder(ContextScope* ownerScope, FunctionScope* functionScope) :
		_functionScope(functionScope),
		_ownerScope(ownerScope),
		_returnDataUnit(nullptr)
	{
	}
	ReturnCommandBuilder::~ReturnCommandBuilder() {}

	void ReturnCommandBuilder::setReturnExpression(CommandUnitBuilder* returnDataUnit) {
		_returnDataUnit = returnDataUnit;
	}


	Executor* ReturnCommandBuilder::buildNativeCommand() {
		ControllerExecutor* pExcutor = new ControllerExecutor();
		CodeUpdater* updateLaterMan = CodeUpdater::getInstance(_functionScope);

		if (_returnDataUnit) {
			auto copyReturnData = new PushParamOffset();
			pExcutor->addCommand(copyReturnData);

			auto updateReturnCommand = new MFunction<void, ReturnCommandBuilder, PushParamOffset*>(this, &ReturnCommandBuilder::fillParams);
			updateReturnCommand->pushParam(copyReturnData);
			updateLaterMan->addUpdateLaterTask(updateReturnCommand);
		}

		auto exitAtReturn = new ExitScriptFuntionAtReturn();
		pExcutor->addCommand(exitAtReturn);

		auto updateReturnCommand2 = new CdeclFunction<void, ContextScope*, ContextScope*, ExitScriptFuntionAtReturn*>(ReturnCommandBuilder_FillParam);
		updateReturnCommand2->pushParam(_ownerScope);
		updateReturnCommand2->pushParam(_functionScope);
		updateReturnCommand2->pushParam(exitAtReturn);
		updateLaterMan->addUpdateLaterTask(updateReturnCommand2);

		return pExcutor;
	}

	void ReturnCommandBuilder::fillParams(PushParamOffset* command) const {
		if (_returnDataUnit == nullptr) return;

		CodeUpdater* updateLaterMan = CodeUpdater::getInstance(_functionScope);
		ExpUnitExecutor* unitExecutor = (ExpUnitExecutor*)updateLaterMan->findUpdateInfo(_returnDataUnit);
		int returnDataOffset = unitExecutor->getReturnOffset();

		ScriptCompiler* scriptCompiler = _functionScope->getCompiler();
		FunctionFactory* functionFactory = scriptCompiler->getFunctionFactory(_functionScope->getFunctionId());

		auto& returnType = functionFactory->getReturnType();
		int dataSize = scriptCompiler->getTypeSize(returnType);

		command->setCommandData(
			returnDataOffset,				// source data
			dataSize,						// return data size
			BEGIN_FUNCTION_OFFSET_DATA);	// return address always start at begin of function data
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ReturnCommandBuilder2::ReturnCommandBuilder2(ContextScope* ownerScope, FunctionScope* functionScope) :
		_functionScope(functionScope),
		_ownerScope(ownerScope),
		_returnDataUnit(nullptr),
		_indexPreventDestructorRun(-1)
	{
	}

	ReturnCommandBuilder2::~ReturnCommandBuilder2() {}

	void ReturnCommandBuilder2::setReturnExpression(CommandUnitBuilder* returnDataUnit) {
		_returnDataUnit = returnDataUnit;
	}

	void ReturnCommandBuilder2::setDestructorIndex(int indexPreventDestructorRun) {
		_indexPreventDestructorRun = indexPreventDestructorRun;
	}

	Executor* ReturnCommandBuilder2::buildNativeCommand() {
		ControllerExecutor* pExcutor = new ControllerExecutor();
		CodeUpdater* updateLaterMan = CodeUpdater::getInstance(_functionScope);

		if (_returnDataUnit) {
			auto copyReturnData = new CopyDataToRef();
			pExcutor->addCommand(copyReturnData);

			auto updateReturnCommand = new MFunction<void, ReturnCommandBuilder2, CopyDataToRef*>(this, &ReturnCommandBuilder2::fillParams);
			updateReturnCommand->pushParam(copyReturnData);
			updateLaterMan->addUpdateLaterTask(updateReturnCommand);
		}

		auto exitAtReturn = new ExitScriptFuntionAtReturn();
		exitAtReturn->setCommandData(_indexPreventDestructorRun);
		pExcutor->addCommand(exitAtReturn);

		auto updateReturnCommand2 = new CdeclFunction<void, ContextScope*, ContextScope*, ExitScriptFuntionAtReturn*>(ReturnCommandBuilder_FillParam);
		updateReturnCommand2->pushParam(_ownerScope);
		updateReturnCommand2->pushParam(_functionScope);
		updateReturnCommand2->pushParam(exitAtReturn);
		updateLaterMan->addUpdateLaterTask(updateReturnCommand2);

		return pExcutor;
	}

	void ReturnCommandBuilder2::fillParams(CopyDataToRef* command) const {
		if (_returnDataUnit == nullptr) return;

		CodeUpdater* updateLaterMan = CodeUpdater::getInstance(_functionScope);
		ExpUnitExecutor* unitExecutor = (ExpUnitExecutor*)updateLaterMan->findUpdateInfo(_returnDataUnit);
		int returnDataOffset = unitExecutor->getReturnOffset();

		ScriptCompiler* scriptCompiler = _functionScope->getCompiler();
		FunctionFactory* functionFactory = scriptCompiler->getFunctionFactory(_functionScope->getFunctionId());

		auto& returnType = functionFactory->getReturnType();
		int dataSize = scriptCompiler->getTypeSize(returnType);

		command->setCommandData(
			returnDataOffset,				// source data
			dataSize,						// return data size
			BEGIN_FUNCTION_OFFSET_DATA);	// return address always start at begin of function data
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BreakCommandBuilder::BreakCommandBuilder(ContextScope* ownerScope, LoopScope* loopScope) :
		_loopScope(loopScope),
		_ownerScope(ownerScope)
	{
	}

	BreakCommandBuilder::~BreakCommandBuilder() {}

	LoopScope* BreakCommandBuilder::getLoopScope() const {
		return _loopScope;
	}

	void BreakCommandBuilder::setLoopScope(LoopScope* loopScope) {
		_loopScope = loopScope;
	}

	Executor* BreakCommandBuilder::buildNativeCommand() {
		ControllerExecutor* pExcutor = new ControllerExecutor();
		

		auto breakCommand = new BreakCommand();
		pExcutor->addCommand(breakCommand);

		CodeUpdater* updateLaterMan = CodeUpdater::getInstance(_ownerScope);
		auto updateBreakCommand = new MFunction<void, BreakCommandBuilder, BreakCommand*>(this, &BreakCommandBuilder::fillParams);
		updateBreakCommand->pushParam(breakCommand);
		updateLaterMan->addUpdateLaterTask(updateBreakCommand);

		return pExcutor;
	}
	
	void BreakCommandBuilder::fillParams(BreakCommand* command) const {
		ContextScope* currScope = _ownerScope;
		auto& breakCommands = command->getCommands();
		while (true) {
			currScope->buildExitScopeCodeCommands(breakCommands);
			if (currScope == _loopScope) {
				break;
			}
			currScope = (ContextScope*)currScope->getParent();
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ContinueCommandBuilder::ContinueCommandBuilder(ContextScope* ownerScope, LoopScope* loopScope) : BreakCommandBuilder(ownerScope, loopScope)
	{}

	ContinueCommandBuilder::~ContinueCommandBuilder() {}

	Executor* ContinueCommandBuilder::buildNativeCommand() {
		ControllerExecutor* pExcutor = new ControllerExecutor();

		auto continueCommand = new ContinueCommand();
		pExcutor->addCommand(continueCommand);

		CodeUpdater* updateLaterMan = CodeUpdater::getInstance(_ownerScope);
		auto updateContinueCommand = new MFunction<void, ContinueCommandBuilder, ContinueCommand*>(this, &ContinueCommandBuilder::fillParams);
		updateContinueCommand->pushParam(continueCommand);
		updateLaterMan->addUpdateLaterTask(updateContinueCommand);

		return pExcutor;
	}

	void ContinueCommandBuilder::fillParams(ContinueCommand* command) const {
		ContextScope* currScope = _ownerScope;
		auto& breakCommands = command->getCommands();
		while (currScope != _loopScope) {
			currScope->buildExitScopeCodeCommands(breakCommands);
			currScope = (ContextScope*)currScope->getParent();
		}

		auto exitCommand = _loopScope->getBeginExitScopeCommand();

		Program* program = currScope->getCompiler()->getProgram();
		CodeUpdater* updateLaterMan = CodeUpdater::getInstance(_ownerScope);
		ExpUnitExecutor* unitExecutor = (ExpUnitExecutor*)updateLaterMan->findUpdateInfo(_loopScope->getConditionExpression());
		auto codeSegment = program->getCode(unitExecutor);

		auto conditionCommand = codeSegment->first;
		auto loopCommand = conditionCommand;
		if(exitCommand < conditionCommand){ 
			loopCommand = exitCommand;
		}

		loopCommand--;
		command->setLoopCommand(loopCommand);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	JumpToSubScopeCommandBuilder::JumpToSubScopeCommandBuilder(ContextScope* subScope) : _subScope(subScope) {
	}

	JumpToSubScopeCommandBuilder::~JumpToSubScopeCommandBuilder() {}

	Executor* JumpToSubScopeCommandBuilder::buildNativeCommand() {
		ControllerExecutor* pExcutor = new ControllerExecutor();
		InstructionCommand* command = new Jump();
		pExcutor->addCommand(command);

		CodeUpdater* updateLaterMan = CodeUpdater::getInstance(_subScope);
		auto updateIfCommand = new MFunction<void, JumpToSubScopeCommandBuilder, Jump*>(this, &JumpToSubScopeCommandBuilder::fillParams);
		updateIfCommand->pushParam(command);
		updateLaterMan->addUpdateLaterTask(updateIfCommand);

		return pExcutor;
	}

	void JumpToSubScopeCommandBuilder::fillParams(Jump* command) const {		
		command->setCommandData(_subScope->getCode()->first - 1);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IfCommandBuilder::IfCommandBuilder(ContextScope* ifScope) : _ifScope(ifScope), _elseScope(nullptr), _conditionUnit(nullptr){
	}

	IfCommandBuilder::~IfCommandBuilder() {}

	ContextScope* IfCommandBuilder::getIfScope() const {
		return _ifScope;
	}

	void IfCommandBuilder::setIfScope(ContextScope* ifScope) {
		_ifScope = ifScope;
	}

	ContextScope* IfCommandBuilder::getElseScope() const {
		return _elseScope;
	}

	void IfCommandBuilder::setElseScope(ContextScope* elseScope) {
		_elseScope = elseScope;
	}

	void IfCommandBuilder::setConditionExpression(CommandUnitBuilder* conditionUnit) {
		_conditionUnit = conditionUnit;
	}
	
	Executor* IfCommandBuilder::buildNativeCommand() {
		ControllerExecutor* pExcutor = new ControllerExecutor();

		InstructionCommand* command;

		if (_elseScope) {
			command = new JumpIfElse();
		}
		else {
			command = new JumpIf();
		}
		pExcutor->addCommand(command);

		CodeUpdater* updateLaterMan = CodeUpdater::getInstance(_ifScope);
		auto updateIfCommand = new MFunction<void, IfCommandBuilder, InstructionCommand*>(this, &IfCommandBuilder::fillParams);
		updateIfCommand->pushParam(command);
		updateLaterMan->addUpdateLaterTask(updateIfCommand);

		return pExcutor;
	}

	void IfCommandBuilder::fillParams(InstructionCommand* command) const {
		CodeUpdater* updateLaterMan = CodeUpdater::getInstance(_ifScope);
		ExpUnitExecutor* unitExecutor = (ExpUnitExecutor*)updateLaterMan->findUpdateInfo(_conditionUnit);

		int conditionOffset = unitExecutor->getReturnOffset();

		((JumpIf*)command)->setCommandData(conditionOffset, _ifScope->getCode()->first - 1);
		if (_elseScope) {
			((JumpIfElse*)command)->setCommandElse(_elseScope->getCode()->first - 1);
		}
		
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	LoopCommandBuilder::LoopCommandBuilder(LoopScope* loopScope) : _loopScope(loopScope) {
	}

	LoopCommandBuilder::~LoopCommandBuilder() {}

	LoopScope* LoopCommandBuilder::getLoopScope() const {
		return _loopScope;
	}

	void LoopCommandBuilder::setLoopScope(LoopScope* loopScope) {
		_loopScope = loopScope;
	}
	
	Executor* LoopCommandBuilder::buildNativeCommand() {
		ControllerExecutor* pExcutor = new ControllerExecutor();

		auto jumpIf = new JumpIf();
		pExcutor->addCommand(jumpIf);

		CodeUpdater* updateLaterMan = CodeUpdater::getInstance(_loopScope);
		auto updateLoopCommand = new MFunction<void, LoopCommandBuilder, JumpIf*>(this, &LoopCommandBuilder::fillParams);
		updateLoopCommand->pushParam(jumpIf);
		updateLaterMan->addUpdateLaterTask(updateLoopCommand);

		return pExcutor;
	}

	void LoopCommandBuilder::fillParams(JumpIf* command) const {
		CodeUpdater* updateLaterMan = CodeUpdater::getInstance(_loopScope);
		ExpUnitExecutor* unitExecutor = (ExpUnitExecutor*)updateLaterMan->findUpdateInfo(_loopScope->getConditionExpression());

		int conditionOffset = unitExecutor->getReturnOffset();

		//the loop command is placed at the end of loop scope but before exit scope command
		//when program run this command, it will check condition to decide whether goto begin of loop scope or run next command to exit scope
		//at the first command of loop scope is always enter scope command
		//so we set command cursor is first command of scope.
		//However, the thread will execute the next command in the next loop.
		//This is satisfy the loop state, when the code is loop but we don't need to reallocated memory in each loop.
		auto firstLoopCommand = _loopScope->getCode()->first;
	
		command->setCommandData(conditionOffset, firstLoopCommand);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//DefaultConstructorCommandBuilder::DefaultConstructorCommandBuilder(OperatorObject* pOperatorObject, Variable* pVariable) :
	//	_pOperatorObject(pOperatorObject),
	//	_pVariable(pVariable) {
	//}

	//DefaultConstructorCommandBuilder::~DefaultConstructorCommandBuilder() {}


	//Executor* DefaultConstructorCommandBuilder::buildNativeCommand() {
	//	return nullptr;
	//}

	//void DefaultConstructorCommandBuilder::fillParams() const {		
	//}

	////////////////////////////////////////////////////////////////////////////	
}
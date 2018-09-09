/******************************************************************
* File:        LoopScope.cpp
* Description: implement LoopScope class. Loop scope is a context
*              scope but specialized for a loop such as while, for.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "LoopScope.h"
#include "Internal.h"
#include "Utils.h"
#include "ScriptCompiler.h"
#include "FunctionScope.h"
#include "Supportfunctions.h"
#include "ExpUnitExecutor.h"
#include "ControllerExecutor.h"
#include "CodeUpdater.h"
#include "FunctionFactory.h"
#include "BasicType.h"

namespace ffscript {
	extern std::string key_while;

	LoopScope::LoopScope(ContextScope* parent, FunctionScope* functionScope) :
		ContextScope(parent, functionScope),
		_conditionExpression(nullptr)
	{
		setLoopScope(this);
		setName(key_while);
	}

	LoopScope::~LoopScope(){}

	CommandUnitBuilder* LoopScope::getConditionExpression() const {
		return _conditionExpression;
	}

	////////////////////////////////////////////////////////////////////////////////
	///
	/// parse while scope
	/// convert:
	/// while(expression) {
	///     ...
    /// }
	/// to:
	/// if(expression) {
	///     begin_while:
	///     ...
	///     if(expression) goto begin_while;
	/// }
	///
	///////////////////////////////////////////////////////////////////////////////
	const wchar_t* LoopScope::parse(const wchar_t* text, const wchar_t* end) {
		const wchar_t* c;
		c = trimLeft(text, end);
		const wchar_t* conditionText = c;

		ContextScope* parent = (ContextScope*)getParent();

		// the first condition command will be used to check if
		// the instruction command can jump to while scope or not
		c = parent->parseCondition(c, end);
		if (c == nullptr) {
			return nullptr;
		}

		auto conditionUnitIter = parent->getLastExpression();
		auto updateLaterMan = CodeUpdater::getInstance(this);
		updateLaterMan->setUpdateInfo(conditionUnitIter->get(), nullptr);

		// build if command from the condition command
		IfCommandBuilder* loopCondition = new IfCommandBuilder(this);
		loopCondition->setConditionExpression(conditionUnitIter->get());
		parent->putCommandUnit(loopCondition);

		// parse while scope as a context scope
		c = ContextScope::parse(c, end);

		// get last command in the context scope is always
		// the exit scope command
		CommandConstRefIter lastComandInScope = getLastExpression();

		if (c != nullptr) {
			CommandUnitRef lastLoopCommand = *lastComandInScope;

			// remove the exit scope command
			remove(lastComandInScope);

			// parse the condition again to create condition at the end of while
			// after parse condition command is also insert at the end of the scope
			parseCondition(conditionText, end);
			conditionUnitIter = getLastExpression();
			_conditionExpression = conditionUnitIter->get();
			updateLaterMan->setUpdateInfo(conditionUnitIter->get(), nullptr);
			// build the loop command base on condition command
			LoopCommandBuilder* loopCommand = new LoopCommandBuilder(this);

			// put the loop command (eventually, will be a if command)
			putCommandUnit(loopCommand);

			// put the exit scope command again at the end of while scope
			putCommandUnit(lastLoopCommand);
		}

		return c;
	}

	bool LoopScope::updateCodeForControllerCommands(Program* program) {
		auto res = ContextScope::updateCodeForControllerCommands(program);

		_conditionCommnand = nullptr;
		if (res) {
			auto updateLaterMan = CodeUpdater::getInstance(this);
			auto conditionExecutor = updateLaterMan->findUpdateInfo(_conditionExpression);
			_conditionCommnand = program->getCode(conditionExecutor)->first;
		}

		return res;
	}

	void LoopScope::buildExitScopeCodeCommands(CommandList& commandList) const {
		auto endCommand = getCode()->second;
		endCommand++;
		for (auto command = getBeginExitScopeCommand(); command != endCommand; command++) {
			if (_conditionCommnand != command) {
				commandList.push_back(*command);
			}
		}
	}
}
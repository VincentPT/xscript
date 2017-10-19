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

	CommandUnit* LoopScope::getConditionExpression() const {
		return _conditionExpression;
	}

	const wchar_t* LoopScope::parse(const wchar_t* text, const wchar_t* end) {
		const wchar_t* c;
		c = trimLeft(text, end);
		const wchar_t* conditionText = c;

		ContextScope* parent = (ContextScope*)getParent();

		c = parent->parseCondition(c, end);
		if (c == nullptr) {
			return nullptr;
		}

		auto conditionUnitIter = parent->getLastExpression();
		auto updateLaterMan = CodeUpdater::getInstance(this);
		updateLaterMan->setUpdateInfo(conditionUnitIter->get(), nullptr);

		IfCommandBuilder* loopCondition = new IfCommandBuilder(this);
		loopCondition->setConditionExpression(conditionUnitIter->get());
		parent->putCommandUnit(loopCondition);

		c = ContextScope::parse(c, end);

		CommandConstRefIter lastComandInScope = getLastExpression();

		if (c != nullptr) {
			CommandUnitRef lastLoopCommand = *lastComandInScope;

			remove(lastComandInScope);

			parseCondition(conditionText, end);
			conditionUnitIter = getLastExpression();
			_conditionExpression = conditionUnitIter->get();
			updateLaterMan->setUpdateInfo(conditionUnitIter->get(), nullptr);

			LoopCommandBuilder* loopCommand = new LoopCommandBuilder(this);

			putCommandUnit(loopCommand);
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
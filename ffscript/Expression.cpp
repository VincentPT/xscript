#include "stdafx.h"
#include "Expression.h"
#include "ScriptCompiler.h"

namespace ffscript {

	Expression::Expression(ExecutableUnitRef rootUnit) :
		_rootUnit(rootUnit)
	{
	}


	Expression::~Expression()
	{
	}

	void Expression::setExpString(const std::wstring& exp) {
		_expressionString = exp;
	}

	const std::wstring& Expression::getExpString() {
		return _expressionString;
	}

	const ExecutableUnitRef& Expression::getRoot() const {
		return _rootUnit;
	}

	ExecutableUnitRef& Expression::getRoot() {
		return _rootUnit;
	}
}

#include "stdafx.h"
#include "Utility.h"
#include "ExpresionParser.h"
#include "ExpUnitExecutor.h"
#include "ScriptCompiler.h"

using namespace ffscript;

ffscript::ExpUnitExecutor* compileExpression(ScriptCompiler* scriptCompiler, const std::wstring& exp) {
	ExpressionParser parser(scriptCompiler);

	list<ExpUnitRef> units;
	EExpressionResult eResult = parser.stringToExpList(exp, units);

	if (eResult != E_SUCCESS) return nullptr;

	list<ExpressionRef> expList;
	bool res = parser.compile(units, expList);
	if (!res) return nullptr;

	eResult = parser.link(expList.front().get());
	if (eResult != E_SUCCESS) return nullptr;

	ExpUnitExecutor* excutor = new ExpUnitExecutor(scriptCompiler->currentScope());
	res = excutor->extractCode(scriptCompiler, expList.front().get());

	if (!res) {
		delete excutor;
		excutor = nullptr;
	}
	return excutor;
}
#pragma once
#include <string>
#include "ExpUnitExecutor.h"
#include "ScriptCompiler.h"

ffscript::ExpUnitExecutor* compileExpression(ffscript::ScriptCompiler* scriptCompiler, const std::wstring& exp);
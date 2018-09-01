/******************************************************************
* File:        Utility.h
* Description: declare common functions used in unit test project.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include <string>
#include "ExpUnitExecutor.h"
#include "ScriptCompiler.h"

ffscript::ExpUnitExecutor* compileExpression(ffscript::ScriptCompiler* scriptCompiler, const std::wstring& exp);
/******************************************************************
* File:        RawStringLib.h
* Description: declare an interface to import String type and its
*              related functions into the script compiler.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"
#include "FunctionRegisterHelper.h"
#include <string>

namespace ffscript {
	class ScriptCompiler;

	template <class T>
	DFunction2Ref createStringNativeFunc(RawString(*f)(T)) {
		return createFunctionDelegateRef<RawString, T>(f);
	}

	void includeRawStringToCompiler(ScriptCompiler* scriptCompiler);

	void constantConstructor(RawString& rawString, const std::wstring& ws);
	void constantConstructor(RawString& rawString, const std::string& s);
}

/******************************************************************
* File:        RawStringLib.h
* Description: declare an interface to import String type and its
*              related functions into the script compiler.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
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
		return createFunctionCdeclRef<RawString, T>(f);
	}

	void includeRawStringToCompiler(ScriptCompiler* scriptCompiler);

	void constantConstructor(RawString& rawString, const std::wstring& ws);
	void constantConstructor(RawString& rawString, const std::string& s);
}

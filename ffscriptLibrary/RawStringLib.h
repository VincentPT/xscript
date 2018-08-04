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

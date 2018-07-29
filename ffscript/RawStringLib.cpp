#include "stdafx.h"
#include "RawStringLib.h"

#include "function\CdeclFunction2.hpp"
#include "function\MemberFunction2.hpp"
#include "BasicFunctionFactory.hpp"
#include "Internal.h"
#include "DynamicFunctionFactory.h"
#include "BasicType.h"
#include "ConditionalOperator.h"
#include "RefFunction.h"
#include "DefaultCommands.h"
#include "Utils.h"
#include <algorithm>

#include <locale>
#include <codecvt>

namespace ffscript {
	template <class T>
	DFunction2Ref createStringNativeFunc(RawString(*f)(T)) {
		return std::make_shared<CdeclFunction2<RawString, T>>(f);
	}

	template <class Rt, class T1, class T2>
	DFunction2* createBinaryOperatorCdecl(Rt(_cdecl *f)(T1, T2)) {
		return new CdeclFunction2<Rt, T1, T2>(f);
	}

	template <class Rt, class T1, class T2>
	FunctionFactory* createBinaryUserFunctionCdecl(ScriptCompiler* scriptCompiler, const char* rt, Rt(_cdecl *f)(T1, T2)) {
		return new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, rt, new CdeclFunction2<Rt, T1, T2>(f), scriptCompiler);
	}

	///////////////////////////// Raw String constructors //////////////////////////////////////////////
	void defaultConstructor(RawString& s) {
		s = allocRawString(0);
	}

	void constantConstructor(RawString& rawString, const std::wstring& ws) {
		if (ws.size() == 0) {
			defaultConstructor(rawString);
		}
		else {
			rawString = allocRawString((int)ws.size());
			memcpy(rawString.elms, ws.c_str(), ws.size() * sizeof(RawChar));
			rawString.elms[rawString.size] = 0;
		}
	}

	inline void assign(RawChar* begin, const std::string& s) {
		auto wc = begin;
		auto c = s.c_str();
		auto cEnd = c + s.size();
		for (; c != cEnd; c++) {
			*wc++ = *c;
		}
		*wc = 0;
	}

	void constantConstructor(RawString& rawString, const std::string& s) {
		if (s.size() == 0) {
			defaultConstructor(rawString);
		}
		else {
			rawString = allocRawString((int)s.size());
			assign(rawString.elms, s);
		}
	}

	void constantConstructor(RawString& rawString, const RawString& s) {
		if (s.size == 0) {
			defaultConstructor(rawString);
		}
		else {
			rawString = allocRawString((int)s.size);
			memcpy(rawString.elms, s.elms, (s.size + 1) * sizeof(RawChar));
		}
	}

	///////////////////////////// Raw String compare functions //////////////////////////////////////////////
	int constantCompare(const RawString& rawString, const std::wstring& ws) {
		if (rawString.elms == nullptr) {
			throw std::runtime_error("String is null");
		}
		return wcsncmp(rawString.elms, ws.c_str(), rawString.size);
	}
	int constantCompare(const std::wstring& ws, const RawString& rawString) {
		return -constantCompare(rawString, ws);
	}

	bool operator==(const RawString& rawString, const std::wstring& ws) {
		return constantCompare(rawString, ws) == 0;
	}
	bool operator==(const std::wstring& ws, const RawString& rawString) {
		return constantCompare(rawString, ws) == 0;
	}
	bool operator!=(const RawString& rawString, const std::wstring& ws) {
		return constantCompare(rawString, ws) != 0;
	}
	bool operator!=(const std::wstring& ws, const RawString& rawString) {
		return constantCompare(rawString, ws) != 0;
	}

	int constantCompare(const RawString& rawString, const std::string& s) {
		if (rawString.elms == nullptr) {
			throw std::runtime_error("String is null");
		}
		auto wc = rawString.elms;
		auto wcEnd = wc + rawString.size;
		auto c = s.c_str();
		auto cEnd = c + s.size();
		for (; c != cEnd && wc != wcEnd; c++, wc++) {
			if (*wc > (RawChar)*c) {
				return 1;
			}
			if (*wc < (RawChar)*c) {
				return -1;
			}
		}

		if (c == cEnd && wc == wcEnd) {
			return 0;
		}

		if (c == cEnd) {
			return 1;
		}
		return -1;
	}
	int constantCompare(const std::string& s, const RawString& rawString) {
		return -constantCompare(rawString, s);
	}
	bool operator==(const RawString& rawString, const std::string& s) {
		return constantCompare(rawString, s) == 0;
	}
	bool operator==(const std::string& s, const RawString& rawString) {
		return constantCompare(rawString, s) == 0;
	}
	bool operator!=(const RawString& rawString, const std::string& s) {
		return constantCompare(rawString, s) != 0;
	}
	bool operator!=(const std::string& s, const RawString& rawString) {
		return constantCompare(rawString, s) != 0;
	}

	int constantCompare(const RawString& rawString, const RawString& s) {
		if (rawString.elms == nullptr) {
			throw std::runtime_error("String is null");
		}
		return wcsncmp(rawString.elms, s.elms, rawString.size);
	}
	bool operator==(const RawString& rawString, const RawString& s) {
		return constantCompare(rawString, s) == 0;
	}
	bool operator!=(const RawString& rawString, const RawString& s) {
		return constantCompare(rawString, s) != 0;
	}

	///////////////////////////// Raw String conversion functions //////////////////////////////////////////////
	RawString ToString(bool val) {
		if (val) {
			RawString rawString = allocRawString(4);
			rawString.elms[0] = 't';
			rawString.elms[1] = 'r';
			rawString.elms[2] = 'u';
			rawString.elms[3] = 'e';
			rawString.elms[4] = 0;

			return rawString;
		}

		RawString rawString = allocRawString(5);

		rawString.elms[0] = 'f';
		rawString.elms[1] = 'a';
		rawString.elms[2] = 'l';
		rawString.elms[3] = 's';
		rawString.elms[4] = 'e';
		rawString.elms[5] = 0;

		return rawString;
	}

	RawString ToString(int val) {
		RawChar buffer[12];
		swprintf(buffer, L"%d", val);

		RawString rawString = allocRawString((int)wcslen(buffer));
		memcpy(rawString.elms, buffer, (rawString.size + 1) * sizeof(RawChar));

		return rawString;
	}

	RawString ToString(const long long& val) {
		RawChar buffer[64];
		swprintf(buffer, L"%lld", val);

		RawString rawString = allocRawString((int)wcslen(buffer));
		memcpy(rawString.elms, buffer, (rawString.size + 1) * sizeof(RawChar));

		return rawString;
	}

	RawString ToString(const float& val) {
		RawChar buffer[15];
		swprintf(buffer, L"%f", val);

		RawString rawString = allocRawString((int)wcslen(buffer));
		memcpy(rawString.elms, buffer, (rawString.size + 1) * sizeof(RawChar));

		return rawString;
	}

	RawString ToString(const double& val) {
		RawChar buffer[64];
		swprintf(buffer, L"%f", val);

		RawString rawString = allocRawString((int)wcslen(buffer));
		memcpy(rawString.elms, buffer, (rawString.size + 1) * sizeof(RawChar));

		return rawString;
	}

	///////////////////////////// Raw String other functions //////////////////////////////////////////////
	template <class Source, class Target>
	void assignStringConst(Source rws, Target s) {
		if (rws.size != (int)s.size()) {
			freeRawString(rws);
		}
		constantConstructor(rws, s);
	}

	void assignString(RawString& rws, const RawString& s) {
		if (rws.size != s.size) {
			freeRawString(rws);
		}
		constantConstructor(rws, s);
	}

	RawString operator+(const RawString& rws, const RawString& s) {
		RawString rwNew;
		if (rws.size == 0) {
			constantConstructor(rwNew, s);
		}
		else if (s.size == 0) {
			constantConstructor(rwNew, rws);
		}
		else {
			rwNew = allocRawString(rws.size + s.size);
			memcpy(rwNew.elms, rws.elms, rws.size * sizeof(RawChar));
			memcpy(rwNew.elms + rws.size, s.elms, (s.size + 1) * sizeof(RawChar));
		}

		return rwNew;
	}

	RawString operator+(const RawString& rws, const std::string& s) {
		RawString rwNew;
		if (rws.size == 0) {
			constantConstructor(rwNew, s);
		}
		else if (s.size() == 0) {
			constantConstructor(rwNew, rws);
		}
		else {
			rwNew = allocRawString(rws.size + (int)s.size());
			memcpy(rwNew.elms, rws.elms, rws.size * sizeof(RawChar));
			assign(rwNew.elms + rws.size, s);
		}

		return rwNew;
	}

	RawString operator+(const std::string& s, const RawString& rws) {
		RawString rwNew;
		if (rws.size == 0) {
			constantConstructor(rwNew, s);
		}
		else if (s.size() == 0) {
			constantConstructor(rwNew, rws);
		}
		else {
			rwNew = allocRawString(rws.size + (int)s.size());
			assign(rwNew.elms, s);
			memcpy(rwNew.elms + s.size(), rws.elms, (rws.size + 1) * sizeof(RawChar));
		}

		return rwNew;
	}

	RawString operator+(const RawString& rws, const std::wstring& s) {
		RawString rwNew;
		if (rws.size == 0) {
			constantConstructor(rwNew, s);
		}
		else if (s.size() == 0) {
			constantConstructor(rwNew, rws);
		}
		else {
			rwNew = allocRawString(rws.size + (int)s.size());
			memcpy(rwNew.elms, rws.elms, rws.size * sizeof(RawChar));
			memcpy(rwNew.elms + rws.size, s.c_str(), (s.size() + 1) * sizeof(RawChar));
		}

		return rwNew;
	}

	RawString operator+(const std::wstring& s, const RawString& rws) {
		RawString rwNew;
		if (rws.size == 0) {
			constantConstructor(rwNew, s);
		}
		else if (s.size() == 0) {
			constantConstructor(rwNew, rws);
		}
		else {
			rwNew = allocRawString(rws.size + (int)s.size());
			memcpy(rwNew.elms, s.c_str(), s.size() * sizeof(RawChar));
			memcpy(rwNew.elms + s.size(), rws.elms, (rws.size + 1) * sizeof(RawChar));
		}

		return rwNew;
	}

	template <class T>
	RawString operator+(const RawString& rws, T val) {
		if (rws.size == 0) {
			return ToString(val);
		}
		auto valStr = ToString(val);
		RawString rwNew = rws + valStr;
		freeRawString(valStr);

		return rwNew;
	}

	template <class T>
	RawString operator+(T val, const RawString& rws) {
		if (rws.size == 0) {
			return ToString(val);
		}
		auto valStr = ToString(val);
		RawString rwNew = valStr + rws;
		freeRawString(valStr);

		return rwNew;
	}

	template <class T, class TString>
	RawString addValWithConsant(T val, TString s) {
		if (s.size() == 0) {
			return ToString(val);
		}
		
		auto valStr = ToString(val);
		RawString rwNew = valStr + s;
		freeRawString(valStr);
		return rwNew;
	}

	template <class TString, class T>
	RawString addConstantWithVal(TString s, T val) {
		if (s.size() == 0) {
			return ToString(val);
		}

		auto valStr = ToString(val);
		RawString rwNew = s + valStr;
		freeRawString(valStr);
		return rwNew;
	}

	void includeRawStringToCompiler(ScriptCompiler* scriptCompiler) {
		FunctionRegisterHelper fb(scriptCompiler);

		auto& basicTypes = scriptCompiler->getTypeManager()->getBasicTypes();

		// register RawString type
		int iTypeString = basicTypes.TYPE_RAWSTRING = scriptCompiler->registType("String");
		scriptCompiler->setTypeSize(iTypeString, sizeof(RawString));

		// type string object
		ScriptType typeString(iTypeString, "String");

		// register constructors and destructor
		int ctor = fb.registFunction("defaultConstructor", "ref String", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, RawString&>(defaultConstructor), scriptCompiler));
		int dtor = fb.registFunction("freeRawString", "ref String", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, RawString&>(freeRawString), scriptCompiler));

		scriptCompiler->registDestructor(iTypeString, dtor);
		scriptCompiler->registConstructor(iTypeString, ctor);

		ctor = fb.registFunction("constantConstructor", "ref String, string&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, RawString&, const std::string&>(constantConstructor), scriptCompiler));
		scriptCompiler->registConstructor(iTypeString, ctor);

		ctor = fb.registFunction("constantConstructor", "ref String, wstring&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, RawString&, const std::wstring&>(constantConstructor), scriptCompiler));
		scriptCompiler->registConstructor(iTypeString, ctor);

		ctor = fb.registFunction("constantConstructor", "ref String, String&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, RawString&, const RawString&>(constantConstructor), scriptCompiler));
		scriptCompiler->registConstructor(iTypeString, ctor);

		// register conversion operators
		fb.registFunction("String", "bool", new ConvertToStringFactory(scriptCompiler, createStringNativeFunc<bool>(ToString), typeString));
		fb.registFunction("String", "int", new ConvertToStringFactory(scriptCompiler, createStringNativeFunc<int>(ToString), typeString));
		fb.registFunction("String", "long&", new ConvertToStringFactory(scriptCompiler, createStringNativeFunc<const long long&>(ToString), typeString));
		fb.registFunction("String", "float&", new ConvertToStringFactory(scriptCompiler, createStringNativeFunc<const float&>(ToString), typeString));
		fb.registFunction("String", "double&", new ConvertToStringFactory(scriptCompiler, createStringNativeFunc<const double&>(ToString), typeString));

		// conversion accurative help engine choose the best overloading function for given arguments
		scriptCompiler->registerTypeConversionAccurative(basicTypes.TYPE_BOOL, basicTypes.TYPE_RAWSTRING, 10000);
		scriptCompiler->registerTypeConversionAccurative(basicTypes.TYPE_INT, basicTypes.TYPE_RAWSTRING, 10000);
		scriptCompiler->registerTypeConversionAccurative(basicTypes.TYPE_LONG, basicTypes.TYPE_RAWSTRING, 10000);
		scriptCompiler->registerTypeConversionAccurative(basicTypes.TYPE_FLOAT, basicTypes.TYPE_RAWSTRING, 10000);
		scriptCompiler->registerTypeConversionAccurative(basicTypes.TYPE_DOUBLE, basicTypes.TYPE_RAWSTRING, 10000);

		// register compare operators and functions
		fb.registPredefinedOperators("==", "String&,string&", "bool", createBinaryOperatorCdecl<bool, const RawString&, const std::string&>(operator==));
		fb.registPredefinedOperators("!=", "String&,string&", "bool", createBinaryOperatorCdecl<bool, const RawString&, const std::string&>(operator!=));
		fb.registPredefinedOperators("==", "string&,String&", "bool", createBinaryOperatorCdecl<bool, const std::string&, const RawString&>(operator==));
		fb.registPredefinedOperators("!=", "string&,String&", "bool", createBinaryOperatorCdecl<bool, const std::string&, const RawString&>(operator!=));

		fb.registPredefinedOperators("==", "String&,wstring&", "bool", createBinaryOperatorCdecl<bool, const RawString&, const std::wstring&>(operator==));
		fb.registPredefinedOperators("!=", "String&,wstring&", "bool", createBinaryOperatorCdecl<bool, const RawString&, const std::wstring&>(operator!=));
		fb.registPredefinedOperators("==", "wstring&,String&", "bool", createBinaryOperatorCdecl<bool, const std::wstring&, const RawString&>(operator==));
		fb.registPredefinedOperators("!=", "wstring&,String&", "bool", createBinaryOperatorCdecl<bool, const std::wstring&, const RawString&>(operator!=));

		fb.registPredefinedOperators("==", "String&,String&", "bool", createBinaryOperatorCdecl<bool, const RawString&, const RawString&>(operator==));
		fb.registPredefinedOperators("!=", "String&,String&", "bool", createBinaryOperatorCdecl<bool, const RawString&, const RawString&>(operator!=));

		fb.registFunction("compare", "String&,string&", createBinaryUserFunctionCdecl<int, const RawString&, const std::string&>(scriptCompiler, "int", constantCompare));
		fb.registFunction("compare", "string&,String&", createBinaryUserFunctionCdecl<int, const std::string&, const RawString&>(scriptCompiler, "int", constantCompare));
		fb.registFunction("compare", "String&,wstring&", createBinaryUserFunctionCdecl<int, const RawString&, const std::wstring&>(scriptCompiler, "int", constantCompare));
		fb.registFunction("compare", "wstring&,String&", createBinaryUserFunctionCdecl<int, const std::wstring&, const RawString&>(scriptCompiler, "int", constantCompare));
		fb.registFunction("compare", "String&,String&", createBinaryUserFunctionCdecl<int, const RawString&, const RawString&>(scriptCompiler, "int", constantCompare));

		// register other operators
		fb.registPredefinedOperators("=", "String&,string&", "void", createBinaryOperatorCdecl<void, RawString&, const std::string&>(assignStringConst));
		fb.registPredefinedOperators("=", "String&,wstring&", "void", createBinaryOperatorCdecl<void, RawString&, const std::wstring&>(assignStringConst));
		fb.registPredefinedOperators("=", "String&,String&", "void", createBinaryOperatorCdecl<void, RawString&, const RawString&>(assignString));

		fb.registPredefinedOperators("+", "String&,string&", "String", createBinaryOperatorCdecl<RawString, const RawString&, const std::string&>(operator+));
		fb.registPredefinedOperators("+", "string&,String&", "String", createBinaryOperatorCdecl<RawString, const std::string&, const RawString&>(operator+));

		fb.registPredefinedOperators("+", "String&,wstring&", "String", createBinaryOperatorCdecl<RawString, const RawString&, const std::wstring&>(operator+));
		fb.registPredefinedOperators("+", "wstring&,String&", "String", createBinaryOperatorCdecl<RawString, const std::wstring&, const RawString&>(operator+));

		fb.registPredefinedOperators("+", "String&,String&", "String", createBinaryOperatorCdecl<RawString, const RawString&, const RawString&>(operator+));

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		fb.registPredefinedOperators("+", "String&,bool", "String", createBinaryOperatorCdecl<RawString, const RawString&, bool>(operator+));
		fb.registPredefinedOperators("+", "bool,String&", "String", createBinaryOperatorCdecl<RawString, bool, const RawString&>(operator+));

		fb.registPredefinedOperators("+", "String&,int", "String", createBinaryOperatorCdecl<RawString, const RawString&, int>(operator+));
		fb.registPredefinedOperators("+", "int,String&", "String", createBinaryOperatorCdecl<RawString, int, const RawString&>(operator+));

		fb.registPredefinedOperators("+", "String&,long&", "String", createBinaryOperatorCdecl < RawString, const RawString&, const long long&> (operator+));
		fb.registPredefinedOperators("+", "long&,String&", "String", createBinaryOperatorCdecl<RawString, const long long&, const RawString&>(operator+));

		fb.registPredefinedOperators("+", "String&,float&", "String", createBinaryOperatorCdecl < RawString, const RawString&, const float&>(operator+));
		fb.registPredefinedOperators("+", "float&,String&", "String", createBinaryOperatorCdecl<RawString, const float&, const RawString&>(operator+));

		fb.registPredefinedOperators("+", "String&,double&", "String", createBinaryOperatorCdecl < RawString, const RawString&, const double&>(operator+));
		fb.registPredefinedOperators("+", "double&,String&", "String", createBinaryOperatorCdecl<RawString, const double&, const RawString&>(operator+));

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		fb.registPredefinedOperators("+", "string&,bool", "String", createBinaryOperatorCdecl<RawString, const string&, bool>(addConstantWithVal));
		fb.registPredefinedOperators("+", "bool,string&", "String", createBinaryOperatorCdecl<RawString, bool, const string&>(addValWithConsant));

		fb.registPredefinedOperators("+", "string&,int", "String", createBinaryOperatorCdecl<RawString, const string&, int>(addConstantWithVal));
		fb.registPredefinedOperators("+", "int,string&", "String", createBinaryOperatorCdecl<RawString, int, const string&>(addValWithConsant));

		fb.registPredefinedOperators("+", "string&,long&", "String", createBinaryOperatorCdecl < RawString, const string&, const long long&>(addConstantWithVal));
		fb.registPredefinedOperators("+", "long&,string&", "String", createBinaryOperatorCdecl<RawString, const long long&, const string&>(addValWithConsant));

		fb.registPredefinedOperators("+", "string&,float&", "String", createBinaryOperatorCdecl < RawString, const string&, const float&>(addConstantWithVal));
		fb.registPredefinedOperators("+", "float&,string&", "String", createBinaryOperatorCdecl<RawString, const float&, const string&>(addValWithConsant));

		fb.registPredefinedOperators("+", "string&,double&", "String", createBinaryOperatorCdecl < RawString, const string&, const double&>(addConstantWithVal));
		fb.registPredefinedOperators("+", "double&,string&", "String", createBinaryOperatorCdecl<RawString, const double&, const string&>(addValWithConsant));

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		fb.registPredefinedOperators("+", "wstring&,bool", "String", createBinaryOperatorCdecl<RawString, const wstring&, bool>(addConstantWithVal));
		fb.registPredefinedOperators("+", "bool,wstring&", "String", createBinaryOperatorCdecl<RawString, bool, const wstring&>(addValWithConsant));

		fb.registPredefinedOperators("+", "wstring&,int", "String", createBinaryOperatorCdecl<RawString, const wstring&, int>(addConstantWithVal));
		fb.registPredefinedOperators("+", "int,wstring&", "String", createBinaryOperatorCdecl<RawString, int, const wstring&>(addValWithConsant));

		fb.registPredefinedOperators("+", "wstring&,long&", "String", createBinaryOperatorCdecl < RawString, const wstring&, const long long&>(addConstantWithVal));
		fb.registPredefinedOperators("+", "long&,wstring&", "String", createBinaryOperatorCdecl<RawString, const long long&, const wstring&>(addValWithConsant));

		fb.registPredefinedOperators("+", "wstring&,float&", "String", createBinaryOperatorCdecl < RawString, const wstring&, const float&>(addConstantWithVal));
		fb.registPredefinedOperators("+", "float&,wstring&", "String", createBinaryOperatorCdecl<RawString, const float&, const wstring&>(addValWithConsant));

		fb.registPredefinedOperators("+", "wstring&,double&", "String", createBinaryOperatorCdecl < RawString, const wstring&, const double&>(addConstantWithVal));
		fb.registPredefinedOperators("+", "double&,wstring&", "String", createBinaryOperatorCdecl<RawString, const double&, const wstring&>(addValWithConsant));
	}
}
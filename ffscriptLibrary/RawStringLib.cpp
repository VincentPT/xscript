/******************************************************************
* File:        RawStringLib.h
* Description: Implement some String functions and implement an
*              interface to import String type and its
*              related functions into the script compiler.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "RawStringLib.h"

#include "BasicFunctionFactory.hpp"
#include "DynamicFunctionFactory.h"
#include "BasicType.h"
#include "BasicFunction.h"
#include "Utils.h"
#include <algorithm>

namespace ffscript {
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
		swprintf(buffer, sizeof(buffer)/sizeof(buffer[0]), L"%d", val);

		RawString rawString = allocRawString((int)wcslen(buffer));
		memcpy(rawString.elms, buffer, (rawString.size + 1) * sizeof(RawChar));

		return rawString;
	}

	RawString ToString(long long val) {
		RawChar buffer[64];
		swprintf(buffer, sizeof(buffer)/sizeof(buffer[0]), L"%lld", val);

		RawString rawString = allocRawString((int)wcslen(buffer));
		memcpy(rawString.elms, buffer, (rawString.size + 1) * sizeof(RawChar));

		return rawString;
	}

	RawString ToString(float val) {
		RawChar buffer[64];
		swprintf(buffer, sizeof(buffer)/sizeof(buffer[0]), L"%f", val);

		RawString rawString = allocRawString((int)wcslen(buffer));
		memcpy(rawString.elms, buffer, (rawString.size + 1) * sizeof(RawChar));

		return rawString;
	}

	RawString ToString(double val) {
		RawChar buffer[64];
		swprintf(buffer, sizeof(buffer)/sizeof(buffer[0]), L"%f", val);

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

	RawString subString(const RawString& rws, int start, int count) {
		if(start >= rws.size || start < 0) {
			throw std::runtime_error("substring: start is out of range");
		}

		RawString rwNew;
		if (count < 0) {
			constantConstructor(rwNew, rws);
			return rwNew;
		}
		if(start + count > rws.size) {
			count = rws.size - start;
		}

		rwNew = allocRawString(count);
		memcpy(rwNew.elms, rws.elms + start, rwNew.size * sizeof(RawChar));
		rwNew.elms[rwNew.size] = 0;
		
		return rwNew;
	}

	int findString(const RawString& rws1, const RawString& rws2, int start) {
		if(start >= rws1.size) {
			return -1;
		}
		if(start < 0) {
			throw std::runtime_error("findString: start is out of range");
		}

		auto res = wcsstr(rws1.elms + start, rws2.elms);
		if(res == nullptr) return -1;
		
		return (int)(res - rws1.elms);
	}

	int stringLength(const RawString& s) {
		return s.size;
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
		int ctor = fb.registFunction("defaultConstructor", "ref String", createUserFunctionFactory<void, RawString&>(scriptCompiler, "void", defaultConstructor));
		int dtor = fb.registFunction("freeRawString", "ref String", createUserFunctionFactory<void, RawString&>(scriptCompiler, "void", freeRawString));

		scriptCompiler->registDestructor(iTypeString, dtor);
		scriptCompiler->registConstructor(iTypeString, ctor);

		ctor = fb.registFunction("constantConstructor", "ref String, string&", createUserFunctionFactory<void, RawString&, const std::string&>(scriptCompiler, "void", constantConstructor));
		scriptCompiler->registConstructor(iTypeString, ctor);

		ctor = fb.registFunction("constantConstructor", "ref String, wstring&", createUserFunctionFactory<void, RawString&, const std::wstring&>(scriptCompiler, "void", constantConstructor));
		scriptCompiler->registConstructor(iTypeString, ctor);

		ctor = fb.registFunction("constantConstructor", "ref String, String&", createUserFunctionFactory<void, RawString&, const RawString&>(scriptCompiler, "void", constantConstructor));
		scriptCompiler->registConstructor(iTypeString, ctor);

		// register conversion operators
		fb.registFunction("String", "bool", new ConvertToStringFactory(scriptCompiler, createStringNativeFunc<bool>(ToString), typeString));
		fb.registFunction("String", "int", new ConvertToStringFactory(scriptCompiler, createStringNativeFunc<int>(ToString), typeString));
		fb.registFunction("String", "long", new ConvertToStringFactory(scriptCompiler, createStringNativeFunc<long long>(ToString), typeString));
		fb.registFunction("String", "float", new ConvertToStringFactory(scriptCompiler, createStringNativeFunc<float>(ToString), typeString));
		fb.registFunction("String", "double", new ConvertToStringFactory(scriptCompiler, createStringNativeFunc<double>(ToString), typeString));

		// conversion accurative help engine choose the best overloading function for given arguments
		scriptCompiler->registerTypeConversionAccurative(basicTypes.TYPE_BOOL, basicTypes.TYPE_RAWSTRING, 10000);
		scriptCompiler->registerTypeConversionAccurative(basicTypes.TYPE_INT, basicTypes.TYPE_RAWSTRING, 10000);
		scriptCompiler->registerTypeConversionAccurative(basicTypes.TYPE_LONG, basicTypes.TYPE_RAWSTRING, 10000);
		scriptCompiler->registerTypeConversionAccurative(basicTypes.TYPE_FLOAT, basicTypes.TYPE_RAWSTRING, 10000);
		scriptCompiler->registerTypeConversionAccurative(basicTypes.TYPE_DOUBLE, basicTypes.TYPE_RAWSTRING, 10000);

		// register compare operators and functions
		fb.registPredefinedOperators("==", "String&,string&", "bool", createFunctionDelegate<bool, const RawString&, const std::string&>(operator==));
		fb.registPredefinedOperators("!=", "String&,string&", "bool", createFunctionDelegate<bool, const RawString&, const std::string&>(operator!=));
		fb.registPredefinedOperators("==", "string&,String&", "bool", createFunctionDelegate<bool, const std::string&, const RawString&>(operator==));
		fb.registPredefinedOperators("!=", "string&,String&", "bool", createFunctionDelegate<bool, const std::string&, const RawString&>(operator!=));

		fb.registPredefinedOperators("==", "String&,wstring&", "bool", createFunctionDelegate<bool, const RawString&, const std::wstring&>(operator==));
		fb.registPredefinedOperators("!=", "String&,wstring&", "bool", createFunctionDelegate<bool, const RawString&, const std::wstring&>(operator!=));
		fb.registPredefinedOperators("==", "wstring&,String&", "bool", createFunctionDelegate<bool, const std::wstring&, const RawString&>(operator==));
		fb.registPredefinedOperators("!=", "wstring&,String&", "bool", createFunctionDelegate<bool, const std::wstring&, const RawString&>(operator!=));

		fb.registPredefinedOperators("==", "String&,String&", "bool", createFunctionDelegate<bool, const RawString&, const RawString&>(operator==));
		fb.registPredefinedOperators("!=", "String&,String&", "bool", createFunctionDelegate<bool, const RawString&, const RawString&>(operator!=));

		fb.registFunction("compare", "String&,string&", createUserFunctionFactory<int, const RawString&, const std::string&>(scriptCompiler, "int", constantCompare));
		fb.registFunction("compare", "string&,String&", createUserFunctionFactory<int, const std::string&, const RawString&>(scriptCompiler, "int", constantCompare));
		fb.registFunction("compare", "String&,wstring&", createUserFunctionFactory<int, const RawString&, const std::wstring&>(scriptCompiler, "int", constantCompare));
		fb.registFunction("compare", "wstring&,String&", createUserFunctionFactory<int, const std::wstring&, const RawString&>(scriptCompiler, "int", constantCompare));
		fb.registFunction("compare", "String&,String&", createUserFunctionFactory<int, const RawString&, const RawString&>(scriptCompiler, "int", constantCompare));

		// register other operators
		fb.registPredefinedOperators("=", "String&,string&", "void", createFunctionDelegate<void, RawString&, const std::string&>(assignStringConst));
		fb.registPredefinedOperators("=", "String&,wstring&", "void", createFunctionDelegate<void, RawString&, const std::wstring&>(assignStringConst));
		fb.registPredefinedOperators("=", "String&,String&", "void", createFunctionDelegate<void, RawString&, const RawString&>(assignString));

		fb.registPredefinedOperators("+", "String&,string&", "String", createFunctionDelegate<RawString, const RawString&, const std::string&>(operator+));
		fb.registPredefinedOperators("+", "string&,String&", "String", createFunctionDelegate<RawString, const std::string&, const RawString&>(operator+));

		fb.registPredefinedOperators("+", "String&,wstring&", "String", createFunctionDelegate<RawString, const RawString&, const std::wstring&>(operator+));
		fb.registPredefinedOperators("+", "wstring&,String&", "String", createFunctionDelegate<RawString, const std::wstring&, const RawString&>(operator+));

		fb.registPredefinedOperators("+", "String&,String&", "String", createFunctionDelegate<RawString, const RawString&, const RawString&>(operator+));

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		fb.registPredefinedOperators("+", "String&,bool", "String", createFunctionDelegate<RawString, const RawString&, bool>(operator+));
		fb.registPredefinedOperators("+", "bool,String&", "String", createFunctionDelegate<RawString, bool, const RawString&>(operator+));

		fb.registPredefinedOperators("+", "String&,int", "String", createFunctionDelegate<RawString, const RawString&, int>(operator+));
		fb.registPredefinedOperators("+", "int,String&", "String", createFunctionDelegate<RawString, int, const RawString&>(operator+));

		fb.registPredefinedOperators("+", "String&,long", "String", createFunctionDelegate<RawString, const RawString&, long long> (operator+));
		fb.registPredefinedOperators("+", "long,String&", "String", createFunctionDelegate<RawString, long long, const RawString&>(operator+));

		fb.registPredefinedOperators("+", "String&,float", "String", createFunctionDelegate<RawString, const RawString&, float>(operator+));
		fb.registPredefinedOperators("+", "float,String&", "String", createFunctionDelegate<RawString, float, const RawString&>(operator+));

		fb.registPredefinedOperators("+", "String&,double", "String", createFunctionDelegate<RawString, const RawString&, double>(operator+));
		fb.registPredefinedOperators("+", "double,String&", "String", createFunctionDelegate<RawString, double, const RawString&>(operator+));

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		fb.registPredefinedOperators("+", "string&,bool", "String", createFunctionDelegate<RawString, const string&, bool>(addConstantWithVal));
		fb.registPredefinedOperators("+", "bool,string&", "String", createFunctionDelegate<RawString, bool, const string&>(addValWithConsant));

		fb.registPredefinedOperators("+", "string&,int", "String", createFunctionDelegate<RawString, const string&, int>(addConstantWithVal));
		fb.registPredefinedOperators("+", "int,string&", "String", createFunctionDelegate<RawString, int, const string&>(addValWithConsant));

		fb.registPredefinedOperators("+", "string&,long", "String", createFunctionDelegate<RawString, const string&, long long>(addConstantWithVal));
		fb.registPredefinedOperators("+", "long,string&", "String", createFunctionDelegate<RawString, long long, const string&>(addValWithConsant));

		fb.registPredefinedOperators("+", "string&,float", "String", createFunctionDelegate<RawString, const string&, float>(addConstantWithVal));
		fb.registPredefinedOperators("+", "float,string&", "String", createFunctionDelegate<RawString, float, const string&>(addValWithConsant));

		fb.registPredefinedOperators("+", "string&,double", "String", createFunctionDelegate<RawString, const string&, double>(addConstantWithVal));
		fb.registPredefinedOperators("+", "double,string&", "String", createFunctionDelegate<RawString, double, const string&>(addValWithConsant));

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		fb.registPredefinedOperators("+", "wstring&,bool", "String", createFunctionDelegate<RawString, const wstring&, bool>(addConstantWithVal));
		fb.registPredefinedOperators("+", "bool,wstring&", "String", createFunctionDelegate<RawString, bool, const wstring&>(addValWithConsant));

		fb.registPredefinedOperators("+", "wstring&,int", "String", createFunctionDelegate<RawString, const wstring&, int>(addConstantWithVal));
		fb.registPredefinedOperators("+", "int,wstring&", "String", createFunctionDelegate<RawString, int, const wstring&>(addValWithConsant));

		fb.registPredefinedOperators("+", "wstring&,long", "String", createFunctionDelegate<RawString, const wstring&, long long>(addConstantWithVal));
		fb.registPredefinedOperators("+", "long,wstring&", "String", createFunctionDelegate<RawString, long long, const wstring&>(addValWithConsant));

		fb.registPredefinedOperators("+", "wstring&,float", "String", createFunctionDelegate<RawString, const wstring&, float>(addConstantWithVal));
		fb.registPredefinedOperators("+", "float,wstring&", "String", createFunctionDelegate<RawString, float, const wstring&>(addValWithConsant));

		fb.registPredefinedOperators("+", "wstring&,double", "String", createFunctionDelegate<RawString, const wstring&, double>(addConstantWithVal));
		fb.registPredefinedOperators("+", "double,wstring&", "String", createFunctionDelegate<RawString, double, const wstring&>(addValWithConsant));

		fb.registFunction("length", "String&", createUserFunctionFactory<int, const RawString&>(scriptCompiler, "int", stringLength));
		fb.registFunction("subString", "String&,int,int", createUserFunctionFactory<RawString, const RawString&, int, int>(scriptCompiler, "String", subString));
		fb.registFunction("findString", "String&,String&,int", createUserFunctionFactory<int, const RawString&, const RawString&, int>(scriptCompiler, "int", findString));
	}
}
#include "stdafx.h"
#include "BasicFunction.h"
#include "expressionunit.h"
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

#include <locale>
#include <codecvt>

//#include "CppUnitTest.h"
//using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "BasicOperators.hpp"

using namespace ffscript::basic::operators;
using namespace ffscript::basic;

namespace ffscript {

	Function* FunctionFactoryCdecl::createFunction(const std::string& name, int id)
	{
		return _fCreator(name, id);
	}

	FunctionFactoryCdecl::FunctionFactoryCdecl(FCreator creator, const ScriptType& returnType) :
		_fCreator(creator)
	{
		this->setReturnType(returnType);
	}

	FunctionFactoryCdecl::~FunctionFactoryCdecl()
	{
	}

	UserFunctionFactory::UserFunctionFactory(ScriptCompiler* scriptCompiler, const char* returnType, int paramSize) : _paramSize(0) {
		this->setReturnType(ScriptType::parseType(scriptCompiler, returnType));
	}

	UserFunctionFactory::~UserFunctionFactory() {}

	Function* UserFunctionFactory::createFunction(const std::string& name, int id) {
		DynamicParamFunction* function = new DynamicParamFunction(name, EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, getReturnType());
		function->setNative(createNative());
		function->setMaxParam(_paramSize);
		return function;
	}

	//string functions
	template <class T>
	size_t stringLength(const T& s) {
		return s.length();
	}

	void* assignStruct(void* struct1, void* struct2, int size1, int size2) {
		memcpy_s(struct1, (size_t)size1, struct2, (size_t)size2);
		return struct1;
	}

	void* assignArray2Struct2(void* pStruct, SimpleVariantArray* pArray, int structSize, int arrayTypeId){
		char* data = (char*)pStruct;
		SimpleVariant* elm = &(pArray->elems[0]);
		SimpleVariant* elmEnd = elm + pArray->size;

		int variantType = DATA_TYPE_POINTER_MASK | arrayTypeId; // BasicTypes::getBasicTypes()->TYPE_VARIANTARRAY;
		while (elm < elmEnd)
		{
			if (variantType == elm->scriptType) {
				data = (char*)assignArray2Struct2(data, *((SimpleVariantArray**)elm->pData), structSize - (int)(data - (char*)pStruct), arrayTypeId);
			}
			else {
				memcpy_s(data, structSize - (data - (char*)pStruct), elm->pData, elm->size);
				data += elm->size;
			}
			elm++;
		}
		return data;
	}

	void* assignArray2Struct1(void* pStruct, SimpleVariantArray* pArray, int structSize, int arrayTypeId) {
		assignArray2Struct2(pStruct, pArray, structSize, arrayTypeId);
		return pStruct;
	}

	void* memberAccess(char* data, int offset) {
		return (void*)(data + offset);
	}

	SimpleVariantArray* dynamicFunctionGateway(SimpleVariantArray* pArray) {
		return pArray;
	}

	void defaultConstructor(RawString& s) {
		s.elms = nullptr;
		s.size = 0;
	}

	void constantConstructor(RawString& rawString, const std::wstring& ws) {
		if (ws.size() == 0) {
			defaultConstructor(rawString);
		}
		else {
			rawString = allocSimpleArray<RawChar>((int)ws.size());
			memcpy(rawString.elms, ws.c_str(), ws.size() * sizeof(RawChar));
		}
	}

	void constantConstructor(RawString& rawString, const std::string& s) {
		if (s.size() == 0) {
			defaultConstructor(rawString);
		}
		else {
			wstring ws = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.from_bytes(s);
			constantConstructor(rawString, ws);
		}
	}

	void constantConstructor(RawString& rawString, const RawString& s) {
		if (s.size == 0) {
			defaultConstructor(rawString);
		}
		else {
			rawString = allocSimpleArray<RawChar>(s.size);
			memcpy(rawString.elms, s.elms, s.size * sizeof(RawChar));
		}
	}

	void importBasicfunction(FunctionRegisterHelper& fb) {

#pragma region integer operators only
		fb.registPredefinedOperators("+", "int,int", "int", make_native(int, int, int, add));
		fb.registPredefinedOperators("-", "int,int", "int", make_native(int, int, int, sub));
		fb.registPredefinedOperators("*", "int,int", "int", make_native(int, int, int, mul));
		fb.registPredefinedOperators("/", "int,int", "int", make_native(int, int, int, div));
		fb.registPredefinedOperators("%", "int,int", "int", make_native(int, int, int, mod));
		//negative operator
		
		//assigment operator
		//fb.registPredefinedOperators("=", "int&,int", "int", make_native(int, int&, int, assign));
		//bitwises
		fb.registPredefinedOperators("&", "int,int", "int", make_native(int, int, int, and));
		fb.registPredefinedOperators("|", "int,int", "int", make_native(int, int, int, or));
		fb.registPredefinedOperators("^", "int,int", "int", make_native(int, int, int, xor));
		fb.registPredefinedOperators("<<", "int,int", "int", make_native(int, int, int, shiftLeft));
		fb.registPredefinedOperators(">>", "int,int", "int", make_native(int, int, int, shiftRight));
		//pre-post fix operators
		fb.registPredefinedOperators("neg", "int", "int", new CdeclFunction2<int, int>(neg));
		fb.registPredefinedOperators("~", "int", "int", new CdeclFunction2<int, int>(not));
		fb.registPredefinedOperators("post_fix_increase", "int&", "int", new CdeclFunction2<int, int&>(post_inc));
		fb.registPredefinedOperators("++", "int&", "int", new CdeclFunction2<int, int&>(pre_inc));
		fb.registPredefinedOperators("post_fix_decrease", "int&", "int", new CdeclFunction2<int, int&>(post_dec));
		fb.registPredefinedOperators("--", "int&", "int", new CdeclFunction2<int, int&>(pre_dec));
		//compound operators
		fb.registPredefinedOperators("+=", "int&,int", "void", new CdeclFunction2<void, int&, int>(add_comp));
		fb.registPredefinedOperators("-=", "int&,int", "void", new CdeclFunction2<void, int&, int>(sub_comp));
		fb.registPredefinedOperators("*=", "int&,int", "void", new CdeclFunction2<void, int&, int>(mul_comp));
		fb.registPredefinedOperators("/=", "int&,int", "void", new CdeclFunction2<void, int&, int>(div_comp));
		fb.registPredefinedOperators("%=", "int&,int", "void", new CdeclFunction2<void, int&, int>(mod_comp));
		fb.registPredefinedOperators("&=", "int&,int", "void", new CdeclFunction2<void, int&, int>(and_comp));
		fb.registPredefinedOperators("|=", "int&,int", "void", new CdeclFunction2<void, int&, int>(or_comp));
		fb.registPredefinedOperators("^=", "int&,int", "void", new CdeclFunction2<void, int&, int>(xor_comp));
		fb.registPredefinedOperators("<<=", "int&,int", "void", new CdeclFunction2<void, int&, int>(shiftLeft_comp));
		fb.registPredefinedOperators(">>=", "int&,int", "void", new CdeclFunction2<void, int&, int>(shiftRight_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "int,int", "bool", new CdeclFunction2<bool, int, int>(operators::less));
		fb.registPredefinedOperators("<=", "int,int", "bool", new CdeclFunction2<bool, int, int>(operators::less_or_equal));
		fb.registPredefinedOperators(">", "int,int", "bool", new CdeclFunction2<bool, int, int>(operators::great));
		fb.registPredefinedOperators(">=", "int,int", "bool", new CdeclFunction2<bool, int, int>(operators::great_or_equal));
		fb.registPredefinedOperators("==", "int,int", "bool", new CdeclFunction2<bool, int, int>(operators::equal));
		fb.registPredefinedOperators("!=", "int,int", "bool", new CdeclFunction2<bool, int, int>(operators::not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "int,int", "bool", new CdeclFunction2<bool, int, int>(logic_and));
		fb.registPredefinedOperators("||", "int,int", "bool", new CdeclFunction2<bool, int, int>(logic_or));
		fb.registPredefinedOperators("!", "int", "bool", new CdeclFunction2<bool, int>(logic_not));
#pragma endregion

#pragma region long operators only
		fb.registPredefinedOperators("+", S_LONG_CP "," S_LONG_CP, S_LONG_T, make_native(LONG_T, LONG_CP, LONG_CP, add));
		fb.registPredefinedOperators("-", S_LONG_CP "," S_LONG_CP, S_LONG_T, make_native(LONG_T, LONG_CP, LONG_CP, sub));
		fb.registPredefinedOperators("*", S_LONG_CP "," S_LONG_CP, S_LONG_T, make_native(LONG_T, LONG_CP, LONG_CP, mul));
		fb.registPredefinedOperators("/", S_LONG_CP "," S_LONG_CP, S_LONG_T, make_native(LONG_T, LONG_CP, LONG_CP, div));
		fb.registPredefinedOperators("%", S_LONG_CP "," S_LONG_CP, S_LONG_T, make_native(LONG_T, LONG_CP, LONG_CP, mod));
		//negative operator

		//assigment operator
		//fb.registPredefinedOperators("=", S_LONG_T "&" "," S_LONG_CP, S_LONG_T, make_native(LONG_T, LONG_T&, LONG_CP, assign));
		//bitwises
		fb.registPredefinedOperators("&", S_LONG_CP "," S_LONG_CP, S_LONG_T, make_native(LONG_T, LONG_CP, LONG_CP, and));
		fb.registPredefinedOperators("|", S_LONG_CP "," S_LONG_CP, S_LONG_T, make_native(LONG_T, LONG_CP, LONG_CP, or ));
		fb.registPredefinedOperators("^", S_LONG_CP "," S_LONG_CP, S_LONG_T, make_native(LONG_T, LONG_CP, LONG_CP, xor));
		fb.registPredefinedOperators("<<", S_LONG_CP "," S_LONG_CP, S_LONG_T, make_native(LONG_T, LONG_CP, LONG_CP, shiftLeft));
		fb.registPredefinedOperators(">>", S_LONG_CP "," S_LONG_CP, S_LONG_T, make_native(LONG_T, LONG_CP, LONG_CP, shiftRight));
		//pre-post fix operators
		fb.registPredefinedOperators("neg",					S_LONG_CP, S_LONG_T, new CdeclFunction2<LONG_T, LONG_CP>(neg));
		fb.registPredefinedOperators("~",					S_LONG_CP, S_LONG_T, new CdeclFunction2<LONG_T, LONG_CP>(not));
		fb.registPredefinedOperators("post_fix_increase", S_LONG_T "&", S_LONG_T, new CdeclFunction2<LONG_T, LONG_T&>(post_inc));
		fb.registPredefinedOperators("++", S_LONG_T "&", S_LONG_T, new CdeclFunction2<LONG_T, LONG_T&>(pre_inc));
		fb.registPredefinedOperators("post_fix_decrease", S_LONG_T "&", S_LONG_T, new CdeclFunction2<LONG_T, LONG_T&>(post_dec));
		fb.registPredefinedOperators("--", S_LONG_T "&", S_LONG_T, new CdeclFunction2<LONG_T, LONG_T&>(pre_dec));
		//compound operators
		fb.registPredefinedOperators("+=", S_LONG_T "&" "," S_LONG_CP, "void", new CdeclFunction2<void, LONG_T&, LONG_CP>(add_comp));
		fb.registPredefinedOperators("-=", S_LONG_T "&" "," S_LONG_CP, "void", new CdeclFunction2<void, LONG_T&, LONG_CP>(sub_comp));
		fb.registPredefinedOperators("*=", S_LONG_T "&" "," S_LONG_CP, "void", new CdeclFunction2<void, LONG_T&, LONG_CP>(mul_comp));
		fb.registPredefinedOperators("/=", S_LONG_T "&" "," S_LONG_CP, "void", new CdeclFunction2<void, LONG_T&, LONG_CP>(div_comp));
		fb.registPredefinedOperators("%=", S_LONG_T "&" "," S_LONG_CP, "void", new CdeclFunction2<void, LONG_T&, LONG_CP>(mod_comp));
		fb.registPredefinedOperators("&=", S_LONG_T "&" "," S_LONG_CP, "void", new CdeclFunction2<void, LONG_T&, LONG_CP>(and_comp));
		fb.registPredefinedOperators("|=", S_LONG_T "&" "," S_LONG_CP, "void", new CdeclFunction2<void, LONG_T&, LONG_CP>(or_comp));
		fb.registPredefinedOperators("^=", S_LONG_T "&" "," S_LONG_CP, "void", new CdeclFunction2<void, LONG_T&, LONG_CP>(xor_comp));
		fb.registPredefinedOperators("<<=", S_LONG_T "&" "," S_LONG_CP, "void", new CdeclFunction2<void, LONG_T&, LONG_CP>(shiftLeft_comp));
		fb.registPredefinedOperators(">>=", S_LONG_T "&" "," S_LONG_CP, "void", new CdeclFunction2<void, LONG_T&, LONG_CP>(shiftRight_comp));
		//comparision operators
		fb.registPredefinedOperators("<", S_LONG_CP "," S_LONG_CP, "bool", new CdeclFunction2<bool, LONG_CP, LONG_CP>(operators::less));
		fb.registPredefinedOperators("<=", S_LONG_CP "," S_LONG_CP, "bool", new CdeclFunction2<bool, LONG_CP, LONG_CP>(less_or_equal));
		fb.registPredefinedOperators(">", S_LONG_CP "," S_LONG_CP, "bool", new CdeclFunction2<bool, LONG_CP, LONG_CP>(great));
		fb.registPredefinedOperators(">=", S_LONG_CP "," S_LONG_CP, "bool", new CdeclFunction2<bool, LONG_CP, LONG_CP>(great_or_equal));
		fb.registPredefinedOperators("==", S_LONG_CP "," S_LONG_CP, "bool", new CdeclFunction2<bool, LONG_CP, LONG_CP>(equal));
		fb.registPredefinedOperators("!=", S_LONG_CP "," S_LONG_CP, "bool", new CdeclFunction2<bool, LONG_CP, LONG_CP>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", S_LONG_CP "," S_LONG_CP, "bool", new CdeclFunction2<bool, LONG_CP, LONG_CP>(logic_and));
		fb.registPredefinedOperators("||", S_LONG_CP "," S_LONG_CP, "bool", new CdeclFunction2<bool, LONG_CP, LONG_CP>(logic_or));
		fb.registPredefinedOperators("!", S_LONG_CP, "bool", new CdeclFunction2<bool, LONG_CP>(logic_not));
#pragma endregion

#pragma region float operators only
		fb.registPredefinedOperators("+", "float&,float&", "float", make_native(float, const float&, const float&, add));
		fb.registPredefinedOperators("-", "float&,float&", "float", make_native(float, const float&, const float&, sub));
		fb.registPredefinedOperators("*", "float&,float&", "float", make_native(float, const float&, const float&, mul));
		fb.registPredefinedOperators("/", "float&,float&", "float", make_native(float, const float&, const float&, div));

		//assigment operator
		//fb.registPredefinedOperators("=", "float&,float&", "float", make_native(float, float&, const float&, assign));
		//pre-post fix operators
		fb.registPredefinedOperators("neg", "float&", "float", new CdeclFunction2<float, const float&>(neg));
		fb.registPredefinedOperators("post_fix_increase", "float&", "float", new CdeclFunction2<float, float&>(post_inc));
		fb.registPredefinedOperators("++", "float&", "float", new CdeclFunction2<float, float&>(pre_inc));
		fb.registPredefinedOperators("post_fix_decrease", "float&", "float", new CdeclFunction2<float, float&>(post_dec));
		fb.registPredefinedOperators("--", "float&", "float", new CdeclFunction2<float, float&>(pre_dec));
		//compound operators
		fb.registPredefinedOperators("+=", "float&,float&", "void", new CdeclFunction2<void, float&, const float&>(add_comp));
		fb.registPredefinedOperators("-=", "float&,float&", "void", new CdeclFunction2<void, float&, const float&>(sub_comp));
		fb.registPredefinedOperators("*=", "float&,float&", "void", new CdeclFunction2<void, float&, const float&>(mul_comp));
		fb.registPredefinedOperators("/=", "float&,float&", "void", new CdeclFunction2<void, float&, const float&>(div_comp));

		//comparision operators
		fb.registPredefinedOperators("<", "float&,float&", "bool", new CdeclFunction2<bool, const float&, const float&>(operators::less));
		fb.registPredefinedOperators("<=", "float&,float&", "bool", new CdeclFunction2<bool, const float&, const float&>(less_or_equal));
		fb.registPredefinedOperators(">", "float&,float&", "bool", new CdeclFunction2<bool, const float&, const float&>(great));
		fb.registPredefinedOperators(">=", "float&,float&", "bool", new CdeclFunction2<bool, const float&, const float&>(great_or_equal));
		fb.registPredefinedOperators("==", "float&,float&", "bool", new CdeclFunction2<bool, const float&, const float&>(equal));
		fb.registPredefinedOperators("!=", "float&,float&", "bool", new CdeclFunction2<bool, const float&, const float&>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "float&,float&", "bool", new CdeclFunction2<bool, const float&, const float&>(logic_and));
		fb.registPredefinedOperators("||", "float&,float&", "bool", new CdeclFunction2<bool, const float&, const float&>(logic_or));
		fb.registPredefinedOperators("!", "float&", "bool", new CdeclFunction2<bool, const float&>(logic_not));
#pragma endregion

#pragma region double operators only
		fb.registPredefinedOperators("+", "double&,double&", "double", make_native(double, const double&, const double&, add));
		fb.registPredefinedOperators("-", "double&,double&", "double", make_native(double, const double&, const double&, sub));
		fb.registPredefinedOperators("*", "double&,double&", "double", make_native(double, const double&, const double&, mul));
		fb.registPredefinedOperators("/", "double&,double&", "double", make_native(double, const double&, const double&, div));

		//assigment operator
		//fb.registPredefinedOperators("=", "double&,double&", "double", make_native(double, double&, const double&, assign));
		//pre-post fix operators
		fb.registPredefinedOperators("neg", "double&", "double", new CdeclFunction2<double, const double&>(neg));
		fb.registPredefinedOperators("post_fix_increase", "double&", "double", new CdeclFunction2<double, double&>(post_inc));
		fb.registPredefinedOperators("++", "double&", "double", new CdeclFunction2<double, double&>(pre_inc));
		fb.registPredefinedOperators("post_fix_decrease", "double&", "double", new CdeclFunction2<double, double&>(post_dec));
		fb.registPredefinedOperators("--", "double&", "double", new CdeclFunction2<double, double&>(pre_dec));
		//compound operators
		fb.registPredefinedOperators("+=", "double&,double&", "void", new CdeclFunction2<void, double&, const double&>(add_comp));
		fb.registPredefinedOperators("-=", "double&,double&", "void", new CdeclFunction2<void, double&, const double&>(sub_comp));
		fb.registPredefinedOperators("*=", "double&,double&", "void", new CdeclFunction2<void, double&, const double&>(mul_comp));
		fb.registPredefinedOperators("/=", "double&,double&", "void", new CdeclFunction2<void, double&, const double&>(div_comp));

		//comparision operators
		fb.registPredefinedOperators("<", "double&,double&", "bool", new CdeclFunction2<bool, const double&, const double&>(operators::less));
		fb.registPredefinedOperators("<=", "double&,double&", "bool", new CdeclFunction2<bool, const double&, const double&>(less_or_equal));
		fb.registPredefinedOperators(">", "double&,double&", "bool", new CdeclFunction2<bool, const double&, const double&>(great));
		fb.registPredefinedOperators(">=", "double&,double&", "bool", new CdeclFunction2<bool, const double&, const double&>(great_or_equal));
		fb.registPredefinedOperators("==", "double&,double&", "bool", new CdeclFunction2<bool, const double&, const double&>(equal));
		fb.registPredefinedOperators("!=", "double&,double&", "bool", new CdeclFunction2<bool, const double&, const double&>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "double&,double&", "bool", new CdeclFunction2<bool, const double&, const double&>(logic_and));
		fb.registPredefinedOperators("||", "double&,double&", "bool", new CdeclFunction2<bool, const double&, const double&>(logic_or));
		fb.registPredefinedOperators("!", "double&", "bool", new CdeclFunction2<bool, const double&>(logic_not));
#pragma endregion

#pragma region bool operators only
		//assigment operator
		//fb.registPredefinedOperators("=", "bool&,bool", "bool", make_native(bool, bool&, bool, assign));
		//comparision operators
		fb.registPredefinedOperators("==", "bool,bool", "bool", new CdeclFunction2<bool, bool, bool>(operators::equal));
		fb.registPredefinedOperators("!=", "bool,bool", "bool", new CdeclFunction2<bool, bool, bool>(operators::not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "bool,bool", "bool", new CdeclFunction2<bool, bool, bool>(logic_and));
		fb.registPredefinedOperators("||", "bool,bool", "bool", new CdeclFunction2<bool, bool, bool>(logic_or));
		fb.registPredefinedOperators("!", "bool", "bool", new CdeclFunction2<bool, bool>(logic_not));
#pragma endregion

#pragma region integer and long operators
		fb.registPredefinedOperators("+", "int," S_LONG_CP, S_LONG_T, make_native(LONG_T, int, LONG_CP, add));
		fb.registPredefinedOperators("-", "int," S_LONG_CP, S_LONG_T, make_native(LONG_T, int, LONG_CP, sub));
		fb.registPredefinedOperators("*", "int," S_LONG_CP, S_LONG_T, make_native(LONG_T, int, LONG_CP, mul));
		fb.registPredefinedOperators("/", "int," S_LONG_CP, S_LONG_T, make_native(LONG_T, int, LONG_CP, div));
		fb.registPredefinedOperators("%", "int," S_LONG_CP, S_LONG_T, make_native(LONG_T, int, LONG_CP, mod));
		//assigment operator
		fb.registPredefinedOperators("=", "int&," S_LONG_CP, "int", make_native(int, int&, LONG_CP, assign));
		//bitwises
		fb.registPredefinedOperators("&", "int," S_LONG_CP, S_LONG_T, make_native(LONG_T, int, LONG_CP, and));
		fb.registPredefinedOperators("|", "int," S_LONG_CP, S_LONG_T, make_native(LONG_T, int, LONG_CP, or ));
		fb.registPredefinedOperators("^", "int," S_LONG_CP, S_LONG_T, make_native(LONG_T, int, LONG_CP, xor));
		fb.registPredefinedOperators("<<", "int," S_LONG_CP, "int", make_native(int, int, LONG_CP, shiftLeft));
		fb.registPredefinedOperators(">>", "int," S_LONG_CP, "int", make_native(int, int, LONG_CP, shiftRight));
		//compound operators
		fb.registPredefinedOperators("+=", "int&," S_LONG_CP, "void", new CdeclFunction2<void, int&, LONG_CP>(add_comp));
		fb.registPredefinedOperators("-=", "int&," S_LONG_CP, "void", new CdeclFunction2<void, int&, LONG_CP>(sub_comp));
		fb.registPredefinedOperators("*=", "int&," S_LONG_CP, "void", new CdeclFunction2<void, int&, LONG_CP>(mul_comp));
		fb.registPredefinedOperators("/=", "int&," S_LONG_CP, "void", new CdeclFunction2<void, int&, LONG_CP>(div_comp));
		fb.registPredefinedOperators("%=", "int&," S_LONG_CP, "void", new CdeclFunction2<void, int&, LONG_CP>(mod_comp));
		fb.registPredefinedOperators("&=", "int&," S_LONG_CP, "void", new CdeclFunction2<void, int&, LONG_CP>(and_comp));
		fb.registPredefinedOperators("|=", "int&," S_LONG_CP, "void", new CdeclFunction2<void, int&, LONG_CP>(or_comp));
		fb.registPredefinedOperators("^=", "int&," S_LONG_CP, "void", new CdeclFunction2<void, int&, LONG_CP>(xor_comp));
		fb.registPredefinedOperators("<<=", "int&," S_LONG_CP, "void", new CdeclFunction2<void, int&, LONG_CP>(shiftLeft_comp));
		fb.registPredefinedOperators(">>=", "int&," S_LONG_CP, "void", new CdeclFunction2<void, int&, LONG_CP>(shiftRight_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "int," S_LONG_CP, "bool", new CdeclFunction2<bool, int, LONG_CP>(operators::less));
		fb.registPredefinedOperators("<=", "int," S_LONG_CP, "bool", new CdeclFunction2<bool, int, LONG_CP>(less_or_equal));
		fb.registPredefinedOperators(">", "int," S_LONG_CP, "bool", new CdeclFunction2<bool, int, LONG_CP>(great));
		fb.registPredefinedOperators(">=", "int," S_LONG_CP, "bool", new CdeclFunction2<bool, int, LONG_CP>(great_or_equal));
		fb.registPredefinedOperators("==", "int," S_LONG_CP, "bool", new CdeclFunction2<bool, int, LONG_CP>(equal));
		fb.registPredefinedOperators("!=", "int," S_LONG_CP, "bool", new CdeclFunction2<bool, int, LONG_CP>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "int," S_LONG_CP, "bool", new CdeclFunction2<bool, int, LONG_CP>(logic_and));
		fb.registPredefinedOperators("||", "int," S_LONG_CP, "bool", new CdeclFunction2<bool, int, LONG_CP>(logic_or));
#pragma endregion

#pragma region integer and float operators
		fb.registPredefinedOperators("+", "int,float&", "float", make_native(float, int, const float&, add));
		fb.registPredefinedOperators("-", "int,float&", "float", make_native(float, int, const float&, sub));
		fb.registPredefinedOperators("*", "int,float&", "float", make_native(float, int, const float&, mul));
		fb.registPredefinedOperators("/", "int,float&", "float", make_native(float, int, const float&, div));
		//assigment operator
		fb.registPredefinedOperators("=", "int&,float&", "int", make_native(int, int&, const float&, assign));
		//compound operators
		fb.registPredefinedOperators("+=", "int&,float&", "void", new CdeclFunction2<void, int&, const float&>(add_comp));
		fb.registPredefinedOperators("-=", "int&,float&", "void", new CdeclFunction2<void, int&, const float&>(sub_comp));
		fb.registPredefinedOperators("*=", "int&,float&", "void", new CdeclFunction2<void, int&, const float&>(mul_comp));
		fb.registPredefinedOperators("/=", "int&,float&", "void", new CdeclFunction2<void, int&, const float&>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "int,float&", "bool", new CdeclFunction2<bool, int, const float&>(operators::less));
		fb.registPredefinedOperators("<=", "int,float&", "bool", new CdeclFunction2<bool, int, const float&>(operators::less_or_equal));
		fb.registPredefinedOperators(">", "int,float&", "bool", new CdeclFunction2<bool, int, const float&>(operators::great));
		fb.registPredefinedOperators(">=", "int,float&", "bool", new CdeclFunction2<bool, int, const float&>(operators::great_or_equal));
		fb.registPredefinedOperators("==", "int,float&", "bool", new CdeclFunction2<bool, int, const float&>(operators::equal));
		fb.registPredefinedOperators("!=", "int,float&", "bool", new CdeclFunction2<bool, int, const float&>(operators::not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "int,float&", "bool", new CdeclFunction2<bool, int, const float&>(logic_and));
		fb.registPredefinedOperators("||", "int,float&", "bool", new CdeclFunction2<bool, int, const float&>(logic_or));
#pragma endregion

#pragma region integer and double operators
		fb.registPredefinedOperators("+", "int,double&", "double", make_native(double, int, const double&, add));
		fb.registPredefinedOperators("-", "int,double&", "double", make_native(double, int, const double&, sub));
		fb.registPredefinedOperators("*", "int,double&", "double", make_native(double, int, const double&, mul));
		fb.registPredefinedOperators("/", "int,double&", "double", make_native(double, int, const double&, div));
		//assigment operator
		fb.registPredefinedOperators("=", "int&,double&", "int", make_native(int, int&, const double&, assign));
		//compound operators
		fb.registPredefinedOperators("+=", "int&,double&", "void", new CdeclFunction2<void, int&, const double&>(add_comp));
		fb.registPredefinedOperators("-=", "int&,double&", "void", new CdeclFunction2<void, int&, const double&>(sub_comp));
		fb.registPredefinedOperators("*=", "int&,double&", "void", new CdeclFunction2<void, int&, const double&>(mul_comp));
		fb.registPredefinedOperators("/=", "int&,double&", "void", new CdeclFunction2<void, int&, const double&>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "int,double&", "bool", new CdeclFunction2<bool, int, const double&>(operators::less));
		fb.registPredefinedOperators("<=", "int,double&", "bool", new CdeclFunction2<bool, int, const double&>(operators::less_or_equal));
		fb.registPredefinedOperators(">", "int,double&", "bool", new CdeclFunction2<bool, int, const double&>(operators::great));
		fb.registPredefinedOperators(">=", "int,double&", "bool", new CdeclFunction2<bool, int, const double&>(operators::great_or_equal));
		fb.registPredefinedOperators("==", "int,double&", "bool", new CdeclFunction2<bool, int, const double&>(operators::equal));
		fb.registPredefinedOperators("!=", "int,double&", "bool", new CdeclFunction2<bool, int, const double&>(operators::not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "int,double&", "bool", new CdeclFunction2<bool, int, const double&>(logic_and));
		fb.registPredefinedOperators("||", "int,double&", "bool", new CdeclFunction2<bool, int, const double&>(logic_or));
#pragma endregion

#pragma region integer and bool operators
		//logic operators
		fb.registPredefinedOperators("&&", "int,bool", "bool", new CdeclFunction2<bool, int, bool>(logic_and));
		fb.registPredefinedOperators("||", "int,bool", "bool", new CdeclFunction2<bool, int, bool>(logic_or));
#pragma endregion

#pragma region long and integer operators
		fb.registPredefinedOperators("+", S_LONG_CP ",int", S_LONG_T, make_native(LONG_T, LONG_CP, int, add));
		fb.registPredefinedOperators("-", S_LONG_CP ",int", S_LONG_T, make_native(LONG_T, LONG_CP, int, sub));
		fb.registPredefinedOperators("*", S_LONG_CP ",int", S_LONG_T, make_native(LONG_T, LONG_CP, int, mul));
		fb.registPredefinedOperators("/", S_LONG_CP ",int", S_LONG_T, make_native(LONG_T, LONG_CP, int, div));
		fb.registPredefinedOperators("%", S_LONG_CP ",int", S_LONG_T, make_native(LONG_T, LONG_CP, int, mod));
		//assigment operator
		fb.registPredefinedOperators("=", S_LONG_T "&" ",int", S_LONG_T, make_native(LONG_T, LONG_T&, int, assign));
		//bitwises
		fb.registPredefinedOperators("&", S_LONG_CP ",int", S_LONG_T, make_native(LONG_T, LONG_CP, int, and));
		fb.registPredefinedOperators("|", S_LONG_CP ",int", S_LONG_T, make_native(LONG_T, LONG_CP, int, or ));
		fb.registPredefinedOperators("^", S_LONG_CP ",int", S_LONG_T, make_native(LONG_T, LONG_CP, int, xor));
		fb.registPredefinedOperators("<<", S_LONG_CP ",int", S_LONG_T, make_native(LONG_T, LONG_CP, int, shiftLeft));
		fb.registPredefinedOperators(">>", S_LONG_CP ",int", S_LONG_T, make_native(LONG_T, LONG_CP, int, shiftRight));
		//compound operators
		fb.registPredefinedOperators("+=", S_LONG_T "&" ",int", "void", new CdeclFunction2<void, LONG_T&, int>(add_comp));
		fb.registPredefinedOperators("-=", S_LONG_T "&" ",int", "void", new CdeclFunction2<void, LONG_T&, int>(sub_comp));
		fb.registPredefinedOperators("*=", S_LONG_T "&" ",int", "void", new CdeclFunction2<void, LONG_T&, int>(mul_comp));
		fb.registPredefinedOperators("/=", S_LONG_T "&" ",int", "void", new CdeclFunction2<void, LONG_T&, int>(div_comp));
		fb.registPredefinedOperators("%=", S_LONG_T "&" ",int", "void", new CdeclFunction2<void, LONG_T&, int>(mod_comp));
		fb.registPredefinedOperators("&=", S_LONG_T "&" ",int", "void", new CdeclFunction2<void, LONG_T&, int>(and_comp));
		fb.registPredefinedOperators("|=", S_LONG_T "&" ",int", "void", new CdeclFunction2<void, LONG_T&, int>(or_comp));
		fb.registPredefinedOperators("^=", S_LONG_T "&" ",int", "void", new CdeclFunction2<void, LONG_T&, int>(xor_comp));
		fb.registPredefinedOperators("<<=", S_LONG_T "&" ",int", "void", new CdeclFunction2<void, LONG_T&, int>(shiftLeft_comp));
		fb.registPredefinedOperators(">>=", S_LONG_T "&" ",int", "void", new CdeclFunction2<void, LONG_T&, int>(shiftRight_comp));
		//comparision operators
		fb.registPredefinedOperators("<", S_LONG_CP ",int", "bool", new CdeclFunction2<bool, LONG_CP, int>(operators::less));
		fb.registPredefinedOperators("<=", S_LONG_CP ",int", "bool", new CdeclFunction2<bool, LONG_CP, int>(less_or_equal));
		fb.registPredefinedOperators(">", S_LONG_CP ",int", "bool", new CdeclFunction2<bool, LONG_CP, int>(great));
		fb.registPredefinedOperators(">=", S_LONG_CP ",int", "bool", new CdeclFunction2<bool, LONG_CP, int>(great_or_equal));
		fb.registPredefinedOperators("==", S_LONG_CP ",int", "bool", new CdeclFunction2<bool, LONG_CP, int>(equal));
		fb.registPredefinedOperators("!=", S_LONG_CP ",int", "bool", new CdeclFunction2<bool, LONG_CP, int>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", S_LONG_CP ",int", "bool", new CdeclFunction2<bool, LONG_CP, int>(logic_and));
		fb.registPredefinedOperators("||", S_LONG_CP ",int", "bool", new CdeclFunction2<bool, LONG_CP, int>(logic_or));
#pragma endregion

#pragma region long and float operators
		fb.registPredefinedOperators("+", S_LONG_CP ",float&", "float", make_native(float, LONG_CP, const float&, add));
		fb.registPredefinedOperators("-", S_LONG_CP ",float&", "float", make_native(float, LONG_CP, const float&, sub));
		fb.registPredefinedOperators("*", S_LONG_CP ",float&", "float", make_native(float, LONG_CP, const float&, mul));
		fb.registPredefinedOperators("/", S_LONG_CP ",float&", "float", make_native(float, LONG_CP, const float&, div));
		//assigment operator
		fb.registPredefinedOperators("=", S_LONG_T "&" ",float&", "float", make_native(LONG_T, LONG_T&, const float&, assign));
		//compound operators
		fb.registPredefinedOperators("+=", S_LONG_T "&" ",float&", "void", new CdeclFunction2<void, LONG_T&, const float&>(add_comp));
		fb.registPredefinedOperators("-=", S_LONG_T "&" ",float&", "void", new CdeclFunction2<void, LONG_T&, const float&>(sub_comp));
		fb.registPredefinedOperators("*=", S_LONG_T "&" ",float&", "void", new CdeclFunction2<void, LONG_T&, const float&>(mul_comp));
		fb.registPredefinedOperators("/=", S_LONG_T "&" ",float&", "void", new CdeclFunction2<void, LONG_T&, const float&>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", S_LONG_CP ",float&", "bool", new CdeclFunction2<bool, LONG_CP, const float&>(operators::less));
		fb.registPredefinedOperators("<=", S_LONG_CP ",float&", "bool", new CdeclFunction2<bool, LONG_CP, const float&>(less_or_equal));
		fb.registPredefinedOperators(">", S_LONG_CP ",float&", "bool", new CdeclFunction2<bool, LONG_CP, const float&>(great));
		fb.registPredefinedOperators(">=", S_LONG_CP ",float&", "bool", new CdeclFunction2<bool, LONG_CP, const float&>(great_or_equal));
		fb.registPredefinedOperators("==", S_LONG_CP ",float&", "bool", new CdeclFunction2<bool, LONG_CP, const float&>(equal));
		fb.registPredefinedOperators("!=", S_LONG_CP ",float&", "bool", new CdeclFunction2<bool, LONG_CP, const float&>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", S_LONG_CP ",float&", "bool", new CdeclFunction2<bool, LONG_CP, const float&>(logic_and));
		fb.registPredefinedOperators("||", S_LONG_CP ",float&", "bool", new CdeclFunction2<bool, LONG_CP, const float&>(logic_or));
#pragma endregion

#pragma region long and double operators
		fb.registPredefinedOperators("+", S_LONG_CP ",double&", "double", make_native(double, LONG_CP, const double&, add));
		fb.registPredefinedOperators("-", S_LONG_CP ",double&", "double", make_native(double, LONG_CP, const double&, sub));
		fb.registPredefinedOperators("*", S_LONG_CP ",double&", "double", make_native(double, LONG_CP, const double&, mul));
		fb.registPredefinedOperators("/", S_LONG_CP ",double&", "double", make_native(double, LONG_CP, const double&, div));
		//assigment operator
		fb.registPredefinedOperators("=", S_LONG_T "&" ",double&", "double", make_native(LONG_T, LONG_T&, const double&, assign));
		//compound operators
		fb.registPredefinedOperators("+=", S_LONG_T "&" ",double&", "void", new CdeclFunction2<void, LONG_T&, const double&>(add_comp));
		fb.registPredefinedOperators("-=", S_LONG_T "&" ",double&", "void", new CdeclFunction2<void, LONG_T&, const double&>(sub_comp));
		fb.registPredefinedOperators("*=", S_LONG_T "&" ",double&", "void", new CdeclFunction2<void, LONG_T&, const double&>(mul_comp));
		fb.registPredefinedOperators("/=", S_LONG_T "&" ",double&", "void", new CdeclFunction2<void, LONG_T&, const double&>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", S_LONG_CP ",double&", "bool", new CdeclFunction2<bool, LONG_CP, const double&>(operators::less));
		fb.registPredefinedOperators("<=", S_LONG_CP ",double&", "bool", new CdeclFunction2<bool, LONG_CP, const double&>(less_or_equal));
		fb.registPredefinedOperators(">", S_LONG_CP ",double&", "bool", new CdeclFunction2<bool, LONG_CP, const double&>(great));
		fb.registPredefinedOperators(">=", S_LONG_CP ",double&", "bool", new CdeclFunction2<bool, LONG_CP, const double&>(great_or_equal));
		fb.registPredefinedOperators("==", S_LONG_CP ",double&", "bool", new CdeclFunction2<bool, LONG_CP, const double&>(equal));
		fb.registPredefinedOperators("!=", S_LONG_CP ",double&", "bool", new CdeclFunction2<bool, LONG_CP, const double&>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", S_LONG_CP ",double&", "bool", new CdeclFunction2<bool, LONG_CP, const double&>(logic_and));
		fb.registPredefinedOperators("||", S_LONG_CP ",double&", "bool", new CdeclFunction2<bool, LONG_CP, const double&>(logic_or));
#pragma endregion

#pragma region long and bool operators
		//logic operators
		fb.registPredefinedOperators("&&", S_LONG_CP ",bool", "bool", new CdeclFunction2<bool, LONG_CP, bool>(logic_and));
		fb.registPredefinedOperators("||", S_LONG_CP ",bool", "bool", new CdeclFunction2<bool, LONG_CP, bool>(logic_or));
#pragma endregion

#pragma region float and integer operators
		fb.registPredefinedOperators("+", "float&,int", "float", make_native(float, const float&, int, add));
		fb.registPredefinedOperators("-", "float&,int", "float", make_native(float, const float&, int, sub));
		fb.registPredefinedOperators("*", "float&,int", "float", make_native(float, const float&, int, mul));
		fb.registPredefinedOperators("/", "float&,int", "float", make_native(float, const float&, int, div));
		//assigment operator
		fb.registPredefinedOperators("=", "float&,int", "float", make_native(float, float&, int, assign));
		//compound operators
		fb.registPredefinedOperators("+=", "float&,int", "void", new CdeclFunction2<void, float&, int>(add_comp));
		fb.registPredefinedOperators("-=", "float&,int", "void", new CdeclFunction2<void, float&, int>(sub_comp));
		fb.registPredefinedOperators("*=", "float&,int", "void", new CdeclFunction2<void, float&, int>(mul_comp));
		fb.registPredefinedOperators("/=", "float&,int", "void", new CdeclFunction2<void, float&, int>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "float&,int", "bool", new CdeclFunction2<bool, const float&, int>(operators::less));
		fb.registPredefinedOperators("<=", "float&,int", "bool", new CdeclFunction2<bool, const float&, int>(less_or_equal));
		fb.registPredefinedOperators(">", "float&,int", "bool", new CdeclFunction2<bool, const float&, int>(great));
		fb.registPredefinedOperators(">=", "float&,int", "bool", new CdeclFunction2<bool, const float&, int>(great_or_equal));
		fb.registPredefinedOperators("==", "float&,int", "bool", new CdeclFunction2<bool, const float&, int>(equal));
		fb.registPredefinedOperators("!=", "float&,int", "bool", new CdeclFunction2<bool, const float&, int>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "float&,int", "bool", new CdeclFunction2<bool, const float&, int>(logic_and));
		fb.registPredefinedOperators("||", "float&,int", "bool", new CdeclFunction2<bool, const float&, int>(logic_or));
#pragma endregion

#pragma region float and long operators
		fb.registPredefinedOperators("+", "float&," S_LONG_CP, "float", make_native(float, const float&, LONG_CP, add));
		fb.registPredefinedOperators("-", "float&," S_LONG_CP, "float", make_native(float, const float&, LONG_CP, sub));
		fb.registPredefinedOperators("*", "float&," S_LONG_CP, "float", make_native(float, const float&, LONG_CP, mul));
		fb.registPredefinedOperators("/", "float&," S_LONG_CP, "float", make_native(float, const float&, LONG_CP, div));
		//assigment operator
		fb.registPredefinedOperators("=", "float&," S_LONG_CP, "float", make_native(float, float&, LONG_CP, assign));
		//compound operators
		fb.registPredefinedOperators("+=", "float&," S_LONG_CP, "void", new CdeclFunction2<void, float&, LONG_CP>(add_comp));
		fb.registPredefinedOperators("-=", "float&," S_LONG_CP, "void", new CdeclFunction2<void, float&, LONG_CP>(sub_comp));
		fb.registPredefinedOperators("*=", "float&," S_LONG_CP, "void", new CdeclFunction2<void, float&, LONG_CP>(mul_comp));
		fb.registPredefinedOperators("/=", "float&," S_LONG_CP, "void", new CdeclFunction2<void, float&, LONG_CP>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "float&," S_LONG_CP, "bool", new CdeclFunction2<bool, const float&, LONG_CP>(operators::less));
		fb.registPredefinedOperators("<=", "float&," S_LONG_CP, "bool", new CdeclFunction2<bool, const float&, LONG_CP>(less_or_equal));
		fb.registPredefinedOperators(">", "float&," S_LONG_CP, "bool", new CdeclFunction2<bool, const float&, LONG_CP>(great));
		fb.registPredefinedOperators(">=", "float&," S_LONG_CP, "bool", new CdeclFunction2<bool, const float&, LONG_CP>(great_or_equal));
		fb.registPredefinedOperators("==", "float&," S_LONG_CP, "bool", new CdeclFunction2<bool, const float&, LONG_CP>(equal));
		fb.registPredefinedOperators("!=", "float&," S_LONG_CP, "bool", new CdeclFunction2<bool, const float&, LONG_CP>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "float&," S_LONG_CP, "bool", new CdeclFunction2<bool, const float&, LONG_CP>(logic_and));
		fb.registPredefinedOperators("||", "float&," S_LONG_CP, "bool", new CdeclFunction2<bool, const float&, LONG_CP>(logic_or));
#pragma endregion

#pragma region float and double operators
		fb.registPredefinedOperators("+", "float&,double&", "double", make_native(double, const float&, const double&, add));
		fb.registPredefinedOperators("-", "float&,double&", "double", make_native(double, const float&, const double&, sub));
		fb.registPredefinedOperators("*", "float&,double&", "double", make_native(double, const float&, const double&, mul));
		fb.registPredefinedOperators("/", "float&,double&", "double", make_native(double, const float&, const double&, div));
		//assigment operator
		fb.registPredefinedOperators("=", "float&,double&", "float", make_native(float, float&, const double&, assign));
		//compound operators
		fb.registPredefinedOperators("+=", "float&,double&", "void", new CdeclFunction2<void, float&, const double&>(add_comp));
		fb.registPredefinedOperators("-=", "float&,double&", "void", new CdeclFunction2<void, float&, const double&>(sub_comp));
		fb.registPredefinedOperators("*=", "float&,double&", "void", new CdeclFunction2<void, float&, const double&>(mul_comp));
		fb.registPredefinedOperators("/=", "float&,double&", "void", new CdeclFunction2<void, float&, const double&>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "float&,double&", "bool", new CdeclFunction2<bool, const float&, const double&>(operators::less));
		fb.registPredefinedOperators("<=", "float&,double&", "bool", new CdeclFunction2<bool, const float&, const double&>(less_or_equal));
		fb.registPredefinedOperators(">", "float&,double&", "bool", new CdeclFunction2<bool, const float&, const double&>(great));
		fb.registPredefinedOperators(">=", "float&,double&", "bool", new CdeclFunction2<bool, const float&, const double&>(great_or_equal));
		fb.registPredefinedOperators("==", "float&,double&", "bool", new CdeclFunction2<bool, const float&, const double&>(equal));
		fb.registPredefinedOperators("!=", "float&,double&", "bool", new CdeclFunction2<bool, const float&, const double&>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "float&,double&", "bool", new CdeclFunction2<bool, const float&, const double&>(logic_and));
		fb.registPredefinedOperators("||", "float&,double&", "bool", new CdeclFunction2<bool, const float&, const double&>(logic_or));
#pragma endregion

#pragma region float and bool operators
		//logic operators
		fb.registPredefinedOperators("&&", "float&,bool", "bool", new CdeclFunction2<bool, const float&, bool>(logic_and));
		fb.registPredefinedOperators("||", "float&,bool", "bool", new CdeclFunction2<bool, const float&, bool>(logic_or));
#pragma endregion

#pragma region double and integer operators
		fb.registPredefinedOperators("+", "double&,int", "double", make_native(double, const double&, int, add));
		fb.registPredefinedOperators("-", "double&,int", "double", make_native(double, const double&, int, sub));
		fb.registPredefinedOperators("*", "double&,int", "double", make_native(double, const double&, int, mul));
		fb.registPredefinedOperators("/", "double&,int", "double", make_native(double, const double&, int, div));
		//assigment operator
		fb.registPredefinedOperators("=", "double&,int", "double", make_native(double, double&, int, assign));
		//compound operators
		fb.registPredefinedOperators("+=", "double&,int", "void", new CdeclFunction2<void, double&, int>(add_comp));
		fb.registPredefinedOperators("-=", "double&,int", "void", new CdeclFunction2<void, double&, int>(sub_comp));
		fb.registPredefinedOperators("*=", "double&,int", "void", new CdeclFunction2<void, double&, int>(mul_comp));
		fb.registPredefinedOperators("/=", "double&,int", "void", new CdeclFunction2<void, double&, int>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "double&,int", "bool", new CdeclFunction2<bool, const double&, int>(operators::less));
		fb.registPredefinedOperators("<=", "double&,int", "bool", new CdeclFunction2<bool, const double&, int>(less_or_equal));
		fb.registPredefinedOperators(">", "double&,int", "bool", new CdeclFunction2<bool, const double&, int>(great));
		fb.registPredefinedOperators(">=", "double&,int", "bool", new CdeclFunction2<bool, const double&, int>(great_or_equal));
		fb.registPredefinedOperators("==", "double&,int", "bool", new CdeclFunction2<bool, const double&, int>(equal));
		fb.registPredefinedOperators("!=", "double&,int", "bool", new CdeclFunction2<bool, const double&, int>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "double&,int", "bool", new CdeclFunction2<bool, const double&, int>(logic_and));
		fb.registPredefinedOperators("||", "double&,int", "bool", new CdeclFunction2<bool, const double&, int>(logic_or));
#pragma endregion

#pragma region double and long operators
		fb.registPredefinedOperators("+", "double&," S_LONG_CP, "double", make_native(double, const double&, LONG_CP, add));
		fb.registPredefinedOperators("-", "double&," S_LONG_CP, "double", make_native(double, const double&, LONG_CP, sub));
		fb.registPredefinedOperators("*", "double&," S_LONG_CP, "double", make_native(double, const double&, LONG_CP, mul));
		fb.registPredefinedOperators("/", "double&," S_LONG_CP, "double", make_native(double, const double&, LONG_CP, div));
		//assigment operator
		fb.registPredefinedOperators("=", "double&," S_LONG_CP, "double", make_native(double, double&, LONG_CP, assign));
		//compound operators
		fb.registPredefinedOperators("+=", "double&," S_LONG_CP, "void", new CdeclFunction2<void, double&, LONG_CP>(add_comp));
		fb.registPredefinedOperators("-=", "double&," S_LONG_CP, "void", new CdeclFunction2<void, double&, LONG_CP>(sub_comp));
		fb.registPredefinedOperators("*=", "double&," S_LONG_CP, "void", new CdeclFunction2<void, double&, LONG_CP>(mul_comp));
		fb.registPredefinedOperators("/=", "double&," S_LONG_CP, "void", new CdeclFunction2<void, double&, LONG_CP>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "double&," S_LONG_CP, "bool", new CdeclFunction2<bool, const double&, LONG_CP>(operators::less));
		fb.registPredefinedOperators("<=", "double&," S_LONG_CP, "bool", new CdeclFunction2<bool, const double&, LONG_CP>(less_or_equal));
		fb.registPredefinedOperators(">", "double&," S_LONG_CP, "bool", new CdeclFunction2<bool, const double&, LONG_CP>(great));
		fb.registPredefinedOperators(">=", "double&," S_LONG_CP, "bool", new CdeclFunction2<bool, const double&, LONG_CP>(great_or_equal));
		fb.registPredefinedOperators("==", "double&," S_LONG_CP, "bool", new CdeclFunction2<bool, const double&, LONG_CP>(equal));
		fb.registPredefinedOperators("!=", "double&," S_LONG_CP, "bool", new CdeclFunction2<bool, const double&, LONG_CP>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "double&," S_LONG_CP, "bool", new CdeclFunction2<bool, const double&, LONG_CP>(logic_and));
		fb.registPredefinedOperators("||", "double&," S_LONG_CP, "bool", new CdeclFunction2<bool, const double&, LONG_CP>(logic_or));
#pragma endregion

#pragma region double and float operators
		fb.registPredefinedOperators("+", "double&,float&", "double", make_native(double, const double&, const float&, add));
		fb.registPredefinedOperators("-", "double&,float&", "double", make_native(double, const double&, const float&, sub));
		fb.registPredefinedOperators("*", "double&,float&", "double", make_native(double, const double&, const float&, mul));
		fb.registPredefinedOperators("/", "double&,float&", "double", make_native(double, const double&, const float&, div));
		//assigment operator
		fb.registPredefinedOperators("=", "double&,float&", "float", make_native(double, double&, const float&, assign));
		//compound operators
		fb.registPredefinedOperators("+=", "double&,float&", "void", new CdeclFunction2<void, double&, const float&>(add_comp));
		fb.registPredefinedOperators("-=", "double&,float&", "void", new CdeclFunction2<void, double&, const float&>(sub_comp));
		fb.registPredefinedOperators("*=", "double&,float&", "void", new CdeclFunction2<void, double&, const float&>(mul_comp));
		fb.registPredefinedOperators("/=", "double&,float&", "void", new CdeclFunction2<void, double&, const float&>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "double&,float&", "bool", new CdeclFunction2<bool, const double&, const float&>(operators::less));
		fb.registPredefinedOperators("<=", "double&,float&", "bool", new CdeclFunction2<bool, const double&, const float&>(less_or_equal));
		fb.registPredefinedOperators(">", "double&,float&", "bool", new CdeclFunction2<bool, const double&, const float&>(great));
		fb.registPredefinedOperators(">=", "double&,float&", "bool", new CdeclFunction2<bool, const double&, const float&>(great_or_equal));
		fb.registPredefinedOperators("==", "double&,float&", "bool", new CdeclFunction2<bool, const double&, const float&>(equal));
		fb.registPredefinedOperators("!=", "double&,float&", "bool", new CdeclFunction2<bool, const double&, const float&>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "double&,float&", "bool", new CdeclFunction2<bool, const double&, const float&>(logic_and));
		fb.registPredefinedOperators("||", "double&,float&", "bool", new CdeclFunction2<bool, const double&, const float&>(logic_or));
#pragma endregion

#pragma region bool and integer operators
		//logic operators
		fb.registPredefinedOperators("&&", "bool,int", "bool", new CdeclFunction2<bool, bool, int>(logic_and));
		fb.registPredefinedOperators("||", "bool,int", "bool", new CdeclFunction2<bool, bool, int>(logic_or));
#pragma endregion

#pragma region bool and long operators
		//logic operators
		fb.registPredefinedOperators("&&", "bool," S_LONG_CP, "bool", new CdeclFunction2<bool, bool, LONG_CP>(logic_and));
		fb.registPredefinedOperators("||", "bool," S_LONG_CP, "bool", new CdeclFunction2<bool, bool, LONG_CP>(logic_or));
#pragma endregion

#pragma region bool and float operators
		//logic operators
		fb.registPredefinedOperators("&&", "bool,float&", "bool", new CdeclFunction2<bool, bool, const float&>(logic_and));
		fb.registPredefinedOperators("||", "bool,float&", "bool", new CdeclFunction2<bool, bool, const float&>(logic_or));
#pragma endregion

#pragma region bool and double operators
		//logic operators
		fb.registPredefinedOperators("&&", "bool,double&", "bool", new CdeclFunction2<bool, bool, const double&>(logic_and));
		fb.registPredefinedOperators("||", "bool,double&", "bool", new CdeclFunction2<bool, bool, const double&>(logic_or));
#pragma endregion

#pragma region other operators
		auto scriptCompiler = fb.getSriptCompiler();
		fb.registFunction("length", "string", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", new CdeclFunction2<int, const std::string&>(stringLength<std::string>), scriptCompiler), true);
		importCoreFunctions(fb);
#pragma endregion

		int iTypeString = scriptCompiler->getTypeManager()->getBasicTypes().TYPE_RAWSTRING;

		int ctor = fb.registFunction("defaultConstructor", "ref String", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, RawString&>(defaultConstructor), scriptCompiler));
		int dtor = fb.registFunction("freeRawString", "ref String", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, RawString&>(freeSimpleArray<RawChar>), scriptCompiler));

		scriptCompiler->registDestructor(iTypeString, dtor);
		scriptCompiler->registConstructor(iTypeString, ctor);

		ctor = fb.registFunction("constantConstructor", "ref String, string&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, RawString&, const std::string&>(constantConstructor), scriptCompiler));
		scriptCompiler->registConstructor(iTypeString, ctor);

		ctor = fb.registFunction("constantConstructor", "ref String, wstring&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, RawString&, const std::wstring&>(constantConstructor), scriptCompiler));
		scriptCompiler->registConstructor(iTypeString, ctor);

		ctor = fb.registFunction("constantConstructor", "ref String, String&", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, RawString&, const RawString&>(constantConstructor), scriptCompiler));
		scriptCompiler->registConstructor(iTypeString, ctor);
	}

	void importCoreFunctions(FunctionRegisterHelper& fb) {
#pragma region other operators

		auto scriptCompiler = fb.getSriptCompiler();

		//register choice operator of conditional operator( condition ? ifClause : elseClause)
		fb.registPredefinedOperators(":", "", "", nullptr);

		//register conditional opearator with param bool type as condition, void type for last two params
		//the void type for param usually don't work, they are just temporary types and they will be corrected later
		//for this very special oprator
		auto conditionalOperatorFactory = new ConditionalOperatorFactory(scriptCompiler);
		scriptCompiler->registConditionalFunction(conditionalOperatorFactory);
		fb.addFactory(conditionalOperatorFactory);

		fb.registFunction("=", "ref void,ref void,int,int ", new BasicFunctionFactory<4>(EXP_UNIT_ID_OPERATOR_ASSIGNMENT, FUNCTION_PRIORITY_ASSIGNMENT, "ref void", new CdeclFunction2<void*, void*, void*, int, int>(assignStruct), scriptCompiler), true);

		auto theNativeFunction1 = new CdeclFunction2<SimpleVariantArray*, SimpleVariantArray*>(dynamicFunctionGateway);
		auto dynamicFunctionFactory1 = new DynamicFunctionFactory("ref " SYSTEM_ARRAY_STRUCT, theNativeFunction1, scriptCompiler);
		scriptCompiler->registDynamicFunction(SYSTEM_ARRAY_FUNCTION, dynamicFunctionFactory1);
		fb.addFactory(dynamicFunctionFactory1);

		//fb.registFunction("=", "ref void,ref " SYSTEM_ARRAY_STRUCT ",int,int", new BasicFunctionFactory<4>(EXP_UNIT_ID_OPERATOR_ASSIGNMENT, FUNCTION_PRIORITY_ASSIGNMENT, "ref void", new CdeclFunction2<void*, void*, SimpleVariantArray*, int, int>(assignArray2Struct1), scriptCompiler), true);

		//register default member access opearator with param void type as return
		//return type and first parameter will be corrected later(when link)
		//fb.registFunction(".", "ref void, int", new BasicFunctionFactory<2>(EXP_UNIT_ID_MEMBER_ACCESS, FUNCTION_PRIORITY_MEMBER_ACCESS, "ref void", new CdeclFunction2<void*, char*, int>(memberAccess), scriptCompiler), true);

		//register making ref function
		fb.registFunction(MAKING_REF_FUNC, "void", new MakeRefFactory(scriptCompiler), true);
		//int derefid = fb.registFunction(DEREF_OPERATOR, "ref void", new DeRefFactory(scriptCompiler), true);
		//scriptCompiler->setDerefFunction(derefid);

		//register default copy operator, this is jus procedure to pass double check when parse expression
		//the real declaration which will be used is defined in pre-defined operator table in ScriptCompiler object
		//and the implementation is defined in ExpUnitExcuter
		auto defaultCopyFunctionFactory = new BasicFunctionFactory<2>(EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR, FUNCTION_PRIORITY_ASSIGNMENT, "void", nullptr, scriptCompiler);
		scriptCompiler->registDefaultCopyFunction(defaultCopyFunctionFactory);
		fb.addFactory(defaultCopyFunctionFactory);

		//register default constructor/destructor
		fb.registFunction(DEFAULT_DUMMY_OPERATOR, "ref void", new BasicFunctionFactory<4>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, void*>(dummyOperator), scriptCompiler), true);

		//register default constructor/destructor for function objects
		int f1 = fb.registFunction(SYSTEM_FUNCTION_CONSTRUCTOR, "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, RuntimeFunctionInfo*>(defaultRuntimeFunctionInfoConstructor), scriptCompiler), true);
		int f2 = fb.registFunction(SYSTEM_FUNCTION_DESTRUCTOR, "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, RuntimeFunctionInfo*>(runtimeFunctionInfoDestructor), scriptCompiler), true);
		//int f3 = fb.registFunction(SYSTEM_FUNCTION_COPY_CONSTRUCTOR, "ref void, ref void", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, RuntimeFunctionInfo*>(runtimeFunctionInfoCopyConstructor), scriptCompiler), true);

		scriptCompiler->setRuntimeInfoConstructor(f1);
		scriptCompiler->setRuntimeInfoDestructor(f2);
		//scriptCompiler->setRuntimeInfoCopyConstructor(f3);
		
		fb.registFunction("createThread", "function<void()>&", new BasicFunctionFactory<1>(EXP_UNIT_ID_CREATE_THREAD, FUNCTION_PRIORITY_USER_FUNCTION, "hthread", new CreateThreadCommand(), scriptCompiler), true);
		fb.registFunction("joinThread", "hthread", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, THREAD_HANDLE>(joinThread), scriptCompiler), true);
		fb.registFunction("closeThread", "hthread", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", new CdeclFunction2<void, THREAD_HANDLE>(closeThread), scriptCompiler), true);
#pragma endregion
	}
}
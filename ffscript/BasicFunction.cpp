/******************************************************************
* File:        BasicFunction.cpp
* Description: implement some basic expression function unit factory
*              classes.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "BasicFunction.h"
#include "expressionunit.h"
#include "BasicFunctionFactory.hpp"
#include "Internal.h"
#include "DynamicFunctionFactory.h"
#include "BasicType.h"
#include "ConditionalOperator.h"
#include "RefFunction.h"
#include "DefaultCommands.h"

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

	UserFunctionFactory::UserFunctionFactory(ScriptCompiler* scriptCompiler, const std::string& returnType, int paramSize) : _paramSize(paramSize) {
		this->setReturnType(ScriptType::parseType(scriptCompiler, returnType));
	}

	UserFunctionFactory::~UserFunctionFactory() {}

	Function* UserFunctionFactory::createFunction(const std::string& name, int id) {
		DynamicParamFunction* function = new DynamicParamFunction(name, EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, getReturnType());
		function->setNative(createNative());
		function->setMaxParam(_paramSize);
		return function;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	DefaultUserFunctionFactory::DefaultUserFunctionFactory(DFunction2Ref nativeFunction, ScriptCompiler* scriptCompiler, const std::string& returnType, int paramSize) :
		_nativeFunction(nativeFunction),
		UserFunctionFactory(scriptCompiler, returnType, paramSize) {}

	DefaultUserFunctionFactory::~DefaultUserFunctionFactory() {}

	DFunction2Ref DefaultUserFunctionFactory::createNative() {
		return _nativeFunction;
	}

	//string functions
	template <class T>
	int stringLength(const T& s) {
		return (int)s.length();
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

	void importBasicfunction(FunctionRegisterHelper& fb) {

#pragma region integer operators only
		fb.registPredefinedOperators("+", "int,int", "int", createFunctionDelegate<int, int, int>(add));
		fb.registPredefinedOperators("-", "int,int", "int", createFunctionDelegate<int, int, int>(sub));
		fb.registPredefinedOperators("*", "int,int", "int", createFunctionDelegate<int, int, int>(mul));
		fb.registPredefinedOperators("/", "int,int", "int", createFunctionDelegate<int, int, int>(div));
		fb.registPredefinedOperators("%", "int,int", "int", createFunctionDelegate<int, int, int>(mod));
		//negative operator
		
		//assigment operator
		fb.registPredefinedOperators("=", "int&,int", "int", createFunctionDelegate(assign<int, int, int>));
		//bitwises
		fb.registPredefinedOperators("&", "int,int", "int", createFunctionDelegate<int, int, int>(bitwise_and));
		fb.registPredefinedOperators("|", "int,int", "int", createFunctionDelegate<int, int, int>(bitwise_or));
		fb.registPredefinedOperators("^", "int,int", "int", createFunctionDelegate<int, int, int>(bitwise_xor));
		fb.registPredefinedOperators("<<", "int,int", "int", createFunctionDelegate<int, int, int>(bitwise_shiftLeft));
		fb.registPredefinedOperators(">>", "int,int", "int", createFunctionDelegate<int, int, int>(bitwise_shiftRight));
		//pre-post fix operators
		fb.registPredefinedOperators("neg", "int", "int", createFunctionDelegate<int, int>(neg));
		fb.registPredefinedOperators("~", "int", "int", createFunctionDelegate<int, int>(bitwise_not));
		fb.registPredefinedOperators("post_fix_increase", "int&", "int", createFunctionDelegate<int, int&>(post_inc));
		fb.registPredefinedOperators("++", "int&", "int", createFunctionDelegate<int, int&>(pre_inc));
		fb.registPredefinedOperators("post_fix_decrease", "int&", "int", createFunctionDelegate<int, int&>(post_dec));
		fb.registPredefinedOperators("--", "int&", "int", createFunctionDelegate<int, int&>(pre_dec));
		//compound operators
		fb.registPredefinedOperators("+=", "int&,int", "void", createFunctionDelegate<void, int&, int>(add_comp));
		fb.registPredefinedOperators("-=", "int&,int", "void", createFunctionDelegate<void, int&, int>(sub_comp));
		fb.registPredefinedOperators("*=", "int&,int", "void", createFunctionDelegate<void, int&, int>(mul_comp));
		fb.registPredefinedOperators("/=", "int&,int", "void", createFunctionDelegate<void, int&, int>(div_comp));
		fb.registPredefinedOperators("%=", "int&,int", "void", createFunctionDelegate<void, int&, int>(mod_comp));
		fb.registPredefinedOperators("&=", "int&,int", "void", createFunctionDelegate<void, int&, int>(and_comp));
		fb.registPredefinedOperators("|=", "int&,int", "void", createFunctionDelegate<void, int&, int>(or_comp));
		fb.registPredefinedOperators("^=", "int&,int", "void", createFunctionDelegate<void, int&, int>(xor_comp));
		fb.registPredefinedOperators("<<=", "int&,int", "void", createFunctionDelegate<void, int&, int>(shiftLeft_comp));
		fb.registPredefinedOperators(">>=", "int&,int", "void", createFunctionDelegate<void, int&, int>(shiftRight_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "int,int", "bool", createFunctionDelegate<bool, int, int>(operators::less));
		fb.registPredefinedOperators("<=", "int,int", "bool", createFunctionDelegate<bool, int, int>(operators::less_or_equal));
		fb.registPredefinedOperators(">", "int,int", "bool", createFunctionDelegate<bool, int, int>(operators::great));
		fb.registPredefinedOperators(">=", "int,int", "bool", createFunctionDelegate<bool, int, int>(operators::great_or_equal));
		fb.registPredefinedOperators("==", "int,int", "bool", createFunctionDelegate<bool, int, int>(operators::equal));
		fb.registPredefinedOperators("!=", "int,int", "bool", createFunctionDelegate<bool, int, int>(operators::not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "int,int", "bool", createFunctionDelegate<bool, int, int>(logic_and));
		fb.registPredefinedOperators("||", "int,int", "bool", createFunctionDelegate<bool, int, int>(logic_or));
		fb.registPredefinedOperators("!", "int", "bool", createFunctionDelegate<bool, int>(logic_not));
#pragma endregion

#pragma region long operators only
		fb.registPredefinedOperators("+", "long,long", "long", createFunctionDelegate<long long, long long, long long>(add));
		fb.registPredefinedOperators("-", "long,long", "long", createFunctionDelegate<long long, long long, long long>(sub));
		fb.registPredefinedOperators("*", "long,long", "long", createFunctionDelegate<long long, long long, long long>(mul));
		fb.registPredefinedOperators("/", "long,long", "long", createFunctionDelegate<long long, long long, long long>(div));
		fb.registPredefinedOperators("%", "long,long", "long", createFunctionDelegate<long long, long long, long long>(mod));
		//negative operator

		//assigment operator
		fb.registPredefinedOperators("=", "long&,long", "long", createFunctionDelegate(assign<long long, long long, long long>));
		//bitwises
		fb.registPredefinedOperators("&", "long,long", "long", createFunctionDelegate<long long, long long, long long>(bitwise_and));
		fb.registPredefinedOperators("|", "long,long", "long", createFunctionDelegate<long long, long long, long long>(bitwise_or));
		fb.registPredefinedOperators("^", "long,long", "long", createFunctionDelegate<long long, long long, long long>(bitwise_xor));
		fb.registPredefinedOperators("<<", "long,long", "long", createFunctionDelegate<long long, long long, long long>(bitwise_shiftLeft));
		fb.registPredefinedOperators(">>", "long,long", "long", createFunctionDelegate<long long, long long, long long>(bitwise_shiftRight));
		//pre-post fix operators
		fb.registPredefinedOperators("neg",					"long", "long", createFunctionDelegate<long long, long long>(neg));
		fb.registPredefinedOperators("~",					"long", "long", createFunctionDelegate<long long, long long>(bitwise_not));
		fb.registPredefinedOperators("post_fix_increase", "long&", "long", createFunctionDelegate<long long, long long&>(post_inc));
		fb.registPredefinedOperators("++", "long&", "long", createFunctionDelegate<long long, long long&>(pre_inc));
		fb.registPredefinedOperators("post_fix_decrease", "long&", "long", createFunctionDelegate<long long, long long&>(post_dec));
		fb.registPredefinedOperators("--", "long&", "long", createFunctionDelegate<long long, long long&>(pre_dec));
		//compound operators
		fb.registPredefinedOperators("+=", "long&,long", "void", createFunctionDelegate<void, long long&, long long>(add_comp));
		fb.registPredefinedOperators("-=", "long&,long", "void", createFunctionDelegate<void, long long&, long long>(sub_comp));
		fb.registPredefinedOperators("*=", "long&,long", "void", createFunctionDelegate<void, long long&, long long>(mul_comp));
		fb.registPredefinedOperators("/=", "long&,long", "void", createFunctionDelegate<void, long long&, long long>(div_comp));
		fb.registPredefinedOperators("%=", "long&,long", "void", createFunctionDelegate<void, long long&, long long>(mod_comp));
		fb.registPredefinedOperators("&=", "long&,long", "void", createFunctionDelegate<void, long long&, long long>(and_comp));
		fb.registPredefinedOperators("|=", "long&,long", "void", createFunctionDelegate<void, long long&, long long>(or_comp));
		fb.registPredefinedOperators("^=", "long&,long", "void", createFunctionDelegate<void, long long&, long long>(xor_comp));
		fb.registPredefinedOperators("<<=", "long&,long", "void", createFunctionDelegate<void, long long&, long long>(shiftLeft_comp));
		fb.registPredefinedOperators(">>=", "long&,long", "void", createFunctionDelegate<void, long long&, long long>(shiftRight_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "long,long", "bool", createFunctionDelegate<bool, long long, long long>(operators::less));
		fb.registPredefinedOperators("<=", "long,long", "bool", createFunctionDelegate<bool, long long, long long>(less_or_equal));
		fb.registPredefinedOperators(">", "long,long", "bool", createFunctionDelegate<bool, long long, long long>(great));
		fb.registPredefinedOperators(">=", "long,long", "bool", createFunctionDelegate<bool, long long, long long>(great_or_equal));
		fb.registPredefinedOperators("==", "long,long", "bool", createFunctionDelegate<bool, long long, long long>(equal));
		fb.registPredefinedOperators("!=", "long,long", "bool", createFunctionDelegate<bool, long long, long long>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "long,long", "bool", createFunctionDelegate<bool, long long, long long>(logic_and));
		fb.registPredefinedOperators("||", "long,long", "bool", createFunctionDelegate<bool, long long, long long>(logic_or));
		fb.registPredefinedOperators("!", "long", "bool", createFunctionDelegate<bool, long long>(logic_not));
#pragma endregion

#pragma region float operators only
		fb.registPredefinedOperators("+", "float,float", "float", createFunctionDelegate<float, float, float>(add));
		fb.registPredefinedOperators("-", "float,float", "float", createFunctionDelegate<float, float, float>(sub));
		fb.registPredefinedOperators("*", "float,float", "float", createFunctionDelegate<float, float, float>(mul));
		fb.registPredefinedOperators("/", "float,float", "float", createFunctionDelegate<float, float, float>(div));

		//assigment operator
		fb.registPredefinedOperators("=", "float&,float", "float", createFunctionDelegate(assign<float, float&, float>));
		//pre-post fix operators
		fb.registPredefinedOperators("neg", "float", "float", createFunctionDelegate<float, float>(neg));
		fb.registPredefinedOperators("post_fix_increase", "float&", "float", createFunctionDelegate<float, float&>(post_inc));
		fb.registPredefinedOperators("++", "float&", "float", createFunctionDelegate<float, float&>(pre_inc));
		fb.registPredefinedOperators("post_fix_decrease", "float&", "float", createFunctionDelegate<float, float&>(post_dec));
		fb.registPredefinedOperators("--", "float&", "float", createFunctionDelegate<float, float&>(pre_dec));
		//compound operators
		fb.registPredefinedOperators("+=", "float&, float", "void", createFunctionDelegate<void, float&, float>(add_comp));
		fb.registPredefinedOperators("-=", "float&, float", "void", createFunctionDelegate<void, float&, float>(sub_comp));
		fb.registPredefinedOperators("*=", "float&, float", "void", createFunctionDelegate<void, float&, float>(mul_comp));
		fb.registPredefinedOperators("/=", "float&, float", "void", createFunctionDelegate<void, float&, float>(div_comp));

		//comparision operators
		fb.registPredefinedOperators("<", "float, float", "bool", createFunctionDelegate<bool, float, float>(operators::less));
		fb.registPredefinedOperators("<=", "float, float", "bool", createFunctionDelegate<bool, float, float>(less_or_equal));
		fb.registPredefinedOperators(">", "float, float", "bool", createFunctionDelegate<bool, float, float>(great));
		fb.registPredefinedOperators(">=", "float, float", "bool", createFunctionDelegate<bool, float, float>(great_or_equal));
		fb.registPredefinedOperators("==", "float, float", "bool", createFunctionDelegate<bool, float, float>(equal));
		fb.registPredefinedOperators("!=", "float, float", "bool", createFunctionDelegate<bool, float, float>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "float, float", "bool", createFunctionDelegate<bool, float, float>(logic_and));
		fb.registPredefinedOperators("||", "float, float", "bool", createFunctionDelegate<bool, float, float>(logic_or));
		fb.registPredefinedOperators("!", "float", "bool", createFunctionDelegate<bool, float>(logic_not));
#pragma endregion

#pragma region double operators only
		fb.registPredefinedOperators("+", "double,double", "double", createFunctionDelegate<double, double, double>(add));
		fb.registPredefinedOperators("-", "double,double", "double", createFunctionDelegate<double, double, double>(sub));
		fb.registPredefinedOperators("*", "double,double", "double", createFunctionDelegate<double, double, double>(mul));
		fb.registPredefinedOperators("/", "double,double", "double", createFunctionDelegate<double, double, double>(div));

		//assigment operator
		fb.registPredefinedOperators("=", "double&,double", "double", createFunctionDelegate(assign<double, double, double>));
		//pre-post fix operators
		fb.registPredefinedOperators("neg", "double", "double", createFunctionDelegate<double, double>(neg));
		fb.registPredefinedOperators("post_fix_increase", "double&", "double", createFunctionDelegate<double, double&>(post_inc));
		fb.registPredefinedOperators("++", "double&", "double", createFunctionDelegate<double, double&>(pre_inc));
		fb.registPredefinedOperators("post_fix_decrease", "double&", "double", createFunctionDelegate<double, double&>(post_dec));
		fb.registPredefinedOperators("--", "double&", "double", createFunctionDelegate<double, double&>(pre_dec));
		//compound operators
		fb.registPredefinedOperators("+=", "double&,double", "void", createFunctionDelegate<void, double&, double>(add_comp));
		fb.registPredefinedOperators("-=", "double&,double", "void", createFunctionDelegate<void, double&, double>(sub_comp));
		fb.registPredefinedOperators("*=", "double&,double", "void", createFunctionDelegate<void, double&, double>(mul_comp));
		fb.registPredefinedOperators("/=", "double&,double", "void", createFunctionDelegate<void, double&, double>(div_comp));

		//comparision operators
		fb.registPredefinedOperators("<", "double,double", "bool", createFunctionDelegate<bool, double, double>(operators::less));
		fb.registPredefinedOperators("<=", "double,double", "bool", createFunctionDelegate<bool, double, double>(less_or_equal));
		fb.registPredefinedOperators(">", "double,double", "bool", createFunctionDelegate<bool, double, double>(great));
		fb.registPredefinedOperators(">=", "double,double", "bool", createFunctionDelegate<bool, double, double>(great_or_equal));
		fb.registPredefinedOperators("==", "double,double", "bool", createFunctionDelegate<bool, double, double>(equal));
		fb.registPredefinedOperators("!=", "double,double", "bool", createFunctionDelegate<bool, double, double>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "double,double", "bool", createFunctionDelegate<bool, double, double>(logic_and));
		fb.registPredefinedOperators("||", "double,double", "bool", createFunctionDelegate<bool, double, double>(logic_or));
		fb.registPredefinedOperators("!", "double", "bool", createFunctionDelegate<bool, double>(logic_not));
#pragma endregion

#pragma region bool operators only
		//assigment operator
		fb.registPredefinedOperators("=", "bool&,bool", "bool", createFunctionDelegate(assign<bool, bool, bool>));
		//comparision operators
		fb.registPredefinedOperators("==", "bool,bool", "bool", createFunctionDelegate<bool, bool, bool>(operators::equal));
		fb.registPredefinedOperators("!=", "bool,bool", "bool", createFunctionDelegate<bool, bool, bool>(operators::not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "bool,bool", "bool", createFunctionDelegate<bool, bool, bool>(logic_and));
		fb.registPredefinedOperators("||", "bool,bool", "bool", createFunctionDelegate<bool, bool, bool>(logic_or));
		fb.registPredefinedOperators("!", "bool", "bool", createFunctionDelegate<bool, bool>(logic_not));
#pragma endregion

#pragma region integer and long operators
		fb.registPredefinedOperators("+", "int,long", "long", createFunctionDelegate<long long, int, long long>(add));
		fb.registPredefinedOperators("-", "int,long", "long", createFunctionDelegate<long long, int, long long>(sub));
		fb.registPredefinedOperators("*", "int,long", "long", createFunctionDelegate<long long, int, long long>(mul));
		fb.registPredefinedOperators("/", "int,long", "long", createFunctionDelegate<long long, int, long long>(div));
		fb.registPredefinedOperators("%", "int,long", "long", createFunctionDelegate<long long, int, long long>(mod));
		//assigment operator
		fb.registPredefinedOperators("=", "int&,long", "int", createFunctionDelegate(assign<int, int, long long>));
		//bitwises
		fb.registPredefinedOperators("&", "int,long", "long", createFunctionDelegate<long long, int, long long>(bitwise_and));
		fb.registPredefinedOperators("|", "int,long", "long", createFunctionDelegate<long long, int, long long>(bitwise_or));
		fb.registPredefinedOperators("^", "int,long", "long", createFunctionDelegate<long long, int, long long>(bitwise_xor));
		fb.registPredefinedOperators("<<", "int,long", "int", createFunctionDelegate<int, int, long long>(bitwise_shiftLeft));
		fb.registPredefinedOperators(">>", "int,long", "int", createFunctionDelegate<int, int, long long>(bitwise_shiftRight));
		//compound operators
		fb.registPredefinedOperators("+=", "int&,long", "void", createFunctionDelegate<void, int&, long long>(add_comp));
		fb.registPredefinedOperators("-=", "int&,long", "void", createFunctionDelegate<void, int&, long long>(sub_comp));
		fb.registPredefinedOperators("*=", "int&,long", "void", createFunctionDelegate<void, int&, long long>(mul_comp));
		fb.registPredefinedOperators("/=", "int&,long", "void", createFunctionDelegate<void, int&, long long>(div_comp));
		fb.registPredefinedOperators("%=", "int&,long", "void", createFunctionDelegate<void, int&, long long>(mod_comp));
		fb.registPredefinedOperators("&=", "int&,long", "void", createFunctionDelegate<void, int&, long long>(and_comp));
		fb.registPredefinedOperators("|=", "int&,long", "void", createFunctionDelegate<void, int&, long long>(or_comp));
		fb.registPredefinedOperators("^=", "int&,long", "void", createFunctionDelegate<void, int&, long long>(xor_comp));
		fb.registPredefinedOperators("<<=", "int&,long", "void", createFunctionDelegate<void, int&, long long>(shiftLeft_comp));
		fb.registPredefinedOperators(">>=", "int&,long", "void", createFunctionDelegate<void, int&, long long>(shiftRight_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "int,long", "bool", createFunctionDelegate<bool, int, long long>(operators::less));
		fb.registPredefinedOperators("<=", "int,long", "bool", createFunctionDelegate<bool, int, long long>(less_or_equal));
		fb.registPredefinedOperators(">", "int,long", "bool", createFunctionDelegate<bool, int, long long>(great));
		fb.registPredefinedOperators(">=", "int,long", "bool", createFunctionDelegate<bool, int, long long>(great_or_equal));
		fb.registPredefinedOperators("==", "int,long", "bool", createFunctionDelegate<bool, int, long long>(equal));
		fb.registPredefinedOperators("!=", "int,long", "bool", createFunctionDelegate<bool, int, long long>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "int,long", "bool", createFunctionDelegate<bool, int, long long>(logic_and));
		fb.registPredefinedOperators("||", "int,long", "bool", createFunctionDelegate<bool, int, long long>(logic_or));
#pragma endregion

#pragma region integer and float operators
		fb.registPredefinedOperators("+", "int,float", "float", createFunctionDelegate<float, int, float>(add));
		fb.registPredefinedOperators("-", "int,float", "float", createFunctionDelegate<float, int, float>(sub));
		fb.registPredefinedOperators("*", "int,float", "float", createFunctionDelegate<float, int, float>(mul));
		fb.registPredefinedOperators("/", "int,float", "float", createFunctionDelegate<float, int, float>(div));
		//assigment operator
		fb.registPredefinedOperators("=", "int&,float", "int", createFunctionDelegate(assign<int, int, float>));
		//compound operators
		fb.registPredefinedOperators("+=", "int&,float", "void", createFunctionDelegate<void, int&, float>(add_comp));
		fb.registPredefinedOperators("-=", "int&,float", "void", createFunctionDelegate<void, int&, float>(sub_comp));
		fb.registPredefinedOperators("*=", "int&,float", "void", createFunctionDelegate<void, int&, float>(mul_comp));
		fb.registPredefinedOperators("/=", "int&,float", "void", createFunctionDelegate<void, int&, float>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "int,float", "bool", createFunctionDelegate<bool, int, float>(operators::less));
		fb.registPredefinedOperators("<=", "int,float", "bool", createFunctionDelegate<bool, int, float>(operators::less_or_equal));
		fb.registPredefinedOperators(">", "int,float", "bool", createFunctionDelegate<bool, int, float>(operators::great));
		fb.registPredefinedOperators(">=", "int,float", "bool", createFunctionDelegate<bool, int, float>(operators::great_or_equal));
		fb.registPredefinedOperators("==", "int,float", "bool", createFunctionDelegate<bool, int, float>(operators::equal));
		fb.registPredefinedOperators("!=", "int,float", "bool", createFunctionDelegate<bool, int, float>(operators::not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "int,float", "bool", createFunctionDelegate<bool, int, float>(logic_and));
		fb.registPredefinedOperators("||", "int,float", "bool", createFunctionDelegate<bool, int, float>(logic_or));
#pragma endregion

#pragma region integer and double operators
		fb.registPredefinedOperators("+", "int,double", "double", createFunctionDelegate<double, int, double>(add));
		fb.registPredefinedOperators("-", "int,double", "double", createFunctionDelegate<double, int, double>(sub));
		fb.registPredefinedOperators("*", "int,double", "double", createFunctionDelegate<double, int, double>(mul));
		fb.registPredefinedOperators("/", "int,double", "double", createFunctionDelegate<double, int, double>(div));
		//assigment operator
		fb.registPredefinedOperators("=", "int&,double", "int", createFunctionDelegate(assign<int, int, double>));
		//compound operators
		fb.registPredefinedOperators("+=", "int&,double", "void", createFunctionDelegate<void, int&, double>(add_comp));
		fb.registPredefinedOperators("-=", "int&,double", "void", createFunctionDelegate<void, int&, double>(sub_comp));
		fb.registPredefinedOperators("*=", "int&,double", "void", createFunctionDelegate<void, int&, double>(mul_comp));
		fb.registPredefinedOperators("/=", "int&,double", "void", createFunctionDelegate<void, int&, double>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "int,double", "bool", createFunctionDelegate<bool, int, double>(operators::less));
		fb.registPredefinedOperators("<=", "int,double", "bool", createFunctionDelegate<bool, int, double>(operators::less_or_equal));
		fb.registPredefinedOperators(">", "int,double", "bool", createFunctionDelegate<bool, int, double>(operators::great));
		fb.registPredefinedOperators(">=", "int,double", "bool", createFunctionDelegate<bool, int, double>(operators::great_or_equal));
		fb.registPredefinedOperators("==", "int,double", "bool", createFunctionDelegate<bool, int, double>(operators::equal));
		fb.registPredefinedOperators("!=", "int,double", "bool", createFunctionDelegate<bool, int, double>(operators::not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "int,double", "bool", createFunctionDelegate<bool, int, double>(logic_and));
		fb.registPredefinedOperators("||", "int,double", "bool", createFunctionDelegate<bool, int, double>(logic_or));
#pragma endregion

#pragma region integer and bool operators
		//logic operators
		fb.registPredefinedOperators("&&", "int,bool", "bool", createFunctionDelegate<bool, int, bool>(logic_and));
		fb.registPredefinedOperators("||", "int,bool", "bool", createFunctionDelegate<bool, int, bool>(logic_or));
#pragma endregion

#pragma region long and integer operators
		fb.registPredefinedOperators("+", "long,int", "long", createFunctionDelegate<long long, long long, int>(add));
		fb.registPredefinedOperators("-", "long,int", "long", createFunctionDelegate<long long, long long, int>(sub));
		fb.registPredefinedOperators("*", "long,int", "long", createFunctionDelegate<long long, long long, int>(mul));
		fb.registPredefinedOperators("/", "long,int", "long", createFunctionDelegate<long long, long long, int>(div));
		fb.registPredefinedOperators("%", "long,int", "long", createFunctionDelegate<long long, long long, int>(mod));
		//assigment operator
		fb.registPredefinedOperators("=", "long&,int", "long", createFunctionDelegate(assign<long long, long long, int>));
		//bitwises
		fb.registPredefinedOperators("&", "long,int", "long", createFunctionDelegate<long long, long long, int>(bitwise_and));
		fb.registPredefinedOperators("|", "long,int", "long", createFunctionDelegate<long long, long long, int>(bitwise_or ));
		fb.registPredefinedOperators("^", "long,int", "long", createFunctionDelegate<long long, long long, int>(bitwise_xor));
		fb.registPredefinedOperators("<<", "long,int", "long", createFunctionDelegate<long long, long long, int>(bitwise_shiftLeft));
		fb.registPredefinedOperators(">>", "long,int", "long", createFunctionDelegate<long long, long long, int>(bitwise_shiftRight));
		//compound operators
		fb.registPredefinedOperators("+=", "long&,int", "void", createFunctionDelegate<void, long long&, int>(add_comp));
		fb.registPredefinedOperators("-=", "long&,int", "void", createFunctionDelegate<void, long long&, int>(sub_comp));
		fb.registPredefinedOperators("*=", "long&,int", "void", createFunctionDelegate<void, long long&, int>(mul_comp));
		fb.registPredefinedOperators("/=", "long&,int", "void", createFunctionDelegate<void, long long&, int>(div_comp));
		fb.registPredefinedOperators("%=", "long&,int", "void", createFunctionDelegate<void, long long&, int>(mod_comp));
		fb.registPredefinedOperators("&=", "long&,int", "void", createFunctionDelegate<void, long long&, int>(and_comp));
		fb.registPredefinedOperators("|=", "long&,int", "void", createFunctionDelegate<void, long long&, int>(or_comp));
		fb.registPredefinedOperators("^=", "long&,int", "void", createFunctionDelegate<void, long long&, int>(xor_comp));
		fb.registPredefinedOperators("<<=", "long&,int", "void", createFunctionDelegate<void, long long&, int>(shiftLeft_comp));
		fb.registPredefinedOperators(">>=", "long&,int", "void", createFunctionDelegate<void, long long&, int>(shiftRight_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "long,int", "bool", createFunctionDelegate<bool, long long, int>(operators::less));
		fb.registPredefinedOperators("<=", "long,int", "bool", createFunctionDelegate<bool, long long, int>(less_or_equal));
		fb.registPredefinedOperators(">", "long,int", "bool", createFunctionDelegate<bool, long long, int>(great));
		fb.registPredefinedOperators(">=", "long,int", "bool", createFunctionDelegate<bool, long long, int>(great_or_equal));
		fb.registPredefinedOperators("==", "long,int", "bool", createFunctionDelegate<bool, long long, int>(equal));
		fb.registPredefinedOperators("!=", "long,int", "bool", createFunctionDelegate<bool, long long, int>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "long,int", "bool", createFunctionDelegate<bool, long long, int>(logic_and));
		fb.registPredefinedOperators("||", "long,int", "bool", createFunctionDelegate<bool, long long, int>(logic_or));
#pragma endregion

#pragma region long and float operators
		fb.registPredefinedOperators("+", "long,float", "float", createFunctionDelegate<float, long long, float>(add));
		fb.registPredefinedOperators("-", "long,float", "float", createFunctionDelegate<float, long long, float>(sub));
		fb.registPredefinedOperators("*", "long,float", "float", createFunctionDelegate<float, long long, float>(mul));
		fb.registPredefinedOperators("/", "long,float", "float", createFunctionDelegate<float, long long, float>(div));
		//assigment operator
		fb.registPredefinedOperators("=", "long&,float", "long", createFunctionDelegate(assign<long long, long long, float>));
		//compound operators
		fb.registPredefinedOperators("+=", "long&,float", "void", createFunctionDelegate<void, long long&, float>(add_comp));
		fb.registPredefinedOperators("-=", "long&,float", "void", createFunctionDelegate<void, long long&, float>(sub_comp));
		fb.registPredefinedOperators("*=", "long&,float", "void", createFunctionDelegate<void, long long&, float>(mul_comp));
		fb.registPredefinedOperators("/=", "long&,float", "void", createFunctionDelegate<void, long long&, float>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "long,float", "bool", createFunctionDelegate<bool, long long, float>(operators::less));
		fb.registPredefinedOperators("<=", "long,float", "bool", createFunctionDelegate<bool, long long, float>(less_or_equal));
		fb.registPredefinedOperators(">", "long,float", "bool", createFunctionDelegate<bool, long long, float>(great));
		fb.registPredefinedOperators(">=", "long,float", "bool", createFunctionDelegate<bool, long long, float>(great_or_equal));
		fb.registPredefinedOperators("==", "long,float", "bool", createFunctionDelegate<bool, long long, float>(equal));
		fb.registPredefinedOperators("!=", "long,float", "bool", createFunctionDelegate<bool, long long, float>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "long,float", "bool", createFunctionDelegate<bool, long long, float>(logic_and));
		fb.registPredefinedOperators("||", "long,float", "bool", createFunctionDelegate<bool, long long, float>(logic_or));
#pragma endregion

#pragma region long and double operators
		fb.registPredefinedOperators("+", "long,double", "double", createFunctionDelegate<double, long long, double>(add));
		fb.registPredefinedOperators("-", "long,double", "double", createFunctionDelegate<double, long long, double>(sub));
		fb.registPredefinedOperators("*", "long,double", "double", createFunctionDelegate<double, long long, double>(mul));
		fb.registPredefinedOperators("/", "long,double", "double", createFunctionDelegate<double, long long, double>(div));
		//assigment operator
		fb.registPredefinedOperators("=", "long&,double", "double", createFunctionDelegate(assign<long long, long long, double>));
		//compound operators
		fb.registPredefinedOperators("+=", "long&,double", "void", createFunctionDelegate<void, long long&, double>(add_comp));
		fb.registPredefinedOperators("-=", "long&,double", "void", createFunctionDelegate<void, long long&, double>(sub_comp));
		fb.registPredefinedOperators("*=", "long&,double", "void", createFunctionDelegate<void, long long&, double>(mul_comp));
		fb.registPredefinedOperators("/=", "long&,double", "void", createFunctionDelegate<void, long long&, double>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "long,double", "bool", createFunctionDelegate<bool, long long, double>(operators::less));
		fb.registPredefinedOperators("<=", "long,double", "bool", createFunctionDelegate<bool, long long, double>(less_or_equal));
		fb.registPredefinedOperators(">", "long,double", "bool", createFunctionDelegate<bool, long long, double>(great));
		fb.registPredefinedOperators(">=", "long,double", "bool", createFunctionDelegate<bool, long long, double>(great_or_equal));
		fb.registPredefinedOperators("==", "long,double", "bool", createFunctionDelegate<bool, long long, double>(equal));
		fb.registPredefinedOperators("!=", "long,double", "bool", createFunctionDelegate<bool, long long, double>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "long,double", "bool", createFunctionDelegate<bool, long long, double>(logic_and));
		fb.registPredefinedOperators("||", "long,double", "bool", createFunctionDelegate<bool, long long, double>(logic_or));
#pragma endregion

#pragma region long and bool operators
		//logic operators
		fb.registPredefinedOperators("&&", "long,bool", "bool", createFunctionDelegate<bool, long long, bool>(logic_and));
		fb.registPredefinedOperators("||", "long,bool", "bool", createFunctionDelegate<bool, long long, bool>(logic_or));
#pragma endregion

#pragma region float and integer operators
		fb.registPredefinedOperators("+", "float,int", "float", createFunctionDelegate<float, float, int>(add));
		fb.registPredefinedOperators("-", "float,int", "float", createFunctionDelegate<float, float, int>(sub));
		fb.registPredefinedOperators("*", "float,int", "float", createFunctionDelegate<float, float, int>(mul));
		fb.registPredefinedOperators("/", "float,int", "float", createFunctionDelegate<float, float, int>(div));
		//assigment operator
		fb.registPredefinedOperators("=", "float&,int", "float", createFunctionDelegate(assign<float, float, int>));
		//compound operators
		fb.registPredefinedOperators("+=", "float&,int", "void", createFunctionDelegate<void, float&, int>(add_comp));
		fb.registPredefinedOperators("-=", "float&,int", "void", createFunctionDelegate<void, float&, int>(sub_comp));
		fb.registPredefinedOperators("*=", "float&,int", "void", createFunctionDelegate<void, float&, int>(mul_comp));
		fb.registPredefinedOperators("/=", "float&,int", "void", createFunctionDelegate<void, float&, int>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "float,int", "bool", createFunctionDelegate<bool, float, int>(operators::less));
		fb.registPredefinedOperators("<=", "float,int", "bool", createFunctionDelegate<bool, float, int>(less_or_equal));
		fb.registPredefinedOperators(">", "float,int", "bool", createFunctionDelegate<bool, float, int>(great));
		fb.registPredefinedOperators(">=", "float,int", "bool", createFunctionDelegate<bool, float, int>(great_or_equal));
		fb.registPredefinedOperators("==", "float,int", "bool", createFunctionDelegate<bool, float, int>(equal));
		fb.registPredefinedOperators("!=", "float,int", "bool", createFunctionDelegate<bool, float, int>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "float,int", "bool", createFunctionDelegate<bool, float, int>(logic_and));
		fb.registPredefinedOperators("||", "float,int", "bool", createFunctionDelegate<bool, float, int>(logic_or));
#pragma endregion

#pragma region float and long operators
		fb.registPredefinedOperators("+", "float,long", "float", createFunctionDelegate<float, float, long long>(add));
		fb.registPredefinedOperators("-", "float,long", "float", createFunctionDelegate<float, float, long long>(sub));
		fb.registPredefinedOperators("*", "float,long", "float", createFunctionDelegate<float, float, long long>(mul));
		fb.registPredefinedOperators("/", "float,long", "float", createFunctionDelegate<float, float, long long>(div));
		//assigment operator
		fb.registPredefinedOperators("=", "float&,long", "float", createFunctionDelegate(assign<float, float, long long>));
		//compound operators
		fb.registPredefinedOperators("+=", "float&,long", "void", createFunctionDelegate<void, float&, long long>(add_comp));
		fb.registPredefinedOperators("-=", "float&,long", "void", createFunctionDelegate<void, float&, long long>(sub_comp));
		fb.registPredefinedOperators("*=", "float&,long", "void", createFunctionDelegate<void, float&, long long>(mul_comp));
		fb.registPredefinedOperators("/=", "float&,long", "void", createFunctionDelegate<void, float&, long long>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "float,long", "bool", createFunctionDelegate<bool, float, long long>(operators::less));
		fb.registPredefinedOperators("<=", "float,long", "bool", createFunctionDelegate<bool, float, long long>(less_or_equal));
		fb.registPredefinedOperators(">", "float,long", "bool", createFunctionDelegate<bool, float, long long>(great));
		fb.registPredefinedOperators(">=", "float,long", "bool", createFunctionDelegate<bool, float, long long>(great_or_equal));
		fb.registPredefinedOperators("==", "float,long", "bool", createFunctionDelegate<bool, float, long long>(equal));
		fb.registPredefinedOperators("!=", "float,long", "bool", createFunctionDelegate<bool, float, long long>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "float,long", "bool", createFunctionDelegate<bool, float, long long>(logic_and));
		fb.registPredefinedOperators("||", "float,long", "bool", createFunctionDelegate<bool, float, long long>(logic_or));
#pragma endregion

#pragma region float and double operators
		fb.registPredefinedOperators("+", "float,double", "double", createFunctionDelegate<double, float, double>(add));
		fb.registPredefinedOperators("-", "float,double", "double", createFunctionDelegate<double, float, double>(sub));
		fb.registPredefinedOperators("*", "float,double", "double", createFunctionDelegate<double, float, double>(mul));
		fb.registPredefinedOperators("/", "float,double", "double", createFunctionDelegate<double, float, double>(div));
		//assigment operator
		fb.registPredefinedOperators("=", "float&,double", "float", createFunctionDelegate(assign<float, float, double>));
		//compound operators
		fb.registPredefinedOperators("+=", "float&,double", "void", createFunctionDelegate<void, float&, double>(add_comp));
		fb.registPredefinedOperators("-=", "float&,double", "void", createFunctionDelegate<void, float&, double>(sub_comp));
		fb.registPredefinedOperators("*=", "float&,double", "void", createFunctionDelegate<void, float&, double>(mul_comp));
		fb.registPredefinedOperators("/=", "float&,double", "void", createFunctionDelegate<void, float&, double>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "float,double", "bool", createFunctionDelegate<bool, float, double>(operators::less));
		fb.registPredefinedOperators("<=", "float,double", "bool", createFunctionDelegate<bool, float, double>(less_or_equal));
		fb.registPredefinedOperators(">", "float,double", "bool", createFunctionDelegate<bool, float, double>(great));
		fb.registPredefinedOperators(">=", "float,double", "bool", createFunctionDelegate<bool, float, double>(great_or_equal));
		fb.registPredefinedOperators("==", "float,double", "bool", createFunctionDelegate<bool, float, double>(equal));
		fb.registPredefinedOperators("!=", "float,double", "bool", createFunctionDelegate<bool, float, double>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "float,double", "bool", createFunctionDelegate<bool, float, double>(logic_and));
		fb.registPredefinedOperators("||", "float,double", "bool", createFunctionDelegate<bool, float, double>(logic_or));
#pragma endregion

#pragma region float and bool operators
		//logic operators
		fb.registPredefinedOperators("&&", "float,bool", "bool", createFunctionDelegate<bool, float, bool>(logic_and));
		fb.registPredefinedOperators("||", "float,bool", "bool", createFunctionDelegate<bool, float, bool>(logic_or));
#pragma endregion

#pragma region double and integer operators
		fb.registPredefinedOperators("+", "double,int", "double", createFunctionDelegate<double, double, int>(add));
		fb.registPredefinedOperators("-", "double,int", "double", createFunctionDelegate<double, double, int>(sub));
		fb.registPredefinedOperators("*", "double,int", "double", createFunctionDelegate<double, double, int>(mul));
		fb.registPredefinedOperators("/", "double,int", "double", createFunctionDelegate<double, double, int>(div));
		//assigment operator
		fb.registPredefinedOperators("=", "double&,int", "double", createFunctionDelegate(assign<double, double, int>));
		//compound operators
		fb.registPredefinedOperators("+=", "double&,int", "void", createFunctionDelegate<void, double&, int>(add_comp));
		fb.registPredefinedOperators("-=", "double&,int", "void", createFunctionDelegate<void, double&, int>(sub_comp));
		fb.registPredefinedOperators("*=", "double&,int", "void", createFunctionDelegate<void, double&, int>(mul_comp));
		fb.registPredefinedOperators("/=", "double&,int", "void", createFunctionDelegate<void, double&, int>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "double,int", "bool", createFunctionDelegate<bool, double, int>(operators::less));
		fb.registPredefinedOperators("<=", "double,int", "bool", createFunctionDelegate<bool, double, int>(less_or_equal));
		fb.registPredefinedOperators(">", "double,int", "bool", createFunctionDelegate<bool, double, int>(great));
		fb.registPredefinedOperators(">=", "double,int", "bool", createFunctionDelegate<bool, double, int>(great_or_equal));
		fb.registPredefinedOperators("==", "double,int", "bool", createFunctionDelegate<bool, double, int>(equal));
		fb.registPredefinedOperators("!=", "double,int", "bool", createFunctionDelegate<bool, double, int>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "double,int", "bool", createFunctionDelegate<bool, double, int>(logic_and));
		fb.registPredefinedOperators("||", "double,int", "bool", createFunctionDelegate<bool, double, int>(logic_or));
#pragma endregion

#pragma region double and long operators
		fb.registPredefinedOperators("+", "double,long", "double", createFunctionDelegate<double, double, long long>(add));
		fb.registPredefinedOperators("-", "double,long", "double", createFunctionDelegate<double, double, long long>(sub));
		fb.registPredefinedOperators("*", "double,long", "double", createFunctionDelegate<double, double, long long>(mul));
		fb.registPredefinedOperators("/", "double,long", "double", createFunctionDelegate<double, double, long long>(div));
		//assigment operator
		fb.registPredefinedOperators("=", "double&,long", "double", createFunctionDelegate(assign<double, double, long long>));
		//compound operators
		fb.registPredefinedOperators("+=", "double&,long", "void", createFunctionDelegate<void, double&, long long>(add_comp));
		fb.registPredefinedOperators("-=", "double&,long", "void", createFunctionDelegate<void, double&, long long>(sub_comp));
		fb.registPredefinedOperators("*=", "double&,long", "void", createFunctionDelegate<void, double&, long long>(mul_comp));
		fb.registPredefinedOperators("/=", "double&,long", "void", createFunctionDelegate<void, double&, long long>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "double,long", "bool", createFunctionDelegate<bool, double, long long>(operators::less));
		fb.registPredefinedOperators("<=", "double,long", "bool", createFunctionDelegate<bool, double, long long>(less_or_equal));
		fb.registPredefinedOperators(">", "double,long", "bool", createFunctionDelegate<bool, double, long long>(great));
		fb.registPredefinedOperators(">=", "double,long", "bool", createFunctionDelegate<bool, double, long long>(great_or_equal));
		fb.registPredefinedOperators("==", "double,long", "bool", createFunctionDelegate<bool, double, long long>(equal));
		fb.registPredefinedOperators("!=", "double,long", "bool", createFunctionDelegate<bool, double, long long>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "double,long", "bool", createFunctionDelegate<bool, double, long long>(logic_and));
		fb.registPredefinedOperators("||", "double,long", "bool", createFunctionDelegate<bool, double, long long>(logic_or));
#pragma endregion

#pragma region double and float operators
		fb.registPredefinedOperators("+", "double,float", "double", createFunctionDelegate<double, double, float>(add));
		fb.registPredefinedOperators("-", "double,float", "double", createFunctionDelegate<double, double, float>(sub));
		fb.registPredefinedOperators("*", "double,float", "double", createFunctionDelegate<double, double, float>(mul));
		fb.registPredefinedOperators("/", "double,float", "double", createFunctionDelegate<double, double, float>(div));
		//assigment operator
		fb.registPredefinedOperators("=", "double&,float", "float", createFunctionDelegate(assign<double, double, float>));
		//compound operators
		fb.registPredefinedOperators("+=", "double&,float", "void", createFunctionDelegate<void, double&, float>(add_comp));
		fb.registPredefinedOperators("-=", "double&,float", "void", createFunctionDelegate<void, double&, float>(sub_comp));
		fb.registPredefinedOperators("*=", "double&,float", "void", createFunctionDelegate<void, double&, float>(mul_comp));
		fb.registPredefinedOperators("/=", "double&,float", "void", createFunctionDelegate<void, double&, float>(div_comp));
		//comparision operators
		fb.registPredefinedOperators("<", "double,float", "bool", createFunctionDelegate<bool, double, float>(operators::less));
		fb.registPredefinedOperators("<=", "double,float", "bool", createFunctionDelegate<bool, double, float>(less_or_equal));
		fb.registPredefinedOperators(">", "double,float", "bool", createFunctionDelegate<bool, double, float>(great));
		fb.registPredefinedOperators(">=", "double,float", "bool", createFunctionDelegate<bool, double, float>(great_or_equal));
		fb.registPredefinedOperators("==", "double,float", "bool", createFunctionDelegate<bool, double, float>(equal));
		fb.registPredefinedOperators("!=", "double,float", "bool", createFunctionDelegate<bool, double, float>(not_equal));
		//logic operators
		fb.registPredefinedOperators("&&", "double,float", "bool", createFunctionDelegate<bool, double, float>(logic_and));
		fb.registPredefinedOperators("||", "double,float", "bool", createFunctionDelegate<bool, double, float>(logic_or));
#pragma endregion

#pragma region bool and integer operators
		//logic operators
		fb.registPredefinedOperators("&&", "bool,int", "bool", createFunctionDelegate<bool, bool, int>(logic_and));
		fb.registPredefinedOperators("||", "bool,int", "bool", createFunctionDelegate<bool, bool, int>(logic_or));
#pragma endregion

#pragma region bool and long operators
		//logic operators
		fb.registPredefinedOperators("&&", "bool,long", "bool", createFunctionDelegate<bool, bool, long long>(logic_and));
		fb.registPredefinedOperators("||", "bool,long", "bool", createFunctionDelegate<bool, bool, long long>(logic_or));
#pragma endregion

#pragma region bool and float operators
		//logic operators
		fb.registPredefinedOperators("&&", "bool,float", "bool", createFunctionDelegate<bool, bool, float>(logic_and));
		fb.registPredefinedOperators("||", "bool,float", "bool", createFunctionDelegate<bool, bool, float>(logic_or));
#pragma endregion

#pragma region bool and double operators
		//logic operators
		fb.registPredefinedOperators("&&", "bool,double", "bool", createFunctionDelegate<bool, bool, double>(logic_and));
		fb.registPredefinedOperators("||", "bool,double", "bool", createFunctionDelegate<bool, bool, double>(logic_or));
#pragma endregion

#pragma region other operators
		auto scriptCompiler = fb.getSriptCompiler();
		fb.registFunction("length", "string&", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "int", createFunctionDelegate<int, const std::string&>(stringLength), scriptCompiler), true);
		importCoreFunctions(fb);
#pragma endregion
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

		fb.registFunction("=", "ref void,ref void,int,int ", new BasicFunctionFactory<4>(EXP_UNIT_ID_OPERATOR_ASSIGNMENT, FUNCTION_PRIORITY_ASSIGNMENT, "ref void", createFunctionDelegate<void*, void*, void*, int, int>(assignStruct), scriptCompiler), true);

		auto theNativeFunction1 = createFunctionDelegate<SimpleVariantArray*, SimpleVariantArray*>(dynamicFunctionGateway);
		auto dynamicFunctionFactory1 = new DynamicFunctionFactory("ref " SYSTEM_ARRAY_STRUCT, theNativeFunction1, scriptCompiler);
		scriptCompiler->registDynamicFunction(SYSTEM_ARRAY_FUNCTION, dynamicFunctionFactory1);
		fb.addFactory(dynamicFunctionFactory1);

		//fb.registFunction("=", "ref void,ref " SYSTEM_ARRAY_STRUCT ",int,int", new BasicFunctionFactory<4>(EXP_UNIT_ID_OPERATOR_ASSIGNMENT, FUNCTION_PRIORITY_ASSIGNMENT, "ref void", createFunctionDelegate<void*, void*, SimpleVariantArray*, int, int>(assignArray2Struct1), scriptCompiler), true);

		//register default member access opearator with param void type as return
		//return type and first parameter will be corrected later(when link)
		//fb.registFunction(".", "ref void, int", new BasicFunctionFactory<2>(EXP_UNIT_ID_MEMBER_ACCESS, FUNCTION_PRIORITY_MEMBER_ACCESS, "ref void", createFunctionDelegate<void*, char*, int>(memberAccess), scriptCompiler), true);

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
		fb.registFunction(DEFAULT_DUMMY_OPERATOR, "ref void", new BasicFunctionFactory<4>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", createFunctionDelegate<void, void*>(dummyOperator), scriptCompiler), true);

		//register default constructor/destructor for function objects
		int f1 = fb.registFunction(SYSTEM_FUNCTION_CONSTRUCTOR, "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", createFunctionDelegate<void, RuntimeFunctionInfo*>(defaultRuntimeFunctionInfoConstructor), scriptCompiler), true);
		int f2 = fb.registFunction(SYSTEM_FUNCTION_DESTRUCTOR, "ref void", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", createFunctionDelegate<void, RuntimeFunctionInfo*>(runtimeFunctionInfoDestructor), scriptCompiler), true);
		//int f3 = fb.registFunction(SYSTEM_FUNCTION_COPY_CONSTRUCTOR, "ref void, ref void", new BasicFunctionFactory<2>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", createFunctionDelegate<void, RuntimeFunctionInfo*>(runtimeFunctionInfoCopyConstructor), scriptCompiler), true);

		scriptCompiler->setRuntimeInfoConstructor(f1);
		scriptCompiler->setRuntimeInfoDestructor(f2);
		//scriptCompiler->setRuntimeInfoCopyConstructor(f3);
#ifndef FFSCRIPT_EXCLUDE_THREAD
		fb.registFunction("createThread", "function<void()>&", new BasicFunctionFactory<1>(EXP_UNIT_ID_CREATE_THREAD, FUNCTION_PRIORITY_USER_FUNCTION, "hthread", new CreateThreadCommand(), scriptCompiler), true);
		fb.registFunction("joinThread", "hthread", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", createFunctionDelegate<void, THREAD_HANDLE>(joinThread), scriptCompiler), true);
		fb.registFunction("closeThread", "hthread", new BasicFunctionFactory<1>(EXP_UNIT_ID_USER_FUNC, FUNCTION_PRIORITY_USER_FUNCTION, "void", createFunctionDelegate<void, THREAD_HANDLE>(closeThread), scriptCompiler), true);
#endif //FFSCRIPT_EXCLUDE_THREAD
#pragma endregion
	}
}
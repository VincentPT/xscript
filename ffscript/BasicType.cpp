/******************************************************************
* File:        BasicType.cpp
* Description: implement BasicTypes class to register basic types,
*              register casting function between types of the
*              scripting language.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "BasicType.h"
#include "FunctionRegisterHelper.h"
#include "StructClass.h"
#include "Internal.h"
#include "ScriptCompiler.h"
#include "BasicOperators.hpp"
#include "ScriptType.h"
#include "function/FunctionDelegate.hpp"
#include "Utils.h"

namespace ffscript {
	static BasicTypes* s_basicType = nullptr;

	BasicTypes* BasicTypes::getBasicTypes() {
		return s_basicType;
	}

	BasicTypes::BasicTypes() :
		TYPE_VOID(DATA_TYPE_UNKNOWN),
		TYPE_INT(DATA_TYPE_UNKNOWN),
		TYPE_LONG(DATA_TYPE_UNKNOWN),
		TYPE_FLOAT(DATA_TYPE_UNKNOWN),
		TYPE_DOUBLE(DATA_TYPE_UNKNOWN),
		TYPE_BOOL(DATA_TYPE_UNKNOWN),
		TYPE_STRING(DATA_TYPE_UNKNOWN),
		TYPE_WSTRING(DATA_TYPE_UNKNOWN),
		TYPE_VARIANT(DATA_TYPE_UNKNOWN),
		TYPE_VARIANTARRAY(DATA_TYPE_UNKNOWN),
		TYPE_SYSTEMARRAY(DATA_TYPE_UNKNOWN),
		TYPE_ELEMENT_INFO(DATA_TYPE_UNKNOWN)
	{
		s_basicType = this;
	}

	BasicTypes::~BasicTypes(){
		s_basicType = nullptr;
	}

	void BasicTypes::registerBasicTypes(ScriptCompiler* scriptCompiler) {
		TYPE_VOID	= scriptCompiler->registType("void");
		TYPE_INT = scriptCompiler->registType("int");
		TYPE_CHAR = scriptCompiler->registType("char");
		TYPE_WCHAR = scriptCompiler->registType("wchar");
		TYPE_LONG	= scriptCompiler->registType("long");
		TYPE_FLOAT	= scriptCompiler->registType("float");
		TYPE_DOUBLE = scriptCompiler->registType("double");
		TYPE_BOOL	= scriptCompiler->registType("bool");
		TYPE_STRING = scriptCompiler->registType("string");
		TYPE_WSTRING = scriptCompiler->registType("wstring");
		TYPE_SYSTEMARRAY = scriptCompiler->registType("array");
		TYPE_NULL = scriptCompiler->registType(SYSTEM_NULL_TYPE);
		int theadType = scriptCompiler->registType("hthread");

		ScriptType typeVoid(TYPE_VOID, "void");
		ScriptType typeInt(TYPE_INT, "int");
		ScriptType typeLong(TYPE_LONG, "long");
		ScriptType typeFloat(TYPE_FLOAT, "float");
		ScriptType typeDouble(TYPE_DOUBLE, "double");
		ScriptType typeBool(TYPE_BOOL, "bool");

		scriptCompiler->setTypeSize(TYPE_VOID, 0);
		scriptCompiler->setTypeSize(TYPE_INT, 4);
		scriptCompiler->setTypeSize(TYPE_CHAR, sizeof(char));
		scriptCompiler->setTypeSize(TYPE_WCHAR, sizeof(wchar_t));
		scriptCompiler->setTypeSize(TYPE_LONG, 8);
		scriptCompiler->setTypeSize(TYPE_FLOAT, 4);
		scriptCompiler->setTypeSize(TYPE_DOUBLE, 8);
		scriptCompiler->setTypeSize(TYPE_BOOL, 1);
		scriptCompiler->setTypeSize(TYPE_STRING, sizeof(void*));
		scriptCompiler->setTypeSize(TYPE_WSTRING, sizeof(void*));
		scriptCompiler->setTypeSize(TYPE_SYSTEMARRAY, sizeof(DynamicArray));
		scriptCompiler->setTypeSize(TYPE_NULL, sizeof(void*));
		scriptCompiler->setTypeSize(theadType, sizeof(void*));

		//register simple variant array struct
		StructClass* elemStruct = new StructClass(scriptCompiler, "_SimpleVariant");
		elemStruct->addMember(typeInt, "type");
		elemStruct->addMember(typeInt, "nType");
		elemStruct->addMember(typeInt, "size");
		elemStruct->addMember(typeVoid.makeRef(), "pData");
		TYPE_VARIANT = scriptCompiler->registStruct(elemStruct);
		ScriptType typeVariant(TYPE_VARIANT, elemStruct->getName());

		StructClass* arrayStruct = new StructClass(scriptCompiler, SYSTEM_ARRAY_STRUCT);
		arrayStruct->addMember(typeInt, "size");
		arrayStruct->addMember(typeVariant, "elems");
		TYPE_VARIANTARRAY = scriptCompiler->registStruct(arrayStruct);

		StructClass* elementInfoStruct = new StructClass(scriptCompiler, "ElementInfo");
		arrayStruct->addMember(typeInt, "type");
		arrayStruct->addMember(typeInt, "offset");
		TYPE_ELEMENT_INFO = scriptCompiler->registStruct(elementInfoStruct);
	}

	void BasicTypes::registerBasicTypeCastFunctions(ScriptCompiler* scriptCompiler, FunctionRegisterHelper& fb) {
		ScriptType typeVoid(TYPE_VOID, "void");
		ScriptType typeInt(TYPE_INT, "int");
		ScriptType typeLong(TYPE_LONG, "long");
		ScriptType typeFloat(TYPE_FLOAT, "float");
		ScriptType typeDouble(TYPE_DOUBLE, "double");
		ScriptType typeBool(TYPE_BOOL, "bool");

		fb.registFunction("int", "long", new ConversionFactory<int, long long>(scriptCompiler, typeInt) , true);
		fb.registFunction("int", "float", new ConversionFactory<int, float>(scriptCompiler, typeInt), true);
		fb.registFunction("int", "double", new ConversionFactory<int, double>(scriptCompiler, typeInt), true);
		fb.registFunction("int", "bool", new ConversionFactoryBoolTo<int>(scriptCompiler, typeInt), true);

		fb.registFunction("long", "int", new ConversionFactory<long long, int>(scriptCompiler, typeLong), true);
		fb.registFunction("long", "double", new ConversionFactory<long long, double&>(scriptCompiler, typeLong), true);
		fb.registFunction("long", "float", new ConversionFactory<long long, float&>(scriptCompiler, typeLong), true);
		fb.registFunction("long", "bool", new ConversionFactoryBoolTo<long long>(scriptCompiler, typeLong), true);

		fb.registFunction("float", "int", new ConversionFactory<float, int>(scriptCompiler, typeFloat), true);
		fb.registFunction("float", "long", new ConversionFactory<float, long long>(scriptCompiler, typeFloat), true);
		fb.registFunction("float", "double", new ConversionFactory<float,double>(scriptCompiler, typeFloat), true);
		fb.registFunction("float", "bool", new ConversionFactoryBoolTo<float>(scriptCompiler, typeFloat), true);

		fb.registFunction("double", "int", new ConversionFactory<double, int>(scriptCompiler, typeDouble), true);
		fb.registFunction("double", "long", new ConversionFactory<double, long long>(scriptCompiler, typeDouble), true);
		fb.registFunction("double", "float", new ConversionFactory<double, float>(scriptCompiler, typeDouble), true);
		fb.registFunction("double", "bool", new ConversionFactoryBoolTo<double>(scriptCompiler, typeDouble), true);

		fb.registFunction("bool", "int", new ConversionFactoryToBool<int>(scriptCompiler, typeBool), true);
		fb.registFunction("bool", "long", new ConversionFactoryToBool<long long>(scriptCompiler, typeBool), true);
		fb.registFunction("bool", "float", new ConversionFactoryToBool<float>(scriptCompiler, typeBool), true);
		fb.registFunction("bool", "double", new ConversionFactoryToBool<double>(scriptCompiler, typeBool), true);
		
		// conversion accurative for each conversion operator
		//float -> double, int -> long : 500
		//int -> double, float -> long : 550
		//long -> double, int -> float : 600
		//double -> long, float -> int : 650		
		//double -> float, long -> int : 700
		//double -> int, long -> float : 750
		//int -> bool : 800
		//long -> bool : 850
		//float -> bool : 900
		//double -> bool : 950
		//bool -> int : 1000
		//bool -> long : 1050
		//bool -> float : 1100
		//bool -> double : 1150

		// conversion accurative help engine choose the best overloading function for given arguments
		scriptCompiler->registerTypeConversionAccurative(TYPE_FLOAT, TYPE_DOUBLE, 500);
		scriptCompiler->registerTypeConversionAccurative(TYPE_INT, TYPE_LONG, 500);

		scriptCompiler->registerTypeConversionAccurative(TYPE_INT, TYPE_DOUBLE, 550);
		scriptCompiler->registerTypeConversionAccurative(TYPE_FLOAT, TYPE_LONG, 550);

		scriptCompiler->registerTypeConversionAccurative(TYPE_LONG, TYPE_DOUBLE, 600);
		scriptCompiler->registerTypeConversionAccurative(TYPE_INT, TYPE_FLOAT, 600);

		scriptCompiler->registerTypeConversionAccurative(TYPE_DOUBLE, TYPE_LONG, 650);
		scriptCompiler->registerTypeConversionAccurative(TYPE_FLOAT, TYPE_INT, 650);

		scriptCompiler->registerTypeConversionAccurative(TYPE_DOUBLE, TYPE_FLOAT, 700);
		scriptCompiler->registerTypeConversionAccurative(TYPE_LONG, TYPE_INT, 700);

		scriptCompiler->registerTypeConversionAccurative(TYPE_DOUBLE, TYPE_INT, 750);
		scriptCompiler->registerTypeConversionAccurative(TYPE_LONG, TYPE_FLOAT, 750);

		scriptCompiler->registerTypeConversionAccurative(TYPE_INT, TYPE_BOOL, 800);
		scriptCompiler->registerTypeConversionAccurative(TYPE_LONG, TYPE_BOOL, 850);
		scriptCompiler->registerTypeConversionAccurative(TYPE_FLOAT, TYPE_BOOL, 900);
		scriptCompiler->registerTypeConversionAccurative(TYPE_DOUBLE, TYPE_BOOL, 950);

		scriptCompiler->registerTypeConversionAccurative(TYPE_BOOL, TYPE_INT, 1000);
		scriptCompiler->registerTypeConversionAccurative(TYPE_BOOL, TYPE_LONG, 1050);		
		scriptCompiler->registerTypeConversionAccurative(TYPE_BOOL, TYPE_FLOAT, 1100);		
		scriptCompiler->registerTypeConversionAccurative(TYPE_BOOL, TYPE_DOUBLE, 1150);
	}

	ConstOperandBase* createBoolConsant(bool cosnt_val) {
		return new CConstOperand<bool>(cosnt_val, "bool");
	}

	ConstOperandBase* createNullConsant() {
		return new CConstOperand<size_t>(0, SYSTEM_NULL_TYPE);
	}

	void BasicTypes::registerConstants(ScriptCompiler* scriptCompiler) const {

		auto falseFunctionObj = std::make_shared<FunctionDelegate<ConstOperandBase*, bool>>(createBoolConsant);
		auto trueFunctionObj = std::make_shared<FunctionDelegate<ConstOperandBase*, bool>>(createBoolConsant);

		falseFunctionObj->pushParam((void*)false);
		trueFunctionObj->pushParam((void*)true);

		scriptCompiler->setConstantMap("false", falseFunctionObj);
		scriptCompiler->setConstantMap("true", trueFunctionObj);

		auto nullFunctionObj = std::make_shared<FunctionDelegate<ConstOperandBase*>>(createNullConsant);
		scriptCompiler->setConstantMap("nullptr", nullFunctionObj);
	}

	/////////////////////////////////////////////////////////////////////////////////////
	CastingFunction::CastingFunction(const std::string& name) :
		FixParamFunction<1>(name, EXP_UNIT_ID_OPERATOR_CAST, FUNCTION_PRIORITY_USER_FUNCTION, name) {

	}
	CastingFunction::~CastingFunction() {}

	/////////////////////////////////////////////////////////////////////////////////////
	//CastingFunctionR::CastingFunctionR(FunctionRef refFunction, const std::string& name) :
	//	FixParamFunction<1>(name, EXP_UNIT_ID_OPERATOR_CAST, FUNCTION_PRIORITY_USER_FUNCTION, name) {
	//	_refFunction = refFunction;
	//}
	//CastingFunctionR::~CastingFunctionR() {}

	//int CastingFunctionR::pushParam(ExecutableUnitRef pExeUnit) {
	//	if (_refFunction == nullptr) {
	//		return -1;
	//	}
	//	int res = _refFunction->pushParam(pExeUnit);
	//	if (res == 0) {
	//		res = FixParamFunction<1>::pushParam(_refFunction);
	//	}
	//	return res;
	//}
}
/******************************************************************
* File:        FunctionRegisterHelper.h
* Description: declare FunctionRegisterHelper class. A class used
*              to help users register the function in a script
*              program.
*              Define some utility template functions used in
*              registering function.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"
#include <list>

#include "function/CdeclFunction3.hpp"
#include "function/MemberFunction3.hpp"

class DFunction2;
namespace ffscript {

	class FunctionFactory;
	class ScriptCompiler;
	class ConstOperandBase;

	struct PredefinedOperator {
		const char* name;
		const char* functionParams;
		const char* returnType;
		DFunction2* nativeFunction;
	};

	struct FactoryItem
	{
		char* functionName;
		char* functionParams;
		FunctionFactory* factory;
		bool autoDelete;
	};

	class FFSCRIPT_API FunctionRegisterHelper
	{
		ScriptCompiler* _scriptCompiler;
	public:
		FunctionRegisterHelper(ScriptCompiler* scriptCompiler);
		virtual ~FunctionRegisterHelper();

		int registFunction(const std::string& name, const std::string& functionParams, FunctionFactory* factory, bool autoDelete = true);
		int registDynamicFunction(const std::string& name, FunctionFactory* factory, bool autoDelete = true);
		int registPredefinedOperators(const std::string& name, const std::string& functionParams, const std::string& returnType, DFunction2*);		
		void addFactory(FunctionFactory* factory, bool autoDelete = true);
		int registFunctions(FactoryItem* factories, int n);
		int registPredefinedOperators(PredefinedOperator* operators, int n);
		ScriptCompiler* getSriptCompiler() const;
	};

	template <class Rt, class... Types>
	DFunction2* createFunctionCdecl(Rt(*f)(Types...)) {
		return new FunctionT<Rt, Types...>(f);
	}

	template <class Rt, class... Types>
	DFunction2Ref createFunctionCdeclRef(Rt(*f)(Types...)) {
		return std::make_shared<FunctionT<Rt, Types...>>(f);
	}

	template <class Class, class Rt, class... Types>
	DFunction2* createFunctionMember(Class* obj, Rt(Class::*f)(Types...)) {
		return new MFunctionT<Class, Rt, Types...>(obj, f);
	}

	template <class Class, class Rt, class... Types>
	DFunction2Ref createFunctionMemberRef(Class* obj, Rt(Class::*f)(Types...)) {
		return std::make_shared<MFunctionT<Class, Rt, Types...>>(obj, f);
	}

	template <class Class, class Rt, class... Types>
	DFunction2* createFunctionMember(Class* obj, Rt(Class::*f)(Types...) const) {
		return new MFunctionT<Class, Rt, Types...>(obj, f);
	}

	template <class Class, class Rt, class... Types>
	DFunction2Ref createFunctionMemberRef(Class* obj, Rt(Class::*f)(Types...) const) {
		return std::make_shared<MFunctionT<Class, Rt, Types...>>(obj, f);
	}

	template <class Rt, class... Types>
	FunctionFactory* createUserFunctionFactory(ScriptCompiler* scriptCompiler, const char* rt, Rt(*f)(Types...)) {
		return new DefaultUserFunctionFactory(createFunctionCdeclRef<Rt, Types...>(f), scriptCompiler, rt, sizeof...(Types));
	}

	template <class Class, class Rt, class... Types>
	FunctionFactory* createUserFunctionFactoryMember(ScriptCompiler* scriptCompiler, Class* obj, const char* rt, Rt(Class::*f)(Types...)) {
		return new DefaultUserFunctionFactory(createFunctionMemberRef<Class, Rt, Types...>(obj, f), scriptCompiler, rt, sizeof...(Types));
	}

	template <class Class, class Rt, class... Types>
	FunctionFactory* createUserFunctionFactoryMember(ScriptCompiler* scriptCompiler, Class* obj, const char* rt, Rt(Class::*f)(Types...) const) {
		return new DefaultUserFunctionFactory(createFunctionMemberRef<Class, Rt, Types...>(obj, f), scriptCompiler, rt, sizeof...(Types));
	}

	template <class ...Args>
	struct ArgumentFunctionCounter {
		static constexpr int count = sizeof...(Args);
	};

	template <class T>
	ConstOperandBase* createConsant(const T& cosnt_val, const char* typeStr) {
		return new CConstOperand<T>(cosnt_val, typeStr);
	}

	template<class RT, class ...Types>
	int registerFunction(FunctionRegisterHelper& fb, RT(*nativeFunction)(Types...), const char* scriptFunction, const char* returnType, const char* paramTypes) {
		return fb.registFunction(
			scriptFunction,
			paramTypes, // parameter type of the function
			createUserFunctionFactory
			<RT, Types...> // native function prototype
			(
				fb.getSriptCompiler(), // script compiler
				returnType, // return type of the script function
				nativeFunction // native function
				)
		);
	}
}
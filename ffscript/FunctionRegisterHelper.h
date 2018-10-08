/******************************************************************
* File:        FunctionRegisterHelper.h
* Description: declare FunctionRegisterHelper class. A class used
*              to help users register the function in a script
*              program.
*              Define some utility template functions used in
*              registering functions and types.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"
#include <list>

#include "function/CdeclFunction3.hpp"
#include "function/MemberFunction3.hpp"
#include "function/MemberFunction4.hpp"
#include "function/DynamicFunction.h"

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
		int registerTypeAutoOperator(int typeId, const std::string& functionParams, FunctionFactory* factory, bool autoDelete, bool constructor);
	public:
		FunctionRegisterHelper(ScriptCompiler* scriptCompiler);
		virtual ~FunctionRegisterHelper();

		int registFunction(const std::string& name, const std::string& functionParams, FunctionFactory* factory, bool autoDelete = true);
		int registDynamicFunction(const std::string& name, FunctionFactory* factory, bool autoDelete = true);
		int registPredefinedOperators(const std::string& name, const std::string& functionParams, const std::string& returnType, DFunction2*);		
		void addFactory(FunctionFactory* factory, bool autoDelete = true);
		int registFunctions(FactoryItem* factories, int n);
		int registPredefinedOperators(PredefinedOperator* operators, int n);
		int registerUserType(const std::string& type, unsigned int size);
		int registerConstructor(int typeId, const std::string& functionParams, FunctionFactory* factory, bool autoDelete = true);
		int registerDestructor(int typeId, FunctionFactory* factory, bool autoDelete = true);
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
	FunctionFactory* createUserFunctionFactory(ScriptCompiler* scriptCompiler, const std::string& rt, Rt(*f)(Types...)) {
		return new DefaultUserFunctionFactory(createFunctionCdeclRef<Rt, Types...>(f), scriptCompiler, rt, sizeof...(Types));
	}

	template <class Class, class Rt, class... Types>
	FunctionFactory* createUserFunctionFactoryMember(ScriptCompiler* scriptCompiler, Class* obj, const std::string& rt, Rt(Class::*f)(Types...)) {
		return new DefaultUserFunctionFactory(createFunctionMemberRef<Class, Rt, Types...>(obj, f), scriptCompiler, rt, sizeof...(Types));
	}

	template <class Class, class Rt, class... Types>
	FunctionFactory* createUserFunctionFactoryMember(ScriptCompiler* scriptCompiler, Class* obj, const std::string& rt, Rt(Class::*f)(Types...) const) {
		return new DefaultUserFunctionFactory(createFunctionMemberRef<Class, Rt, Types...>(obj, f), scriptCompiler, rt, sizeof...(Types));
	}

	template <class Class, class Rt, class... Types>
	FunctionFactory* createUserFunctionFactoryContext(ScriptCompiler* scriptCompiler, const std::string& rt, Rt(Class::*f)(Types...)) {
		return new DefaultUserFunctionFactory(std::make_shared<CtxFunctionT<Class, Rt, Types...>>(f) , scriptCompiler, rt, sizeof...(Types) + 1);
	}

	template <class Class, class Rt, class... Types>
	FunctionFactory* createUserFunctionFactoryContext(ScriptCompiler* scriptCompiler, const std::string& rt, Rt(Class::*f)(Types...) const) {
		return new DefaultUserFunctionFactory(std::make_shared<CtxFunctionT<Class, Rt, Types...>>(f), scriptCompiler, rt, sizeof...(Types) + 1);
	}

	template <class ...Args>
	struct ArgumentFunctionCounter {
		static constexpr int count = sizeof...(Args);
	};

	template<class Rt, class ...Types>
	int registerFunction(FunctionRegisterHelper& fb, Rt(*nativeFunction)(Types...), const std::string& scriptFunction, const std::string& returnType, const std::string& paramTypes) {
		return fb.registFunction(
			scriptFunction,
			paramTypes, // parameter type of the function
			createUserFunctionFactory
			<Rt, Types...> // native function prototype
			(
				fb.getSriptCompiler(), // script compiler
				returnType, // return type of the script function
				nativeFunction // native function
				)
		);
	}

	template <class Class, class Rt, class... Types>
	int registerFunction(FunctionRegisterHelper& fb, Class* obj,  Rt(Class::*nativeFunction)(Types...), const std::string& scriptFunction, const std::string& returnType, const std::string& paramTypes) {
		return fb.registFunction(
			scriptFunction,
			paramTypes, // parameter type of the function
			createUserFunctionFactoryMember
			<Class, Rt, Types...> // native function prototype
			(
				fb.getSriptCompiler(), // script compiler
				obj, // context object
				returnType, // return type of the script function
				nativeFunction // native function
				)
		);
	}

	template <class Class, class Rt, class... Types>
	int registerFunction(FunctionRegisterHelper& fb, Class* obj, Rt(Class::*nativeFunction)(Types...) const, const std::string& scriptFunction, const std::string& returnType, const std::string& paramTypes) {
		return fb.registFunction(
			scriptFunction,
			paramTypes, // parameter type of the function
			createUserFunctionFactoryMember
			<Class, Rt, Types...> // native function prototype
			(
				fb.getSriptCompiler(), // script compiler
				obj, // context object
				returnType, // return type of the script function
				nativeFunction // native function
				)
		);
	}

	template<class Rt, class ...Types>
	int registerOperator(FunctionRegisterHelper& fb, Rt(*nativeFunction)(Types...), const std::string& operatorName, const std::string& returnType, const std::string& paramTypes) {
		return fb.registPredefinedOperators(
			operatorName,
			paramTypes, // parameter type of the function
			returnType,
			createFunctionCdecl(nativeFunction)
		);
	}

	template <class Class, class Rt, class... Types>
	int registerOperator(FunctionRegisterHelper& fb, Class* obj, Rt(Class::*nativeFunction)(Types...), const std::string& scriptFunction, const std::string& returnType, const std::string& paramTypes) {
		return fb.registPredefinedOperators(
			scriptFunction,
			paramTypes, // parameter type of the function
			returnType,
			createFunctionMember(obj, nativeFunction)
		);
	}

	template <class Class, class Rt, class... Types>
	int registerOperator(FunctionRegisterHelper& fb, Class* obj, Rt(Class::*nativeFunction)(Types...) const, const std::string& scriptFunction, const std::string& returnType, const std::string& paramTypes) {
		return fb.registPredefinedOperators(
			scriptFunction,
			paramTypes, // parameter type of the function
			returnType,
			createFunctionMember(obj, nativeFunction)
		);
	}

	template<class Rt>
	int registerDynamicFunction(FunctionRegisterHelper& fb, Rt(*nativeFunction)(SimpleVariantArray*), const std::string& scriptFunction, const std::string& returnType) {
		auto functionObj = createFunctionCdecl(nativeFunction);
		auto functionUnitFactory = new DynamicFunctionFactory(returnType, functionObj, fb.getSriptCompiler());
		return fb.registDynamicFunction(scriptFunction, functionUnitFactory);
	}

	template<class Class, class Rt>
	int registerDynamicFunction(FunctionRegisterHelper& fb, Class* obj, Rt(Class::*nativeFunction)(SimpleVariantArray*), const std::string& scriptFunction, const std::string& returnType) {
		auto functionObj = createFunctionMember(obj, nativeFunction);
		auto functionUnitFactory = new DynamicFunctionFactory(returnType, functionObj, fb.getSriptCompiler());
		return fb.registDynamicFunction(scriptFunction, functionUnitFactory);
	}

	template<class Class, class Rt>
	int registerDynamicFunction(FunctionRegisterHelper& fb, Class* obj, Rt(Class::*nativeFunction)(SimpleVariantArray*) const, const std::string& scriptFunction, const std::string& returnType) {
		auto functionObj = createFunctionMember(obj, nativeFunction);
		auto functionUnitFactory = new DynamicFunctionFactory(returnType, functionObj, fb.getSriptCompiler());
		return fb.registDynamicFunction(scriptFunction, functionUnitFactory);
	}

	template<class ...Types>
	int registerContructor(FunctionRegisterHelper& fb, void(*nativeFunction)(Types...), int typeId, const std::string& paramTypes) {
		return fb.registerConstructor(
			typeId,
			paramTypes, // parameter type of the function
			createUserFunctionFactory
			<void, Types...> // native function prototype
			(
				fb.getSriptCompiler(), // script compiler
				"void", // return type of the script function
				nativeFunction // native function
				)
		);
	}

	template<class Class, class ...Types>
	int registerContructor(FunctionRegisterHelper& fb, Class* obj, void(Class::*nativeFunction)(Types...), int typeId, const std::string& paramTypes) {
		return fb.registerConstructor(
			typeId,
			paramTypes, // parameter type of the function
			createUserFunctionFactoryMember
			<Class, void, Types...> // native function prototype
			(
				fb.getSriptCompiler(), // script compiler
				obj,
				"void", // return type of the script function
				nativeFunction // native function
				)
		);
	}

	template<class Class, class ...Types>
	int registerContructor(FunctionRegisterHelper& fb, Class* obj, void(Class::*nativeFunction)(Types...) const, int typeId, const std::string& paramTypes) {
		return fb.registerConstructor(
			typeId,
			paramTypes, // parameter type of the function
			createUserFunctionFactoryMember
			<Class, void, Types...> // native function prototype
			(
				fb.getSriptCompiler(), // script compiler
				obj,
				"void", // return type of the script function
				nativeFunction // native function
				)
		);
	}

	template<class T>
	int registerDestructor(FunctionRegisterHelper& fb, void(*nativeFunction)(T), int typeId) {
		return fb.registerDestructor(
			typeId,
			createUserFunctionFactory
			<void, T> // native function prototype
			(
				fb.getSriptCompiler(), // script compiler
				"void", // return type of the script function
				nativeFunction // native function
				)
		);
	}

	template<class Class, class T>
	int registerDestructor(FunctionRegisterHelper& fb, Class* obj, void(Class::*nativeFunction)(T), int typeId) {
		return fb.registerDestructor(
			typeId,
			createUserFunctionFactoryMember
			<Class, void, T> // native function prototype
			(
				fb.getSriptCompiler(), // script compiler
				obj,
				"void", // return type of the script function
				nativeFunction // native function
				)
		);
	}

	template<class Class, class T>
	int registerDestructor(FunctionRegisterHelper& fb, Class* obj, void(Class::*nativeFunction)(T) const, int typeId) {
		return fb.registerDestructor(
			typeId,
			createUserFunctionFactoryMember
			<Class, void, T> // native function prototype
			(
				fb.getSriptCompiler(), // script compiler
				obj,
				"void", // return type of the script function
				nativeFunction // native function
				)
		);
	}

	template<class T>
	class ContantFactory : public DFunction {
		std::string _type;
		T _val;
		ConstOperandBase* _retStorage;
	public:
		ContantFactory(const T& val, const std::string& type) : _val(val), _type(type) {
			_ret = &_retStorage;
		}

		void call() {
			_retStorage = new CConstOperand<T>(_val, _type);
		}
		bool pushParam(void* param) { return false;}
		void* popParam() { return nullptr;}
		DFunction* clone() {
			return new ContantFactory(_val, _type);
		}
	};

	template<class T>
	void setConstantMap(ScriptCompiler* compiler, const std::string& constantName, const std::string& constantType, const T& val) {
		auto constantFactoryRef = std::make_shared<ContantFactory<T>>(val, constantType);
		compiler->setConstantMap(constantName, constantFactoryRef);
	}
}
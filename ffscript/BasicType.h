/******************************************************************
* File:        BasicType.h
* Description: declare BasicTypes class to register basic types,
*              register casting function between types of the
*              scripting language.
*              define some casting function factory classes.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"
#include "expressionunit.h"
#include "FunctionFactory.h"
#include "FunctionRegisterHelper.h"

namespace ffscript {
	class ScriptCompiler;
	class FunctionRegisterHelper;
	class BasicTypes
	{
	private:
	public:
		int TYPE_VOID;
		int TYPE_INT;
		int TYPE_LONG;
		int TYPE_FLOAT;
		int TYPE_DOUBLE;
		int TYPE_BOOL;
		int TYPE_CHAR;
		int TYPE_WCHAR;
		int TYPE_STRING;
		int TYPE_WSTRING;
		int TYPE_VARIANT;
		int TYPE_VARIANTARRAY;
		int TYPE_SYSTEMARRAY;
		int TYPE_ELEMENT_INFO;
		int TYPE_NULL;
		int TYPE_RAWSTRING;
	public:
		BasicTypes();
		~BasicTypes();
		void registerBasicTypes(ScriptCompiler* scriptCompiler);
		void registerConstants(ScriptCompiler* scriptCompiler) const;
		void registerBasicTypeCastFunctions(ScriptCompiler* scriptCompiler, FunctionRegisterHelper& functionLib);
		static BasicTypes* getBasicTypes();
	};

	class CastingFunction : public FixParamFunction<1> {
	public:
		CastingFunction(const std::string& name);
		virtual ~CastingFunction();
	};

	template <class TD, class TS>
	class ConversionFactoryBase : public FunctionFactory {
	protected:
		DFunction2Ref _nativeFunction;
		static TD convert(TS originVal) {
			return (TD)originVal;
		}
	protected:
		ConversionFactoryBase(ScriptCompiler* scriptCompiler, const ScriptType& returnType) :FunctionFactory(nullptr, scriptCompiler) {
			this->setReturnType(returnType);
			_nativeFunction = createFunctionCdeclRef<TD, TS>(ConversionFactoryBase::convert);
		}
	};

	template <class TD, class TS>
	class ConversionFactory : public ConversionFactoryBase<TD,TS> {
	public:
		ConversionFactory(ScriptCompiler* scriptCompiler, const ScriptType& returnType) : ConversionFactoryBase(scriptCompiler, returnType) {}

		Function* createFunction(const std::string& name, int id) {
			NativeFunction* pFunction = new CastingFunction(name);
			pFunction->setNative(_nativeFunction);
			return pFunction;
		}
	};

	template <class TS, class TD = bool>
	class ConversionFactoryToBool : public FunctionFactory {
		DFunction2Ref _nativeFunction;

		static TD convert(TS originVal) {
			return  originVal != 0;
		}

	public:
		ConversionFactoryToBool(ScriptCompiler* scriptCompiler, const ScriptType& returnType) : FunctionFactory(nullptr, scriptCompiler) {
			this->setReturnType(returnType);
			_nativeFunction = createFunctionCdeclRef<TD, TS>(ConversionFactoryToBool::convert);
		}
		Function* createFunction(const std::string& name, int id) {
			NativeFunction* pFunction = new CastingFunction(name);
			pFunction->setNative(_nativeFunction);
			return pFunction;
		}
	};

	template <class TD, class TS = bool>
	class ConversionFactoryBoolTo : public FunctionFactory {
		DFunction2Ref _nativeFunction;

		static TD convert(TS originVal) {
			return (TD)(originVal ? 1 : 0);
		}

	public:
		ConversionFactoryBoolTo(ScriptCompiler* scriptCompiler, const ScriptType& returnType) :FunctionFactory(nullptr, scriptCompiler) {
			this->setReturnType(returnType);
			_nativeFunction = createFunctionCdeclRef<TD, TS>(ConversionFactoryBoolTo::convert);
		}
		Function* createFunction(const std::string& name, int id) {
			NativeFunction* pFunction = new CastingFunction(name);
			pFunction->setNative(_nativeFunction);
			return pFunction;
		}
	};

	class ConvertToStringFactory : public FunctionFactory {
	protected:
		DFunction2Ref _nativeFunction;
	public:
		ConvertToStringFactory(ScriptCompiler* scriptCompiler, DFunction2Ref nativeFunction, const ScriptType& returnType) :FunctionFactory("String", scriptCompiler) {
			this->setReturnType(returnType);
			_nativeFunction = nativeFunction;
		}

		Function* createFunction(const std::string& name, int id) {
			NativeFunction* pFunction = new CastingFunction(name);
			pFunction->setNative(_nativeFunction);
			return pFunction;
		}
	};
}
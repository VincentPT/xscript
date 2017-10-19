#pragma once
#include "ffscript.h"
#include "expressionunit.h"
#include "FunctionFactory.h"
#include "function\CdeclFunction2.hpp"

#define BASIC_TYPE_int TYPE_INT
#define BASIC_TYPE___int64 TYPE_LONG
#define BASIC_TYPE_float TYPE_FLOAT
#define BASIC_TYPE_double TYPE_DOUBLE
#define BASIC_TYPE_bool TYPE_BOOL
#define CPPTYPE_TO_FSCRIPTTYPE(type)  BASIC_TYPE_##type

#define BASIC_CAST_FACTORY_ITEM(returnType, paramType) { #returnType, #paramType, new ConversionFactory<returnType, paramType>(scriptCompiler,CPPTYPE_TO_FSCRIPTTYPE(returnType)) , true}
#define BASIC_CAST_FACTORY_ITEM_R(returnType, paramType) { #returnType, "ref " #paramType, new ConversionFactory<returnType, const paramType&>(scriptCompiler,CPPTYPE_TO_FSCRIPTTYPE(returnType)) , true}
#define BASIC_CAST_BOOL_TO_FACTORY_ITEM(returnType, paramType) { #returnType, #paramType, new ConversionFactoryBoolTo<returnType, paramType>(scriptCompiler,CPPTYPE_TO_FSCRIPTTYPE(returnType)) , true}
#define BASIC_CAST_TO_BOOL_FACTORY_ITEM(returnType, paramType) { #returnType, #paramType, new ConversionFactoryToBool<paramType, returnType>(scriptCompiler,CPPTYPE_TO_FSCRIPTTYPE(returnType)) , true}
#define BASIC_CAST_TO_BOOL_FACTORY_ITEM_R(returnType, paramType) { #returnType, "ref " #paramType, new ConversionFactoryToBool<const paramType&, returnType>(scriptCompiler,CPPTYPE_TO_FSCRIPTTYPE(returnType)) , true}


#ifndef _WIN64
#define BASIC_CAST_FACTORY_ITEM_L(returnType) { #returnType, "ref long", new ConversionFactory<returnType, const __int64&>(scriptCompiler,CPPTYPE_TO_FSCRIPTTYPE(returnType)) , true}
#define BASIC_CAST_TO_BOOL_FACTORY_ITEM_L() { "bool", "ref long", new ConversionFactoryToBool<const __int64&, bool>(scriptCompiler,CPPTYPE_TO_FSCRIPTTYPE(bool)) , true}
#else
#define BASIC_CAST_FACTORY_ITEM_L(returnType) { #returnType, "long", new ConversionFactory<returnType, __int64>(scriptCompiler,CPPTYPE_TO_FSCRIPTTYPE(returnType)) , true}
#define BASIC_CAST_TO_BOOL_FACTORY_ITEM_L() { "bool", "long", new ConversionFactoryToBool<__int64, bool>(scriptCompiler,CPPTYPE_TO_FSCRIPTTYPE(bool)) , true}
#endif


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
		int TYPE_STRING;
		int TYPE_WSTRING;
		int TYPE_VARIANT;
		int TYPE_VARIANTARRAY;
		int TYPE_SYSTEMARRAY;
		int TYPE_ELEMENT_INFO;
		int TYPE_NULL;
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

	class CastingFunctionR : public FixParamFunction<1> {
		FunctionRef _refFunction;
	public:
		CastingFunctionR(FunctionRef refFunction, const std::string& name);
		virtual ~CastingFunctionR();
		virtual int pushParam(ExecutableUnitRef pExeUnit);
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
			_nativeFunction = (DFunction2Ref)(new CdeclFunction2<TD, TS>(ConversionFactoryBase<TD,TS>::convert));
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

	template <class TD, class TS>
	class ConversionFactoryR : public ConversionFactoryBase<TD, const TS&> {
	public:
		ConversionFactoryR(ScriptCompiler* scriptCompiler, const ScriptType& returnType) : ConversionFactoryBase(scriptCompiler, returnType) {}

		Function* createFunction(const std::string& name, int id) {
			int functionId = getCompiler()->getMakingRefFunction();
			FunctionRef makeRefFunc((Function*)(getCompiler()->createFunctionFromId(functionId)));

			NativeFunction* pFunction = new CastingFunctionR(makeRefFunc,  name);
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
			_nativeFunction = (DFunction2Ref)(new CdeclFunction2<TD, TS>(ConversionFactoryToBool::convert));
		}
		Function* createFunction(const std::string& name, int id) {
			NativeFunction* pFunction = new CastingFunction(name);
			pFunction->setNative(_nativeFunction);
			return pFunction;
		}
	};

	template <class TS, class TD = bool>
	class ConversionFactoryToBoolR : public FunctionFactory {
		DFunction2Ref _nativeFunction;

		static TD convert(const TS& originVal) {
			return  originVal != 0;
		}

	public:
		ConversionFactoryToBoolR(ScriptCompiler* scriptCompiler, const ScriptType& returnType) : FunctionFactory(nullptr, scriptCompiler) {
			this->setReturnType(returnType);
			_nativeFunction = (DFunction2Ref)(new CdeclFunction2<TD, const TS&>(ConversionFactoryToBoolR::convert));
		}
		Function* createFunction(const std::string& name, int id) {
			int functionId = getCompiler()->getMakingRefFunction();
			FunctionRef makeRefFunc((Function*)(getCompiler()->createFunctionFromId(functionId)));

			NativeFunction* pFunction = new CastingFunctionR(makeRefFunc, name);
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
			_nativeFunction = (DFunction2Ref)(new CdeclFunction2<TD, TS>(ConversionFactoryBoolTo::convert));
		}
		Function* createFunction(const std::string& name, int id) {
			NativeFunction* pFunction = new CastingFunction(name);
			pFunction->setNative(_nativeFunction);
			return pFunction;
		}
	};
}
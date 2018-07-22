#pragma once
#include "ScriptCompiler.h"
#include "FunctionRegisterHelper.h"
#include "FunctionFactory.h"

#define make_native(RT, T1, T2, func) new CdeclFunction2<RT, T1, T2>(func)


namespace ffscript {
	class FunctionFactoryCdecl : public FunctionFactory
	{
		typedef Function* (*FCreator) (const std::string&,int);

		FCreator _fCreator;

	public:
		FunctionFactoryCdecl(FCreator creator, const ScriptType& returnType);
		virtual ~FunctionFactoryCdecl();
		Function* createFunction(const std::string& name, int id);
	};

	class UserFunctionFactory : public FunctionFactory
	{
		int _paramSize;
	public:
		UserFunctionFactory(ScriptCompiler* scriptCompiler, const char* returnType, int paramSize);
		virtual ~UserFunctionFactory();
		Function* createFunction(const std::string& name, int id);
		virtual DFunction2Ref createNative() = 0;
	};


	Function* createMultiplicative(const std::string& name, int id);	

	///////////////////////////////////////////////
	void importBasicfunction(FunctionRegisterHelper& functionLib);
	void importCoreFunctions(FunctionRegisterHelper& functionLib);
}
